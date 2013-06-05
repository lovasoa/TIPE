#include <Servo.h> 
//#include <math.h> 
 
Servo servoNS;  // Servomoteur contrôlant l'orientation nord-sud
const int servoNSPin = 9; //pin sur lequel il est connecté

Servo servoEO; //Servomoteur contrôlant l'orientation est-ouest
const int servoEOPin = 8; //pin sur lequel il est connecté

const float lattitudeDeg = 48.5;
float lattitudeRad;

const int numJourInit = 1.5; // Jour de l'année du démarrage du programme 1.5 = 1er janvier à midi
double numJour = 1.5;

const double pi = 3.14159265359;

double equationDuTemps () {
  //Cf. http://fr.wikipedia.org/wiki/%C3%89quation_du_temps#Version_simplifi.C3.A9e_de_l.27.C3.A9quation_temps
  //Le résultat est en minutes
  double b = 2*pi*(numJour-81)/365;
  return 7.678*sin(b+1.374)-9.87*sin(2*b);
}

double declinaison () {
  double va = 360.0/365.25;
  return asin(0.3978 * va * sin(va * (numJour - (81 - 2*sin(va * (numJour-2))))));
}

double angleHoraire (double tempsSolaire) {
  return pi*tempsSolaire/11.;
}

double hauteur (double tempsSolaire) {
  double ah = angleHoraire(tempsSolaire);
  double dec = declinaison();
  return asin(sin(lattitudeRad)*sin(dec)+cos(lattitudeRad)*cos(dec)*cos(ah));
}

double azimut(double tempsSolaire) {
  return asin(cos(declinaison())*sin(angleHoraire(tempsSolaire))/cos(hauteur(tempsSolaire)));
}


double heureUTC () {
  return (numJour-floor(numJour))*12;
}


double MAJnumJour () {
  numJour = numJourInit + millis()/(24*60*60*1000);
}

void setup() {
  servoNS.attach(servoNSPin);  // attaches the servo on pin 9 to the servo object
  servoEO.attach(servoEOPin);
  
  lattitudeRad = lattitudeDeg*pi/180.;
}

void loop() 
{                          // waits for the servo to get there 
} 
