SUMMARY = "Example recipe for using inherit useradd"
DESCRIPTION = "This recipe serves as an example for using features from useradd.bbclass"
SECTION = "examples"
PR = "r1"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://imx8mn_ssd1306.py \
           file://imx8mn_stats.py \
           file://__init__.py"

S = "${WORKDIR}"

RDEPENDS:${PN} = "python3 python3-smbus i2c-tools python3-pillow python3-pip python3-setuptools"

inherit useradd

# --------------- Service USER ---------------
USERADD_PACKAGES = "${PN}"
GROUPADD_PARAM:${PN} = "--system oled"
# This creates a non-root user that cannot be logged in as
USERADD_PARAM:${PN} = "--system -s /sbin/nologin -g oled oled"
# --------------------------------------------

do_install () {
	# ${datadir} means /usr/share/
    install -d -m 755 ${D}${datadir}/oled
    install -p -m 644 imx8mn_ssd1306.py ${D}${datadir}/oled/
    install -p -m 644 imx8mn_stats.py ${D}${datadir}/oled/
    install -p -m 644 __init__.py ${D}${datadir}/oled/
    # The new users and groups are created before the do_install
    # step, so you are now free to make use of them:
    chown -R oled ${D}${datadir}/oled
    chgrp -R oled ${D}${datadir}/oled
}

FILES:${PN} = "${datadir}/oled/*"

# Prevents do_package failures with:
# debugsources.list: No such file or directory:
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"