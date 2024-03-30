touch eternity
find ~ -type f -mmin -15 ! -name ".*" -printf "%p %A@\n" > eternity
