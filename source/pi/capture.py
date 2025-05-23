import serial
import sys
import threading
from collections import deque
from PyQt5 import QtWidgets
import pyqtgraph as pg

# --- Parameters ---
PORT = '/dev/ttyUSB0'
BAUDRATE = 1000000
MAX_LEN = 500  # Display window size

# --- Buffers ---
ch1 = deque([0]*MAX_LEN, maxlen=MAX_LEN)
ch2 = deque([0]*MAX_LEN, maxlen=MAX_LEN)
ch3 = deque([0]*MAX_LEN, maxlen=MAX_LEN)
ch4 = deque([0]*MAX_LEN, maxlen=MAX_LEN)

# --- Serial reader thread ---
def read_serial():
    ser = serial.Serial(PORT, BAUDRATE, timeout=1)
    while True:
        try:
            line = ser.readline().decode('ascii', errors='ignore').strip()
            parts = line.split()
            if len(parts) == 4 and all(p.lstrip('-').isdigit() for p in parts):
                c1, c2, c3, c4 = map(int, parts)
                ch1.append(c1)
                ch2.append(c2)
                ch3.append(c3)
                ch4.append(c4)
        except Exception as e:
            print(f"Error: {e}")

# --- GUI Setup ---
app = QtWidgets.QApplication(sys.argv)
win = pg.GraphicsLayoutWidget(title="Real-Time Serial Plot")
win.resize(1000, 600)
plots = []
curves = []

for i, title in enumerate(['Channel 1', 'Channel 2', 'Channel 3', 'Channel 4']):
    p = win.addPlot(title=title)
    p.showGrid(x=True, y=True)
    curve = p.plot()
    plots.append(p)
    curves.append(curve)
    if i < 3:
        win.nextRow()

win.show()

# --- Update timer ---
def update():
    x = list(range(MAX_LEN))
    for i, data in enumerate([ch1, ch2, ch3, ch4]):
        curves[i].setData(x, list(data))

timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(30)  # update every 30 ms

# --- Start serial thread ---
t = threading.Thread(target=read_serial, daemon=True)
t.start()

# --- Start GUI event loop ---
sys.exit(app.exec_())
