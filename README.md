# CY-Sorcier
Programme de traitement de données

## Prérequis

• Compilateur gcc
• Gnuplot
• Bash
• Linux

## Téléchargement
Pour télécharger CY-Sorcier, clonez le dépôt git dans le répertoire de votre choix :
```bash
git clone https://github.com/Arthur-N42/CY-Sorcier.git
```

Pour la suite du README, on supposera que ce dossier est le dossier courant.

## Utilisation

Executez le script `shell.sh` avec le chemin vers le fichier CSV en premier argument, 
puis les arguments pour les traitements souhaités (`-d1`, `-d2`, `-l`, `-t`, `-s`).
L'option `-h` ou `--help` affiche une aide afin de comprendre les différentes options. Exemple d'exécution :

```bash
# Lance les traitements D1, L et T avec le fichier data.csv.
bash shell.sh data/data.csv -d2 -s -l
```

Suite aux traitements et à la génération des graphiques, ces derniers seront accessibles dans le dossier `images`, 
à l'intérieur du dossier images. Vous trouverez dans le dossier `temp` les résultats sous forme de fichiers .txt des différents traitements effectués.
Tous les résultats des traitements sont aussi prévisualisables dans le dossier `demo`.

## Compilation manuelle des traitements en C

Il est possible de compiler manuellement la partie C du programme via le Makefile dans le sous-répertoire `progc`.
Se déplacer dedans, et effectuer la commande :
```bash
make
```

Pour supprimer tous les éxecutables :
```bash
make clean
```
