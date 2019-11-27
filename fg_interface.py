# Jamison Bauer and Dezeray Kowalski
# 22 April 2019

# This code creates a flight gear object and runs the connection
# to Flight Gear to receive the aircraft data.

from FlightGear import FlightGear
import time
import serial

ser = serial.Serial(port='/dev/tty.usbserial-A906J5LC', baudrate=9600)

def main():

    fg = FlightGear('localhost', 5400)

    # Wait five seconds for simulator to settle down
    while True:
        if fg['/sim/time/elapsed-sec'] > 5.0:
            break
        time.sleep(5.0)
        print(fg['/sim/time/elapsed-sec'])

    # Switch to external view for for 'walk around'.
    # fg.view_next()
    while True:
        try:
            roll = fg['/orientation/roll-deg']
            pitch = fg['/orientation/pitch-deg']
            string = str(int(roll)) + "," + str(int(pitch)) + "\r\n"
            ser.write(string)
            print(string)# response = ser.readline()
        except Exception as e:
            print(str(e))

    fg.quit()
    ser.close()

if __name__ == '__main__':
    main()
