# OLED SSD1306 Linux driver
This is plain c code using linux I2C device node (/dev/i2c-X X for 0,1,2,3...).
Please make sure the linux has run "modprobe i2c-dev".
128x32 only supported (but just call init with other setting to change) and using I2C interface ONLY
## Files
**linux_i2c.c** source code for i2c control in linux device node.
**linux_i2c.h** header file for i2c control in linux.
**ssd1306.c** i2c protocol functions to control oled SSD1306.
**ssd1306.h** header file for i2c protocol functions and defined commands in SSD1306.
**font.h** font header file define 5x7 small font and 8x8 normal font. ascii value from 0x20(SPACE) to 0x7e(~).
**main.c** main file to take params and control oled SSD1306.
**Makefile** plain Makefile to build the source code. It works in raspberry pi.
**Readme.md** this readme file.
## How to compile
Require make and gcc. If you use cross compile, please self define $(CC).
Type "make" to build binary "ssd1306".
Type "make clean" to clean the project.
## How to use
- the software will init oled module ONCE when power up.
- set the device node address EVERYTIME if not using default value /dev/i2c-2
- the init oled module always reset XY cursor to (0,0)
- all params can set together
- clear screen if new text need to write, otherwise wording overlapping happened
- resolution value stored in /tmp/.ssd1306_oled_type with format like "128x32"
- always do display rotation first and then filling text. otherwise the text cause mirror
- make sure the XY cursor setting in correct location before printing text
### Params
```sh    
-h  Help message.
-c  Clear (line number or all).
-d  Display OFF [0] or ON [1].
-f  Small font 5x7 [0] or Normal font 8x8 [1].
-i  Display Normal [0] or Invert [1].
-r  Display rotation Normal [0] or Rotate [180].
-l  Print your line to display.
-m  Print your strings to display.
-n  I2C device node address (0,1,2..., default 2).
-x  X position.
-y  Y position.

```
## Example
### init the OLED
```
- The software will Init the Oled when you run it for the first time and save 
the file "/tmp/.ssd1306_oled_type" with the related settings.
- The resolution it is gonna trigger is 128x32.
```
### clear display
- clear 1st line
```sh
./ssd1306 -c0
```
- clear 2nd line
```sh
$ ./ssd1306 -c1
```
- clear 4th line
```sh
$ ./ssd1306_bin -c3
```
- clear whole screen
```sh
$ ./ssd1306_bin -c
```
### display on/off
- turn off display
```sh
$ ./ssd1306_bin -d 0
```
- turn on display
```sh
$ ./ssd1306_bin -d 1
```
### inverting display
- normal oled (0 is off, 1 is on)
```sh
$ ./ssd1306_bin -i 0
```
- invert oled (0 is on, 1 is off)
```sh
$ ./ssd1306_bin -i 1
```
### print words
- write line "Hello World"
```sh
$ ./ssd1306_bin -l "Hello World"
```
- write message "alpha\nbravo\ncharlie\ndelta" (please place \n for next line)
```sh
$ ./ssd1306_bin -m "alpha\nbravo\ncharlie\ndelta"
```
### I2C device address (default is /dev/i2c-2)
- using /dev/i2c-1
```sh
$ ./ssd1306_bin -n 1
```
### rotate display
- normal orientation
```sh
$ ./ssd1306_bin -r 0
```
- turn 180 orientation
```sh
$ ./ssd1306_bin -r 180
```
### set cursor location
- set XY cursor 8,1(x is column, 8 columns skipping, y is row, 2nd line)
```sh
$ ./ssd1306_bin -x 8 -y 1
```
