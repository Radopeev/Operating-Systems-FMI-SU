 cat planets.txt | sort -k 3 -t ';' | grep '$(cat planets.txt | sort -k 2 -t ';' | head -n | cut -d ';' 2' | head -n -1 | tr ';' ' ' | awk -F '{print $1 "\t" $4}'
