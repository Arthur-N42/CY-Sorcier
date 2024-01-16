#!/bin/bash
d1=0
d2=0
l=0
t=0
s=0
if [ $# -lt  2 ] ; then
    echo "Erreur : deux arguments au moins sont nécessaires."
    exit 1
elif [ ! -f "$1" ] || [ "${1##*.}" != "csv" ]; then
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
            d2=1
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
    ini_Doss
    # NR > 1 ignore la ligne 1 
    # Utilise un dictionnaire et incremente le nombre de trajet pour un conducteur si la paire (ID - Conducteur) n'a pas ete "ajoutee"
    # Trie dans l'ordre decroissant et affiche les 10 premiers
    time mawk -F';' 'NR > 1 { if (!added[$1, $6]) { trips[$6]++ } added[$1, $6]++ } END { for (driver in trips) print driver, trips[driver] }' $1 | sort -k3 -nr | head -10 # ~4.3s
fi  

if [ $d2 = 1 ]; then
    
    time LC_NUMERIC="C" awk -F';' 'NR > 1 { trips[$6]+=$5 } END { for (driver in trips) print driver, trips[driver] }' $1 | sort -k3 -nr | head -10 # ~2.2s exec
fi

if [ $l = 1 ]; then
    #A tester
    time LC_NUMERIC="C" awk -F';' 'NR > 1 { trips[$1]+=$5 } END { for (distance in trips) print distance, trips[distance] }' $1 | sort -k2 -nr | head -10 # ~4.6s
    
fi

if [ $t = 1 ]; then
    gcc -o traitements main.c
    ./traitements -t
fi

if [ $s = 1 ]; then
    echo "cool"
    gcc -o traitements main.c
    ./traitements -s
fi