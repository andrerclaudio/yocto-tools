SUMMARY = "Example recipe for using inherit useradd"
DESCRIPTION = "This recipe serves as an example for using features from useradd.bbclass"
SECTION = "examples"
PR = "r1"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://agnes/file1 \
           file://agnes/file2"

S = "${WORKDIR}"

inherit useradd

# --------------- Regular USER ---------------
# This password is generated with `openssl passwd -6 password`, where -6 stands for SHA-512 hashing alorithgm
# The resulting string is in format $<ALGORITHM_ID>$<SALT>$<PASSWORD_HASH>, the dollar signs have been escaped
# This'll allow user to login with the least secure password there is, "12345678" (without quotes)
PASSWD = "\$6\$aWkWCMANJREcS8Lj\$92bVEBHX5UnYGia6fJJ/KspecJ4sev4W4SK8Pg6lIBL3JPXVQ4yNXJHs0u6ZfexwZxAKJMfvCwjvBZgo5wMaj1"
# This creates a user with name agnes and UID 1200. The password is stored in the PASSWD variable, and home-folder
# is /home/agnes, and the login-shell is set as sh. Finally, this user is added to the sudo-group.
# and also changing the root password to ${PASSWD}

USERADD_PACKAGES = "${PN}"
USERADD_PARAM:${PN} = "-u 1200 -d /home/agnes -m -s /bin/sh -p '${PASSWD}' agnes"
GROUPADD_PARAM:${PN} = "-g 880 group1"

do_install () {
	# ${datadir} means /usr/share/
    install -d -m 755 ${D}${datadir}/agnes
    install -p -m 644 agnes/file1 ${D}${datadir}/agnes/
    install -p -m 644 agnes/file2 ${D}${datadir}/agnes/
    # The new users and groups are created before the do_install
    # step, so you are now free to make use of them:
    chown -R agnes ${D}${datadir}/agnes
    chgrp -R group1 ${D}${datadir}/agnes
}

FILES:${PN} = "${datadir}/agnes/*"

# Prevents do_package failures with:
# debugsources.list: No such file or directory:
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"