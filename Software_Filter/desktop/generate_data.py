from pydub import AudioSegment
from glob import glob
from pydub.utils import get_array_type
import array

sample_rate = 16000

with open("data.txt", "w") as f:

    for file in glob("data/A1-0001_Bare Whistle Sounds00000228.mp4"):
        # https://stackoverflow.com/a/32383120/5771000
        sound = AudioSegment.from_file(file)
        sound_resample = sound.set_frame_rate(sample_rate)
        print(file,"length:",sound_resample.duration_seconds,"seconds")

        bit_depth = sound_resample.sample_width * 8
        array_type = get_array_type(bit_depth)

        arr= array.array(array_type, sound_resample._data)
        for x in arr:
            f.write(str(x))
            f.write("\n")
