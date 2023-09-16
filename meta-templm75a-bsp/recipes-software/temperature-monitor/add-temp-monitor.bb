DESCRIPTION = "Add a service to show the environment temperature to a Oled Display"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = " file://temp-monitor.service \
            file://imx8mn_ssd1306.py \
            file://imx8mn_temp.py \
            file://__init__.py \
        "

S = "${WORKDIR}"

RDEPENDS:${PN} = "python3 python3-smbus i2c-tools python3-pillow python3-pip python3-setuptools"

inherit systemd
inherit features_check

# ---------------- Systemd -------------------
SYSTEMD_AUTO_ENABLE${PN} = "enable"
SYSTEMD_SERVICE:${PN} = "temp-monitor.service"

do_install () {
	# ${datadir} means /usr/share/
    install -d -m 755 ${D}${datadir}/temp-monitor
    install -p -m 644 imx8mn_ssd1306.py ${D}${datadir}/temp-monitor/
    install -p -m 644 imx8mn_temp.py ${D}${datadir}/temp-monitor/
    install -p -m 644 __init__.py ${D}${datadir}/temp-monitor/
    chown -R root:root ${D}${datadir}/temp-monitor
    # Install the service file to the systemd folders
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/temp-monitor.service ${D}${systemd_system_unitdir}/
    chown root:root ${D}${systemd_system_unitdir}/temp-monitor.service
}

FILES:${PN} = "${datadir}/temp-monitor/*"
FILES:${PN} += "${systemd_system_unitdir}/"

# Prevents do_package failures with:
# debugsources.list: No such file or directory:
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
# Tell the image you want to use Systemd service manager
REQUIRED_DISTRO_FEATURES= " systemd"