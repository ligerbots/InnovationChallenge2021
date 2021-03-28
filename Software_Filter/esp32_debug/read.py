import serial
import numpy as np

ser = serial.Serial("/dev/cu.usbserial-0001", 115200, timeout=1)
buf =  b""

with open("../desktop/data.txt", "w") as f:
    while True:
        buf+=ser.read(256)
        if(b"BUFFSTART" in buf and b"BUFFEND" in buf):
            *_, midend = buf.split(b"BUFFSTART")
            mid, *_ = midend.split(b"BUFFEND")
            data = np.frombuffer(mid,dtype=np.uint16)
            for x in data:
                f.write(str(x))
                f.write("\n")
            print("wrote" ,len(data))
            buf =  b""
