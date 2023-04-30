#!/usr/bin/env python3
"""
Adjust the wifi hardware and settings to properly get connected.
"""

import os
import time
import subprocess
import signal

HARDWARE_NAME = 'mlan0'  # Set the deafult hardware name that is gonna be used.

COMMAND_INDEX = 0
TIMEOUT_INDEX = 1

commands = {

    "load_module": ["modprobe moal mod_para=nxp/wifi_mod_para.conf", False],
    "hardware_up": [f"ifconfig {HARDWARE_NAME} up", False],
    "associate_config": [f"wpa_supplicant -B -i {HARDWARE_NAME} -c /etc/wpa_supplicant.conf", True],
    "connection_confirmation": [f"{HARDWARE_NAME}: link becomes ready", False],
    "enable_dhcp": [f"udhcpc -i {HARDWARE_NAME}", False],
}

def create_wifi_config(ssid, password):
    """
    Set the wifi parameters.
    """

    config_lines = [
        'ctrl_interface=/var/run/wpa_supplicant',
        'ctrl_interface_group=0',
        'update_config=1\n',
        'network={',
        '\tscan_ssid=1',
        f'\tssid="{ssid}"',
        f'\tpsk="{password}"',
        '}'
    ]

    config = '\n'.join(config_lines)

    with open("/etc/wpa_supplicant.conf", "w", encoding='utf-8') as wpa:
        wpa.write(config)


def command_to_execute(commmand):
    """
    Execute a given command.
    """

    timeout = commmand[TIMEOUT_INDEX]
    cmd = str(commmand[COMMAND_INDEX])
    os.system(cmd)

    if timeout:
        time.sleep(10)
        process = subprocess.Popen('ifconfig')  # pass cmd and args to the function
        process.send_signal(signal.SIGINT)      # send Ctrl-C signal


def main():
    """
    Run the mainscript.
    """
    global HARDWARE_NAME

    ssid = input("Please, enter with the wifi SSID:  ")
    pwd = input("Please, enter with the wifi password:  ")
    HARDWARE_NAME = input("Please, enter with the hardware name:  ")

    command_to_execute(commands["load_module"])
    command_to_execute(commands["hardware_up"])
    create_wifi_config(ssid, pwd)
    command_to_execute(commands["associate_config"])
    command_to_execute(commands["enable_dhcp"])
    return None


if __name__ == "__main__":
    # Run the main function.
    main()