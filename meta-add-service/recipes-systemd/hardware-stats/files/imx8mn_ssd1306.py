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

from __future__ import division
import logging
from smbus import SMBus

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')

# Constants
SSD1306_I2C_ADDRESS = 0x3C  # 011110+SA0+RW - 0x3C or 0x3D
SSD1306_SETCONTRAST = 0x81
SSD1306_DISPLAYALLON_RESUME = 0xA4
SSD1306_DISPLAYALLON = 0xA5
SSD1306_NORMALDISPLAY = 0xA6
SSD1306_INVERTDISPLAY = 0xA7
SSD1306_DISPLAYOFF = 0xAE
SSD1306_DISPLAYON = 0xAF
SSD1306_SETDISPLAYOFFSET = 0xD3
SSD1306_SETCOMPINS = 0xDA
SSD1306_SETVCOMDETECT = 0xDB
SSD1306_SETDISPLAYCLOCKDIV = 0xD5
SSD1306_SETPRECHARGE = 0xD9
SSD1306_SETMULTIPLEX = 0xA8
SSD1306_SETLOWCOLUMN = 0x00
SSD1306_SETHIGHCOLUMN = 0x10
SSD1306_SETSTARTLINE = 0x40
SSD1306_MEMORYMODE = 0x20
SSD1306_COLUMNADDR = 0x21
SSD1306_PAGEADDR = 0x22
SSD1306_COMSCANINC = 0xC0
SSD1306_COMSCANDEC = 0xC8
SSD1306_SEGREMAP = 0xA0
SSD1306_CHARGEPUMP = 0x8D
SSD1306_EXTERNALVCC = 0x1
SSD1306_SWITCHCAPVCC = 0x2

# Scrolling constants
SSD1306_ACTIVATE_SCROLL = 0x2F
SSD1306_DEACTIVATE_SCROLL = 0x2E
SSD1306_SET_VERTICAL_SCROLL_AREA = 0xA3
SSD1306_RIGHT_HORIZONTAL_SCROLL = 0x26
SSD1306_LEFT_HORIZONTAL_SCROLL = 0x27
SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29
SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A


class SSD1306_128_32(object):
    """
    Initializing oled Display settings and functions.
    """

    def __init__(self, i2c_bus, i2c_address=SSD1306_I2C_ADDRESS, vccstate=SSD1306_SWITCHCAPVCC):
        """Base parameters"""
        self._log = logging.getLogger('Diplay Oled SSD1306 implementation!')
        self.width = 128
        self.height = 32
        self._pages = self.height // 8
        self._buffer = [0] * (self.width * self._pages)
        # Save vcc state.
        self._vccstate = vccstate
        # Handle hardware I2C
        self._i2c_address = i2c_address
        self._i2c = SMBus(i2c_bus)
        logging.info(self._i2c)
        self._initialize()

    def _initialize(self):
        # 128x32 pixel specific initialization.
        self.command(SSD1306_DISPLAYOFF)  # 0xAE
        self.command(SSD1306_SETDISPLAYCLOCKDIV)  # 0xD5
        self.command(0x80)  # the suggested ratio 0x80
        self.command(SSD1306_SETMULTIPLEX)  # 0xA8
        self.command(0x1F)
        self.command(SSD1306_SETDISPLAYOFFSET)  # 0xD3
        self.command(0x0)  # no offset
        self.command(SSD1306_SETSTARTLINE | 0x0)  # line #0
        self.command(SSD1306_CHARGEPUMP)  # 0x8D
        if self._vccstate == SSD1306_EXTERNALVCC:
            self.command(0x10)
        else:
            self.command(0x14)
        self.command(SSD1306_MEMORYMODE)  # 0x20
        self.command(0x00)  # 0x0 act like ks0108
        self.command(SSD1306_SEGREMAP | 0x1)
        self.command(SSD1306_COMSCANDEC)
        self.command(SSD1306_SETCOMPINS)  # 0xDA
        self.command(0x02)
        self.command(SSD1306_SETCONTRAST)  # 0x81
        self.command(0x8F)
        self.command(SSD1306_SETPRECHARGE)  # 0xd9
        if self._vccstate == SSD1306_EXTERNALVCC:
            self.command(0x22)
        else:
            self.command(0xF1)
        self.command(SSD1306_SETVCOMDETECT)  # 0xDB
        self.command(0x40)
        self.command(SSD1306_DISPLAYALLON_RESUME)  # 0xA4
        self.command(SSD1306_NORMALDISPLAY)  # 0xA6

    def close(self):
        """Turn off the Display and freed the bus."""
        self.command(SSD1306_DISPLAYOFF)  # 0xAE
        self._i2c.close()

    def command(self, c):
        """Send command byte to display."""
        # I2C write.
        control = 0x00  # Co = 0, DC = 0
        self._i2c.write_byte_data(self._i2c_address, control, c)

    def data(self, c):
        """Send byte of data to display."""
        # I2C write.
        control = 0x40  # Co = 0, DC = 0
        self._i2c.write_byte_data(self._i2c_address, control, c)

    def begin(self):
        """Initialize display."""
        # Turn on the display.
        self.command(SSD1306_DISPLAYON)

    def display(self):
        """Write display buffer to physical display."""
        self.command(SSD1306_COLUMNADDR)
        self.command(0)  # Column start address. (0 = reset)
        self.command(self.width - 1)  # Column end address.
        self.command(SSD1306_PAGEADDR)
        self.command(0)  # Page start address. (0 = reset)
        self.command(self._pages - 1)  # Page end address.
        # Write buffer data.
        for i in range(0, len(self._buffer), 16):
            control = 0x40  # Co = 0, DC = 0
            self._i2c.write_i2c_block_data(self._i2c_address, control, self._buffer[i:i + 16])

    def image(self, image):
        """Set buffer to value of Python Imaging Library image. The image should
        be in 1 bit mode and a size equal to the display size.
        """
        if image.mode != '1':
            raise ValueError('Image must be in mode 1.')
        width, height = image.size
        if width != self.width or height != self.height:
            raise ValueError('Image must be same dimensions as display ({0}x{1})'.format(self.width, self.height))
        # Grab all the pixels from the image, faster than get pixel.
        pix = image.load()
        # Iterate through the memory pages
        index = 0
        for page in range(self._pages):
            # Iterate through all x-axis columns.
            for x in range(self.width):
                # Set the bits for the column of pixels at the current position.
                bits = 0
                # Don't use range here as it's a bit slow
                for bit in [0, 1, 2, 3, 4, 5, 6, 7]:
                    bits = bits << 1
                    bits |= 0 if pix[(x, page * 8 + 7 - bit)] == 0 else 1
                # Update buffer byte and increment to next byte.
                self._buffer[index] = bits
                index += 1

    def clear(self):
        """Clear contents of image buffer."""
        self._buffer = [0] * (self.width * self._pages)

    def set_contrast(self, contrast):
        """Sets the contrast of the display.  Contrast should be a value between
        0 and 255."""
        if contrast < 0 or contrast > 255:
            raise ValueError('Contrast must be a value from 0 to 255 (inclusive).')
        self.command(SSD1306_SETCONTRAST)
        self.command(contrast)

    def dim(self, dim):
        """Adjusts contrast to dim the display if dim is True, otherwise sets the
        contrast to normal brightness if dim is False.
        """
        # Assume dim display.
        contrast = 0
        # Adjust contrast based on VCC if not dimming.
        if not dim:
            if self._vccstate == SSD1306_EXTERNALVCC:
                contrast = 0x9F
            else:
                contrast = 0xCF
            self.set_contrast(contrast)
