cat /etc/passwd | cut -d ':' -f 1,5,6 | grep '.:.a: Inf' | cut -d ':' -f 1 | cut -c 3,4 | sort -n | uniq -c
