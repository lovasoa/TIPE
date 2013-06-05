/*
Définition d'un nouveau type de variable : le type Axe. Il contient toutes les
données permettant de lire et d'analyser les données, puis de faire tourner un moteur
sur un axe
*/
typedef struct Axe Axe;
struct Axe
{
//Constantes:
    int pinMoteurPlus; // Le pin de la carte sur lequel la borne + du moteur est branchée 
    int pinMoteurMoins;
    
    int pinPhotoPlus; //Le pin de la photorésistance qui, si elle est plus éclairée que l'autre, doit faire alimenter pinMoteurPlus
    int pinPhotoMoins;

    int vMin; //La valeur minimale à écrire sur pinPlus et pinMoins, qui correspond à la vitesse minimale du moteur
    int vMax;

    int diffMin; //Valeur de la différence entre les valeurs retournées par les photorésistances considérée comme significative. En deçà de cette valeur, le moteur est arrêté. Lorsque cette valeur est atteinte, la valeur vMin est envoyée sur le moteur
    int diffMax; //Valeur de la différence entre les valeurs retournées par les photorésistances correspondant à vMax

//Variables:
    float tauxStandard; //Valeur définie lors de l'initialisation de la carte, qui permet de calibrer les photorésistances. Elle indique un taux par lequel il faut multiplier la valeur lue sur pinPhotoPlus pour obtenir une valeur comparable à celle lue sur pinPhotoMoins 
    int attente; //nombre de minutes qu'il reste avant de réactionner le moteur si nécessaire
    int attenteMax; //périodicité (en minutes) à laquelle vérifier si le moteur est bien positionné
};

void loop();
void setup();
