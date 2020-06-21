#!/usr/bin/env python
from time import sleep
import RPi.GPIO as GPIO
import os
import threading 

GPIO.setmode(GPIO.BCM)

GPIO_output_pins = [17, 16, 13, 12, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 6, 5, 10, 9, 11, 8]
for pin in GPIO_output_pins:
    GPIO.setup(pin,GPIO.OUT)
    GPIO.output(pin, False) 

# Servo pin definitions
cracker_pusher   = servo_a = {"IN1":17,"IN2":16,"IN3":13,"IN4": 12}
marshmallow_cart = servo_b = {"IN1":18,"IN2":19,"IN3":20,"IN4":21}
rotisserie       = servo_c = {"IN1":22,"IN2":23,"IN3":24,"IN4":25}
cracker_cart     = servo_d = {"IN1":26,"IN2":27,"IN3":6,"IN4":5}
hopper           = servo_e = {"IN1":10,"IN2":9,"IN3":11,"IN4":8}

GPIO_input_pins = [4, 15]

for pin in GPIO_input_pins:
    GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) 

## Configure buttons
marshmallow_end = button_a = 4
cracker_cart_end = button_b = 15 # RX1 == 15 
# TX0 == 14

# Waiting time regulates the speed of how fast the motor turns
time = 0.001

'''
The 28BYJ-48 stepper motor is designed so that the motor in the
Inside 8 steps needed for one turn. By the companies
But it takes 512 x 8 steps so that the axis turns around once
rotates 360 degrees  itself.
Definition of steps 1 - 8 via pins IN1 to IN4
There is a short wait between each movement of the motor
Motor armature reaches its position
'''
def Step1(servo):
    GPIO.output(servo["IN4"], True)
    sleep (time)
    GPIO.output(servo["IN4"], False)

def Step2(servo):
    GPIO.output(servo["IN4"], True)
    GPIO.output(servo["IN3"], True)
    sleep (time)
    GPIO.output(servo["IN4"], False)
    GPIO.output(servo["IN3"], False)

def Step3(servo):
    GPIO.output(servo["IN3"], True)
    sleep (time)
    GPIO.output(servo["IN3"], False)

def Step4(servo):
    GPIO.output(servo["IN2"], True)
    GPIO.output(servo["IN3"], True)
    sleep (time)
    GPIO.output(servo["IN2"], False)
    GPIO.output(servo["IN3"], False)

def Step5(servo):
    GPIO.output(servo["IN2"], True)
    sleep (time)
    GPIO.output(servo["IN2"], False)

def Step6(servo):
    GPIO.output(servo["IN1"], True)
    GPIO.output(servo["IN2"], True)
    sleep (time)
    GPIO.output(servo["IN1"], False)
    GPIO.output(servo["IN2"], False)

def Step7(servo):
    GPIO.output(servo["IN1"], True)
    sleep (time)
    GPIO.output(servo["IN1"], False)

def Step8(servo):
    GPIO.output(servo["IN4"], True)
    GPIO.output(servo["IN1"], True)
    sleep (time)
    GPIO.output(servo["IN4"], False)
    GPIO.output(servo["IN1"], False)

# Turn Left
def turn_left(step,servo):	
	for i in range (step):   
		Step1(servo)
		Step2(servo)
		Step3(servo)
		Step4(servo)
		Step5(servo)
		Step6(servo)
		Step7(servo)
		Step8(servo)  
		print "Stepping left: ",i

# Turn Right		
def turn_right(step,servo):
	for i in range (step):
		Step8(servo)
		Step7(servo)
		Step6(servo)
		Step5(servo)
		Step4(servo)
		Step3(servo)
		Step2(servo)
		Step1(servo)  
		print "Stepping right: ",i	

def home_marshmallow_cart():
    while True: # Run forever
        turn_left(1,marshmallow_cart) 
        if GPIO.input(marshmallow_end) == 1:
            break

def home_cracker_cart():
    while True: # Run forever
        turn_left(1,cracker_cart) 
        if GPIO.input(cracker_cart_end) == 1:
            break



# Setup - move carts to 0
home_marshmallow_cart()
# home_cracker_cart() 

# # Move the hopper down
# turn_right(110,hopper) # Move hopper down

# # Move the cart forward/back to get the marshmallow
# turn_right(300,marshmallow_cart) # move cart forwards

# home_marshmallow_cart()# move cart backwards until button

# # Move the hopper back up
# turn_left(110,hopper) # Move hopper up

# # Move the cart all the way to the burner
# turn_right(2430,marshmallow_cart) # move cart forwards

# # Push crackers out of hopper
# turn_right(350,cracker_pusher) # move cart forwards
# sleep(1)
# turn_left(350,cracker_pusher) # move cart backwards

# # Move crackers to heater
# turn_right(600,cracker_cart) # move cart forwards

# # Move the cart back into staging for the crackers

# turn_left(900,marshmallow_cart) # move cart backwards

# # Move the crackers all the way to the front
# turn_right(700,cracker_cart) # move cart forwards

# # Move the hopper back to remove the marshmallow and back to 0
# home_marshmallow_cart()# move cart backwards until button

# # Move cracker cart back to 0
# home_cracker_cart() # move cart backwards

# # Repeat




GPIO.cleanup()
