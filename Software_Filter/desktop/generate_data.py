from pydub import AudioSegment
from pathlib import Path
from pydub.utils import get_array_type
import array
import argparse
import math

parser = argparse.ArgumentParser(
    description="Convert sound file to list of samples")
parser.add_argument("-s", "--sample_rate", type=int, default=16000,
                    help="sample rate")
parser.add_argument("-l", "--max_length", type=float,
                    help="max length")
parser.add_argument("-n", "--noise", type=float, default=0,
                    help="noise (0 = no noise, 1 = equal sound and noise, >1 = more noise)")
parser.add_argument("-f", "--noise_file", type=str, default="data/other/backgroundnoise.mp3",
                    help="noise file")
parser.add_argument("-o", "--output_file", type=str, default="data.txt",
                    help="output file")
parser.add_argument("-os", "--output_sound_file", type=str,
                    help="output sound file")
parser.add_argument("input_files", default=[
                    "data/*"], nargs="*", help="input files")

args = parser.parse_args()
print("sample_rate =", args.sample_rate, "max_length =", args.max_length, "noise =",
      args.noise, "noise_file =", args.noise_file, "output_file =", args.output_file, "output_sound_file =",args.output_sound_file)


sample_rate = args.sample_rate // 2  # for some reason it needs to be halved

if(args.noise > 0):
    db_diff = math.log(args.noise)
    noise_path = Path(args.noise_file)
    noise = AudioSegment.from_file(noise_path)
else:
    noise_path = None
    noise = None
files = set()
for globs in args.input_files:
    for file in Path.cwd().glob(globs):
        if(noise is not None and file.samefile(noise_path)):
            continue
        files.add(file)

alloc_left = args.max_length or 1000000000

if(args.output_sound_file):
    output_sound = AudioSegment.silent(duration=0)
else:
    output_sound = None
with open(args.output_file, "w") as f:
    for file in files:
        # https://stackoverflow.com/a/32383120/5771000
        sound = AudioSegment.from_file(file)[:int(alloc_left * 1000)]
        alloc_left -= sound.duration_seconds
        if noise is not None:
            sound_with_noise = sound.overlay(noise, loop=True)
        else:
            sound_with_noise = sound
        sound_resample = sound_with_noise.set_frame_rate(sample_rate)

        print(file, "length:", sound_resample.duration_seconds, "seconds")

        bit_depth = sound_resample.sample_width * 8
        array_type = get_array_type(bit_depth)

        arr = array.array(array_type, sound_resample._data)

        for x in arr:
            f.write(str(x))
            f.write("\n")

        if output_sound is not None:
            output_sound += sound_resample
        if alloc_left <= 0.01:
            break
if output_sound is not None:
    output_sound.export(args.output_sound_file, format="wav")
