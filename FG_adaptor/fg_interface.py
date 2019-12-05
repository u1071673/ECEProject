# Jamison Bauer and Dezeray Kowalski
# 22 April 2019

# This code creates a flight gear object and runs the connection
# to Flight Gear to receive the aircraft data.

from FlightGear import FlightGear
import time
import serial

ser = serial.Serial(port='/dev/cu.usbserial-AH06F9RP', baudrate=115200)

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

            roll = roll + 90
            pitch = pitch + 90

            # Reserve start 0xFF for start byte
            r_num = int(roll + 1 if roll == 0xFF else roll)
            p_num = int(pitch + 1 if pitch == 0xFF else pitch)

            p_num = 180 - p_num  # Invert Pitch

            r_lsb = chr(int(r_num & 0xFF))
            r_msb = chr(int(r_num >> 8))
            p_lsb = chr(int(p_num & 0xFF))
            p_msb = chr(int(p_num >> 8))

            ser.write(chr(0xFF))
            ser.write(r_lsb)
            ser.write(r_msb)
            ser.write(p_lsb)
            ser.write(p_msb)

            print(ord(r_lsb), ord(r_msb), ord(p_lsb), ord(p_msb))

            print(ser.readline())
        except Exception as e:
            print(str(e))

    fg.quit()
    ser.close()

if __name__ == '__main__':
    main()
