find ~ -maxdepth 1 -type f -user $(whoami) -exec chmod 775 {} \;
