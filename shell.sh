if [ $# -lt  2 ] ; then
    echo "Erreur : deux arguments au moins sont nécessaires."
    exit 1
elif [ ! -f "$1" ] || [ "${1##*.}" != "csv" ] ; then
    echo "Erreur : premier argument n'est pas un fichier csv."
    exit 1
fi

for i in $* ; do
    if [ "$i" = "-h"] ; then
        echo "oui"
        exit 1
    fi
done