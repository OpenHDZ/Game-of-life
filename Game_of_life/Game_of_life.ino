#include <SPI.h>
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"

// les valeurs par défeaut pour de la liason entre la carte arduino et l'écrans tft
#define TFT_DC 9
#define TFT_CS 10

#define CELL 9  // nombre de cellule pris en compte lors des calcules
#define RAW  24 // taille de la grille du jeux
#define COL  32

// world est l'état actuelle du jeu 
// next est l'état future du jeu
uint8_t world [RAW][COL];
uint8_t next  [RAW][COL];

// nombre de voisins vivants de la cellule 
int neighbour;

// création de l'objet TFT
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {

  // initialisation de l'écran
  tft.begin();
  // mode paysage de l'écran
  tft.setRotation(1);
  // arrier plan de l'ecran en noire
  tft.fillScreen(ILI9341_BLACK);

  // initialisation de la grille du jeu
  grid_init();
    
}

void loop() {

  cal_neighbour();
  show_grid();
  delay(50);
  
}

void show_grid(){

  // affichage d e l'état du jeu 
  uint16_t x=0;
  uint16_t y=0;
  
  for (uint8_t i = 0; i < RAW; i++){
    x=0;
    for (uint8_t j = 0; j < COL; j++){      
      if (world[i][j] == 1) tft.fillRect(x,y,CELL,CELL, ILI9341_RED);             
      
      else                  tft.fillRect(x,y,CELL,CELL,ILI9341_WHITE);      

      x+=10;    
    }
    y+=10;    
  }  
}

void grid_init(){

  // initialisation aléatoire de la grille du jeu
  
  randomSeed(analogRead(0));
  for (uint8_t i = 0; i< RAW; i++){
    for (uint8_t j = 0; j< COL; j++){
      
      world[i][j]=int(random(2));
    
    }
  }
  show_grid();
}

void copy_tab(){

  // Enregistrement du nouvelle état du jeu 
  for (uint8_t i = 0; i< RAW; i++){
    for (uint8_t j = 0; j< COL; j++){
      
      world[i][j]=next[i][j];    
    }
  }
}

void cal_neighbour(){

  // calcule des voisins de chaque cellule du jeu et détermination du nouvelle état de la grille
  for (int i = 1; i< RAW-1; i++){
    for (int j = 1; j< COL-1; j++){
      
      neighbour=0;
      
      for (int k = -1; k <=1; k++){
        for (int l = -1; l<= 1; l++){
          neighbour += world[i+k][j+l];
        }
      }
      
      neighbour -= world[i][j];
            
      if      ((world[i][j] == 1) && (neighbour <  2)) next[i][j]=0;
      else if ((world[i][j] == 1) && (neighbour >= 4)) next[i][j]=0;
      else if ((world[i][j] == 0) && (neighbour == 3)) next[i][j]=1;
      else                                             next[i][j]=world[i][j];
  
    }
  }
  copy_tab();
}
