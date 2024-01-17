#!/bin/bash 

if [[ -f /usr/local/bin/edgerunner  ]]; then 
   rm /usr/local/bin/edgerunner 
fi

if [[ -e /usr/local/share/man/man1/edgerunner.1  ]]; then 
   rm -rf "/usr/local/share/man/man1/edgerunner.1"
fi
