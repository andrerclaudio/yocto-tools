DESCRIPTION = "Turn On a led and keep it on until the Button is pressed."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://imx8mn-libgpiod.py"

DEPENDS += "python3-gpiod"
RDEPENDS:${PN} += "python3 python3-gpiod"

do_install(){
  install -d ${D}/${sysconfdir}/scripts/
  install -m 755 ${WORKDIR}/imx8mn-libgpiod.py ${D}/${sysconfdir}/scripts/imx8mn-libgpiod.py
}

FILES:${PN} += "${sysconfdir}/scripts"