SUMMARY = "Just blink a led and read a button using libgpiod in C lang."
DESCRIPTION = "${SUMMARY}"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://main.c"

DEPENDS = "libgpiod"

S = "${WORKDIR}"

do_compile() {
	${CC} ${LDFLAGS} main.c -o imx8mn-libgpiod -lgpiod
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 imx8mn-libgpiod ${D}${bindir}
}

# Find the bin file in /usr/bin folder
FILES_${PN} += "${bindir}/*"
