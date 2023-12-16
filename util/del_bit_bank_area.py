#!/usr/bin/python3

# A script that receives a file as argument, and deletes lines from ".area BIT_BANK" to the line before "--------------"
import sys
import re
import os
import shutil

# Get the file name from the command line argument
filename = sys.argv[1]
shutil.copy(filename,f'{filename}.bk')

# A flag to indicate if we are in the range of lines to be deleted
delete = False

# Open the file in read mode
with open(filename, "r") as f:
  # Read all the lines from the file
  lines = f.readlines()

# Open the file in write mode
with open(filename, "w") as f:
  # Iterate over the lines
  for line in lines:
# delete the line containing push bits or pop bits
    if re.match("\s*(push|pop)\s+bits",line):
      pass
    # Check if the line starts with ".area BIT_BANK"
   #elif line.startswith("\s*.area BIT_BANK"):
    elif re.match("\s*.area BIT_BANK",line):
      # Set the flag to True
      delete = True
    # Check if the line starts with "--------------"
    elif line.startswith(";--------------"):
      # Set the flag to False
      delete = False
      # Write the line to the file
      f.write(line)
    # If the flag is False, write the line to the file
    elif not delete:
      f.write(line)

