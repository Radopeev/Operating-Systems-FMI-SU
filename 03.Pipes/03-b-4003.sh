find . -type f -exec cat {} \; | grep -Eo '.' | sort | uniq -c
