 cat /etc/group  | cut -d ':' -f 3 | awk -v user_group_id=$(id -g) '$1==user_group_id { print "Hello, " $1 " - I am here!"} $1!=user_group_id { print "Hello, " $1 }'
