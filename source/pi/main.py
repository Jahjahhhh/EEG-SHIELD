import serial
import struct

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

with open('data.txt', 'w') as f:
    while True:
        data = ser.read(16)  # 4 * 4 bytes = 16 bytes
        if len(data) == 16:
            chan1, chan2, chan3, chan4 = struct.unpack('>4i', data)  # big-endian 4 signed ints
            line = f"{chan1} {chan2} {chan3} {chan4}\n"
            print(line, end='')     # print to terminal
            f.write(line)           # write to file
