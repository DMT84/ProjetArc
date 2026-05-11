🗺️ Projet AMS : Modélisation du Réseau Routier d'Avignon
Ce projet a été réalisé dans le cadre de l'UE Algorithmique Avancée et Graphes (L2 Informatique). L'objectif est de représenter et d'analyser une partie du réseau routier d'Avignon sous forme de graphe.

📝 Description du Projet
Le programme utilise des données réelles pour construire un graphe où :

Les nœuds représentent des localisations géographiques précises (intersections, points d'intérêt).

Les arcs représentent les segments de route reliant ces nœuds, avec des informations sur la distance, le nom de la rue et le sens de circulation.

🚀 Fonctionnalités
Le logiciel permet d'effectuer plusieurs opérations sur le réseau routier :

Chargement des données : Lecture des fichiers nodes.csv et arcs.csv.

Analyse de structure : Calcul automatique des incidences et du degré de chaque nœud.

Recherche de chemin : Vérification de l'existence d'un itinéraire entre deux points donnés (Parcours en profondeur/largeur).

Plus court chemin : Calcul de la distance minimale entre une origine et une destination.

🛠️ Stack Technique
Langage : C++11 / C++17

Structures de données : Utilisation intensive de la STL (unordered_map, vector, stack, queue).

Outils : Expressions régulières (regex) pour le parsing des fichiers CSV.

📂 Structure des Fichiers
main.cpp : Point d'entrée du programme et gestion de l'interface utilisateur.

classe.h : Définition des classes noeud, arc et graphe.

projet.cpp : Implémentation des méthodes de traitement et des algorithmes de graphes.

nodes.csv / arcs.csv : Données sources du réseau routier d'Avignon.

⚙️ Compilation et Exécution
Pour compiler le projet, utilise un compilateur C++ (comme g++) :
g++ -o projetAMS main.cpp projet.cpp
Puis lance l'exécutable :
./projetAMS

👥 Auteur
Dimitri Botella (Licence 2 Informatique, 2024-2025)
