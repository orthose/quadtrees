/************************************************/
/* TP ALGO L3 Université Paris-Saclay 2020-2021 */
/* Étudiant: Maxime Vincent                     */
/* Exercice 1: Quelques calculs simples         */
/* Compilation: gcc calculation.c -o calculation*/
/************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

//    Dans toutes les fonctions que j'écrirai
//    je ne ferai pas de gestion d'exception.
//    Il est donc important de respecter la pré-condition
//    de chacune des fonctions concernant les arguments passés.

//    Variable gloable pour compter le nombre d'appels
//    des fonctions récursives
int count = 0;

/**
 * Calcule la valeur de exp^1
 * @param max: entier > 0 pour borne max de la somme
 * @return flottant de la valeur de l'exp
 **/
float exp_value(int max) {
  float exp_value(int n, int factn, float acc) {
    if (n > max) {
      return acc;
    }
    else {
        if (n > 0) factn = factn * n;
        return exp_value(n + 1, factn, acc + (1. / (float)factn));
    }
  }
  //    Initialisation de l'accumulateur de la somme à 0
  //    et de l'accumulateur de la factorielle à 1
  //    On somme à partir de n = 0
  return exp_value(0, 1, 0);
}

/**
 * Calcule x à la puissance n de manière naïve
 * @param x: réel != 0.
 * @param n: entier >= 0
 * @return le réel x^n
 **/
float fpower1(float x, int n) {
  float power(int n, float acc) {
    count++;
    if (n == 1) { return acc; }
    else { return power(n - 1, acc * x); }
  }
  if (n == 0) { count++; return 1.; } // Par convention
  else { return power(n, x); }
}

/**
 * Identique à fpower1 mais avec des réels double précision
 **/
double dpower1(double x, int n) {
  double power(int n, double acc) {
    count++;
    if (n == 1) { return acc; }
    else { return power(n - 1, acc * x); }
  }
  if (n == 0) { count++; return 1.; } // Par convention
  else { return power(n, x); }
}

/**
 * Calcule x à la puissance n avec la propriété
 * x^(n + p) = x^n * x^p
 * @param x: réel != 0.
 * @param n: entier >= 0
 * @return le réel x^n
 **/
float fpower2(float x, int n) {
  count++;
  if (n == 0) { return 1.; }
  else if (n == 1) { return x; }
  else {
    int m = n / 2;
    float y = fpower2(x, m);
    //    Cas où n modulo 2 = 0
    if (n == m * 2) { return y * y; }
    //    Cas où n modulo 2 = 1
    else { return x * y * y; }
  }
}

/**
 * Identique à fpower2 mais avec des réels double précision
 **/
double dpower2(double x, int n) {
  count++;
  if (n == 0) { return 1.; }
  else if (n == 1) { return x; }
  else {
    int m = n / 2;
    double y = dpower2(x, m);
    //    Cas où n modulo 2 = 0
    if (n == m * 2) { return y * y; }
    //    Cas où n modulo 2 = 1
    else { return x * y * y; }
  }
}
/**
 * Calcule x à la puissance n avec la propriété
 * x^(n * p) = (x^n)^p
 * @param x: réel != 0.
 * @param n: entier >= 0
 * @return le réel x^n
 **/
float fpower3(float x, int n) {
  count++;
  if (n == 0) { return 1.; }
  else if (n == 1) { return x; }
  else {
    int m = n / 2;
    float y = x * x;
    //    Cas où n modulo 2 = 0
    if (n == m * 2) { return fpower3(y, m); }
    //    Cas où n modulo 2 = 1
    else { return x * fpower3(y, m); }
  }
}

/**
 * Identique à fpower3 mais avec des réels double précision
 **/
double dpower3(double x, int n) {
  count++;
  if (n == 0) { return 1.; }
  else if (n == 1) { return x; }
  else {
    int m = n / 2;
    double y = x * x;
    //    Cas où n modulo 2 = 0
    if (n == m * 2) { return dpower3(y, m); }
    //    Cas où n modulo 2 = 1
    else { return x * dpower3(y, m); }
  }
}

/**
 * Calcule la fonction de Ackermann de manière semi-récursive
 * @param m: entier >= 0 correspondant à l'étage d'appel récursif
 * @param n: entier >= 0 correspondant dans le cas général 
 * au nombre d'appels récursif du premier étage à calculer
 **/
int ack1(int m, int n) {
  if (m == 0) { return n + 1; }
  int res = 1;
  for (int i = 0; i < n + 1; i++) {
    res = ack1(m - 1, res);
  }
  return res;
}

/**
 * Calcule la fonction de Ackermann de manière récursive pure
 * @param m: entier >= 0 correspondant à l'étage d'appel récursif
 * @param n: entier >= 0 correspondant dans le cas général 
 * au nombre d'appels récursif du premier étage à calculer
 **/
int ack2(int m, int n) {
  if (m == 0) { return n + 1; }
  else if (n == 0) { return ack2(m - 1, 1); }
  else { return ack2(m - 1, ack2(m, n - 1)); } 
}

/**
 * Suite X version récursive
 * @param n: entier >= 0
 **/
float x_rec(int n) {
  if (n == 0) { return 1.; }
  float y = x_rec(n - 1);
  return y + (1. / y);
}

/**
 * Suite X version itérative
 * @param n: entier >= 0
 **/
float x_iter(int n) {
  float res = 1.;
  for(int i = 1; i <= n; i++) {
    res = res + (1. / res);
  }
  return res;
}

/**
 * Vérifie que 2 nombres réels sont égaux à epsilon près
 * en effectuant la différence entre les deux
 * @param x, y: nombres réels
 * @param epsilon: réel de la différence de test entre x et y
 * @return true si |x - y| <= epsilon
 **/
bool egal_float(float x, float y, float epsilon) {
  float diff = x - y;
  if (diff >= 0.) {
    return diff <= epsilon;
  }
  else {
    return - diff <= epsilon;
  }
}

/**
 * Identique à egal_float mais avec des réels double précision
 **/
bool egal_double(double x, double y, double epsilon) {
  double diff = x - y;
  if (diff >= 0.) {
    return diff <= epsilon;
  }
  else {
    return - diff <= epsilon;
  }
}

int main(int argc, char **argv) {
    
  //    Test de la valeur de l'exponentielle
  assert(egal_float(exp_value(10), 2.718281, 0.000001));
  
  //    Tests de la fonction puissance
  assert(egal_float(fpower1(2., 0), 1., 0.1));
  assert(count == 1); count = 0;
  assert(egal_float(fpower1(2., 1), 2., 0.1));
  assert(count == 1); count = 0;
  assert(egal_float(fpower1(2., 2), 4., 0.1));
  assert(count == 2); count = 0;
  assert(egal_float(fpower1(2., 3), 8., 0.1));
  assert(count == 3); count = 0;
  assert(egal_float(fpower1(2., 4), 16., 0.1));
  assert(count == 4); count = 0;
  assert(egal_float(fpower1(2., 5), 32., 0.1));
  assert(count == 5); count = 0;
  
  assert(egal_float(fpower2(2., 0), 1., 0.1));
  assert(count == 1); count = 0;
  assert(egal_float(fpower2(2., 1), 2., 0.1));
  assert(count == 1); count = 0;
  assert(egal_float(fpower2(2., 2), 4., 0.1));
  assert(count == 2); count = 0;
  assert(egal_float(fpower2(2., 3), 8., 0.1));
  assert(count == 2); count = 0;
  assert(egal_float(fpower2(2., 4), 16., 0.1));
  assert(count == 3); count = 0;
  assert(egal_float(fpower2(2., 5), 32., 0.1));
  assert(count == 3); count = 0;
  
  assert(egal_float(fpower3(2., 0), 1., 0.1));
  assert(count == 1); count = 0;
  assert(egal_float(fpower3(2., 1), 2., 0.1));
  assert(count == 1); count = 0;
  assert(egal_float(fpower3(2., 2), 4., 0.1));
  assert(count == 2); count = 0;
  assert(egal_float(fpower3(2., 3), 8., 0.1));
  assert(count == 2); count = 0;
  assert(egal_float(fpower3(2., 4), 16., 0.1));
  assert(count == 3); count = 0;
  assert(egal_float(fpower3(2., 5), 32., 0.1));
  assert(count == 3); count = 0;
  
  //    Calculs avec la fonction puissance
  assert(egal_float(fpower1(1.1, 10), 2.593742, 0.000001));
  assert(count == 10); count = 0;
  assert(egal_float(fpower2(1.1, 10), 2.593742, 0.000001));
  assert(count == 4); count = 0;
  //    fpower3 est moins précise que fpower1 fpower2
  assert(egal_float(fpower3(1.1, 10), 2.593742, 0.000002));
  assert(count == 4); count = 0;
  
  assert(egal_float(fpower1(1.1, 100), 13780.61, 0.03));
  assert(count == 100); count = 0;
  //    fpower2 perd un peu de précision par rapport à fpower1
  assert(egal_float(fpower2(1.1, 100), 13780.61, 0.04));
  assert(count == 7); count = 0;
  assert(egal_float(fpower3(1.1, 100), 13780.61, 0.04));
  assert(count == 7); count = 0;
  
  //    power1(1.1, 1000) = infini
  assert(!egal_float(fpower1(1.1, 1000), 2.469932918e+41, 1.e+30)); count = 0;
  //    Par contre avec double précison cela fonctionne !
  //    Par contre il y a quand même une grande marge d'erreur 
  //    de l'ordre de 1.e+30 
  assert(egal_double(dpower1(1.1, 1000), 2.469932918e+41, 1.e+30));
  assert(count == 1000); count = 0;
  assert(egal_double(dpower2(1.1, 1000), 2.469932918e+41, 1.e+30));
  assert(count == 10); count = 0;
  assert(egal_double(dpower3(1.1, 1000), 2.469932918e+41, 1.e+30));
  assert(count == 10); count = 0;
  
  /** 
   * Conclusions sur les puissances.
   *
   * Globalement power1(x, n) fait n appels récursifs
   * tandis que power2(x, n) et power2(x, n) font ln2(n) appels récursifs.
   * Néanmoins power2 et power3 semblent faire plus d'approximations que
   * power1. Les fonctions power2 et power3 sont exactement identiques
   * en nombre d'appels récursifs. Enfin, avec n = 1000 il vaut mieux 
   * utiliser des double pour éviter de dépasser la taille des float 
   * et de se retrouver avec un résultat considéré comme infini.
   **/
  
  //    Tests fonction d'Ackermann
  assert(ack1(0, 0) == 1);
  assert(ack1(0, 1) == 2);
  assert(ack1(0, 2) == 3);
  assert(ack1(0, 3) == 4);
  assert(ack1(0, 4) == 5);
  assert(ack1(1, 0) == 2);
  assert(ack1(1, 1) == 3);
  assert(ack1(1, 2) == 4);
  assert(ack1(1, 3) == 5);
  assert(ack1(1, 4) == 6);
  assert(ack1(2, 0) == 3);
  assert(ack1(2, 1) == 5);
  assert(ack1(2, 2) == 7);
  assert(ack1(2, 3) == 9);
  assert(ack1(2, 4) == 11);
  assert(ack1(3, 4) == 125);
  
  assert(ack2(0, 0) == 1);
  assert(ack2(0, 1) == 2);
  assert(ack2(0, 2) == 3);
  assert(ack2(0, 3) == 4);
  assert(ack2(0, 4) == 5);
  assert(ack2(1, 0) == 2);
  assert(ack2(1, 1) == 3);
  assert(ack2(1, 2) == 4);
  assert(ack2(1, 3) == 5);
  assert(ack2(1, 4) == 6);
  assert(ack2(2, 0) == 3);
  assert(ack2(2, 1) == 5);
  assert(ack2(2, 2) == 7);
  assert(ack2(2, 3) == 9);
  assert(ack2(2, 4) == 11);
  assert(ack2(3, 4) == 125);
  
  //    Tests de la suite X
  assert(egal_float(x_rec(0), 1., 0.01));
  assert(egal_float(x_rec(1), 2., 0.01));
  assert(egal_float(x_rec(2), 2.5, 0.01));
  assert(egal_float(x_rec(3), 2.9, 0.01));
  assert(egal_float(x_iter(0), 1., 0.01));
  assert(egal_float(x_iter(1), 2., 0.01));
  assert(egal_float(x_iter(2), 2.5, 0.01));
  assert(egal_float(x_iter(3), 2.9, 0.01));
  float x_rec_100 = x_rec(100);
  float x_iter_100 = x_iter(100);
  assert(egal_float(x_rec_100, 14.28, 0.01));
  assert(egal_float(x_iter_100, 14.28, 0.01));
  assert(egal_float(x_rec_100, x_iter_100, 0.01));
  
  return 0;
}
