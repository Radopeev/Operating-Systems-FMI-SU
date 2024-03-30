 cat /etc/services | tr ' ' '\n' | tr '\t' '\n' | sort | uniq -c | head
