DESCRIPTION = "A simple blinking led application"
SECTION = "Leds"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://led-blink.c"

DEPENDS = "libgpiod"

S = "${WORKDIR}"

do_compile() {
	${CC} ${LDFLAGS} led-blink.c -o led-blink -lgpiod
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 led-blink ${D}${bindir}
}
