SUMMARY = "Just read a button and toggle a Led."
DESCRIPTION = "${SUMMARY}"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "	\
				file://Makefile \
				file://main.c \
			"
			
S = "${WORKDIR}"

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 imx8mn-gpiotoggle ${D}${bindir}
}

# Find the bin file in /usr/bin folder
FILES_${PN} += "${bindir}/*"