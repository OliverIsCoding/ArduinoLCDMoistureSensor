
/*
 * Code de base pour l'utilisation des capteurs capacitifs SKU:SEN0193
 * La valeur lu exprime le poucentage de remplissage de la RU*/

/* Dépendances */
#include <LiquidCrystal.h>
int percent;
const int Veau = 192; /*valeur de calibration du capteur dans l'eau */
const float b =5.485958085; /* b=ln(Vair-Veau) */
const long a = -3.610108303; /* a= -1/k avec k le coefficient spécifique au sol mesuré*/
float x;
float pf = 7.03; /* point de flétrissement en Hv%*/
float cpp = 10.87; /*capacité au champs exprimée en Hv% /*

/** Objet LiquidCrystal pour communication avec l'écran LCD */

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
const int LCD_NB_ROWS = 2;
const int LCD_NB_COLUMNS = 16;

/* Caractères personnalisés */
byte START_DIV_0_OF_1[8] = {
  B01111, 
  B11000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11000,
  B01111
}; // Char début 0 / 1

byte START_DIV_1_OF_1[8] = {
  B01111, 
  B11000,
  B10011,
  B10111,
  B10111,
  B10011,
  B11000,
  B01111
}; // Char début 1 / 1

byte DIV_0_OF_2[8] = {
  B11111, 
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
}; // Char milieu 0 / 2

byte DIV_1_OF_2[8] = {
  B11111, 
  B00000,
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B11111
}; // Char milieu 1 / 2

byte DIV_2_OF_2[8] = {
  B11111, 
  B00000,
  B11011,
  B11011,
  B11011,
  B11011,
  B00000,
  B11111
}; // Char milieu 2 / 2

byte END_DIV_0_OF_1[8] = {
  B11110, 
  B00011,
  B00001,
  B00001,
  B00001,
  B00001,
  B00011,
  B11110
}; // Char fin 0 / 1

byte END_DIV_1_OF_1[8] = {
  B11110, 
  B00011,
  B11001,
  B11101,
  B11101,
  B11001,
  B00011,
  B11110
}; // Char fin 1 / 1


/**
 * Fonction de configuration de l'écran LCD pour la barre de progression.
 * Utilise les caractères personnalisés de 0 à 6 (7 reste disponible).
 */
void setup_progressbar() {

  /* Enregistre les caractères personnalisés dans la mémoire de l'écran LCD */
  lcd.createChar(0, START_DIV_0_OF_1);
  lcd.createChar(1, START_DIV_1_OF_1);
  lcd.createChar(2, DIV_0_OF_2);
  lcd.createChar(3, DIV_1_OF_2);
  lcd.createChar(4, DIV_2_OF_2);
  lcd.createChar(5, END_DIV_0_OF_1);
  lcd.createChar(6, END_DIV_1_OF_1);
}
void draw_progressbar(byte percent) {
 
  /* Affiche la nouvelle valeur sous forme numérique sur la première ligne */
  lcd.setCursor(0, 0);
  lcd.print(percent);
  lcd.print(F(" %  ")); 
  /*N.B. Les deux espaces en fin de ligne permettent d'effacer les chiffres du pourcentage
 précédent quand on passe d'une valeur à deux ou trois chiffres à une valeur à deux ou un chiffres.*/
 
  /* Déplace le curseur sur la seconde ligne */
  lcd.setCursor(0, 1);
 
  /* Map la plage (0 ~ 100) vers la plage (0 ~ LCD_NB_COLUMNS * 2 - 2) */
  byte nb_columns = map(percent, 0, 100, 0, LCD_NB_COLUMNS * 2 - 2);
  // Chaque caractère affiche 2 barres verticales, mais le premier et dernier caractère n'en affiche qu'une.

  /* Dessine chaque caractère de la ligne */
  for (byte i = 0; i < LCD_NB_COLUMNS; ++i) {

    if (i == 0) { // Premiére case

      /* Affiche le char de début en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        lcd.write(1); // Char début 1 / 1
        nb_columns -= 1;

      } else {
        lcd.write((byte) 0); // Char début 0 / 1
      }

    } else if (i == LCD_NB_COLUMNS -1) { // Derniére case

      /* Affiche le char de fin en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        lcd.write(6); // Char fin 1 / 1

      } else {
        lcd.write(5); // Char fin 0 / 1
      }

    } else { // Autres cases

      /* Affiche le char adéquat en fonction du nombre de colonnes */
      if (nb_columns >= 2) {
        lcd.write(4); // Char div 2 / 2
        nb_columns -= 2;

      } else if (nb_columns == 1) {
        lcd.write(3); // Char div 1 / 2
        nb_columns -= 1;

      } else {
        lcd.write(2); // Char div 0 / 2
      }
    }
  }
}
/*Affiche le nom de l'utilisateur*/
void setup(){
  lcd.begin(16, 2);
 lcd.print("Boitier Hubis");
 delay(1000);
 lcd.clear();
  /* Initialise l'écran LCD */
  lcd.begin(LCD_NB_COLUMNS, LCD_NB_ROWS);
  setup_progressbar();
  lcd.clear();
}
 

/** Fonction loop() */

void loop(){
 x= a*(log(analogRead(A1)-Veau )-b);
  
if (analogRead(A1)<= Veau) /*renvoi 100% si la valeur est inférieur à la valeur de calibration de l'eau*/
{
percent = 100;
}
else if (x <= pf)
 {
percent = 0;
 }
 else if (x >= cpp) 
 {
 percent = 100;
 }
 else
 {
  x=(x-pf)*100/(cpp-pf); /*exprime la valeur en % de RU*/
  percent = x;
 
 }
 
  /* Affiche la valeur */
  draw_progressbar(percent);

 
    delay(1000);
  }
