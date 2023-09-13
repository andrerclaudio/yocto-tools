SUMMARY = "A console-only image that fully supports the target device \
hardware."

IMAGE_FEATURES += "splash"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit core-image

# Install extra packages
IMAGE_INSTALL:append = " nano ntp python3-pip openssh helloworld"