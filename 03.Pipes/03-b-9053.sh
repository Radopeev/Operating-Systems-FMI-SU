cat population.csv | grep '2016' | sort -k 4 -n -t ',' | tail -n 1 | cut -d ',' -f 1,2 | tr '\"' ' '
cat population.csv | grep '2016' | sort -k 4 -n -t ',' | head -n 1 | cut -d ',' -f 1
