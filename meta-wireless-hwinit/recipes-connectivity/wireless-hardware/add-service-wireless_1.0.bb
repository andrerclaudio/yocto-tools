DESCRIPTION = "Add a service to start the wireless hardware."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = " file://wireless-hwinit.service \
            file://udhcpc-mlan0.service \
        "

S = "${WORKDIR}"

inherit systemd
inherit features_check

# ---------------- Systemd -------------------
SYSTEMD_AUTO_ENABLE${PN} = "enable"
SYSTEMD_SERVICE:${PN} = "wireless-hwinit.service udhcpc-mlan0.service"

do_install () {
    # Install the service file to the systemd folders
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/wireless-hwinit.service ${D}${systemd_system_unitdir}/
    install -m 0644 ${WORKDIR}/udhcpc-mlan0.service ${D}${systemd_system_unitdir}/
    chown root:root ${D}${systemd_system_unitdir}/wireless-hwinit.service
    chown root:root ${D}${systemd_system_unitdir}/udhcpc-mlan0.service
}

FILES:${PN} += "${systemd_system_unitdir}/"

# Prevents do_package failures with:
# debugsources.list: No such file or directory:
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
# Tell the image you want to use Systemd service manager
REQUIRED_DISTRO_FEATURES= " systemd"