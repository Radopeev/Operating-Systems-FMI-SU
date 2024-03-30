find /usr -regex '.*\.sh' -exec head -n1 {} \; | grep '^#!' | sort | uniq -c
