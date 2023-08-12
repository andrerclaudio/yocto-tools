SUMMARY = "Example recipe for using inherit useradd"
DESCRIPTION = "This recipe serves as an example for using features from useradd.bbclass"
SECTION = "examples"
PR = "r1"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://file1 \
           file://file2"

S = "${WORKDIR}"

inherit useradd

USERADD_PACKAGES = "${PN}"
USERADD_PARAM:${PN} = "-u 1200 -d /home/agnes -m -s /bin/sh agnes"
GROUPADD_PARAM:${PN} = "-g 880 group1"

do_install () {
	# ${datadir} means /usr/share/
    install -d -m 755 ${D}${datadir}/agnes
    install -p -m 644 file1 ${D}${datadir}/agnes/
    install -p -m 644 file2 ${D}${datadir}/agnes/
    # The new users and groups are created before the do_install
    # step, so you are now free to make use of them:
    chown -R agnes ${D}${datadir}/agnes
    chgrp -R group1 ${D}${datadir}/agnes
}

FILES:${PN} = "${datadir}/agnes/*"

# Prevents do_package failures with:
# debugsources.list: No such file or directory:
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"