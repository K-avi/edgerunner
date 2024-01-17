#!/bin/bash 

if [[ ! -f /usr/include/ncurses.h ]]; then 
    echo "ncurses is not detected ; please install ncurses "
fi

make edgerunner || echo "compile error"

cp edgerunner /usr/local/bin 

manpath="/usr/local/share/man"

if [[ ! -d $manpath/man1 ]]; then 
    mkdir $manpath || echo "couldn't create the man directory for edgerunner in /usr/share/man"
fi

if [[ ! -d $manpath/man1 ]]; then 
    mkdir $manpath/man1 || echo "couldn't create the man1 directory for edgerunner in $manpath"
fi

make clean

cp manpages/man1/* $manpath/man1/