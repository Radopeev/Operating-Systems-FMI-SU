find /usr/include | grep -E '^.*\.h$|^.*\.c$'q | xargs -I {} cat {}  | wc -l
find /usr/include | grep -E '^.*\.h$|^.*\.c$'q | wc -l
