import serial
from tkinter import *
import tkinter as tk
import time

commPort = "COM15"
ser = serial.Serial(commPort, baudrate= 115200, timeout= 1)

def ledON():
    if blinkState.get() == 1:
        blinkLED()
    else:
        ser.write(b'o')

def blinkLED():
    if blinkState.get() == 1:
        ser.write(b'b')
        time.sleep(1)
        delay = userDelay.get()
        ser.write(delay.encode())

def ledOFF():
    ser.write(b'x')

root = Tk()
root.title("Arduino GUI Test")
root.geometry("350x350")

blinkState = IntVar()
userDelay = StringVar()
userDelay.set('1000')
blinkTime = ['50', '100', '200', '400', '600', '800', '1000', '1200']

btn_On = tk.Button(root, text="LED ON", command=ledON)
btn_On.grid(row=0, column=0, padx=5, pady=5)

btn_Off = tk.Button(root, text="LED OFF", command=ledOFF)
btn_Off.grid(row=0, column=1, padx=5, pady=5)

chk_Blink = tk.Checkbutton(root, text="Blink", variable=blinkState, command=blinkLED)
chk_Blink.grid(row=0, column=2, padx=5, pady=5)

delayLabel = tk.Label(root, text="Blink(ms)")
delayLabel.grid(row=1, column=0, padx=5, pady=5)

delayMenu = tk.OptionMenu(root, userDelay, *blinkTime)
delayMenu.grid(row=1, column=1, padx=5, pady=5)

root.mainloop()