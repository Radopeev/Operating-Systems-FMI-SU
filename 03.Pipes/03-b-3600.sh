find / -maxdepth 3 -type d 2>temp.txt 1>/dev/null
cat temp.txt | cut -d ':' -f 2
