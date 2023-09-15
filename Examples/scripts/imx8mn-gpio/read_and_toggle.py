"""
Just a simple script in python to toggle a led state each time a button is pressed.
The button here is being pulled and not by checked by interruption.
See the Device Tree in order to confirm physical pin designation (9 and 8) as below.
"""

# pylint: disable=import-error, multiple-imports, wrong-import-order, missing-function-docstring

# Import necessary modules
import gpiod  # Import the gpiod module for GPIO control
import time   # Import the time module for sleep functionality

# Define GPIO pin numbers
LED = 9      # Assign pin 9 to the LED
BUTTON = 8   # Assign pin 8 to the BUTTON

# Initialize the GPIO chip
chip = gpiod.chip('gpiochip4')  # Open the GPIO chip labeled 'gpiochip4'

# Retrieve the GPIO lines for the LED and BUTTON pins
ret = chip.get_lines([LED, BUTTON])  # Get access to the specified GPIO pins

# Assign the GPIO lines to variables for easier access
led = ret[0]     # LED line
button = ret[1]  # BUTTON line

# Define configuration for the LED line
led_config = gpiod.line_request()
led_config.consumer = "Led"  # Set the consumer name to 'Led'
led_config.request_type = gpiod.line_request.DIRECTION_OUTPUT  # Configure as output

# Define configuration for the BUTTON line
button_config = gpiod.line_request()
button_config.consumer = "Button"  # Set the consumer name to 'Button'
button_config.request_type = gpiod.line_request.DIRECTION_INPUT  # Configure as input

# Request control of the LED and BUTTON lines
led.request(led_config)       # Request control of the LED line with the specified configuration
button.request(button_config) # Request control of the BUTTON line with the specified configuration

# Define the main function for running the script
def run():
    delay = 0.05  # Set the delay time to 0.05 seconds (50 ms)

    try:
        while True:  # Run indefinitely
            if not button.get_value():  # Check if the BUTTON is pressed
                while not button.get_value():
                    # Wait for the BUTTON to be released before proceeding
                    pass

                # Toggle the state of the LED
                if led.get_value():
                    # Turn off the LED
                    led.set_value(0)
                else:
                    # Turn on the LED
                    led.set_value(1)

            time.sleep(delay)  # Wait for the specified delay

    finally:
        # Release GPIO resources when the script exits
        led.release()     # Release the LED line
        button.release()  # Release the BUTTON line

# Execute the 'run' function when the script is run directly
if __name__ == "__main__":
    run()
