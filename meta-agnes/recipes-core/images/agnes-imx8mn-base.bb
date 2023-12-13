SUMMARY = "A console-only image that fully supports the target device \
hardware."

IMAGE_FEATURES += "splash"

LICENSE = "MIT"

inherit core-image

IMAGE_FEATURES += " \
    debug-tweaks \
    splash \
    ssh-server-openssh \
    hwcodecs \
"

IMAGE_INSTALL += " \
    firmwared \
    packagegroup-imx-core-tools \
    packagegroup-imx-security \
"

# Set machine Hostname
hostname:pn-base-files = "iMX8mn-evk"

export IMAGE_BASENAME = "agnes-imx8mn-base"
