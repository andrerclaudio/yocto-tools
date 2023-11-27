"""
Turn On a led and keep it on until the Button is pressed.
"""

import logging
from enum import Enum, auto
import threading
import time
import gpiod
from gpiod.edge_event import EdgeEvent
from gpiod.line_settings import Direction, Value, timedelta, Edge

# Create a logger
logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s]: %(message)s")

CONSUMER_NAME = 'App'  # Consumer name

# Define GPIO pin numbers
LED = 9  # Assign pin 9 to the LED
BUTTON = 8  # Assign pin 8 to the Button

# Input particular settings
INPUT_DEBOUNCE_PERIOD = 0.05

# Input particular settings
OUTPUT_BLINK_DELAY = 0.2

# Led state definitions
LED_ON = Value.ACTIVE
LED_OFF = Value.INACTIVE

# Path to Line
PATH = '/dev/gpiochip4'


class ButtonStatus(Enum):
    CLOSED = auto()
    OPEN = auto()


class GpioOutput(threading.Thread):
    """

    """

    def __init__(self, control: dict, lock: threading.Lock, gpio_controller: gpiod.LineRequest,
                 output_number: int) -> None:

        threading.Thread.__init__(self)

        self.control = control
        self.lock = lock
        self.led_state = LED_OFF
        self.output = gpio_controller
        self.output_number = output_number
        self.thread = threading.Thread()
        self.thread.name = f"[Output-{output_number}]"
        self.start()

    def run(self):
        """

        """

        logging.info(f"Starting led [Line: {self.output_number}] Thread.")

        try:

            while True:

                with self.lock:
                    running = self.control['__KEEP_RUNNING']
                    if not running:
                        break

                self.led_state = (LED_ON if self.led_state == LED_OFF else LED_OFF)
                self.output.set_value(line=self.output_number, value=self.led_state)
                time.sleep(OUTPUT_BLINK_DELAY)

            self.output.set_value(line=self.output_number, value=LED_OFF)
            logging.info(f"Finishing led [Line: {self.output_number}] Thread!")

        except threading.ThreadError as e:
            logging.debug(e, exc_info=False)


class GpioInput(threading.Thread):
    """
    GpioInput class representing a thread monitoring a GPIO input.

    Args:
        control (dict): A dictionary containing control flags.
        gpio_controller (gpiod.LineRequest): GPIO input configuration.
        input_number (int): The number associated with this input.
        lock (threading.Lock): Lock for synchronization with shared resources.
    """

    def __init__(self, control: dict, lock: threading.Lock, gpio_controller: gpiod.LineRequest,
                 input_number: int) -> None:

        threading.Thread.__init__(self)

        self.control = control
        self.lock = lock
        self.bt_state = ButtonStatus.OPEN
        self.input = gpio_controller
        self.input_number = input_number
        self.thread = threading.Thread()
        self.thread.name = f"[Input-{input_number}]"
        self.start()

    def run(self):
        """
        Run method for the thread.
        Continuously monitors the GPIO input and updates the button state.
        """

        logging.info(f"Starting button [Line: {self.input_number}] Thread.")

        try:

            while True:

                self.input.wait_edge_events(timeout=INPUT_DEBOUNCE_PERIOD)

                with self.lock:
                    running = self.control['__KEEP_RUNNING']
                    if not running:
                        break

                event: EdgeEvent = list(self.input.read_edge_events(max_events=1))[0]
                event_type = event.event_type.name
                self.bt_state = ButtonStatus.CLOSED if event_type == 'FALLING_EDGE' else ButtonStatus.OPEN

            logging.info(f"Finishing button [Line: {self.input_number}] Thread!")

        except threading.ThreadError as e:
            logging.debug(e, exc_info=False)

    @staticmethod
    def __conv_button_value(val: Value) -> Enum:
        """
        Convert GPIO button value from 'gpiod.request_lines.get_value'
        to a boolean.

        Args:
            val (gpiod.line_settings.Value): GPIO button (get) value.

        Returns:
            bool: True if the button value is active, False otherwise.
        """

        return ButtonStatus.OPEN if val == Value.ACTIVE else ButtonStatus.CLOSED

    def state(self) -> Enum:
        """
        Get the current state of the button.

        Returns:
            ButtonStatus: The current state of the button.
        """
        return self.bt_state


# Define the main function for running the script
def application():
    """
    Main function for running the script.

    This function initializes GPIO lines, turns on a LED, waits for a button press,
    and then turns off the LED.

    Raises:
        gpiod.exception: If an error occurs during GPIO operations.
    """

    logging.info("Starting the application!")

    # Request LINES config and others relevant values
    lines = gpiod.request_lines(PATH,
                                consumer=CONSUMER_NAME,
                                config={
                                    LED:
                                        gpiod.LineSettings(
                                            direction=Direction.OUTPUT,
                                            output_value=LED_OFF,
                                        ),
                                    BUTTON:
                                        gpiod.LineSettings(
                                            direction=Direction.INPUT,
                                            edge_detection=Edge.BOTH,
                                            debounce_period=timedelta(seconds=INPUT_DEBOUNCE_PERIOD),
                                        )
                                }
                                )

    # General Application control flags
    # Values can change as the application requires it.
    control_flags = {

        '__KEEP_RUNNING': True  # Tell the software must keep running.
    }

    # Define a lock for synchronization
    lock_flag = threading.Lock()

    try:

        GpioOutput(control=control_flags,
                   lock=lock_flag,
                   gpio_controller=lines,
                   output_number=LED,
                   )

        thr_input = GpioInput(control=control_flags,
                              lock=lock_flag,
                              gpio_controller=lines,
                              input_number=BUTTON,
                              )

        logging.info("Waiting button being pressed ...")
        while thr_input.state() is ButtonStatus.OPEN:
            pass

        with lock_flag:
            # Signalize the application will stop
            control_flags["__KEEP_RUNNING"] = False

        # Hold a while to make sure the threads are down
        time.sleep(1)
        logging.info("Releasing resources ...")

    except Exception as e:
        logging.debug(e, exc_info=False)

    finally:
        lines.release()


if __name__ == "__main__":
    # Run the script
    application()
