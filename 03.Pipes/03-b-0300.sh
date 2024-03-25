cat /etc/passwd | grep $(whoami) | cut -d ':' -f 4

