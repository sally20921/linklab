#!/bin/bash
#
# submit solution

SUBMIT_SVR="http://csap.snu.ac.kr/"
SUBMIT_URL="${SUBMIT_SVR}~bernhard/teaching/M1522.000800/16FS/linklab/submit.php"
SUBDIR=".submissions/"

# relative path of this script
DIR="`dirname \"$0\"`"

# make sure the necessary information is set
$DIR/checkID.sh STUDENT.ID
if [ $? -ne 0 ]; then exit 1; fi

# filename: <student-id>.<time>.tgz
source STUDENT.ID
FILENAME="${StudentID}.`date +"%H%M%S"`.tgz"

# create tarball
echo ""
echo "Preparing submission ($FILENAME)..."
mkdir -p $SUBDIR
tar czf $SUBDIR$FILENAME --exclude-backups *

# submit tarball
echo ""
echo "Submitting to ${SUBMIT_SVR}..."
exec 3>&1
HTTP_STATUS=$(curl -s -w "%{http_code}" -o >(cat >&3) -F "action=submit" -F "sid=${StudentID}" -F "submission=@$SUBDIR$FILENAME" ${SUBMIT_URL})

# print result
echo ""
echo ""
case $HTTP_STATUS in
  200) echo "Submission successful." ;;
  400) echo "Submission failed: Invalid student ID." ;;
  405) echo "Submission failed: Unknown request." ;;
  500) echo "Submission failed: Internal server error." ;;
  *)   echo "Submission failed (HTTP error code: $HTTP_STATUS)" ;;
esac
echo ""
echo ""
