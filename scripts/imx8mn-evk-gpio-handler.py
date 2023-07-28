import time
import os

MENU_OPTIONS = [
    "\n\n"
    "______________________________________ IMX8MN-EVK __",
    "\n"
    "   (1) Turn ON the led",
    "   (2) Turn OFF the led",
    "   (3) Toggle the led state",
    "   (4) Give it Heartbeat state",
    "\n",
    "   (0) Exit",
    "",
]

GPIO_OUTPUT_DEFINITIONS = {
    'TURN_ON': '1',
    'TURN_OFF': '0',
    'HEARTBEAT': 'heartbeat'
}

GPIO_OUTPUT_FILE_PATH = "/sys/devices/platform/gpio-leds/leds/green:agnes/"

def clear_screen():
    """ Clear screen tool """
    os.system('cls' if os.name == 'nt' else 'clear')

def write_to_file(file_path, content):
    """ Write strings to file """
    with open(file_path, 'w') as file:
        file.write(content)

def clear_output():
    """ Clear the output state """
    content_to_write = GPIO_OUTPUT_DEFINITIONS['TURN_OFF']
    write_to_file(GPIO_OUTPUT_FILE_PATH + 'brightness', content_to_write)
    print("Brightness turned off.")

def set_output():
    """ Set the output on """
    content_to_write = GPIO_OUTPUT_DEFINITIONS['TURN_ON']
    write_to_file(GPIO_OUTPUT_FILE_PATH + 'brightness', content_to_write)
    print("Brightness turned on.")

def application():

    clear_screen()
    for lines in MENU_OPTIONS:
        print(lines)

    while True:

        try:
            choice = int(input("\nEnter your choice:  "))

            if choice == 1:
                clear_output()
                set_output()

            elif choice == 2:
                clear_output()

            elif choice == 3:
                pass

            elif choice == 4:
                content_to_write = GPIO_OUTPUT_DEFINITIONS['HEARTBEAT']
                write_to_file(GPIO_OUTPUT_FILE_PATH + 'trigger', content_to_write)
                print("The heart is beating.")

            elif choice == 0:
                print("Exiting...")
                # Only for psicologic efects, let's delay it
                time.sleep(1)
                break

            else:
                print("Invalid choice. Please select a valid option.")

        except ValueError:
            print("Invalid input. Please enter a number.")


if __name__ == "__main__":
    application()