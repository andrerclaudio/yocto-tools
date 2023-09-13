SUMMARY = "Install and start a systemd service"
SECTION = "examples"
LICENSE = "CLOSED"

SRC_URI = "file://hello.c \
           file://hello.service \
          "

inherit systemd

S = "${WORKDIR}"

SYSTEMD_AUTO_ENABLE${PN} = "enable"
SYSTEMD_SERVICE:${PN} = "hello.service"

do_compile () {
	${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/hello.c -o ${WORKDIR}/hello
}

do_install () {

	install -d ${D}${sbindir}
	install -m 0755 ${WORKDIR}/hello ${D}${sbindir}/

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/hello.service ${D}${systemd_system_unitdir}/
}

FILES:${PN} += "${systemd_unitdir}/"

REQUIRED_DISTRO_FEATURES= " systemd"