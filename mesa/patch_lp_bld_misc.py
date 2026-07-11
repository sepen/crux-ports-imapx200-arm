#!/usr/bin/env python3
"""
Patch Mesa's src/gallium/auxiliary/gallivm/lp_bld_misc.cpp to replace an
explicit llvm::StringMapIterator<bool> loop with a range-based for loop.

LLVM's internal StringMap iterator class is not part of its stable API and
can be renamed/restructured between point releases (e.g. gcc suggesting
StringMapKeyIterator instead of StringMapIterator on LLVM 22.1.8). A
range-based for loop only relies on features.begin()/end(), which is stable
regardless of what the underlying iterator class is called.

Usage:
    python3 patch_lp_bld_misc.py /path/to/lp_bld_misc.cpp
    python3 patch_lp_bld_misc.py /path/to/mesa-26.1.4   (searches the tree for it)
"""
import sys
from pathlib import Path

TARGET_NAME = "lp_bld_misc.cpp"

OLD = """   for (llvm::StringMapIterator<bool> f = features.begin();
        f != features.end();
        ++f) {
      MAttrs.push_back(((*f).second ? "+" : "-") + (*f).first().str());
   }"""

NEW = """   for (auto &f : features) {
      MAttrs.push_back((f.second ? "+" : "-") + f.first().str());
   }"""


def resolve_path(arg):
    p = Path(arg)

    if p.is_file():
        return p

    if p.is_dir():
        matches = list(p.rglob(TARGET_NAME))
        if not matches:
            print(f"ERROR: no '{TARGET_NAME}' found under {p}", file=sys.stderr)
            sys.exit(1)
        if len(matches) > 1:
            print(
                f"ERROR: multiple '{TARGET_NAME}' found under {p}, "
                "pass the exact file path instead:",
                file=sys.stderr,
            )
            for m in matches:
                print(f"  {m}", file=sys.stderr)
            sys.exit(1)
        return matches[0]

    print(f"ERROR: {arg} is neither a file nor a directory", file=sys.stderr)
    sys.exit(1)


def main():
    if len(sys.argv) != 2:
        print(
            f"Usage: {sys.argv[0]} <path to {TARGET_NAME} or a source dir containing it>",
            file=sys.stderr,
        )
        sys.exit(1)

    path = resolve_path(sys.argv[1])

    with open(path) as fh:
        content = fh.read()

    if OLD not in content:
        print(
            "ERROR: expected pattern not found in file — it may already be "
            "patched, or the whitespace/indentation differs from what this "
            "script expects. No changes made.",
            file=sys.stderr,
        )
        sys.exit(1)

    content = content.replace(OLD, NEW)

    with open(path, "w") as fh:
        fh.write(content)

    print(f"Patched: {path}")


if __name__ == "__main__":
    main()
