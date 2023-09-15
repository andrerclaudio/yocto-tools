SUMMARY = "A simple string loopback added as external Linux kernel module using proc virtual file"
DESCRIPTION = "${SUMMARY}"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit module

SRC_URI = "file://Makefile \
           file://read_write.c \
           file://COPYING \
          "

# The module will added at /lib/modules/$(uname -r)/extra/
S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES:${PN} += "kernel-module-devicefunc"
