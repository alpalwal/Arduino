#!/usr/bin/env python
from time import sleep
import RPi.GPIO as GPIO
import board
import busio
import digitalio
import time

from adafruit_mcp230xx.mcp23017 import MCP23017
i2c = busio.I2C(board.SCL, board.SDA)
mcp = MCP23017(i2c)

pin1 = mcp.get_pin(1)
pin1.direction = digitalio.Direction.INPUT


while True:
    print(pin1.value)
    sleep(1)

# pin1 = mcp.get_pin(1)
# pin2 = mcp.get_pin(2)
# pin3 = mcp.get_pin(3)
# pin4 = mcp.get_pin(4)
 
# Setup pin0 as an output that's at a high logic level.
# pin0.switch_to_output(value=False)
# pin1.switch_to_output(value=False)
# pin2.switch_to_output(value=False)
# pin3.switch_to_output(value=False)
# pin4.switch_to_output(value=False)
 
# # Setup pin1 as an input with a pull-up resistor enabled.  Notice you can also
# # use properties to change this state.


# pin4.pull = digitalio.Pull.DOWN


