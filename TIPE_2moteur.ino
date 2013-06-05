/*
Émil Garnel, TIPE 2012

LDR:    blanc : bas gauche    orange : haut droite    bleu : haut gauche
pin 0 : orange        pin 1 : bleu            pin 2 : blanc

moteur 2: sur l'arduino:  pin 10: vert ( haut droite du L293)    pin 11: blanc (bas droite du L293)
moteur 2: sur la plaque:  bleu: haut   rouge: bas

moteur 1 : sur l'arduino: pin 3: blanc (bas gauche du L293)     pin 9: orange (haut gauche du L293)
moteur 1 : sur la plaque: rouge: haut    noir: bas 
*/

int MOTORPLUS = 3;
int MOTORMOINS = 9;   

int MOTORPLUS2 = 10;     
int MOTORMOINS2 = 11;    


int Vmin1=120; //Valeur minimale à envoyer sur le premier moteur pour qu'il tourne
int Vmax1=150; //Valeur maximale à envoyer sur le premier moteur, correspondant à la vitesse maximale à laquelle la casserole est toujours en équilibre
int DIFFmax1=200;

int Vmin2=90;
int Vmax2=150;
int DIFFmax2=200;

int EPSILON2 =30;
int EPSILON = 30;

int V1;
int V2;

int lightPin0 = A0; 
int lightPin1 = A1;

int lightPin2 = A2;
// int lightPin3 = A3;

int initialise  = 0;
float tauxStandard  = 1;

int initialise2  = 0;
float tauxStandard2  = 1;


void setup() 
{ 
  // pinMode(MOTORPLUS , OUTPUT);  
  // pinMode(MOTORMOINS , OUTPUT);
  pinMode(lightPin0 , INPUT);
  pinMode(lightPin1 , INPUT); 

  // pinMode(MOTORPLUS2 , OUTPUT);  
  // pinMode(MOTORMOINS2 , OUTPUT);
  pinMode(lightPin2 , INPUT);
  //pinMode(lightPin3 , INPUT);

  Serial.begin(9600);
} 

void loop()
{

  float  lightLevel0 = tauxStandard*analogRead(lightPin0); 
  float lightLevel1 = analogRead(lightPin1);   

  int diff = lightLevel0-lightLevel1;
  diff = (diff>0) ? diff : -diff;

  if (initialise<5){
    Serial.println("Initialisation...");
    delay(400);
    if (lightLevel0 != 0){
      Serial.println("J'initialise le taux standard");
      tauxStandard = lightLevel1/lightLevel0;
    }
    initialise+=1;
  }

  if (diff<DIFFmax1){
  V1=diff*(Vmax1-Vmin1)/(DIFFmax1-EPSILON)+(Vmin1*DIFFmax1-Vmax1*EPSILON)/(DIFFmax1-EPSILON);
  } 
  else { 
  V1=Vmax1;
  }


  if (diff>EPSILON && initialise==5) {
    if (lightLevel0 > lightLevel1  ) {
      analogWrite(MOTORMOINS, 0);
      analogWrite(MOTORPLUS, V1);
    }
    else{
      analogWrite(MOTORMOINS, V1);
      analogWrite(MOTORPLUS, 0);
    }
  }
  else {
    analogWrite(MOTORMOINS, 0);
    analogWrite(MOTORPLUS, 0);  
  }

  Serial.print( "lightLevel0 : "); 
  Serial.print( lightLevel0);
  Serial.print ("   |  lightLevel1 : ");
  Serial.print( lightLevel1); 
  Serial.print("  |  diff : " ); 
  Serial.print( diff);
  Serial.print(" |  taux standard");
  Serial.print(tauxStandard);
  Serial.println ("");
  //delay(10);





  float  lightLevel2 = tauxStandard2*analogRead(lightPin2); 
  float lightLevel3 = analogRead(lightPin1);  

  int diff2 = lightLevel2-lightLevel3;
  diff2 = (diff2>0) ? diff2 : -diff2;

  if (initialise2<5){
    Serial.println("Initialisation...");
    delay(400);
    if (lightLevel2 != 0){
      Serial.println("J'initialise le taux standard2");
      tauxStandard2 = lightLevel3/lightLevel2;
    }
    initialise2+=1;
  }

  if (diff2<DIFFmax2){
   V2=diff2*(Vmax2-Vmin2)/(DIFFmax2-EPSILON2)+(Vmin2*DIFFmax2-Vmax2*EPSILON2)/(DIFFmax2-EPSILON2);
  } 
  else { 
    V2=Vmax2;
  }


  if (diff2>EPSILON2 && initialise2==5 && diff<EPSILON) {
    if (lightLevel2 > lightLevel3  ) {
      analogWrite(MOTORMOINS2, 0);
      analogWrite(MOTORPLUS2, V2);
    }
    else{
      analogWrite(MOTORMOINS2, V2*2);
      analogWrite(MOTORPLUS2, 0);
    }
  }
  else {
    analogWrite(MOTORMOINS2, 0);
    analogWrite(MOTORPLUS2, 0);  
  }

  Serial.print( "lightLevel2 : "); 
  Serial.print( lightLevel2);
  Serial.print ("   |  lightLevel3 : ");
  Serial.print( lightLevel3); 
  Serial.print("  |  diff2 : " ); 
  Serial.print( diff2);
  Serial.print(" |  taux standard2:");
  Serial.print(tauxStandard2);
  Serial.println ("");
  //delay(10);

}

