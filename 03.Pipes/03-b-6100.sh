 cat /etc/passwd | head -n 46 | tail -n 28 | cut -d ':' -f 3 | rev | cut -c 1
