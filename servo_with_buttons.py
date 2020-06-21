#!/usr/bin/env python
from time import sleep
import RPi.GPIO as GPIO
import os
import threading 

GPIO.setmode(GPIO.BCM)

def set_up_button(pin):
    GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) # Se

## Configure buttons
marshmallow_end = button_a = 7

set_up_button(button_a)
# set_up_botton(button_b)

while True: # Run forever
    turn_left(1,marshmallow_cart) # move cart backwards
    if GPIO.input(marshmallow_end) == 1:
        print("HIGH")
        break




GPIO.cleanup()
