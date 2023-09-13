DESCRIPTION = "A simple blinking led application"
SECTION = "Leds"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

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
