 cat population.csv | grep '2008' | cut -d ',' -f 4 | awk '{sum+=$1} END{ print sum } '
 cat population.csv | grep '2016' | cut -d ',' -f 4 | awk '{sum+=$1} END{ print sum } '
