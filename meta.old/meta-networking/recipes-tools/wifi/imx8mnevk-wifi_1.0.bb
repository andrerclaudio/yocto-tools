DESCRIPTION = "Ask for wifi access point details and set hardware and wifi connection."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://imx8mnevk-wifi_1.0.py"

DEPENDS = "python3"

do_install(){
  install -d ${D}/${sysconfdir}/scripts/
  install -m 755 ${WORKDIR}/imx8mnevk-wifi_1.0.py ${D}/${sysconfdir}/scripts/imx8mnevk-wifi_1.0.py
}

FILES:${PN} += "${sysconfdir}/scripts"