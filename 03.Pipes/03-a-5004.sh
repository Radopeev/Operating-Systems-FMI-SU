cat /etc/passwd | cut -d ':' -f 5 | cut -d ',' -f 1 | cut -d ' ' -f2 | grep -v -E '.{8,}$' | grep -E '[а-яА-я]' | sort | uniq | xargs -I{} grep -w {} /etc/passwd
