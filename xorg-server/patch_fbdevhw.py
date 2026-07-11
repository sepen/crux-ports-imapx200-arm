#!/usr/bin/env python3
"""
Patch xorg-server's hw/xfree86/fbdevhw/fbdevhw.c to fix a bus-detection bug
that rejects framebuffers with no "device" symlink under
/sys/class/graphics/<node>/ at all (e.g. embedded SoC framebuffers wired up
as virtual devices, with no real PCI/platform bus behind them).

The original code treats readlink() failure (ENOENT -- no "device" symlink
present) the same as "this is a PCI device", and rejects it:

    if (asprintf(...) < 0 ||
        readlink(sysfs_path, buf, sizeof(buf) - 1) < 0 ||
        strstr(buf, "bus/pci")) {
        ...
        return -1;
    }

But a missing symlink just means there's no bus information to read at all
-- it says nothing about the device being PCI. Only an actual "bus/pci"
match in the resolved path should cause rejection. This patch splits the
asprintf() failure check (a real allocation error) from the
readlink()/strstr() check (bus detection), so a bus-less framebuffer is
accepted instead of being incorrectly treated as a PCI device.

Usage:
    python3 patch_fbdevhw.py /path/to/fbdevhw.c
    python3 patch_fbdevhw.py /path/to/xorg-server-21.1.24   (searches the tree for it)
"""
import sys
from pathlib import Path

TARGET_NAME = "fbdevhw.c"

OLD = """    /* only touch non-PCI devices on this path */
    {
        char buf[PATH_MAX] = {0};
        char *sysfs_path = NULL;
        char *node = strrchr(dev, '/') + 1;

        if (asprintf(&sysfs_path, "/sys/class/graphics/%s/device/subsystem", node) < 0 ||
            readlink(sysfs_path, buf, sizeof(buf) - 1) < 0 ||
            strstr(buf, "bus/pci")) {
            free(sysfs_path);
            close(fd);
            return -1;
        }
        free(sysfs_path);
    }"""

NEW = """    /* only touch non-PCI devices on this path.
     * Note: readlink() failing (e.g. ENOENT) just means this fb has no
     * "device" symlink at all -- a virtual/bus-less framebuffer (as found
     * on some embedded SoCs), not a PCI device. That case must NOT be
     * rejected here; only an actual "bus/pci" match should be. */
    {
        char buf[PATH_MAX] = {0};
        char *sysfs_path = NULL;
        char *node = strrchr(dev, '/') + 1;

        if (asprintf(&sysfs_path, "/sys/class/graphics/%s/device/subsystem", node) < 0) {
            close(fd);
            return -1;
        }
        if (readlink(sysfs_path, buf, sizeof(buf) - 1) >= 0 && strstr(buf, "bus/pci")) {
            free(sysfs_path);
            close(fd);
            return -1;
        }
        free(sysfs_path);
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

    if NEW in content:
        print(f"Already patched: {path}")
        return

    if OLD not in content:
        print(
            "ERROR: expected pattern not found in file -- whitespace/"
            "indentation may differ from what this script expects, or the "
            "upstream code has changed. No changes made.",
            file=sys.stderr,
        )
        sys.exit(1)

    content = content.replace(OLD, NEW)

    with open(path, "w") as fh:
        fh.write(content)

    print(f"Patched: {path}")


if __name__ == "__main__":
    main()
