"""
Copyright (c) 2023 Andre Ribeiro Claudio
Author: Andre Ribeiro Claudio:
Based on Tony DiCola python code
Original link:
https://github.com/adafruit/Adafruit_Python_SSD1306/blob/master/Adafruit_SSD1306/SSD1306.py

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
"""

# pylint: disable=import-error, multiple-imports, wrong-import-order, missing-function-docstring, line-too-long
# pylint: disable=consider-using-f-string, invalid-name, useless-object-inheritance, too-many-instance-attributes

import time
import logging

from imx8mn_ssd1306 import SSD1306_128_32

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont


logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')

# 128x32 display with hardware I2C:
disp = SSD1306_128_32(i2c_bus=2)

# Initialize library.
disp.begin()

# Clear display.
disp.clear()
disp.display()

# Create blank image for drawing.
# Make sure to create image with mode '1' for 1-bit color.
width = disp.width
height = disp.height
image = Image.new('1', (width, height))

# Get drawing object to draw on image.
draw = ImageDraw.Draw(image)

# Draw a black filled box to clear the image.
draw.rectangle((0, 0, width, height), outline=0, fill=0)

# Draw some shapes.
# First define some constants to allow easy resizing of shapes.
PADDING = -2
top = PADDING
bottom = height-PADDING

# Move left to right keeping track of the current x position for drawing shapes.
x = 0

# Load default font.
font = ImageFont.load_default()

# Alternatively load a TTF font.  Make sure the .ttf font file is in the same directory as the python script!
# Some other nice fonts to try: http://www.dafont.com/bitmap.php
# font = ImageFont.truetype('Minecraftia.ttf', 8)

try:

    while True:

        # Draw a black filled box to clear the image.
        draw.rectangle((0, 0, width, height), outline=0, fill=0)

        temp = ""

        # Open a file for reading ("r" mode)
        with open('/sys/bus/i2c/devices/2-0048/hwmon/hwmon0/temp1_input', 'r', encoding='utf-8') as file:
            temp = file.read()

        temp = str(int(temp) / 1000)
        temp = temp + " C"

        draw.text((x, top),             "TEMP AMBIENTE ...",  font=font, fill=255)
        draw.text((x, top + 16),        temp,  font=font, fill=255)

        # Display image.
        disp.image(image)
        disp.display()
        time.sleep(.1)

except KeyboardInterrupt as e:
    logging.exception("Keyboard interruption.", exc_info=False)

finally:
    logging.info("Cleaning up the bus and Display!")
    disp.close()
