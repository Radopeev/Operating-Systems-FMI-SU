cat /etc/passwd | cut -d ':' -f 4 | sort | uniq -c | sort -k 1 -nr | head -n 5
