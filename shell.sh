#!/bin/bash

d1=0
d2=0
l=0
t=0
s=0

if [ $# -lt  2 ] ; then
    echo "Erreur : deux arguments au moins sont nécessaires."
    exit 1
elif [ ! -f "$1" ] || [ "${1##*.}" != "csv" ] ; then
    echo "Erreur : Le premier argument n'est pas un fichier csv."
    exit 1
fi

# Erreur trouvee : il manquait un espace "-h"ici]

for i in $* ; do
    if [ "$i" = "-h" ] || [ "$i" = "--help" ]; then
        echo "HELP ME"
        exit 1
    fi
done

for (( i=2 ; i<=$# ; i++ )) ; do
    case ${!i} in
        "-d1")
            d1=1
            ;;
        "-d2")
            d2=0
            ;;
        "-l")
            l=1
            ;;
        "-t")
            t=1
            ;;
        "-s")
            s=1
            ;;
        *)
            echo "${!i} n'est pas un argument valide"
            ;;
    esac
done

ini_Doss() {
    if [ -d "temp" ]; then
        rm -rf temp
    fi
    mkdir temp
    if [ ! -d "images" ]; then
        mkdir images
    fi
}

if [ $d1 = 1 ]; then
    echo "cool"
    ini_Doss
fi

if [ $d2 = 1 ]; then
    echo "cool"
fi

if [ $l = 1 ]; then
    echo "cool"
fi

if [ $t = 1 ]; then
    echo "cool"
fi

if [ $s = 1 ]; then
    echo "cool"
fi