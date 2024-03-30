cat /etc/services |tr ' ' '\n' | tr '\t' '\n' | grep -E '^[a-zA-Z]+$' |  sort | uniq -c | head
