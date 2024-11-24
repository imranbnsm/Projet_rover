# Projet Rover

Lien du github : https://github.com/imranbnsm/Projet_rover

## Description

Ce programme est un jeu interactif qui permet à l'utilisateur de choisir une carte parmi cinq options disponibles. Chaque carte est chargée à partir d'un fichier et affichée avec les informations suivantes :
- Dimensions de la carte.
- Contenu de la carte sous forme de tableau 2D.
- Coûts associés à chaque case.
- Représentation visuelle de la carte.

Ensuite, le programme démarre le jeu en permettant au robot ROVER de rejoindre la base.

## Fonctionnalités

- **Choix de la carte** : L'utilisateur peut choisir une carte entre 1 et 5.
- **Chargement de la carte** : Les cartes sont chargées à partir de fichiers `.map`.
- **Affichage des dimensions de la carte** : Le programme affiche la taille de la carte choisie.
- **Affichage du contenu de la carte** : Les valeurs de la carte sont affichées sous forme de tableau 2D.
- **Affichage des coûts associés** : Les coûts de chaque case de la carte sont affichés dans un format aligné.
- **Affichage visuel de la carte** : La carte est affichée visuellement grâce à la fonction `displayMap`.
- **Démarrage du jeu** : Le jeu commence après l'affichage de la carte.

## Prérequis

- Un compilateur C (par exemple, GCC).
- Les fichiers de cartes au format `.map` doivent être présents dans le répertoire `maps`.

## Installation

1. Clonez le dépôt ou téléchargez les fichiers source.
2. Assurez-vous que les fichiers de carte sont disponibles dans le répertoire `maps`.

## Utilisation

1. Exécutez le fichier `main.c`.
2. Suivez les instructions à l'écran pour choisir une carte.
3. Profitez du jeu !

## Structure des fichiers implémentés

- **`main.c`** : Contient le point d'entrée du programme et la logique principale.
- **`tree.c`** : Contient les fonctions pour gérer les aspects liés aux arbres.
- **`tree.h`** : Fichier d'en-tête pour les définitions liées aux arbres.

## Auteurs

- Antoine GOSSE
- Enzo JUZYNA
- Imrân BENESSAM

