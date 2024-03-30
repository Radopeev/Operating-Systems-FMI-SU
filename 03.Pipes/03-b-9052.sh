cat population.csv | grep 'Bulgaria' | sort -n -k 4 -t ',' | tail -n 1 | awk -F ',' '{print $3}'
