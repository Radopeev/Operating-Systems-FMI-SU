grep $(whoami) /etc/passwd
grep $(whoami) /etc/passwd -B 2 
grep $(whoami) /etc/passwd -B 2 -A 3
grep $(whoami) /etc/passwd -B 2 | head -n 1
