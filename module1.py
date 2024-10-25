import serial
import time

def toarduino(arduino, message):
    for num in message:
        res = bytes(num, 'utf-8')
        arduino.write(res)

if __name__ == '__main__':

    ser = serial.Serial("COM6", 9600, timeout=1);
    ser.reset_input_buffer()

    for i in range(10):
        userinput = input("Words: ")
        toarduino(ser, userinput)

        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        time.sleep(0.1)
