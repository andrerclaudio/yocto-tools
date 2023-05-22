SUMMARY = "Echo state to a GPIO added external Linux kernel module using proc virtual file"
DESCRIPTION = "${SUMMARY}"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"

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