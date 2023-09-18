DESCRIPTION = "Add a service to show the system information to a Oled Display"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = " file://hw-stats.service \
            file://imx8mn_ssd1306.py \
            file://imx8mn_stats.py \
            file://__init__.py \
        "

S = "${WORKDIR}"

RDEPENDS:${PN} = "python3 python3-smbus i2c-tools python3-pillow python3-pip python3-setuptools"

inherit systemd
inherit features_check

# ---------------- Systemd -------------------
SYSTEMD_AUTO_ENABLE${PN} = "enable"
SYSTEMD_SERVICE:${PN} = "hw-stats.service"

do_install () {
	# ${datadir} means /usr/share/
    install -d -m 755 ${D}${datadir}/hw-stats
    install -p -m 644 imx8mn_ssd1306.py ${D}${datadir}/hw-stats/
    install -p -m 644 imx8mn_stats.py ${D}${datadir}/hw-stats/
    install -p -m 644 __init__.py ${D}${datadir}/hw-stats/
    chown -R root:root ${D}${datadir}/hw-stats
    # Install the service file to the systemd folders
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/hw-stats.service ${D}${systemd_system_unitdir}/
    chown root:root ${D}${systemd_system_unitdir}/hw-stats.service
}

FILES:${PN} = "${datadir}/hw-stats/*"
FILES:${PN} += "${systemd_system_unitdir}/"

# Prevents do_package failures with:
# debugsources.list: No such file or directory:
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
# Tell the image you want to use Systemd service manager
REQUIRED_DISTRO_FEATURES= " systemd"