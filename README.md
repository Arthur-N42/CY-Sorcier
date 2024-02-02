# CY-Sorcier
Programme de traitement de données

## Téléchargement
Pour télécharger CY-Sorcier, clonez le dépôt git dans le répertoire de votre choix :
```bash
git clone https://github.com/Arthur-N42/CY-Sorcier.git
```

Pour la suite du README, on supposera que ce dossier est le dossier courant.

## Utilisation

Executez le script `shell.sh` avec le fichier CSV en premier argument, 
puis les arguments pour les traitements souhaités (`-d1`, `-d2`, `-l`, `-t`, `-s`). 
L'option `-h` ou `--help` affiche une aide afin de comprendre les différentes options. Exemple d'exécution :

```bash
# Lance les traitements D1, L et T avec le fichier data.csv.
bash shell.sh data/data.csv -d2 -s -l
```

Suite aux traitements et à la génération des graphiques, ces derniers seront accessibles dans le dossier `images`, 
à l'intérieur du dossier images. Vous trouverez dans le dossier `temp` les résultats des différents traitements effectués.
