# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "This is the basic core image with minimal tests"

inherit core-image

IMAGE_FEATURES += " \
    debug-tweaks \
    dev-pkgs \
    tools-profile \
    tools-sdk \
    package-management \
    splash \
    nfs-server \
    tools-debug \
    ssh-server-openssh \
    hwcodecs \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'weston','', d)} \
"

SDKIMAGE_FEATURES:append = " \
    staticdev-pkgs \
"

IMAGE_INSTALL += " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11 wayland', 'weston-xwayland xterm', '', d)} \
    imx-test \
    firmwared \
    packagegroup-imx-core-tools \
    packagegroup-imx-security \
    curl \
    tzdata \
    nano \
    htop \
    iperf3 \
    git \
    libgpiod \
    python3 \
    python3-pip \
    python3-modules \
    python3-misc \
    ${CLINFO} \
"

# Image Timezone settings
# DEFAULT_TIMEZONE = "America/Sao_Paulo"

# Extra settings
# CORE_IMAGE_EXTRA_INSTALL += "agnes-packagegroup-testapps"

# Add to your layers "meta-wireless-hwinit" and change SSID and PASSWORD in "wpa_supplicant-nl80211-mlan0.conf"
# to enable board Wifi.

CLINFO              ?= ""
CLINFO:imxgpu        = "clinfo"
CLINFO:mx8mm-nxp-bsp = ""
CLINFO:mx7-nxp-bsp   = ""

# Set machine Hostname
hostname:pn-base-files = "iMX8mn-evk"

# Defines additional free disk space created in the image in Kbytes. By default, this variable is set to “0”. 
# This free disk space is added to the image after the build system determines the image size as described in IMAGE_ROOTFS_SIZE.
# IMAGE_ROOTFS_EXTRA_SPACE = "655360"

export IMAGE_BASENAME = "agnes-imx8mn-core"
