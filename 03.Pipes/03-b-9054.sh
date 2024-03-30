cat population.csv | grep '1969' | sort -n -k 4 -t ',' -r | head -n 42 | tail -n 1 | cut -d ',' -f 1
