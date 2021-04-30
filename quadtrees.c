/************************************************/
/* TP ALGO L3 Université Paris-Saclay 2020-2021 */
/* Étudiant: Maxime Vincent                     */
/* Exercice 3: Quadtrees                        */
/* Compilation: gcc quadtrees.c -o quadtrees -lm*/
/* NB: L'option -lm permet d'inclure math.h     */
/************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>

/********************************/
/* Définition du type Quadtrees */
/********************************/

#define NTREES 4

typedef struct bloc_image {
  bool toutnoir;
  struct bloc_image *fils[NTREES];
} bloc_image;

typedef bloc_image *image;

/*****************************************/
/* Prototypes de fonctions et procédures */
/*****************************************/

//    Constructeurs de Quadtrees

image construit_blanc();
image construit_noir();
image construit_composee(image i1, image i2, image i3, image i4);
image copie(image I);
void rend_memoire(image *I);

//    Primitives

//    Renvoie la ième sous-image avec 0 <= i < 4 
//    L'image I doit être composée
image sous_image(image I, int i);
//    Pour modifier les fils d'une image
image *pointeur_sous_image(image I, int i);
image HG(image I); //   Sous-image haut-gauche
image HD(image I); //   Sous-image haut-droite
image BG(image I); //   Sous-image bas-gauche
image BD(image I); //   Sous-image bas-droite
//    Renvoie vrai si I est une image blanche non-composée
bool est_blanche(image I);
//    Renvoie vrai si I est une image noir non-composée
//    Ne peut être appelée que si I n'est pas blanche
bool est_noire(image I);
//    Renvoie vrai si I est une image composée
//    Ne peut être appelée que si I n'est pas blanche
bool est_composee(image I);

//    Affichages

void afficher_prefixe(image I);
void afficher_profondeur(image I);
//    afficher_2kpixel renommé en qqch qui fait moins peur
//    Avec p la profondeur de l'affichage ~ résolution
void afficher_matrice_ver1(image I, int p);
void afficher_matrice_ver2(image I, int p);

//    Prédicats

//    Renvoie vrai si I n'est composée 
//    que de sous-images blanches
bool est_toute_blanche(image I);
//    Renvoie vrai si I n'est composée 
//    que de sous-images noires
bool est_toute_noire(image I);
//    Même dessin mais pas forcément même structure
bool meme_dessin(image i1, image i2);

//    Autres fonctions

float aire(image I);
image difference(image i1, image i2);
//    Construit une image à partir de sa notation préfixe
image construit_image(const char *c);
//    Entrée utilisateur pour construire image avec
//    notation préfixe
void lecture_clavier();
//    h >= 0 représente la hauteur 
//    des sous-images pleines à compter
int compteSousImagesPleines(image I, int h);
//    p >= 0 représente la profondeur
void arrondit(image *I, int p);
void negatif(image *I);
void simplifie(image *I);
image alea(int p, int n);
image nebuleuse(int p);

/*******************************************/
/* Définitions des fonctions et procédures */
/*******************************************/

/**********************/
/*    Constructeurs   */
/**********************/

image construit_blanc() {
  return NULL;
}

image construit_noir() {
  image res = (image)malloc(sizeof(bloc_image));
  res->toutnoir = true;
  for (int i = 0; i < NTREES; i++) { res->fils[i] = NULL; }
  return res;
}

image construit_composee(image i1, image i2, image i3, image i4) {
  image res = (image)malloc(sizeof(bloc_image));
  res->toutnoir = false;
  res->fils[0] = i1; res->fils[1] = i2;
  res->fils[2] = i3; res->fils[3] = i4;
  return res;
}

image copie(image I) {
  if (est_blanche(I)) { return construit_blanc(); }
  else if (est_noire(I)) { return construit_noir(); }
  //    L'image est composée
  else {
    return construit_composee(copie(HG(I)), copie(HD(I)), 
                              copie(BG(I)), copie(BD(I)));
  }
}

//    Ne fonctionne pas avec des images compressées
//    (avec plusieurs fois le même pointeur)

//    @Nouveau: Factorisation du free final
void rend_memoire(image *I) {
  if (!est_blanche(*I)) { //    I != NULL
    if (est_composee(*I)) { 
      for (int i = 0; i < NTREES; i++) {
        rend_memoire(pointeur_sous_image(*I, i));
        (*I)->fils[i] = NULL;
      } 
      free(*I); *I = NULL;
    }
    // L'image est noire ou composée
    free(*I); *I = NULL;
  }
}

/********************/
/*    Primitives    */
/********************/
 
image sous_image(image I, int i) { return I->fils[i]; }
image *pointeur_sous_image(image I, int i) { return &(I->fils[i]); }

image HG(image I) { return I->fils[0]; }
image HD(image I) { return I->fils[1]; }
image BG(image I) { return I->fils[2]; }
image BD(image I) { return I->fils[3]; }

bool est_blanche(image I) { return I == NULL; }

bool est_noire(image I) { return I->toutnoir; }

bool est_composee(image I) { return !I->toutnoir; }

/********************/
/*    Affichages    */
/********************/

void afficher_prefixe(image I) {
  if (est_blanche(I)) { printf("B"); }
  else if (est_noire(I)) { printf("N"); }
  //    L'image est composée
  else {
    printf(".");
    for (int i = 0; i < NTREES; i++) {
      afficher_prefixe(sous_image(I, i));
    }
  }
}

void afficher_profondeur(image I) {
  void ap(image I, int p) {
    if (est_blanche(I)) { printf("B%d", p); }
    else if (est_noire(I)) { printf("N%d", p); }
    //    L'image est composée
    else {
      printf(".%d", p);
      for (int i = 0; i < NTREES; i++) {
        ap(sous_image(I, i), p + 1);
      }
    }
  }
  ap(I, 0);
}

//    Puissance entière avec n >= 0
__int128 power(int x, int n) {
  if (n == 0) { return 1; }
  else if (n > 0){
    __int128 res = 1;
    for (int i = 0; i < n; i++) { res *= x; }
    return res;
  }
  //    Erreur
  else { return -1; }
}

//    Cette version 1 respecte l'énoncé puisqu'elle
//    utilise un tableau statique à 2 dimensions
//    Il n'est donc pas de taille variable !
//    Sa taille est de D * D avec D une constante
void afficher_matrice_ver1(image I, int p) {
  //    Dimension de la matrice
  const int D = power(2, p);
  void ecrire(char matrice[D][D], bool blanc, int offset_i, int offset_j, int cote) {
    char c;
    if (blanc) { c = '.'; } else { c = '8'; }
    for (int i = offset_i; i < offset_i + cote; i++) {
      for (int j = offset_j; j < offset_j + cote; j++) {
        matrice[i][j] = c;
      }
    }
  }
  void fill(image I, char matrice[D][D], int offset_i, int offset_j, int cote) {
    if (est_blanche(I)) { 
      ecrire(matrice, true, offset_i, offset_j, cote); 
    }
    else if (est_noire(I)) {
      ecrire(matrice, false, offset_i, offset_j, cote);
    }
    //    L'image I est composée
    else if (cote / 2 < 1) { matrice[offset_i][offset_j] = '-'; }
    else {
      fill(HG(I), matrice, offset_i, offset_j, cote / 2);
      fill(HD(I), matrice, offset_i, offset_j + cote / 2, cote / 2);
      fill(BG(I), matrice, offset_i + cote / 2, offset_j, cote / 2);
      fill(BD(I), matrice, offset_i + cote / 2, offset_j + cote / 2, cote / 2);
    }
  }
  char matrice[D][D];
  fill(I, matrice, 0, 0, D);
  for (int i = 0; i < D; i++) {
    for (int j = 0; j < D; j++) {
      printf("%c", matrice[i][j]);
    }
    printf("\n");
  }
}

//    @Nouveau: Affichage à la volée sans tableau
//    Cette version 2 est moins efficace
//    car on parcourt plusieurs fois l'image
//    mais est moins coûteuse en mémoire
void afficher_matrice_ver2(image I, int p) {
  //    Dimension de la matrice
  const int D = power(2, p);
  //    Détermine le caractère à afficher
  //    à la coordonnée (x, y) dans la
  //    matrice carrée de taille cote
  char find_char(image I, int x, int y, int cote) {
    if (est_blanche(I)) { return '.'; }
    else if (est_noire(I)) { return '8'; }
    //    L'image est composée
    else {
      int cote_div_2 = cote / 2;
      //    On a atteint la profondeur
      //    maximale d'affichage
      if (cote_div_2 < 1) { return '-'; }
      //    Haut gauche
      else if (x < cote_div_2 && y < cote_div_2) {
        return find_char(HG(I), x, y, cote_div_2);
      }
      //    Haut droite
      else if (cote_div_2 <= x && y < cote_div_2) {
        return find_char(HD(I), x - cote_div_2, y, cote_div_2);
      }
      //    Bas gauche
      else if (x < cote_div_2 && cote_div_2 <= y) {
        return find_char(BG(I), x, y - cote_div_2, cote_div_2);
      }
      //    Bas droite
      else {
        return find_char(BD(I), x - cote_div_2, y - cote_div_2, cote_div_2);
      }
    }
  }
  
  //    On passe sur toutes les coordonées
  for (int y = 0; y < D; y++) {
    for (int x = 0; x < D; x++) {
      printf("%c", find_char(I, x, y, D));
    }
    printf("\n");
  }
}

/*******************/
/*    Prédicats    */
/*******************/

bool est_toute_blanche(image I) {
  if (est_blanche(I)) { return true; }
  else if (est_noire(I)) { return false; }
  //    L'image est composée
  else {
    bool res = true;
    for (int i = 0; i < NTREES && res; i++) {
      res = res && est_toute_blanche(sous_image(I, i));
    }
    return res;
  }
}

bool est_toute_noire(image I) {
  if (est_blanche(I)) { return false; }
  else if (est_noire(I)) { return true; }
  //    L'image est composée
  else {
    bool res = true;
    for (int i = 0; i < NTREES && res; i++) {
      res = res && est_toute_noire(sous_image(I, i));
    }
    return res;
  }
}

//    Le principe c'est de vérifier que 
//    pour une structure de i1 donnée
//    de couleur unie, dans la même partie 
//    de l'image de i2 on ne doit pas trouver 
//    l'autre couleur.
 
//    @Nouveau: Simplification des 2 premiers cas
bool meme_dessin(image i1, image i2) {
  if (est_blanche(i1)) {
    return est_toute_blanche(i2);
  }
  else if (est_noire(i1)) {
    return est_toute_noire(i2);
  }
  //    i1 est composée
  else {
    if (est_blanche(i2)) { return est_toute_blanche(i1); }
    else if (est_noire(i2)) { return est_toute_noire(i1); }
    //    i2 est composée
    else {
      bool res = true;
      for (int i = 0; i < NTREES && res; i++) {
        res = res && meme_dessin(sous_image(i1, i), sous_image(i2, i));
      }
      return res;
    }
  }
}

/***************************/
/*    Autres fonctions     */
/***************************/

//    @Nouveau: Sans sous-fonction
float aire(image I) {
  if (est_blanche(I)) { return 0.; }
  else if (est_noire(I)) { return 1.; }
  //    L'image est composée
  else {
    float res = 0.;
    //    L'aire de chaque sous-image vaut
    //    1 quart de l'image actuelle
    for (int i = 0; i < NTREES; i++) {
      res += aire(sous_image(I, i)) / 4.;
    }
    return res;
  }
}

//    Même schéma à 9 couples de meme_dessin
//    sauf qu'on construit une image ici
//    Quand les 2 images sont de couleurs différentes 
//    à en endroit on met du noir sinon du blanc

//    @Nouveau: Libération des images-outil
//    et simplification par symétrie des cas
image difference(image i1, image i2) {
  if (est_blanche(i1)) {
    if (est_blanche(i2)) { return construit_blanc(); }
    else if (est_noire(i2)) { return construit_noir(); }
    //    i2 est composée
    else { 
      //    On crée une image outil divisée en 4
      //    faite que de blanc pour concorder 
      //    avec la structure de i2
      image temp = construit_composee(
        construit_blanc(), construit_blanc(),
        construit_blanc(), construit_blanc()
      );
      image res = difference(temp, i2);
      //    On libère l'image outil
      rend_memoire(&temp);
      return res; 
    }
  }
  else if (est_noire(i1)) {
    if (est_blanche(i2)) { return construit_noir(); }
    else if (est_noire(i2)) { return construit_blanc(); }
    //    i2 est composée
    else {
      //    On crée une image outil divisée en 4
      //    faite que de noir pour concorder 
      //    avec la structure de i2 
      image temp = construit_composee(
        construit_noir(), construit_noir(),
        construit_noir(), construit_noir()
      );
      image res = difference(temp, i2);
      //    On libère l'image outil
      rend_memoire(&temp);
      return res;
    }
  }
  //    i1 est composée
  else {
    if (est_blanche(i2) || est_noire(i2)) {
      //    Symétrie des cas
      return difference(i2, i1);
    }
    //    i2 est composée
    //    On fait coincider chaque sous-image
    else {
      image sous_images[NTREES];
      for (int i = 0; i < NTREES; i++) {
        sous_images[i] = difference(sous_image(i1, i), sous_image(i2, i));
      }
      return construit_composee(
        sous_images[0], sous_images[1], 
        sous_images[2], sous_images[3]
      );
    }
  }
}

image construit_image(const char *c) {
  image construit(char **p) {
    //    Fin de chaîne
    if (**p != '\0') {
      if (**p == 'B') {
        (*p)++;
        return construit_blanc();
      }
      else if (**p == 'N') {
        (*p)++;
        return construit_noir();
      }
      else if (**p == '.') {
        bloc_image *images[NTREES]; (*p)++;
        for (int i = 0; i < NTREES; i++) {
          images[i] = construit(p);
        }
        return construit_composee(
          images[0], images[1], images[2], images[3]
        );
      }
      //    Dans les autres cas on passe au caractère suivant
      else {
        (*p)++;
        return construit(p);
      }
    }
  }
  char **p = (char **)&c;
  return construit(p);
}

//    Lit au maximum 256 caractères
//    @Nouveau: Construction de l'image à la volée
void lecture_clavier() {
  image lecture() {
    //    Appuyer sur entrée est nécessaire pour
    //    passer getchar. Si on a entré plus d'un
    //    caractère, les prochains appels à getchar
    //    prendront ces caractères.
    char c = getchar();
    if (c == 'B') { return construit_blanc(); }
    else if (c == 'N') { return construit_noir(); }
    else if (c == '.') {
      image images[NTREES];
      for (int i = 0; i < NTREES; i++) {
        images[i] = lecture();
      }
      return construit_composee(
        images[0], images[1],
        images[2], images[3]
      );
    }
    //    On ignore les autres caractères
    else { return lecture(); }
  }
  image res = lecture();
  //    Affichages divers
  printf("aire=%f\n", aire(res));
  afficher_prefixe(res); printf("\n");
  afficher_profondeur(res); printf("\n");
  //    On fixe la résolution à 3 (8x8)
  afficher_matrice_ver2(res, 3);
  rend_memoire(&res);
}

//    Voir dans test.ml pour le prototypage en OCaml
//    Mais la version C se suffit à elle-même
int compteSousImagesPleines(image I, int h) {
  //    (n, hsip) avec n le nombre de sous-images pleines
  //    et hsip la hauteur de la sous-image pleine
  void csip(image I, int *n, int *hsip) {
    //    Une feuille est considérée comme 
    //    une image pleine de hauteur 0
    if (est_blanche(I)) { *n = 1; *hsip = 0; }
    else if (est_noire(I)) { *n = 1; *hsip = 0; }
    //    L'image I est composée
    else {
      //    On parcourt l'arbre jusqu'à ses feuilles
      //    On enregistre les couples (n, hsip)
      //    de chaque appel à csip
      int tab_n[NTREES], tab_h[NTREES];
      csip(HG(I), n, hsip); tab_n[0] = *n; tab_h[0] = *hsip;
      csip(HD(I), n, hsip); tab_n[1] = *n; tab_h[1] = *hsip;
      csip(BG(I), n, hsip); tab_n[2] = *n; tab_h[2] = *hsip;
      csip(BD(I), n, hsip); tab_n[3] = *n; tab_h[3] = *hsip;
      //    Si l'une des hauteurs correspond à la hauteur 
      //    voulue de l'entête il faut faire remonter 
      //    le nombre de sous-images pleines de cette hauteur
      *n = 0; //    Réinitialisation obligatoire
      for (int i = 0; i < NTREES; i++) {
        if (tab_h[i] == h) { *n += tab_n[i]; }
      }
      //    Si on est passé par une des conditions précédentes
      //    il faut modifier la hauteur du couple 
      //    par la hauteur de l'entête
      if (*n > 0) { *hsip = h; }
      else {
        //    Si on compte le même nombre de sous_images 
        //    pleines d'une même hauteur g alors 
        //    l'image courante est 1 seule image 
        //    pleine de hauteur g + 1
        bool cond = true;
        for (int i = 1; i < NTREES && cond; i++) {
          cond = cond && tab_n[0] == tab_n[i] && tab_h[0] == tab_h[i];  
        }
        if (cond) { *n = 1; *hsip = tab_h[0] + 1; }
        //    Il n'y a pas sous_image pleine de hauteur h 
        //    de l'entête dans l'image courante
        else { *n = 0; *hsip = h; }
      }
    }
  }
  int n = 0, hsip = 0;
  csip(I, &n, &hsip);
  //    Il est possible d'avoir trouvé des sous-images pleines       
  //    de hauteur inférieure à h
  if (hsip == h) { return n; } 
  else { return 0; }
  return n;
}

//    @Nouveau: Suppression de la sous-fonction
void arrondit(image *I, int p) {
  //    l'arrondi n'a de sens que sur une image composée
  if (!est_blanche(*I) && est_composee(*I)) {
    //    profondeur de l'arrondi atteinte    
    if(p == 0) {
      float aire_sous_image = aire(*I);
      rend_memoire(I);
      if (aire_sous_image <= 0.5) {
          *I = construit_blanc();
      }
      else {
        *I = construit_noir();
      }
    }
    //    Cas récursif
    else {
      for (int i = 0; i < NTREES; i++) {
        arrondit(pointeur_sous_image(*I, i), p - 1);
      }
    }
  }
}

void negatif(image *I) {
  if (est_blanche(*I)) {
    *I = construit_noir();
  }
  else if (est_noire(*I)) {
    rend_memoire(I);
    *I = construit_blanc();
  }
  //    L'image I est composée
  else {
    for (int i = 0; i < NTREES; i++) {
      negatif(pointeur_sous_image(*I, i));
    } 
  }
}

/**************************************************************/
/* let rec simpl i =                                          */
/*     match i with                                           */
/*     | B -> B                                               */
/*     | N -> N                                               */
/*     | C(a, b, c, d) ->                                     */
/*         let res = C(simpl a, simpl b, simpl c, simpl d) in */
/*         match res with                                     */
/*         | C(B, B, B, B) -> B                               */
/*         | C(N, N, N, N) -> N                               */
/*         | _ -> res                                         */
/**************************************************************/

//    @Nouveau: Variable is_white
void simplifie(image *I) {
  //    La simplification ne s'applique que sur image composée
  if (!est_blanche(*I) && est_composee(*I)) {
    bool all_blanc = true, all_noir = true;
    for (int i = 0; i < NTREES; i++) {
      //    Appel récursif pour chaque sous-image
      simplifie(pointeur_sous_image(*I, i));
      //    Calculs de prédicats
      image sub_image = sous_image(*I, i);
      bool is_white = est_blanche(sub_image);
      all_blanc = all_blanc && is_white;
      all_noir = all_noir && !is_white && est_noire(sub_image);
    }
    //    Tout est blanc donc on simplifie par du blanc
    if (all_blanc) {
      rend_memoire(I);
      *I = construit_blanc();
    }
    //    Tout est noir donc on simplifie par du noir
    else if (all_noir) {
      rend_memoire(I);
      *I = construit_noir();
    }
    //    Dans les autres cas on ne fait rien
  }
}

//    Renvoie un nombre aléatoire floattant dans [0;1]
float randf() {
  return (float)rand() / (float)RAND_MAX;
}

/****************************************************************************/
/* let alea p n =                                                           */
/*   let rec alea k n m =                                                   */
/*     if k = p then (                                                      */
/*       if (float_of_int !n /. float_of_int !m) >= Random.float 1.0 then ( */
/*         n := !n - 1; m := !m - 1; N                                      */
/*       )                                                                  */
/*       else (                                                             */
/*         m := !m - 1; B                                                   */
/*       )                                                                  */
/*     )                                                                    */
/*     else                                                                 */
/*       C(alea (k+1) n m, alea (k+1) n m, alea (k+1) n m, alea (k+1) n m)  */
/*   in                                                                     */
/*   let dim = 2.** float_of_int p in                                       */
/*   alea 0 (ref n) (ref (int_of_float (dim *. dim)))                       */
/****************************************************************************/

//    @Nouveau: Simplification des images
image alea(int p, int n) {
  //    Le principe c'est de remplir 
  //    les pixels à la profondeur p
  //    en une seule passe en adaptant 
  //    la probabilité au fur et à mesure
  //    du remplissage. 
  //    On peut normalement prouver que cet algorithme
  //    conserve l'équiprobabilité d'obtenir 
  //    les différentes images possibles.
  image alea(int k, int *n, __int128 *card) {
    //    k est la profondeur courante
    if (k == p) {
      double proba_noir = (double) *n / (double) *card;
      //    Le pixel est noir avec une proba de n/card
      if (proba_noir > 0. && proba_noir >= (double)randf()) {
        //    Puisqu'on a placé un pixel noir on décrémente n
        //    et on décrémente le nombre de pixels 
        //    vus qui est la cardinal
        *n -= 1; *card -= 1;
        return construit_noir();
      }
      //    Le pixel a 1-(n/card) chance d'être blanc
      else {
        *card -= 1;
        return construit_blanc();
      }
    }
    //    On n'a pas atteint la profondeur maximale
    else {
      image res = construit_composee(
        alea(k+1, n, card),
        alea(k+1, n, card),
        alea(k+1, n, card),
        alea(k+1, n, card)
      );
      //    Pour éviter explosion mémoire
      simplifie(&res);
      return res;
    }
  }
  //    int128 nécessaire pour pouvoir calculer du 2^100
  const __int128 D = power(2, p); //   longueur côté 2^p
  __int128 card = D * D; //    aire de la matrice
  return alea(0, &n, &card);
}

image nebuleuse(int p) {
  
  //    Distance euclidienne entre 2 points du plan
  float dist(int x1, int y1, int x2, int y2) {
    return sqrtf(powf(((float)x1 - (float)x2), 2.) + powf(((float)y1 - (float)y2), 2.));
  }
  
  //    Calcul du milieu
  const int D = power(2, p);
  //    Même coord en x et y pour le milieu
  const int mxy = D / 2;
  
  //    Calcul de la distance maximale entre le milieu
  //    et le coin supérieur gauche de coord (0, 0)
  const float dmax = dist(mxy, mxy, 0, 0);
  
  //    Renvoie la probabilité d'être noir pour un pixel
  //    de coord (x, y). Plus le pixel est loin du milieu
  //    plus la proba tend vers 0 et plus le pixel est proche
  //    du milieu plus la proba tend vers 1.
  float proba(int x, int y) {
    float d = dist(mxy, mxy, x, y);
    return (dmax - d) / dmax;
  }
  
  image nebuleuse(int cote, int x, int y) {
    //    On est arrivé à la profondeur souhaitée
    //    car le cote est de taille 1
    if (cote == 1) {
      float proba_noir = proba(x, y);
      if (proba_noir > 0. && proba_noir >= randf()) {
        return construit_noir();
      }
      else {
        return construit_blanc();
      }
    }
    //    On n'a pas atteint la profondeur maximale
    else {
      //    On donne aux nouvelles coord (x, y) la valeur
      //    du coin haut gauche de chaque sous-image
      return construit_composee(
        nebuleuse(cote / 2, x, y),
        nebuleuse(cote / 2, x + (cote / 2), y),
        nebuleuse(cote / 2, x, y + (cote / 2)),
        nebuleuse(cote / 2, x + (cote / 2), y + (cote / 2))
      );
    }
  }
  return nebuleuse(D, 0, 0);
}

/*******************************/
/* Point d'entrée du programme */
/*******************************/

int main() {
  
  //    Initialisation de l'aléatoire
  srand(time(NULL));
  
  //    Tests des constructeurs, prédicats et affichages de base
  {
    image blanc = construit_blanc();
    assert(est_blanche(blanc));
    assert(est_toute_blanche(blanc) && !est_toute_noire(blanc));
    assert(aire(blanc) == 0.);
    
    image noir = construit_noir(); 
    assert(!est_blanche(noir) && est_noire(noir));
    assert(est_toute_noire(noir) && !est_toute_blanche(noir));
    assert(aire(noir) == 1.);
    
    image tout_blanc = construit_composee(
      construit_composee(
        construit_composee(blanc, blanc, blanc, blanc),
        blanc, blanc,
        construit_composee(blanc, blanc, blanc, 
          construit_composee(blanc, blanc, blanc, blanc)
        )
      ),
      blanc, blanc, construit_composee(blanc, blanc, blanc, blanc)
    );
    assert(est_toute_blanche(tout_blanc) && !est_toute_noire(tout_blanc));
    assert(aire(tout_blanc) == 0.);
    
    //    Tests est_toute_blanche et est_toute_noire
    image presque_tout_blanc = construit_composee(
      construit_composee(
        construit_composee(blanc, blanc, blanc, blanc),
        blanc, blanc,
        construit_composee(blanc, blanc, blanc, 
          construit_composee(blanc, blanc, blanc, blanc)
        )
      ),
      blanc, blanc, construit_composee(blanc, blanc, blanc, noir)
    );
    assert(!est_toute_blanche(presque_tout_blanc) && !est_toute_noire(presque_tout_blanc));
    assert(aire(presque_tout_blanc) == 0.25 * 0.25);
    
    image tout_noir = construit_composee(
      construit_composee(
        construit_composee(noir, noir, noir, noir),
        noir, noir,
        construit_composee(noir, noir, noir, 
          construit_composee(noir, noir, noir, noir)
        )
      ),
      noir, noir, construit_composee(noir, noir, noir, noir)
    );
    assert(est_toute_noire(tout_noir) && !est_toute_blanche(tout_noir));
    
    image presque_tout_noir = construit_composee(
      construit_composee(
        construit_composee(noir, noir, noir, noir),
        noir, noir,
        construit_composee(noir, noir, noir, 
          construit_composee(noir, noir, noir, noir)
        )
      ),
      noir, noir, construit_composee(noir, blanc, noir, noir)
    );
    assert(!est_toute_noire(presque_tout_noir) && !est_toute_blanche(presque_tout_noir));
    
    //    On rend la mémoire
    rend_memoire(&blanc); rend_memoire(&noir);
    //    On ne peut pas rendre le reste 
    //    car on a créé des images compressées...
  }
  
  //    Tests de constructions complexes, aire, copie et affichages de base
  {
    image qt1 = construit_composee(construit_noir(), construit_blanc(), 
    construit_blanc(), construit_noir());
    assert(est_noire(HG(qt1))); assert(est_blanche(HD(qt1)));
    assert(est_blanche(BG(qt1))); assert(est_noire(BD(qt1)));
    assert(!est_toute_blanche(qt1) && !est_toute_noire(qt1));
    assert(aire(qt1) == 0.5 * 0.5 * 2);
    
    image qt2 = construit_composee(
      construit_composee(construit_blanc(), construit_blanc(), 
      construit_blanc(), construit_noir()),
      construit_composee(construit_blanc(), construit_blanc(), 
      construit_noir(), construit_blanc()),
      construit_composee(construit_blanc(), construit_noir(), 
      construit_blanc(), construit_blanc()),
      construit_composee(construit_noir(), construit_blanc(), 
      construit_blanc(), construit_blanc())
    ); 
    assert(est_blanche(HG(HG(qt2)))); assert(est_blanche(HD(HG(qt2))));
    assert(est_blanche(BG(HG(qt2)))); assert(est_noire(BD(HG(qt2))));
    assert(est_blanche(HG(HD(qt2)))); assert(est_blanche(HD(HD(qt2))));
    assert(est_noire(BG(HD(qt2)))); assert(est_blanche(BD(HD(qt2))));
    assert(est_blanche(HG(BG(qt2)))); assert(est_noire(HD(BG(qt2))));
    assert(est_blanche(BG(BG(qt2)))); assert(est_blanche(BD(BG(qt2))));
    assert(est_noire(HG(BD(qt2)))); assert(est_blanche(HD(BD(qt2))));
    assert(est_blanche(BG(BD(qt2)))); assert(est_blanche(BD(BD(qt2))));
    assert(aire(qt2) == 0.25 * 0.25 * 4);
    
    //    Test de la copie
    image blanc = construit_blanc();
    image noir = construit_noir();
    image qt3 = construit_composee(noir, blanc, blanc, noir);
    assert(est_noire(HG(qt3))); assert(est_blanche(HD(qt3)));
    assert(est_blanche(BG(qt3))); assert(est_noire(BD(qt3)));
    image qt4 = copie(qt3); *noir = *qt1;
    assert(est_noire(HG(qt4))); assert(est_blanche(HD(qt4)));
    assert(est_blanche(BG(qt4))); assert(est_noire(BD(qt4)));
    assert(est_noire(HG(HG(qt3)))); assert(est_blanche(HD(HG(qt3))));
    assert(est_blanche(BG(HG(qt3)))); assert(est_noire(BD(HG(qt3))));
    assert(est_blanche(HD(qt3))); assert(est_blanche(BG(qt3)));
    assert(est_noire(HG(BD(qt3)))); assert(est_blanche(HD(BD(qt3))));
    assert(est_blanche(BG(BD(qt3)))); assert(est_noire(BD(BD(qt3))));
    
    image qt5 = construit_image(".N.BBNBB.N.NNB.NBNNBN");
    image qt6 = copie(qt5);
    assert(est_noire(HG(qt6)));
    assert(est_blanche(HG(HD(qt6)))); assert(est_blanche(HD(HD(qt6))));
    assert(est_noire(BG(HD(qt6)))); assert(est_blanche(BD(HD(qt6))));
    assert(est_blanche(BG(qt6)));
    assert(est_noire(HG(BD(qt6)))); 
    assert(est_noire(HG(HD(BD(qt6))))); assert(est_noire(HD(HD(BD(qt6)))));
    assert(est_blanche(BG(HD(BD(qt6))))); 
    assert(est_noire(HG(BD(HD(BD(qt6)))))); assert(est_blanche(HD(BD(HD(BD(qt6))))));
    assert(est_noire(BG(BD(HD(BD(qt6)))))); assert(est_noire(BD(BD(HD(BD(qt6))))));
    assert(est_blanche(BG(BD(qt6)))); assert(est_noire(BD(BD(qt6))));
    
    //    On rend la mémoire pour les images non-compressées
    rend_memoire(&qt1); rend_memoire(&qt2);
    rend_memoire(&qt5); rend_memoire(&qt6);
  }
  
  //    Test de meme_dessin
  {
    image qt1 = construit_composee(
      construit_noir(), construit_blanc(),
      construit_noir(), construit_composee(
        construit_blanc(), construit_blanc(),
        construit_blanc(), construit_composee(
          construit_noir(), construit_noir(),
          construit_noir(), construit_noir()
        )
      )
    );
    
    image qt2 = construit_composee(
      construit_composee(
        construit_noir(), construit_noir(),
        construit_noir(), construit_composee(
          construit_noir(), construit_noir(),
          construit_noir(), construit_composee(
            construit_noir(), construit_noir(),
            construit_noir(), construit_noir()
          )
        )
      ), construit_blanc(),
      construit_noir(), construit_composee(
        construit_blanc(), construit_composee(
          construit_blanc(), construit_blanc(),
          construit_blanc(), construit_blanc()
        ),
        construit_blanc(), construit_noir()
      )
    );
    
    assert(aire(qt1) == aire(qt2));
    assert(meme_dessin(qt1, qt2));
    
    //    On rend la mémoire
    rend_memoire(&qt1); rend_memoire(&qt2);
  }
  
  //    Tests de construit_image et difference
  {
    image blanc = construit_image("B");
    assert(est_blanche(blanc));
    assert(aire(blanc) == 0.);
    
    image noir = construit_image("N");
    assert(est_noire(noir));
    assert(aire(noir) == 1.);
    
    image qt1 = construit_image(".NBBN");
    assert(est_noire(HG(qt1))); assert(est_blanche(HD(qt1)));
    assert(est_blanche(BG(qt1))); assert(est_noire(BD(qt1)));
    assert(!est_toute_blanche(qt1) && !est_toute_noire(qt1));
    assert(aire(qt1) == 0.5 * 0.5 * 2);
    
    image qt2 = construit_image(".BNNB");
    assert(est_blanche(HG(qt2))); assert(est_noire(HD(qt2)));
    assert(est_noire(BG(qt2))); assert(est_blanche(BD(qt2)));
    assert(!est_toute_blanche(qt2) && !est_toute_noire(qt2));
    assert(aire(qt2) == 0.5 * 0.5 * 2);
    
    image qt3 = difference(qt1, qt2);
    assert(est_noire(HG(qt3))); assert(est_noire(HD(qt3)));
    assert(est_noire(BG(qt3))); assert(est_noire(BD(qt3)));
    assert(aire(qt3) == 0.5 * 0.5 * 4);
    
    image qt4 = construit_image(".N.BBNBB.N.NNB.NBNNBN");
    image qt5 = construit_image(".B.BBBNB.N.NBN.NBNBNB");
    image qt6 = difference(qt4, qt5);
    assert(est_noire(HG(qt6)));
    assert(est_blanche(HG(HD(qt6)))); assert(est_blanche(HD(HD(qt6))));
    assert(est_noire(BG(HD(qt6)))); assert(est_noire(BD(HD(qt6))));
    assert(est_blanche(BG(qt6)));
    assert(est_blanche(HG(BD(qt6)))); 
    assert(est_blanche(HG(HD(BD(qt6))))); assert(est_noire(HD(HD(BD(qt6)))));
    assert(est_noire(BG(HD(BD(qt6))))); 
    assert(est_blanche(HG(BD(HD(BD(qt6)))))); assert(est_blanche(HD(BD(HD(BD(qt6))))));
    assert(est_blanche(BG(BD(HD(BD(qt6)))))); assert(est_noire(BD(BD(HD(BD(qt6))))));
    assert(est_noire(BG(BD(qt6)))); assert(est_noire(BD(BD(qt6))));
    
    //    Avec des images de structures différentes ?
    image qt7 = construit_image(".N .BBNB B .N.NNB.NBNN B N");
    image qt8 =  construit_image(".B N .NBBB B");
    image qt9 = difference(qt7, qt8);
    assert(est_noire(HG(qt9)));
    assert(est_noire(HG(HD(qt9)))); assert(est_noire(HD(HD(qt9))));
    assert(est_blanche(BG(HD(qt9)))); assert(est_noire(BD(HD(qt9))));
    assert(est_noire(HG(BG(qt9)))); assert(est_blanche(HD(BG(qt9))));
    assert(est_blanche(BG(BG(qt9)))); assert(est_blanche(BD(BG(qt9))));
    assert(est_noire(HG(BD(qt9)))); 
    assert(est_noire(HG(HD(BD(qt9))))); assert(est_noire(HD(HD(BD(qt9)))));
    assert(est_blanche(BG(HD(BD(qt9))))); 
    assert(est_noire(HG(BD(HD(BD(qt9)))))); assert(est_blanche(HD(BD(HD(BD(qt9))))));
    assert(est_noire(BG(BD(HD(BD(qt9)))))); assert(est_noire(BD(BD(HD(BD(qt9))))));
    assert(est_blanche(BG(BD(qt9)))); assert(est_noire(BD(BD(qt9))));
    
    //    On rend la mémoire
    rend_memoire(&blanc); rend_memoire(&noir);
    rend_memoire(&qt1); rend_memoire(&qt2);
    rend_memoire(&qt3); rend_memoire(&qt4);
    rend_memoire(&qt5); rend_memoire(&qt6);
    rend_memoire(&qt7); rend_memoire(&qt8);
    rend_memoire(&qt9);
  }
  
  //    Test de compteSousImagesPleines
  {
    image blanc = construit_image("B");
    image noir = construit_image("N");
    image qt1 = construit_image(".NNBN");
    image qt2 = construit_image("..NNBN.NBBN.BBBN.NNNB");
    image qt3 = construit_image(". ..BBNB.NNBN.BBBN.NNNB N .NBN.BBNB .BNB ..BBNB.NBBN.BNBN.NBNB");
    image qt4 = construit_image("..BBB.BBBN .BBB.BBBN .BBB.BBBN .BBB.BBBN");
    
    assert(compteSousImagesPleines(blanc, 0) == 1);
    assert(compteSousImagesPleines(blanc, 1) == 0);
    assert(compteSousImagesPleines(blanc, -1) == 0);
    assert(compteSousImagesPleines(noir, 0) == 1);
    assert(compteSousImagesPleines(noir, 1) == 0);
    assert(compteSousImagesPleines(qt1, 0) == 4);
    assert(compteSousImagesPleines(qt1, 1) == 1);
    assert(compteSousImagesPleines(qt1, 2) == 0);
    assert(compteSousImagesPleines(qt2, 0) == 16);
    assert(compteSousImagesPleines(qt2, 1) == 4);
    assert(compteSousImagesPleines(qt2, 2) == 1);
    assert(compteSousImagesPleines(qt2, 3) == 0);
    assert(compteSousImagesPleines(qt3, 0) == 43);
    assert(compteSousImagesPleines(qt3, 1) == 9);
    assert(compteSousImagesPleines(qt3, 2) == 2);
    assert(compteSousImagesPleines(qt3, 3) == 0);
    assert(compteSousImagesPleines(qt4, 2) == 0);
    
    //    On rend la mémoire
    rend_memoire(&blanc); rend_memoire(&noir);
    rend_memoire(&qt1); rend_memoire(&qt2);
    rend_memoire(&qt3);
  }
  
  //    Test de arrondit 
  {
    image blanc = construit_image("B");
    image noir = construit_image("N");
    image qt1 = construit_image(".NNBN");
    image qt2 = construit_image(".NBBN");
    image qt3 = construit_image(".BBBN");
    image qt4 = construit_image(".N .BBNB B .N[.NNB(.NBNN)]BN");
    
    arrondit(&blanc, 0); arrondit(&noir, 0);
    arrondit(&qt1, 0); arrondit(&qt2, 0);
    arrondit(&qt3, 0); arrondit(&qt4, 2);
    
    assert(est_blanche(blanc)); assert(est_noire(noir));
    assert(est_noire(qt1)); assert(est_blanche(qt2));
    assert(est_blanche(qt3));
    assert(est_noire(HG(qt4))); 
    assert(est_blanche(HG(HD(qt4)))); assert(est_blanche(HD(HD(qt4))));
    assert(est_noire(BG(HD(qt4)))); assert(est_blanche(BD(HD(qt4))));
    assert(est_blanche(BG(qt4)));
    assert(est_noire(HG(BD(qt4)))); assert(est_noire(HD(BD(qt4))));
    assert(est_blanche(BG(BD(qt4)))); assert(est_noire(BD(BD(qt4))));
    
    //    On rend la mémoire
    rend_memoire(&blanc); rend_memoire(&noir);
    rend_memoire(&qt1); rend_memoire(&qt2);
    rend_memoire(&qt3); rend_memoire(&qt4);
  }
  
  //    Test de negatif
  {
    image blanc = construit_image("B");
    image noir = construit_image("N");
    image qt1 = construit_image(".NNBN");
    image qt2 = construit_image(".N .BBNB B .N[.NNB(.NBNN)]BN");
    
    negatif(&blanc); negatif(&noir);
    negatif(&qt1); negatif(&qt2);
    
    assert(est_noire(blanc)); assert(est_blanche(noir));
    assert(est_blanche(HG(qt1))); assert(est_blanche(HD(qt1)));
    assert(est_noire(BG(qt1))); assert(est_blanche(BD(qt1)));
    assert(est_blanche(HG(qt2))); 
    assert(est_noire(HG(HD(qt2)))); assert(est_noire(HD(HD(qt2))));
    assert(est_blanche(BG(HD(qt2)))); assert(est_noire(BD(HD(qt2))));
    assert(est_noire(BG(qt2)));
    assert(est_blanche(HG(BD(qt2)))); 
    assert(est_blanche(HG(HD(BD(qt2))))); assert(est_blanche(HD(HD(BD(qt2)))));
    assert(est_noire(BG(HD(BD(qt2)))));
    assert(est_blanche(HG(BD(HD(BD(qt2)))))); assert(est_noire(HD(BD(HD(BD(qt2))))));
    assert(est_blanche(BG(BD(HD(BD(qt2)))))); assert(est_blanche(BD(BD(HD(BD(qt2))))));
    assert(est_noire(BG(BD(qt2)))); assert(est_blanche(BD(BD(qt2))));
    
    //    On rend la mémoire
    rend_memoire(&blanc); rend_memoire(&noir);
    rend_memoire(&qt1); rend_memoire(&qt2);
  }
  
  //    Test de simplifie
  {
    image blanc = construit_image("B");
    image noir = construit_image("N");
    image qt1 = construit_image("..NN.NNNNN.BBBBN.BNBN");
    
    simplifie(&blanc); simplifie(&noir);
    simplifie(&qt1);
    
    assert(est_blanche(blanc)); assert(est_noire(noir));
    assert(est_noire(HG(qt1))); assert(est_blanche(HD(qt1)));
    assert(est_noire(BG(qt1)));
    assert(est_blanche(HG(BD(qt1)))); assert(est_noire(HD(BD(qt1))));
    assert(est_blanche(BG(BD(qt1)))); assert(est_noire(BD(BD(qt1))));
    
    //    On rend la mémoire
    rend_memoire(&blanc); rend_memoire(&noir);
    rend_memoire(&qt1);
  }
  
  //    Test de alea
  {
    image blanc = alea(0, 0);
    image noir = alea(0, 1);
    image qt1 = alea(1, 1);
    image qt2 = alea(1, 2);
    image qt3 = alea(1, 3);
    image qt4 = alea(1, 4);
    image qt5 = alea(4, 13);
    image qt6 = alea(4, 13);
    
    assert(est_blanche(blanc));
    assert(!est_blanche(noir) && est_noire(noir));
    assert(aire(qt1) == 1. * (0.5 * 0.5));
    assert(aire(qt2) == 2. * (0.5 * 0.5));
    assert(aire(qt3) == 3. * (0.5 * 0.5));
    assert(aire(qt4) == 4. * (0.5 * 0.5));
    assert(aire(qt5) == 13. * (0.0625 * 0.0625));
    assert(aire(qt6) == 13. * (0.0625 * 0.0625));
    
    printf("alea(4, 13)=\n"); afficher_matrice_ver2(qt5, 4); printf("\n");
    printf("alea(4, 13)=\n");afficher_matrice_ver2(qt6, 4); printf("\n");
    
    //    On rend la mémoire
    rend_memoire(&blanc); rend_memoire(&noir);
    rend_memoire(&qt1); rend_memoire(&qt2); rend_memoire(&qt3);
    rend_memoire(&qt4); rend_memoire(&qt5); rend_memoire(&qt6);
  }
  
  //    Test de nebuleuse
  {
    image qt1 = nebuleuse(6);
    printf("nebuleuse(6)=\n"); afficher_matrice_ver2(qt1, 6); printf("\n");
    rend_memoire(&qt1);
  }
  
  //    Mini-interpréteur de quadtrees
  printf("Please press CTRL+C to exit\n");
  while(true) {
    lecture_clavier();
  }
  
  return 0;
}
