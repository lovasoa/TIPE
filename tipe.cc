/*
Sketch pour une carte Arduino permettant de commander la rotation de moteurs à
partir de photorésistances, afin d'orienter un plan face au soleil.
Émil Garnel, TIPE 2012

Rappel de syntaxe du C:
 écrire
    variable = (condition) ? a : b;
 est équivalent à:
    if (condition) variable = a;
    else variable = b;

Je laisse ici tes commentaires tels quels, car je n'ai pas tout compris:
LDR:    blanc : bas gauche    orange : haut droite    bleu : haut gauche
pin 0 : orange        pin 1 : bleu            pin 2 : blanc

moteur 2: sur l'arduino:  pin 10: vert ( haut droite du L293)    pin 11: blanc (bas droite du L293)
moteur 2: sur la plaque:  bleu: haut   rouge: bas

moteur 1 : sur l'arduino: pin 3: blanc (bas gauche du L293)     pin 9: orange (haut gauche du L293)
moteur 1 : sur la plaque: rouge: haut    noir: bas 
*/

#include "tipe.h"

#ifdef SIMULATION
/*
Tout le code entre #ifdef SIMULATION et #endif  sert à obtenir un programme compilable
sur PC sans avoir la bibliothèque standard d'Arduino. Il contient notamment une définition
de certaines fonctions permettant d'exécuter le programme sur PC en loggant tout ce qu'il
fait, et en entrant des valeurs à la main à la place des valeurs mesurées par les composants réels
*/
#include <iostream>

using namespace std;

#define abs(x) ((x>0)?x:-x)
#define A0 0
#define A1 1
#define A2 2

#define INPUT 666
#define OUTPUT 999

class Port {
    public:
    void begin(int speed){
        cerr << "Serial.begin " << speed << endl;
    }
    void print(string str) {
        cout << str;
    }
    void print(int str) {
        cout << str;
    }
    void print(float str) {
        cout << str;
    }
    void println(string str) {
        cout << str << endl;
    }
};
Port Serial;
void pinMode(int pin, int mode) {
    cerr << "pinMode " << pin << " " << mode << endl;
}
void analogWrite(int pin, int value) {
    cerr << endl << "analogWrite " << pin << " " << value << endl << endl;
}
int analogRead (int pin) {
    int ret;
    cerr << "Serial.read " << pin << endl;
    cerr << "Entrez la valeur à retourner : ";
    cin >> ret;
    return ret;
}
void delay(int ms) {
    cerr << "delay " << ms << endl;
}


int main () {
    setup();
    while (1) loop();
	return 1;
}
#endif


#define NBRAXES 2 //nombre d'éléments dans le tableau axes

//Données des moteurs. Les constantes ont été trouvées empiriquement
Axe axes[] = {
//Premier axe (axes[0])
{
    3, //pinMoteurPlus
    9, //pinMoteurMoins
    
    A0, //pinPhotoPlus
    A1, //pinPhotoMoins
    
    120, //vMin
    150, //vMax
    
    30, //diffMin
    200, //diffMax
    
    1, //tauxStandard (cette valeur sera modifiée lors de l'initialisation)
    0, //attente
    5, // attenteMax
},
//Deuxième moteur (axes[1])
{
    10, //pinMoteurPlus
    11, //pinMoteurMoins

    A2, //pinPhotoPlus
    A1, //pinPhotoMoins

    90, //vMin
    150, //vMax

    30, //diffMin
    200, //diffMax
    
    1, //tauxStandard (cette valeur sera modifiée lors de l'initialisation)
    0, //attente
    3, // attenteMax
}
};



void setup() 
{
// Pour l'instant, on n'utilise aucun 'pin digital', don on n'a pas besoin de pinMode(pin, INPUT) 
  Serial.begin(9600);

    //Calibrage
    Serial.println("Début de l'initialisation, veuillez exposer toutes les photorésistances à la même intensité lumineuse...");
    delay(1000); //Inutile ?
    for (int i=0; i<NBRAXES;i++){ //Pour chaque axe
        //On suppose que les photorésistances retournent une valeur définie à une constante multiplicative près
        axes[i].tauxStandard = (float)analogRead(axes[i].pinPhotoMoins) / (float)analogRead(axes[i].pinPhotoPlus);
        Serial.print("Taux standard : "); Serial.print(axes[i].tauxStandard);
        Serial.println("\nInitialisé!\n");
    }
}

//Positionne le moteur correspondant à l'axe i
void positionner_moteur(int i) {
    int V;
    do {
    float lightLevelPlus = axes[i].tauxStandard * analogRead(axes[i].pinPhotoPlus),    //Valeur retournée par la photorésistance
                lightLevelMoins = analogRead(axes[i].pinPhotoMoins);
        
        /*Interpolation de la valeur à écrire à partir des données suivantes:
            Pour une différence de diffMin, on écrit Vmin
            Pour une différence de diffMax, on écrit Vmax
            Pour une différence entre diffMin et diffMax, on interpole la valeur à écrire linéairement
            Pour une valeur supérieure à diffmax, on écrit vMax
        */
        float pente = (float) (axes[i].vMax - axes[i].vMin) / (axes[i].diffMax - axes[i].diffMin),
                point = (float) (axes[i].vMin*axes[i].diffMax  - axes[i].vMax*axes[i].diffMin) / (axes[i].diffMax - axes[i].diffMin),
                diff = lightLevelPlus - lightLevelMoins;

        V = abs(diff) * pente + point;

        if (V>axes[i].vMax) V = axes[i].vMax; //On empêche V de dépasser vMax
        if (V<axes[i].vMin) V = 0; //Si on allait écrire une valeur qui n'allait de toute façon pas faire tourner le moteur, autant écrire 0 

        analogWrite(axes[i].pinMoteurPlus, (diff>0) ? V : 0 ); //On écrit la composante positive de V sur pinMoteurPlus
        analogWrite(axes[i].pinMoteurMoins, (diff<0) ? V : 0 ); // Et la composante négative sur pinMoteurMoins
        delay(20); //Attente de 20ms avant de changer la vitesse du moteur
    } while (V!=0);
    axes[i].attente = axes[i].attenteMax;
}

void loop() {
    for (int i=0; i<NBRAXES;i++) { //Pour chaque axe
        if  (axes[i].attente == 0) positionner_moteur(i);
        else axes[i].attente--;
    }
    delay(60*1000);
}
