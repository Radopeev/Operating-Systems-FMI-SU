cat emp.data | awk 'END {print NR}'

cat emp.data | awk '{if (NR==3) {print $0}}'

cat emp.data | awk '{print $NF}'

cat emp.data | awk ' END{print $NF}'

cat emp.data | awk '{if(NF>4) {print $0}}'

cat emp.data | awk '{if($NF>4) {print}}'

cat emp.data | awk '{sum+=NF} END{print sum}'

cat emp.data | awk '/.*Beth.*/{sum+=1} END{print sum}'

cat emp.data | awk '{if($3>max) {max=$3; line=$0}} END{print max;print line}'

cat emp.data | awk 'NF>2 {print}'

cat emp.data | awk '/^.{18,}$/ {print $0}'

cat emp.data | awk '{print NF;print $0}'

cat emp.data | awk '{print $2,$1}'

cat emp.data | awk '{{swap=$1;$1=$2;$2=swap;} print $0}'

cat emp.data | awk '{{$1=NR} print $0}'

cat emp.data | awk '{{$2=""} print $0}'

cat emp.data | awk '{ print $2+$3}'

cat emp.data | awk '{sum+=$2+$3} END{print sum}'
