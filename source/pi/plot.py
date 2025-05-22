import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

filename = 'data.txt'

ch1, ch2, ch3, ch4 = [], [], [], []

with open(filename, 'r') as f:
    for line in f:
        values = line.strip().split()
        if len(values) == 4:
            c1, c2, c3, c4 = map(int, values)
            ch1.append(c1)
            ch2.append(c2)
            ch3.append(c3)
            ch4.append(c4)

time = range(len(ch1))  # x-axis: sample index

plt.figure(figsize=(10, 8))

# Channel 1
plt.subplot(4, 1, 1)
plt.plot(time, ch1, color='b')
plt.title('Channel 1')
plt.ylabel('Amplitude')
plt.grid(True)

# Channel 2
plt.subplot(4, 1, 2)
plt.plot(time, ch2, color='g')
plt.title('Channel 2')
plt.ylabel('Amplitude')
plt.grid(True)

# Channel 3
plt.subplot(4, 1, 3)
plt.plot(time, ch3, color='r')
plt.title('Channel 3')
plt.ylabel('Amplitude')
plt.grid(True)

# Channel 4
plt.subplot(4, 1, 4)
plt.plot(time, ch4, color='m')
plt.title('Channel 4')
plt.xlabel('Sample Number')
plt.ylabel('Amplitude')
plt.grid(True)

plt.tight_layout()
plt.show()

