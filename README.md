# crux-ports-arm-imapx200

Unofficial CRUX-ARM port overlay for devices powered by the IMAPX200 processor, including the Airis Kira N7000 and Coby NPC724

To use these ports, download the `imapx200-arm.httpup` file to `/etc/ports`
```
$ sudo wget -P /etc/ports https://raw.githubusercontent.com/sepen/crux-ports-imapx200-arm/main/imapx200-arm.httpup
$ sudo ports -u imapx200-arm
```

You may want to list it first in `/etc/prt-get.conf` to take advantage of ports overlay:
```
###
### prt-get conf
###

# note: the order matters: the package found first is used
prtdir /usr/ports/imapx200-arm
prtdir /usr/ports/core-arm
prtdir /usr/ports/opt-arm
prtdir /usr/ports/xorg-arm
prtdir /usr/ports/core
prtdir /usr/ports/opt
prtdir /usr/ports/xorg
```

## CRUX 3.8 Legacy Overlay

This repository provides an overlay for CRUX 3.8 with updated ports for legacy ARM platforms, primarily targeting the InfoTM IMAPX200 (ARMv6).

Due to the limitations of the Linux 2.6.32.9 kernel, the overlay is based on the following toolchain:

- glibc 2.22
- gcc 7.5.0
- binutils 2.30
- util-linux 2.38.1

All other ports are updated and, where necessary, minimally patched to remain compatible with this toolchain and the legacy kernel while staying as close as possible to upstream.

