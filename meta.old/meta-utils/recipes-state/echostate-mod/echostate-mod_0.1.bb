SUMMARY = "Echo state to a GPIO added external Linux kernel module using proc virtual file"
DESCRIPTION = "${SUMMARY}"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

# MODULES_MODULE_SYMVERS_LOCATION = "~/usr/src/linux-headers-$(uname -r)/Module.symvers"
# DEPENDS += "libgpiod"

inherit module

SRC_URI = "file://Makefile \
           file://echo-state.c \
           file://COPYING \
          "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES:${PN} += "kernel-module-echoestate"