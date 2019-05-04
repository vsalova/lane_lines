from shutil import copy
from shutil import copyfile
import os
import sys

"""
Takes a file with contents of filepaths like:
----
driver_193_90frame/06051129_0637.MP4/03510.jpg
driver_193_90frame/06051129_0637.MP4/03510.lines.txt
driver_193_90frame/06051220_0654.MP4/03420.jpg
driver_193_90frame/06051220_0654.MP4/03420.lines.txt
driver_37_30frame/05181432_0203.MP4/00810.jpg 
driver_37_30frame/05181432_0203.MP4/00810.lines.txt
-----
and copies them into the destination folder

ONLY SUPPORTS FILES WITH EXACTLY ONE '.' SEPARATOR

usage: $python gather.py <files>
"""
# Credit: https://stackoverflow.com/questions/287871/print-in-terminal-with-colors
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


cwd = os.getcwd()  
print("CWD: ", cwd)
print("Number input files=", len(sys.argv)-1)
for filename in sys.argv[1:]:
    filename_no_ext = filename.strip().split('.')[0]
    filename_ext = filename.strip().split('.')[1]
    dest_folder = cwd + '/' + filename_no_ext
    dest_fileno = 0

    # Make destination folder
    if (os.path.exists(dest_folder)):
        print("Destination folder already exists")
    else:
        try:
            os.mkdir(dest_folder)
        except OSError:
            print("Could not create destination folder: ", dest_folder)
            exit()
        else:
            print("Sucessfully made destination folder: ", dest_folder)

    # Copy files
    try:
        fp = open(filename)
        print("Sucessfully opened input file ", filename)
        lines = fp.read().splitlines()
        for line_num, line in enumerate(lines):
            line = cwd + '/' + line
            line = line.strip()
            file_to_copy = line.split('/')[-1]
            whole_ext = file_to_copy[file_to_copy.find('.'):]   # extention with leading .
            print(line_num, ": ", line, "\t", file_to_copy, sep="")
            dest_filename = dest_folder + '/' + str(dest_fileno) + whole_ext    # number the output files
            # Check if dest file already exsits
            dest_exists = os.path.isfile(dest_filename)
            if dest_exists:
                print(bcolors.WARNING, "FILE ALREADY EXISTS: ", file_to_copy, bcolors.FAIL, "\tNO COPY DONE", bcolors.ENDC)
                continue
            try:
                copy(line, dest_filename)
                print(bcolors.OKGREEN, "Sucessfully", bcolors.ENDC, " copied line ", line_num)
            except IOError as e:
                print("Failed to copy file on line ", line_num)
                print("dest file: ", dest_filename, "\n")
                print(sys.exc_info()[0])
                print(e)
                print("\n")
            if whole_ext == ".lines.txt":
                dest_fileno += 1

    finally:
        fp.close()

print("Ending program")
