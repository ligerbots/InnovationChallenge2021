import matplotlib.pyplot as plt
import numpy as np
arr = []

with open('spectrogram.txt') as f:
    for line in f:
        arr.append([float(x) for x in line.split()])

arr_np=np.array(arr)
plt.imshow(arr_np.T, cmap='gray', origin='lower', interpolation='nearest', aspect='auto')
plt.show()
