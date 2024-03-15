cut -d ':' -f 5 /etc/passwd | cut -d ' ' -f 2 | cut -d ',' -f 1 | grep -v -E '.{8,}$'
