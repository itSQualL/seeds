#!/usr/bin/env python

from time import sleep           # Allows us to call the sleep function to slow down our loop
import RPi.GPIO as GPIO           # Allows us to call our GPIO pins and names it just GPIO
import paho.mqtt.client as paho

GPIO.setmode(GPIO.BCM)           # Set's GPIO pins to BCM GPIO numbering
INPUT_PIN = 18          # Sets our input pin, in this example I'm connecting our button to pin 4. Pin 0 is the SDA pin so I avoid using it for sensors/buttons
GPIO.setup(INPUT_PIN, GPIO.IN)           # Set our input pin to be an input

broker = ""
client = paho.Client("sunflowex-engine")

# Start a loop that never ends
while True:
    client.connect(broker)
    if (GPIO.input(INPUT_PIN) == True): # Physically read the pin now
        client.publish("engine/status","1")
    else:
        client.publish("engine/status","0")

    client.disconnect()
    sleep(1);           # Sleep for a full second before restarting our loop
