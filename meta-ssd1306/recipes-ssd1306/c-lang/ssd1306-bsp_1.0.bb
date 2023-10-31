SUMMARY = "Oled display SSD1306 driver."
DESCRIPTION = "${SUMMARY}"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "	\
				file://Makefile \
				file://main.c \
				file://ssd1306.c \
				file://linux_i2c.c \
				file://commons.h \
				file://ssd1306.h \
				file://linux_i2c.h \
				file://font.h \
			"
			
S = "${WORKDIR}"

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ssd1306 ${D}${bindir}
}

# Find the bin file in /usr/bin folder
FILES_${PN} += "${bindir}/*"