import serial

ser = serial.Serial('/dev/ttyUSB0', 1000000, timeout=1)

with open('data.txt', 'w') as f:
    while True:
        line = ser.readline().decode('ascii', errors='ignore').strip()
        parts = line.split()

        if len(parts) == 4 and all(p.isdigit() or (p[0] == '-' and p[1:].isdigit()) for p in parts):
            f.write(f"{parts[0]} {parts[1]} {parts[2]} {parts[3]}\n")
