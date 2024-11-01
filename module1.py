import serial
import time
import serial.tools.list_ports

def toarduino(arduino, message):
    for num in message:
        res = bytes(num, 'utf-8')
        arduino.write(res)
        print(res)

if __name__ == '__main__':
    
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)

    ser = serial.Serial("COM1", 9600, timeout=1);
    ser.reset_input_buffer()

    for i in range(10):
        userinput = input("Words: ")
        toarduino(ser, userinput)

        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        time.sleep(0.1)
