# stty -f /dev/cu.usbserial-0001 115200 & cat /dev/cu.usbserial-0001 | tee spectrogram.txt

import matplotlib.pyplot as plt
import matplotlib
import numpy as np

f, axarr = plt.subplots(2,1, gridspec_kw={'height_ratios': [25, 1]})

arr = []

colors = []
i=0

pt_xs = []
pt_ys = []
pt_cs = []

error_reading = False
with open('spectrogram.txt', encoding="latin-1") as f:
    for line in f:
        data = line.split()
        if(data[0] == "L"):
            if(data[1] == "curr"):
                x = i
            else:
                x = float(data[1])

            y = float(data[2])

            c = float(data[3])

            pt_xs.append(x)
            pt_ys.append(y)
            pt_cs.append(c)

            if(len(data)>4):
                axarr[0].annotate(data[4], (x,y), size=4)
        else:
            try:
                *freqs, detected = data

                if(len(freqs)<1):
                    error_reading=True
                    continue

                arr.append([float(x) for x in freqs])
                colors.append([255,0,0] if detected=="y" else [0,0,0])
                i+=1
            except ValueError:
                 error_reading=True

if(len(arr[-1])!=len(arr[0])):
    arr.pop()
    error_reading=True

if(error_reading):
    print("warning: corrupted file")

colormap = plt.cm.bwr
normalize = matplotlib.colors.Normalize()
axarr[0].scatter(pt_xs, pt_ys, c=pt_cs, s=10, cmap=colormap, norm=normalize, marker='x', linewidth=1)

arr_np=np.array(arr)
colors_np=np.array(colors)

axarr[0].imshow(arr_np.T, origin='lower', interpolation='nearest', aspect='auto')
axarr[1].imshow(colors_np.reshape((1,-1,3)), origin='lower', interpolation='nearest', aspect='auto')
axarr[1].axis('off')

plt.show()
