FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://support-lm75a.cfg file://support-lm75a.scc file://0001-support-lm75a.patch"

do_configure:append() {
        # Force the bitbake to append the .cfg file
        cat ../*.cfg >> ${B}/.config
}