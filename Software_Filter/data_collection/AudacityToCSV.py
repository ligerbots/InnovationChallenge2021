import csv

def convert(filepath):
    f = open(filepath, "r")
    Lines = f.readlines()

    # To automatically get the filename (given a filepath)
    filename = filepath
    filename = filename.split("\\")
    filename = filename[-1]
    filename = filename.split(".")
    filename = filename[0]

    # Write outputfile
    with open("data\\" + filename + ".csv", mode='w') as output_file: 
        output_writer = csv.writer(output_file, delimiter=",")

        for line in Lines:
            split = line.split("\t") # Audacity outputs using tab as a delimiter 
            freq = split[0]
            level = split[1]
            output_writer.writerow([freq, level])

    f.close()

# Driver Code:
# convert("Software_Filter\data_collection\data\standard_whistle.txt")

# Call this from CMD:
# python -c 'import AudacityToCSV; AudacityToCSV.convert("data\\standard_whistle.txt")'