import serial
import sys
import threading
from collections import deque
from PyQt5 import QtWidgets
import pyqtgraph as pg
import numpy as np

# --- Parameters ---
PORT = '/dev/ttyUSB0'
BAUDRATE = 1000000
SAMPLE_RATE = 1000  # Hz
MAX_LEN = 500       # Display window size
FFT_SIZE = 1024     # Must be power of 2

# --- Buffers ---
ch1 = deque([0]*MAX_LEN, maxlen=MAX_LEN)
ch2 = deque([0]*MAX_LEN, maxlen=MAX_LEN)
ch3 = deque([0]*MAX_LEN, maxlen=MAX_LEN)
ch4 = deque([0]*MAX_LEN, maxlen=MAX_LEN)

fft_buf = deque([0]*FFT_SIZE, maxlen=FFT_SIZE)  # for FFT

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
                fft_buf.append(c1)  # Only using ch1 for FFT
        except Exception as e:
            print(f"Error: {e}")

# --- GUI Setup ---
app = QtWidgets.QApplication(sys.argv)
win = pg.GraphicsLayoutWidget(title="Real-Time Serial Plot + FFT")
win.resize(1000, 800)
plots = []
curves = []

# --- Time domain plots ---
for i, title in enumerate(['Channel 1', 'Channel 2', 'Channel 3', 'Channel 4']):
    p = win.addPlot(title=title)
    p.showGrid(x=True, y=True)
    curve = p.plot()
    plots.append(p)
    curves.append(curve)
    if i < 3:
        win.nextRow()

# --- FFT plot ---
win.nextRow()
fft_plot = win.addPlot(title="FFT (Channel 1)")
fft_plot.showGrid(x=True, y=True)
fft_curve = fft_plot.plot(pen='y')

# --- Time-domain plot update ---
def update():
    x = list(range(MAX_LEN))
    for i, data in enumerate([ch1, ch2, ch3, ch4]):
        curves[i].setData(x, list(data))

# --- FFT update ---
def update_fft():
    if len(fft_buf) >= FFT_SIZE:
        y = np.array(fft_buf)
        y = y - np.mean(y)  # remove DC
        window = np.hanning(len(y))
        y_win = y * window
        fft = np.abs(np.fft.rfft(y_win))
        freqs = np.fft.rfftfreq(len(y), d=1.0 / SAMPLE_RATE)

        fft_curve.setData(freqs, fft)

        # Print peak frequency
        peak_freq = freqs[np.argmax(fft)]
        print(f"Peak Frequency: {peak_freq:.2f} Hz")

# --- Start timers ---
timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(30)  # GUI update rate

fft_timer = pg.QtCore.QTimer()
fft_timer.timeout.connect(update_fft)
fft_timer.start(500)  # FFT every 0.5 s

# --- Start serial thread ---
t = threading.Thread(target=read_serial, daemon=True)
t.start()

# --- Enable Ctrl+C to work ---
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

# --- Start GUI event loop ---
sys.exit(app.exec_())
