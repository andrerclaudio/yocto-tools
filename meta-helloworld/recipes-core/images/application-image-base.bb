SUMMARY = "A console-only image that fully supports the target device \
hardware."

IMAGE_FEATURES += "splash"

LICENSE = "MIT"

inherit core-image

# Install extra packages
IMAGE_INSTALL:append = " nano ntp python3-pip openssh helloworld"