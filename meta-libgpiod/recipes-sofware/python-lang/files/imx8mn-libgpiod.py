"""
Turn On a led and keep it on until the Button is pressed.
"""

# pylint: disable=import-error, multiple-imports, wrong-import-order, missing-function-docstring, broad-exception-caught

import logging
import gpiod
from gpiod.line_settings import Direction, Value, timedelta

# Create a logger
logger = logging.getLogger('libgpiod')
# Set the logging level (DEBUG, INFO, WARNING, ERROR, CRITICAL)
logger.setLevel(logging.DEBUG)

CONSUMER_NAME = 'App' # Consumer name

# Define GPIO pin numbers
LED = 9      # Assign pin 9 to the LED
BUTTON = 8      # Assign pin 8 to the Button

# Led state definitions
LED_ON = Value.ACTIVE
LED_OFF = Value.INACTIVE

# Path to Line
PATH = '/dev/gpiochip4'


def conv_button_value(val:Value) -> bool:
    """
    Convert GPIO button value from 'gpiod.request_lines.get_value'
    to a boolean.

    Args:
        val (gpiod.line_settings.Value): GPIO button (get) value.

    Returns:
        bool: True if the button value is active, False otherwise.
    """

    return True if val == Value.ACTIVE else False



# Define the main function for running the script
def run():
    """
    Main function for running the script.

    This function initializes GPIO lines, turns on a LED, waits for a button press,
    and then turns off the LED.

    Raises:
        gpiod.exception: If an error occurs during GPIO operations.
    """

    # Request LINES config and others relevants values
    lines = gpiod.request_lines(PATH, 
                                consumer=CONSUMER_NAME,
                                config={LED: 
                                        gpiod.LineSettings(
                                            direction=Direction.OUTPUT, 
                                            output_value=LED_OFF,
                                            ),
                                        BUTTON:
                                        gpiod.LineSettings(
                                            direction=Direction.INPUT,
                                            debounce_period=timedelta(milliseconds=100),
                                        )
                                    }
                                )

    try:

        lines.set_value(LED, value=LED_ON)

        while conv_button_value(lines.get_value(BUTTON)):
            pass

        lines.set_value(line=LED, value=LED_OFF)

    except gpiod.exception as e:
        logger.debug(e, exc_info=False)

    finally:
        lines.release()

if __name__ == "__main__":
    # Run the script
    run()
