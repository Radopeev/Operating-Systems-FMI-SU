df -P > file
cat file | tail +2 | sort -k 2 -n
