#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream> 
#include <string>
#include <vector>
#include <unordered_map> // bibli table hachage
#include <stack> //bibli pile
#include <queue> //bibli file

using namespace std;

// Classe représentant un arc
class arc {
public:
    string id;        // Identifiant
    string o;         // Origine
    string d;         // Destination
    double length;    // Longueur
    string name;      // Nom
    bool oneway;      // Sens unique ou non

    arc(string id, string o, string d, double length = 0, string name = "", bool oneway = false);
    void affiche();
};

// Classe représentant un nœud
class noeud {
public:
    string id;                      // Identifiant
    vector<arc*> arcs_sortants;     // Arcs partant du nœud
    vector<arc*> arcs_entrants;     // Arcs arrivant au nœud
    bool visite;                    // Attribut pour les parcours

    noeud(string id);
    void affiche();
};

// Classe représentant le graphe
class graphe {
public:
    unordered_map<string, noeud*> lesnoeuds; // Table des nœuds
    unordered_map<string, arc*> lesarcs;     // Table des arcs

    graphe() = default;
    ~graphe();

    int lecture_arcs(string nomfichier);
    int lecture_noeuds(string nomfichier);
    void liste_incidence();
    void degre(int n);
    bool chemin(string o, string d);
    int pluscourtchemin(string o, string d);
};

#endif // GRAPHE_H
