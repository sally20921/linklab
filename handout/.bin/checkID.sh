#!/bin/bash
#
# make sure StudentID and StudentName are set

# argument $1 must point to the ID file
if [ -z "$1" ]; then
  echo "Usage: $0 <ID file>"
  exit 1
fi

# check whether the ID file exists
if [ ! -f $1 ]; then
  echo "ID file not found ($1)"
  exit 1
fi

# source the ID file
source  $1

# check if 'StudentID' is set
if [ -z "$StudentID" ]; then
  echo "You need to set your student ID in '$1'."
  exit 1
fi

# check if 'StudentName' is set
if [ -z "$StudentName" ]; then
  echo "You need to set your name ID in '$1'."
  exit 1
fi

# all good
exit 0
