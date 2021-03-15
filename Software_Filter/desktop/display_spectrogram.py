import matplotlib.pyplot as plt
import numpy as np
arr = []

colors = []
with open('spectrogram.txt') as f:
    for line in f:
        *freqs, detected = line.split()
        arr.append([float(x) for x in freqs])
        colors.append([255,0,0] if detected=="y" else [0,0,0])
arr_np=np.array(arr)
colors_np=np.array(colors)
f, axarr = plt.subplots(2,1, gridspec_kw={'height_ratios': [25, 1]})

axarr[0].imshow(arr_np.T, origin='lower', interpolation='nearest', aspect='auto')
axarr[1].imshow(colors_np.reshape((1,-1,3)), origin='lower', interpolation='nearest', aspect='auto')
axarr[1].axis('off')

plt.show()
