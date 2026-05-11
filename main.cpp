#include "classe.h"

int main() {
    graphe g;

    cout << "Lecture des arcs..." << endl;
    g.lecture_arcs("arcs.csv");

    cout << "Lecture des nœuds..." << endl;
    g.lecture_noeuds("nodes.csv");

    cout << "Mise à jour des incidences..." << endl;
    g.liste_incidence();

    // Affichage des nœuds avec leur degré
    int n;
    cout << "\nEntrez le nombre de nœuds à afficher avec leur degré : ";
    cin >> n;
    cout << "\nAffichage des nœuds avec leur degré : " << endl;
    g.degre(n);
    
    // Saisie des identifiants
    string origine, destination;
    cout << "\nEntrez l'identifiant du nœud d'origine : ";
    cin >> origine;
    cout << "Entrez l'identifiant du nœud de destination : ";
    cin >> destination;

    // Recherche de chemin
    cout << "\nRecherche de chemin entre " << origine << " et " << destination << " :" << endl;
    g.chemin(origine, destination);

    // Plus court chemin
    cout << "\nPlus court chemin entre " << origine << " et " << destination << " :" << endl;
    int distance = g.pluscourtchemin(origine, destination);
    if (distance > 0) {
        cout << "Distance en nombre d'arcs : " << distance << endl;
    } else {
        cout << "Aucun chemin trouvé." << endl;
    }

    return 0;
}

