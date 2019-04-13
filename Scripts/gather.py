from shutil import copy
from shutil import copyfile
import os
import sys

"""
Takes a file with contents of filepaths like:
----
driver_193_90frame/06051129_0637.MP4/03510.jpg
driver_193_90frame/06051220_0654.MP4/03420.jpg
driver_37_30frame/05181432_0203.MP4/00810.jpg 
driver_37_30frame/05181743_0267.MP4/01020.jpg 
driver_37_30frame/05190751_0363.MP4/02940.jpg 
-----
and copies them into the destination folder
"""

cwd = os.getcwd()  
print("CWD: ", cwd)
filename = 'test4_noline.txt' 
dest_folder = cwd + '/' + 'noline'
dest_fileno = 0

# Make destination
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
		print(line_num, ": ", line, sep="")
		dest_filename = dest_folder + '/' + str(dest_fileno) + '.jpg' 
		try:
			copy(line, dest_filename)
			print("Sucessfully copied line ", line_num)
		except IOError as e:
			print("Failed to copy file on line ", line_num)
			print("dest file: ", dest_filename, "\n")
			print(sys.exc_info()[0])
			print(e)
			print("\n")
		dest_fileno += 1

finally:
	fp.close()

print("Ending program")
