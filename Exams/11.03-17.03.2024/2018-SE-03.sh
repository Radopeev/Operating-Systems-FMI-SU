 cat /etc/passwd | cut -c 2- | cut -d ':' -f 1,5,6 | sort -k 1 -n | cut -d ':' -f 2,3 | tr -d ','
