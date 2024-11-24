#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H

#include "node.h"
#include "map.h"
#include "moves.h"
static char _orientation[8][8] = {"Nord", "Est", "Sud", "Ouest"};

typedef struct s_tree
{
    t_node *root;  // Racine de l'arbre
    int height;    // Hauteur maximale de l'arbre (nombre de mouvements disponibles)
} t_tree;

/**
 * @brief Crée un arbre à partir de la carte et d'un nombre de mouvements disponibles.
 * @param map La carte où évolue le robot.
 * @param available_moves Nombre maximum de mouvements disponibles.
 * @return L'arbre créé avec la position initiale du robot.
 */
t_tree createTree(t_map map, int available_moves);

/**
 * @brief Recherche un noeud dans l'arbre en fonction de sa localisation.
 * @param tree L'arbre à explorer.
 * @param loc La localisation cible à rechercher.
 * @return Un pointeur vers le noeud correspondant ou NULL s'il n'est pas trouvé.
 */
t_node *searchNode(t_tree tree, t_localisation loc);

/**
 * @brief Insère un noeud enfant dans l'arbre à partir d'un mouvement et de la carte.
 * @param nd Le noeud parent où insérer l'enfant.
 * @param move Le mouvement effectué.
 * @param map La carte utilisée pour vérifier les positions.
 * @param available_moves Nombre maximum de mouvements restants.
 */
void insertInTree(t_node *nd, int move, t_map map, int available_moves);

/**
 * @brief Complète l'arbre en générant tous les noeuds possibles pour un nombre de mouvements donné.
 * @param tree L'arbre à compléter.
 * @param map La carte utilisée pour vérifier les positions.
 * @param available_moves Nombre maximum de mouvements restants.
 */
void completeTree(t_tree *tree, t_map map, int available_moves);

/**
 * @brief Fonction auxiliaire pour compléter l'arbre récursivement.
 * @param node Le noeud actuel où ajouter des enfants.
 * @param map La carte utilisée pour vérifier les positions.
 * @param available_moves Nombre maximum de mouvements restants.
 */
void auxiCompleteTree(t_node *node, t_map map, int available_moves);

/**
 * @brief Affiche l'arbre dans la console, en montrant sa structure et les coûts des noeuds.
 * @param node Le noeud racine à partir duquel afficher.
 * @param level Niveau d'indentation pour refléter la profondeur.
 */
void displayTree(t_node *node, int level);

/**
 * @brief Libère la mémoire associée à un arbre.
 * @param root Le noeud racine de l'arbre.
 */
void freeTree(t_node *root);

/**
 * @brief Retourne une chaîne de caractères représentant une orientation donnée.
 * @param ori L'orientation (0: Nord, 1: Est, 2: Sud, 3: Ouest).
 * @return Une chaîne représentant l'orientation.
 */
char* getOrientation(int ori);

/**
 * @brief Fonction auxiliaire pour rechercher la feuille de coût minimal dans l'arbre.
 * @param node Le noeud actuel.
 * @param min_cost_node Pointeur vers le noeud ayant le coût minimal (mise à jour dans la fonction).
 * @param min_cost Coût minimal actuel (mis à jour dans la fonction).
 */
void SearchLeafMinAuxiliaire(t_node *node, t_node **min_cost_node, int *min_cost);

/**
 * @brief Trouve la feuille de l'arbre ayant le coût minimal.
 * @param tree L'arbre à explorer.
 * @return Le noeud feuille avec le coût minimal.
 */
t_node *SearchLeafMin(t_tree tree);

/**
 * @brief Génère les mouvements possibles pour le robot, avec des ajustements selon le terrain.
 * @param is_on_erg Indique si le robot est sur un erg (sable mouvant ou terrain difficile).
 */
void getMoves(int is_on_erg);

/**
 * @brief Trouve le chemin de la racine à une feuille donnée.
 * @param tree L'arbre à explorer.
 * @param node La feuille cible.
 * @return Un tableau de pointeurs vers les noeuds constituant le chemin.
 */
t_node** CheminRacineFeuille(t_tree tree, t_node* node);

/**
 * @brief Lance le processus principal pour déplacer le robot à travers la carte.
 * @param map La carte où évolue le robot.
 */
void play(t_map);

#endif //UNTITLED1_TREE_H