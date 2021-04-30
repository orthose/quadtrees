/************************************************/
/* TP ALGO L3 Université Paris-Saclay 2020-2021 */
/* Étudiant: Maxime Vincent                     */
/* Exercice 2: Listes-Piles et Files            */
/* Compilation: gcc list.c -o list              */
/************************************************/

#include <stdio.h>
#include <stdlib.h>
//    Je n'aime pas le sucre syntaxique proposé
//    La librairie standard dispose déjà des booléens
#include <stdbool.h>
//    Pour exécuter des tests unitaires
#include <assert.h>

/*************************************************/
/*                                               */
/*          definition type liste et file        */
/*                                               */
/*************************************************/

typedef struct Bloc
{
    int nombre;
    struct Bloc *suivant;
} Bloc;
//    Une liste est un pointeur vers un Bloc
typedef Bloc *Liste;

//    Structure pour la liste de liste
typedef struct BlocListe
{
    Liste liste;
    struct BlocListe *suivant;
} BlocListe;
//    Une liste de liste est un pointeur vers un BlocListe
typedef BlocListe *ListeListe;

//    Une file est un pointeur vers un Bloc
//    La structure de Bloc reste la même
typedef Bloc *File;

/*************************************************/
/*                                               */
/*                predeclarations                */
/*                                               */
/*************************************************/

/* initialise une Liste à vide */
void initVide(Liste *L);

/* renvoie 1 si la Liste en parametre est vide, 0 sinon */
bool estVide(Liste l);
bool estVideLL(ListeListe l);

/* renvoie le premier element de la Liste en parametre */
int premier(Liste l);
Liste premierLL(ListeListe l);

/* renvoie une nouvelle Liste correspondant a celle en parametre, avec l'element x ajoute en haut de la pile */
Liste ajoute(int x, Liste l);
ListeListe ajouteLL(Liste x, ListeListe l);

/* modifie la Liste en parametre: x est ajoute comme premier element */
void empile(int x, Liste* L);
void empileLL(Liste l, ListeListe *L);

/* renvoie une nouvelle Liste correspondant a celle en parametre sans son premier element */
Liste suite(Liste l);
Liste *pointeurSuite(Liste l);
ListeListe suiteLL(ListeListe l);

/* modifie la Liste en parametre: le premier element est retire */
void depile(Liste* l);

/* affichage simple en recursif et en iteratif */
void affiche_rec(Liste l);
void affiche_iter(Liste l);
void affiche(Liste l);
void afficheLL(ListeListe l);

/* longueur en recursif et en iteratif */
int longueur_rec (Liste l);
int longueur_iter (Liste l);

/*  Elimination du dernier element en recursif et en iteratif  */
/*  VD est la sousprocedure utilitaire de la version recursive */
void VD (Liste *L);
void VireDernier_rec (Liste *L);
void VireDernier_iter (Liste *L);

//    Pour initialiser plus facilement une liste
Liste init_liste(int tab[], int n);

//    Fonctions et procédures du TP-Projet 
//    ...sur les listes
bool debutDeuxIdentiques(Liste l);
bool queDesZeros(Liste l);
bool sousEnsemble(Liste l1, Liste l2);
ListeListe permutations(int n);
void eliminePositionsPaires(Liste *l);
void begaye(Liste *l);
int maxZerosConsecutifs_iter(Liste l);
int maxZerosConsecutifs_rec1(Liste l);
int maxZerosConsecutifs_rec2(Liste l);
bool estPalindrome(Liste l);
bool sommeAvantApres(Liste l);

//    ...sur les files
void initVideFile(File *f);
bool estVideFile(File f);
int dernierFile(File f);
int premierFile(File f);
void ajouteFile(int x, File *f);
void sortirFile(int *x, File *f);
  
/*************************************************/
/*                                               */
/*                briques de base                */
/*                                               */
/*************************************************/

void initVide(Liste *L)
{
    *L = NULL ;
}

bool estVide(Liste l)
{
    return l == NULL ;
}

bool estVideLL(ListeListe l) {
  return l == NULL;
}

int premier(Liste l)
{
    return l->nombre; 
}

Liste premierLL(ListeListe l) {
  return l->liste;
}

Liste ajoute(int x, Liste l)
{
    Liste tmp = (Liste) malloc(sizeof(Bloc)) ;
    tmp->nombre = x ;
    tmp->suivant = l ;
    return tmp ;
}

ListeListe ajouteLL(Liste x, ListeListe l)
{
    ListeListe tmp = (ListeListe) malloc(sizeof(BlocListe)) ;
    tmp->liste = x ;
    tmp->suivant = l ;
    return tmp ;
}

void empile(int x, Liste *L)
{
    *L = ajoute(x,*L) ; 
}

void empileLL(Liste l, ListeListe *L) {
  *L = ajouteLL(l, *L);
}

Liste suite(Liste l)
{
    return l->suivant ;
}

Liste *pointeurSuite(Liste l) {
  return &(l->suivant);
}

ListeListe suiteLL(ListeListe l) {
  return l->suivant;
}

void depile(Liste *L)
{
    Liste tmp = *L ;
    *L = suite(*L) ;
    free(tmp) ;
}

/*************************************************/
/*                                               */
/*     Affiche, avec les briques de base         */
/*                                               */
/*************************************************/

void affiche_rec(Liste l)
{
    if(estVide(l))
        printf("\n");
    else
    {
        printf("%d ", premier(l));
        affiche_rec(suite(l));
    }
}


void affiche_iter(Liste l)
{
    Liste L2 = l;
    while(!estVide(L2))
    {
        printf("%d ", premier(L2));
        L2 = suite(L2);
    }
    printf("\n");
}

//    Bel affichage style Python
void affiche(Liste l) {
  void affiche_bis(Liste l) {
    if (!estVide(suite(l))) {
      printf("%d, ", premier(l));
      affiche_bis(suite(l));
    }
    else {
      printf("%d", premier(l));
    }
  }
  printf("[");
  if (!estVide(l)) {
     affiche_bis(l);
  }
  printf("]");
}

//    Bel affichage pour liste de liste
void afficheLL(ListeListe l) {
  void affiche_bis(ListeListe l) {
    if (!estVideLL(suiteLL(l))) {
      affiche(premierLL(l));
      printf(", ");
      affiche_bis(suiteLL(l));
    }
    else {
      affiche(premierLL(l));
    }
  }
  printf("[");
  if (!estVideLL(l)) {
    affiche_bis(l);
  }
  printf("]\n");
}

/**
 * Crée une liste à partir de sa représentation sous forme de tableau
 * @param tab: {} ou {1, 2, ..., 3}
 * @param n: Longueur du tableau (jusqu'où regarder)
 * @return: nouvelle liste instanciée dans l'ordre du tableau
 **/
Liste init_liste(int tab[], int n) {
  Liste res;
  initVide(&res);
  for (int i = (n - 1); i >= 0; i--) {
    empile(tab[i], &res);
  }
  return res;
}

/*************************************************/
/*                                               */
/*     Longueur, sans les briques de base        */
/*                                               */
/*************************************************/

int longueur_rec (Liste l)
{
    if (l == NULL)
         return 0 ;
    else return (1 + longueur_rec(l->suivant)) ;
}

int longueur_iter (Liste l)
{
    Liste P = l;
    int cpt = 0 ;
    while (P != NULL)
    {   P = P->suivant ;
        cpt++ ;
    }
    return cpt ;
}

/*************************************************/
/*                                               */
/*       VireDernier,                            */
/*               sans les briques de base,       */
/*               ni le "->"                      */
/*                                               */
/*************************************************/

void VD (Liste *L)
          // *L non NULL ie liste non vide
{
     if ( ((**L).suivant) == NULL )
            depile(L) ;   // moralement : depile(& (*L)) ;
     else VD (& ( (**L).suivant )) ;
}

void VireDernier_rec (Liste *L)
{
     if ( (*L) != NULL )
          VD(L);        // moralement : VD(& (*L)) ;
}

void VireDernier_iter (Liste *L)
{
    if ( (*L) != NULL)
    {
        while ( ((**L).suivant) != NULL )
                 L = & ( (**L).suivant ) ;
        free(*L) ;
        *L = NULL ;
     }
}


/*************************************************/
/*                                               */
/*       Libere la memoire                       */
/*                                               */
/*************************************************/

void VideListe(Liste *L)
{
    if(!(estVide(*L)))
    {
        depile(L);
        VideListe(L);
    }      
}

/*************************************************/
/*                                               */
/*           Main et mes fonctions               */
/*                                               */
/*************************************************/

bool debutDeuxIdentiques(Liste l) {
  //    Évaluation paresseuse nécessaire
  return !estVide(l) && !estVide(suite(l)) &&
    premier(l) == premier(suite(l));
}

bool queDesZeros(Liste l) {
  bool queDesZeros(Liste l, bool acc) {
    if (estVide(l) || !acc) {
      return acc;
    }
    else {
      return queDesZeros(suite(l), acc && premier(l) == 0);
    }
  }
  return queDesZeros(l, true);
}

/**********************************************/
/* Cette fonction n'étant pas évidente        */
/* Je l'ai préalablement implémentée en OCaml */
/* Je considère que les listes peuvent avoir  */
/* des doublons (pas de vrais ensembles)      */
/*                                            */
/* let rec se l1 l2 =                         */
/*  match (l1, l2) with                       */
/*  (* Cas de base *)                         */
/*  | ([], _) -> true                         */
/*  | (_, []) -> false                        */
/*  (* Élimination des doublons *)            */
/*  | (a::b::c, _) when a = b -> se (b::c) l2 */
/*  | (_, a::b::c) when a = b -> se l1 (b::c) */
/*  (* Cas général *)                         */
/*  | (a::b, c::d) ->                         */
/*      if a > c then se l1 d                 */
/*      else if a < c then false              */
/*      else se b d (* a = c *)               */
/**********************************************/
bool sousEnsemble(Liste l1, Liste l2) {
  //    Cas de base
  //    L'ensemble vide appartient à n'importe quel ensemble
  if (estVide(l1)) { return true; }
  //    On n'a pas parcouru tout l'ensemble l1
  else if (estVide(l2)) { return false; }
  //    Élimination des doublons
  else if (!estVide(suite(l1)) && premier(l1) == premier(suite(l1))) {
    return sousEnsemble(suite(l1), l2);
  }
  else if (!estVide(suite(l2)) && premier(l2) == premier(suite(l2))) {
    return sousEnsemble(l1, suite(l2));
  }
  //    Cas général
  else {
    //    On avance dans l2 sans avancer dans l1
    if (premier(l1) > premier(l2)) { return sousEnsemble(l1, suite(l2)); }
    //    On ne pourra jamais trouver le premier élément de l1 dans l2
    else if (premier(l1) < premier(l2)) { return false; }
    //    Cas où premier(l1) == premier(l2) on avance dans les 2 listes
    else { return sousEnsemble(suite(l1), suite(l2)); }
  }
}

//    Fonctions auxiliaires pour permutations
ListeListe concatLL(ListeListe l1, ListeListe l2) {
    if (estVideLL(l1)) { return l2; }
    else {
      return ajouteLL(premierLL(l1), concatLL(suiteLL(l1), l2));
    }
  }
  ListeListe ajouteEnTeteToutesListes(int x, ListeListe l) {
    if (estVideLL(l)) { return NULL; } // []
    else {
      return ajouteLL(ajoute(x, premierLL(l)), ajouteEnTeteToutesListes(x, suiteLL(l)));
    }
  }
  ListeListe ajouteToutesPositions(int x, Liste l) {
    if (estVide(l)) {
      ListeListe res = NULL; // [[]]
      Liste m; initVide(&m); // []
      empile(x, &m);
      empileLL(m, &res);
      return res; // [[x]]
    }
    else {
      return ajouteLL(ajoute(x, l), 
        ajouteEnTeteToutesListes(premier(l), ajouteToutesPositions(x, suite(l))));
    }
  }
  ListeListe ajouteToutesListesToutesPositions(int x, ListeListe l) {
    if (estVideLL(l)) { return NULL; } // []
    else {
      return concatLL(ajouteToutesPositions(x, premierLL(l)), 
        ajouteToutesListesToutesPositions(x, suiteLL(l)));
    }
  }

ListeListe permutations(int n) {
  if (n == 0) {
    ListeListe res = NULL;
    Liste vide; initVide(&vide);
    empileLL(vide, &res); 
    return res; // [[]] 
  }
  else {
    return ajouteToutesListesToutesPositions(n, permutations(n - 1));
  }
}

void eliminePositionsPaires(Liste *l) {
  void eliminePositionsPaires(Liste *l, int i) {
    if (!estVide(*l)) {
      int j = i + 1;
      if (i % 2 == 0) {
        //    On a enlevé un Bloc
        depile(l); j = j + 1;
      }
      eliminePositionsPaires(pointeurSuite(*l), j);
    }
  }
  eliminePositionsPaires(l, 1);
}

void begaye(Liste *l) {
  if (!estVide(*l)) {
    if (premier(*l) > 0) {
      empile(premier(*l), l);
      //    On doit avancer de 2 car on a ajouté un Bloc
      begaye(pointeurSuite(*pointeurSuite(*l)));
    }
    else {
      //    Il faut regarder le prochain élément
      //    depile fait avancer de 1
      depile(l);
      begaye(l); 
    }
  }
}

int maxZerosConsecutifs_iter(Liste l) {
  //    La copie de sécurité de l est inutile
  //    car le passage de paramètres en C est par copie
  int res = 0, current = 0;
  while(!estVide(l)) {
    if (premier(l) == 0) { 
      current++;
      if (current > res) {
        res = current;
      } 
    }
    else {
      current = 0;
    }
    l = suite(l);
  }
  return res;
}

int maxZerosConsecutifs_rec1(Liste l) {
  int mzc(Liste l, int max, int current) {
    if (estVide(l)) { return max; }
    else {
      if (premier(l) == 0) {
        current++;
        if (current > max) { return mzc(suite(l), current, current); }
        else { return mzc(suite(l), max, current); }
      }
      else {
        return mzc(suite(l), max, 0);
      }
    }
  }
  return mzc(l, 0, 0);
}

int maxZerosConsecutifs_rec2(Liste l) {
  void mzc(Liste l, int *max, int *current) {
    if (estVide(l)) {
      *max = 0; *current = 0;
    }
    else {
      mzc(suite(l), max, current);
      if (premier(l) == 0) {
        *current = *current + 1;
        if (*current > *max) { *max = *current; }
      }
      else {
        *current = 0;
      }
    }
  }
  int max, current;
  mzc(l, &max, &current);
  return max;
}

bool estPalindrome(Liste l) {
  void estPalindrome(Liste l, Liste *p, bool *res) {
    if (estVide(l)) { *res = true; }
    else {
      estPalindrome(suite(l), p, res);
      //    Quand res vaut false il le reste 
      if (*res) {
        if (premier(*p) != premier(l)) { *res = false; }
        else { *p = suite(*p); }
      }
    }
  }
  Liste p = l; bool res;
  estPalindrome(l, &p, &res);
  return res;
}

/******************************************************/
/* let sap l =                                        */
/*     let rec sap l before after =                   */
/*         match l with                               */
/*         | [] -> false                              */
/*         | x::s ->                                  */
/*             let r = sap s (x + before) after in    */
/*             if before = !after then true           */
/*             else let () = after := !after + x in r */
/*     in                                             */
/*     let a = ref 0 in                               */
/*     sap l 0 a                                      */
/******************************************************/
bool sommeAvantApres(Liste l) {
  bool sommeAvantApres(Liste l, int before, int *after) {
    if (estVide(l)) { return false; }
    else {
      bool res = sommeAvantApres(suite(l), before + premier(l), after);
      //    Pour aller plus vite quand on a déjà trouvé true
      if (res) { return res; }
      if (before == *after) { return true; }
      else { *after = *after + premier(l); }
      return res;
    }
  }
  int a = 0;
  return sommeAvantApres(l, 0, &a);
}

/*************************************************/
/*                                               */
/*           Implémentation de la file           */
/*                                               */
/*************************************************/

void initVideFile(File *f) { *f = NULL; }

bool estVideFile(File f) { return f == NULL; }

//    Celui arrivé le plus récemment
int dernierFile(File f) { return f->nombre; }

//    Celui arrivé le plus anciennement
int premierFile(File f) { return f->suivant->nombre; }

//    On ajoute par l'arrière de la file (dans le sens de parcours)
void ajouteFile(int x, File *f) {
  if (*f == NULL) {
    Bloc *first = malloc(sizeof(Bloc));
    first->nombre = x;
    first->suivant = first;
    *f = first;
  } 
  else {
    Bloc *tmp = malloc(sizeof(Bloc));
    tmp->nombre = x;
    tmp->suivant = (*f)->suivant;
    (*f)->suivant = tmp;
    *f = tmp;
  }
}

//    On sort par l'avant de la file (dans le sens de parcours)
//    L'entier pointé par x sera le champ nombre du Bloc détruit
void sortirFile(int *x, File *f) {
  if ((*f)->suivant == (*f)->suivant->suivant) {
    *x = (*f)->nombre;
    free(*f);
    *f = NULL;
  } 
  else {
    *x = (*f)->suivant->nombre;
    Bloc *tmp = (*f)->suivant->suivant;
    free((*f)->suivant);
    (*f)->suivant = tmp;
  }
}

//    Les tests sont enfermés dans des {...}
//    afin de pouvoir réutiliser les noms de variables
int main() {
  
  //    debutDeuxIdentiques
  {
    int t1[] = {}; Liste l1 = init_liste(t1, 0);
    assert(!debutDeuxIdentiques(l1));
    int t2[] = {8}; Liste l2 = init_liste(t2, 1);
    assert(!debutDeuxIdentiques(l2));
    int t3[] = {8, 8}; Liste l3 = init_liste(t3, 2);
    assert(debutDeuxIdentiques(l3));
    int t4[] = {8, 8, 7, 6}; Liste l4 = init_liste(t4, 4);
    assert(debutDeuxIdentiques(l4));
    int t5[] = {8, 7, 7, 6}; Liste l5 = init_liste(t5, 4);
    assert(!debutDeuxIdentiques(l5));
    //    Pour éviter les fuites mémoire
    VideListe(&l1); VideListe(&l2); VideListe(&l3);
    VideListe(&l4); VideListe(&l5);
  }
  
  //    queDesZeros
  {
    int t1[] = {}; Liste l1 = init_liste(t1, 0);
    assert(queDesZeros(l1));
    int t2[] = {1}; Liste l2 = init_liste(t2, 1);
    assert(!queDesZeros(l2));
    int t3[] = {1, 0}; Liste l3 = init_liste(t3, 2);
    assert(!queDesZeros(l3));
    int t4[] = {0, 1, 0}; Liste l4 = init_liste(t4, 3);
    assert(!queDesZeros(l4));
    int t5[] = {0, 0, 0, 1}; Liste l5 = init_liste(t5, 4);
    assert(!queDesZeros(l5));
    int t6[] = {0}; Liste l6 = init_liste(t6, 1);
    assert(queDesZeros(l6));
    int t7[] = {0, 0, 0, 0}; Liste l7 = init_liste(t7, 4);
    assert(queDesZeros(l7));
    VideListe(&l1); VideListe(&l2); VideListe(&l3);
    VideListe(&l4); VideListe(&l5); VideListe(&l6);
    VideListe(&l7);
  }
  
  //    sousEnsemble
  {
    int t1[] = {}; Liste l1 = init_liste(t1, 0);
    assert(sousEnsemble(l1, l1));
    int t2[] = {4, 5, 8, 9}; Liste l2 = init_liste(t2, 4);
    assert(sousEnsemble(l1, l2));
    int t3[] = {4, 5}; Liste l3 = init_liste(t3, 2);
    assert(sousEnsemble(l3, l2));
    int t4[] = {5, 8}; Liste l4 = init_liste(t4, 2);
    assert(sousEnsemble(l4, l2));
    int t5[] = {8, 9}; Liste l5 = init_liste(t5, 2);
    assert(sousEnsemble(l5, l2));
    int t6[] = {4, 9}; Liste l6 = init_liste(t6, 2);
    assert(sousEnsemble(l6, l2));
    int t7[] = {5, 10}; Liste l7 = init_liste(t7, 2);
    assert(!sousEnsemble(l7, l2));
    int t8[] = {4, 4, 5, 8, 8, 8, 9}; Liste l8 = init_liste(t8, 7);
    int t9[] = {5, 5, 5, 9, 9, 9, 9, 9}; Liste l9 = init_liste(t9, 8);
    int t10[] = {5, 5, 5, 5, 7}; Liste l10 = init_liste(t10, 5);
    assert(sousEnsemble(l9, l8));
    assert(!sousEnsemble(l10, l8));
    VideListe(&l1); VideListe(&l2); VideListe(&l3);
    VideListe(&l4); VideListe(&l5); VideListe(&l6);
    VideListe(&l7); VideListe(&l8); VideListe(&l9);
    VideListe(&l10);
  }
  
  //    concatLL
  {
    int t1[] = {1,2,3,5,8}; Liste l1 = init_liste(t1, 5);
    int t2[] = {4,7,8,9}; Liste l2 = init_liste(t2, 4);
    int t3[] = {10, 5, 777}; Liste l3 = init_liste(t3, 3);
    int t4[] = {42, 98,128}; Liste l4 = init_liste(t4, 3);
    ListeListe ll1 = NULL, ll2 = NULL;
    empileLL(l1, &ll1); empileLL(l2, &ll1);
    empileLL(l3, &ll2); empileLL(l4, &ll2);
    printf("ll1="); afficheLL(ll1);
    printf("ll2="); afficheLL(ll2);
    ListeListe ll12 = concatLL(ll1, ll2);
    printf("concat(ll1, ll2)="); afficheLL(ll12);
    VideListe(&l1); VideListe(&l2); VideListe(&l3);
    VideListe(&l4);
  }
  
  //    ajouteEnTeteToutesListes
  {
    int t1[] = {1,2,3,5,8}; Liste l1 = init_liste(t1, 5);
    int t2[] = {4,7,8,9}; Liste l2 = init_liste(t2, 4);
    int t3[] = {10, 5, 777}; Liste l3 = init_liste(t3, 3);
    ListeListe ll1 = NULL;
    empileLL(l1, &ll1); empileLL(l2, &ll1); empileLL(l3, &ll1);
    printf("ll1="); afficheLL(ll1);
    ListeListe ll2 = ajouteEnTeteToutesListes(2048, ll1);
    printf("ajouteEnTeteToutesListes(2048, ll1)="); afficheLL(ll2);
    VideListe(&l1); VideListe(&l2); VideListe(&l3);
  }
  
  //    ajouteToutesPositions
  {
    int t1[] = {1,2,3}; Liste l1 = init_liste(t1, 3);
    ListeListe ll1 = ajouteToutesPositions(8, l1);
    printf("l1="); affiche(l1); printf("\n");
    printf("ajouteToutesPositions(8, l1)="); afficheLL(ll1);
    VideListe(&l1);
  }
  
  //    ajouteToutesListesToutesPositions
  {
    int t1[] = {1,2,3}; Liste l1 = init_liste(t1, 3);
    int t2[] = {4,5}; Liste l2 = init_liste(t2, 2);
    int t3[] = {7}; Liste l3 = init_liste(t3, 1);
    ListeListe ll1 = NULL;
    empileLL(l1, &ll1); empileLL(l2, &ll1); empileLL(l3, &ll1);
    printf("ll1="); afficheLL(ll1);
    ListeListe ll2 = ajouteToutesListesToutesPositions(2048, ll1);
    printf("ajouteToutesListesToutesPositions(2048, ll1)="); afficheLL(ll2);
    VideListe(&l1); VideListe(&l2); VideListe(&l3);
  }
  
  //    permutations
  for (int i = 0; i <= 4; i++) {
    printf("permutations(%d)=", i);
    afficheLL(permutations(i));
  }
  
  //    eliminePositionsPaires
  {
    int t1[] = {1,2,3,4,5,6,7}; Liste l1 = init_liste(t1, 7);
    printf("l1="); affiche(l1); printf("\n");
    eliminePositionsPaires(&l1);
    printf("eliminePositionsPaires(l1)="); affiche(l1); printf("\n");
    VideListe(&l1);
  }
  
  //    begaye
  {
    int t1[] = {2,1,0,6,-2,8}; Liste l1 = init_liste(t1, 6);
    printf("l1="); affiche(l1); printf("\n");
    begaye(&l1);
    printf("begaye(l1)="); affiche(l1); printf("\n");
    VideListe(&l1);
  }
  
  //    maxZerosConsecutifs
  {
    int t1[] = {9,7,0,0,0,0,8,0,7,0,0,0}; Liste l1 = init_liste(t1, 12);
    assert(maxZerosConsecutifs_iter(l1) == 4);
    assert(maxZerosConsecutifs_rec1(l1) == 4);
    assert(maxZerosConsecutifs_rec2(l1) == 4);
    int t2[] = {4,5,8,8,8,9}; Liste l2 = init_liste(t2, 6);
    assert(maxZerosConsecutifs_iter(l2) == 0);
    assert(maxZerosConsecutifs_rec1(l2) == 0);
    assert(maxZerosConsecutifs_rec2(l2) == 0);
    VideListe(&l1); VideListe(&l2);
  }
  
  //    estPalindrome
  {
    int t1[] = {1,2,3,5,3,2,1}; Liste l1 = init_liste(t1, 7);
    int t2[] = {1,2,3,3,2,1}; Liste l2 = init_liste(t2, 6);
    int t3[] = {}; Liste l3 = init_liste(t3, 0);
    int t4[] = {2048}; Liste l4 = init_liste(t4, 1);
    int t5[] = {7,7,7}; Liste l5 = init_liste(t5, 3);
    int t6[] = {1,2,3,3,2,2}; Liste l6 = init_liste(t6, 6);
    int t7[] = {1,2,3,4,2,1}; Liste l7 = init_liste(t7, 6);
    assert(estPalindrome(l1));
    assert(estPalindrome(l2));
    assert(estPalindrome(l3));
    assert(estPalindrome(l4));
    assert(estPalindrome(l5));
    assert(!estPalindrome(l6));
    assert(!estPalindrome(l7));
    VideListe(&l1); VideListe(&l2); VideListe(&l3);
    VideListe(&l4); VideListe(&l5); VideListe(&l6);
    VideListe(&l7);
  }
  
  //    sommeAvantApres
  {
    int t1[] = {}; Liste l1 = init_liste(t1, 0);
    int t2[] = {2048}; Liste l2 = init_liste(t2, 1);
    int t3[] = {12, 8, 12}; Liste l3 = init_liste(t3, 3);
    int t4[] = {2, 3, 5, -2, 4, 8, 9, -7, 10}; Liste l4 = init_liste(t4, 9);
    int t5[] = {2,3,0,4,-2,7}; Liste l5 = init_liste(t5, 6);
    int t6[] = {3,2,4}; Liste l6 = init_liste(t6, 3);
    int t7[] = {2,3,0,4,-2,6}; Liste l7 = init_liste(t7, 6);
    assert(!sommeAvantApres(l1));
    //    On considère l2 = [0, 2048, 0]
    assert(sommeAvantApres(l2));
    assert(sommeAvantApres(l3));
    assert(sommeAvantApres(l4));
    assert(sommeAvantApres(l5));
    assert(!sommeAvantApres(l6));
    assert(!sommeAvantApres(l7));
    VideListe(&l1); VideListe(&l2); VideListe(&l3);
    VideListe(&l4); VideListe(&l5); VideListe(&l6);
    VideListe(&l7);
  }
  
  //    Test des files
  {
    File f; initVideFile(&f);
    assert(estVideFile(f));
    ajouteFile(1, &f);
    assert(premierFile(f) == 1 && dernierFile(f) == 1);
    ajouteFile(2, &f);
    assert(premierFile(f) == 1 && dernierFile(f) == 2);
    ajouteFile(3, &f);
    assert(premierFile(f) == 1 && dernierFile(f) == 3);
    ajouteFile(4, &f);
    assert(premierFile(f) == 1 && dernierFile(f) == 4);
    //    f = [1,2,3,4]
    int x; sortirFile(&x, &f);
    assert(x == 1 && premierFile(f) == 2 && dernierFile(f) == 4);
    sortirFile(&x, &f);
    assert(x == 2 && premierFile(f) == 3 && dernierFile(f) == 4);
    sortirFile(&x, &f);
    assert(x == 3 && premierFile(f) == 4 && dernierFile(f) == 4);
    sortirFile(&x, &f);
    assert(x == 4 && estVideFile(f));
  }

  return 0;
}
