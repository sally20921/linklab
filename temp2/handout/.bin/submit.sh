#!/bin/bash
#
# submit solution

# filename: <student-id>.<time>.tgz
source STUDENT.ID
FILENAME="${StudentID}.`date +"%H%M%S"`.tgz"

# create tarball
echo ""
echo "Preparing submission ($FILENAME)..."
tar czf $FILENAME --exclude-backups *

