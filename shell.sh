#!/bin/bash

export PATH="C:\MinGW\bin:$PATH"

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
        echo "
Programme d'execution de tri des donnees d'un fichier csv

SYNOPSIS 
    bash shell.sh [FICHIER] [OPTIONS]

DESCRIPTION

    FICHIER doit etre un fichier csv valide

    -d1
        Les 10 premiers conducteurs en nombre de trajets par ordre decroissant

    -d2
        Les 10 plus grandes distances totales parcourues 

    -l
        Les 10 trajets les plus longs

    -t
        Les 10 villes les plus traversees A
        Affiche egalement le nombre de fois ou ces villes ont ete la ville de depart

    -s
        Les 50 trajets avec la plus grande difference distance max - min
        Affiche egalement la distance moyenne des trajets"
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

# Creation dossiers, nettoyage de temp

if [ -d "temp" ]; then
    rm -rf temp
fi
mkdir temp

if [ ! -d "images" ]; then
    mkdir images
fi


if [ $d1 = 1 ]; then
    # NR > 1 ignore la ligne 1 
    # Utilise un dictionnaire et incremente le nombre de trajet pour un conducteur si la paire (ID - Conducteur) n'a pas ete "ajoutee"
    # Trie dans l'ordre decroissant et affiche les 10 premiers
    time mawk -F';' 'NR > 1 { if (!added[$1, $6]) { trips[$6]++ } added[$1, $6]++ } END { for (driver in trips) print driver, ";" , trips[driver] }' $1 | sort -k2 -nr -t ";" | head -10 > temp/data_d1.txt # ~4.3s
    #creation graph
fi  

if [ $d2 = 1 ]; then
    time LC_NUMERIC="C" awk -F';' 'NR > 1 { trips[$6]+=$5 } END { for (driver in trips) print driver, ";", trips[driver] }' $1 | sort -k2 -nr -t ";" | head -10 > temp/data_d2.txt # ~2.2s exec
    #creation graph
fi

if [ $l = 1 ]; then
    time LC_NUMERIC="C" awk -F';' 'NR > 1 { trips[$1]+=$5 } END { for (distance in trips) print distance, ";" , trips[distance] }' $1 | sort -k2 -nr -t ";" | head -10 > temp/data_l.txt # ~4.6s
    #creation graph
fi

if [ $t = 1 ]; then
    gcc -o traitements.exe main.c
    time ./traitements.exe -t $1
fi

if [ $s = 1 ]; then
    gcc -o traitements.exe main.c
    time ./traitements.exe -s $1
fi

rm -rf temp