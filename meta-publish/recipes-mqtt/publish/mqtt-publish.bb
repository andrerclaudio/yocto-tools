DESCRIPTION = "Add python script to image."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://mqtt-publish.py"

S = "${WORKDIR}"

RDEPENDS:${PN} = "python3 python3-paho-mqtt"

do_install () {
	# ${datadir} means /usr/share/
    install -d -m 755 ${D}${datadir}/mqtt-publish
    install -p -m 644 mqtt-publish.py ${D}${datadir}/mqtt-publish/
    chown -R root:root ${D}${datadir}/mqtt-publish
}

FILES:${PN} = "${datadir}/mqtt-publish/*"
