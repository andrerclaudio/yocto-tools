import gpiod, time

# pins
LED = 9
BUTTON = 8

chip = gpiod.chip('gpiochip4')
chip = gpiod.chip('gpiochip4')

ret = chip.get_lines([LED, BUTTON])

led = ret[0]
button = ret[1]

led_config = gpiod.line_request()
led_config.consumer = "Led"
led_config.request_type = gpiod.line_request.DIRECTION_OUTPUT

button_config = gpiod.line_request()
button_config.consumer = "Button"
button_config.request_type = gpiod.line_request.DIRECTION_INPUT

led.request(led_config)
button.request(button_config)

def run():

    delay = 0.05

    try:

        while True:

            if not button.get_value(): 

                while not button.get_value():
                    pass

                if led.get_value():
                    led.set_value(0)
                else:
                    led.set_value(1)

            time.sleep(delay)

    finally:
        cleanup()

def cleanup():
    led.release()
    button.release()

if __name__ == "__main__":
    run()