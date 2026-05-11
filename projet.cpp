#include "classe.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex> // Pour extraire les valeurs des dictionnaires JSON

int graphe::lecture_noeuds(string nomfichier) { // lire les noeuds
    ifstream fichier(nomfichier);
    if (!fichier.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << nomfichier << endl;
        return 0;
    }

    string ligne, id, json_part;
    regex regex_y("'y': ([0-9\\.]+)"); //coordonée y
    regex regex_x("'x': ([0-9\\.]+)"); //coordonnée x
    smatch match;

    int count = 0; //count noeul lu
    while (getline(fichier, ligne)) {
        istringstream iss(ligne);
        getline(iss, id, ',');       // Récupère l'ID du nœud
        getline(iss, json_part);     // Récupère la partie JSON

        if (!id.empty()) {
            string y, x;

            // Extraction des valeurs y et x avec regex
            if (regex_search(json_part, match, regex_y)) y = match[1];
            if (regex_search(json_part, match, regex_x)) x = match[1];

            lesnoeuds[id] = new noeud(id); //ajout des noeud dans graphe
            count++;
        }
    }
    return count;
}

//classe arc 
arc::arc(string id, string o, string d, double length, string name, bool oneway)
    : id(id), o(o), d(d), length(length), name(name), oneway(oneway) {}

void arc::affiche() {
    cout << "Arc: " << id << " | Origine: " << o << " | Destination: " << d
         << " | Longueur: " << length << " | Nom: " << name
         << " | Sens unique: " << (oneway ? "Oui" : "Non") << endl;
}

//classe noeud
noeud::noeud(string id) : id(id), visite(false) {}

void noeud::affiche() {
    cout << "Noeud: " << id << " | Arcs sortants: " << arcs_sortants.size()
         << " | Arcs entrants: " << arcs_entrants.size() << endl;
}

//classe graphe
graphe::~graphe() {
    // Pour les noeuds
    for (map<string, noeud*>::iterator it = lesnoeuds.begin(); it != lesnoeuds.end(); ++it) {
        noeud* n = it->second;
        delete n;
    }
    // Pour les arcs
    for (map<string, arc*>::iterator it = lesarcs.begin(); it != lesarcs.end(); ++it) {
        arc* a = it->second;
        delete a;
    }
}

int graphe::lecture_arcs(string nomfichier) {
    ifstream fichier(nomfichier);
    if (!fichier.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << nomfichier << endl;
        return 0;
    }

    string ligne, origine, destination, json_part;
    regex regex_length("'length': ([0-9\\.]+)"); //avoir longueur arc
    regex regex_name("'name': '([^']*)'"); //avoir nom arc
    regex regex_oneway("'oneway': (True|False)");  //connaitre sens unique
    smatch match;

    int count = 0;
    while (getline(fichier, ligne)) {
        istringstream iss(ligne);
        getline(iss, origine, ','); //noeur origine
        getline(iss, destination, ','); //noeux destination
        getline(iss, json_part);

        if (!origine.empty() && !destination.empty()) {
            string name = "unknown"; //nom arc default
            double length = 0.0; //longueur default
            bool oneway = false; //sens unique par default

            // Extraction des attributs avec regex
            if (regex_search(json_part, match, regex_length)) length = stod(match[1]);
            if (regex_search(json_part, match, regex_name)) name = match[1];
            if (regex_search(json_part, match, regex_oneway)) oneway = (match[1] == "True");

            string id = origine + "-" + destination;
            lesarcs[id] = new arc(id, origine, destination, length, name, oneway);
            count++;
        }
    }
    return count; //retourne nb arc lus
}

void graphe::liste_incidence() {
    for (map<string, arc*>::iterator it = lesarcs.begin(); it != lesarcs.end(); ++it) {
        arc* a = it->second; // Ajout des arcs aux nœuds d'origine et de destination
        lesnoeuds[a->o]->arcs_sortants.push_back(a);
        lesnoeuds[a->d]->arcs_entrants.push_back(a);
    }
}

void graphe::degre(int n) {
    vector<pair<string, int>> degres;
    for (map<string, noeud*>::iterator it = lesnoeuds.begin(); it != lesnoeuds.end(); ++it) {
        noeud* noeud = it->second;
        int total = noeud->arcs_sortants.size() + noeud->arcs_entrants.size();
        degres.push_back({it->first, total});
    }
    //tri
    sort(degres.begin(), degres.end(), [](auto& a, auto& b) { return a.second > b.second; });
    //affiche plus haut degree
    for (int i = 0; i < n && i < degres.size(); ++i) {
        cout << "Noeud " << degres[i].first << " | Degré: " << degres[i].second << endl;
    }
}

bool graphe::chemin(string o, string d) {
    if (lesnoeuds.find(o) == lesnoeuds.end()) { //recherche du nom dans la liste des noeuds
        cerr << "Erreur : Le nœud d'origine '" << o << "' n'existe pas dans le graphe." << endl;
        return false;
    }
    if (lesnoeuds.find(d) == lesnoeuds.end()) { //recherche du nom dans la liste des noeuds
        cerr << "Erreur : Le nœud de destination '" << d << "' n'existe pas dans le graphe." << endl;
        return false;
    }

    // Réinitialisation de l'attribut visite pour tous les nœuds
    for (map<string, noeud*>::iterator it = lesnoeuds.begin(); it != lesnoeuds.end(); ++it) {
        it->second->visite = false;
    }

    // Utilisation d'une pile pour le parcours DFS
    stack<pair<string, vector<pair<string, string>>>> pile;
    pile.push({o, {}}); // {Noeud actuel, Chemin emprunté (liste de transitions)}

    while (!pile.empty()) {
        auto current = pile.top().first;
        auto chemin_arcs = pile.top().second;
        pile.pop();

        if (current == d) { // Si le nœud destination est trouvé, afficher le chemin
            cout << "Chemin trouvé :" << endl;
            for (const auto& transition : chemin_arcs) {
                cout << " -> \"" << transition.first << "\" - \"" << transition.second << "\"" << endl;
            }
            return true;
        }

        noeud* n = lesnoeuds[current];
        if (!n->visite) {
            n->visite = true;
            for (arc* a : n->arcs_sortants) {
                if (a && !lesnoeuds[a->d]->visite) {
                    vector<pair<string, string>> nouveau_chemin = chemin_arcs;

                    // Nom de la rue de départ (arc) et ID du nœud de départ
                    string rue_depart = a->name.empty() ? "Arc sans nom" : a->name;
                    string rue_arrivee = lesnoeuds[a->d]->id + " - " + a->d; // ID du nœud de destination

                    // Ajout du couple "rue départ - id départ" - "rue arrivée - id arrivée"
                    nouveau_chemin.emplace_back(rue_depart + " - " + current, a->name + " - " + a->d);
                    pile.push({a->d, nouveau_chemin});
                }
            }
        }
    }

    cout << "Aucun chemin trouvé." << endl;
    return false;
}

int graphe::pluscourtchemin(string o, string d) {
    if (lesnoeuds.find(o) == lesnoeuds.end()) { //recherche du nom dans la liste des noeuds
        cerr << "Erreur : Le nœud d'origine '" << o << "' n'existe pas dans le graphe." << endl;
        return 0;
    }
    if (lesnoeuds.find(d) == lesnoeuds.end()) { //recherche du nom dans la liste des noeuds
        cerr << "Erreur : Le nœud de destination '" << d << "' n'existe pas dans le graphe." << endl;
        return 0;
    }

    for (map<string, noeud*>::iterator it = lesnoeuds.begin(); it != lesnoeuds.end(); ++it) {
        it->second->visite = false;
    }

    queue<pair<string, vector<pair<string, string>>>> file;
    file.push({o, {}}); // {Nœud actuel, Chemin emprunté (liste de transitions)}

    while (!file.empty()) {
        auto current = file.front().first;
        auto chemin_arcs = file.front().second;
        file.pop();

        if (current == d) {  // Si le nœud destination est trouvé, afficher le chemin
            cout << "Plus court chemin trouvé :" << endl;
            for (const auto& transition : chemin_arcs) {
                cout << " -> \"" << transition.first << "\" - \"" << transition.second << "\"" << endl;
            }
            return chemin_arcs.size();
        }

        noeud* n = lesnoeuds[current];
        if (!n->visite) {
            n->visite = true;
            for (arc* a : n->arcs_sortants) {
                if (a && !lesnoeuds[a->d]->visite) {
                    vector<pair<string, string>> nouveau_chemin = chemin_arcs;

                    // Nom de la rue de départ (arc) et ID du nœud de départ
                    string rue_depart = a->name.empty() ? "Arc sans nom" : a->name;
                    string rue_arrivee = lesnoeuds[a->d]->id + " - " + a->d; // ID du nœud de destination

                    // Ajout du couple "rue départ - id départ" - "rue arrivée - id arrivée"
                    nouveau_chemin.emplace_back(rue_depart + " - " + current, a->name + " - " + a->d);
                    file.push({a->d, nouveau_chemin});
                }
            }
        }
    }

    cout << "Aucun chemin trouvé." << endl;
    return 0;
}
