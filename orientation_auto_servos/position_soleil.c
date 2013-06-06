#include <stdio.h>
#include <math.h>


//Coordonnées décimales de Fontenay aux roses (48° 46′ 43″ Nord et 2° 17′ 26″ Est )
float latitudeDeg = 48.7786;
float longitudeDeg = 2.2905;
float latitudeRad;

float fuseauHoraire = 1; //Nombre d'heures à enlever aux heures données pour tomber sur l'heure GMT (prend en compte l'heure d'été)

double numJourInit = 0; // Jour de l'année du démarrage du programme 1.5 = 1er janvier à midi
double numJour = 0;

//---------- Constantes mathématiques
const double pi = 3.141592653589795;
double deg2rad = 0.01745329251;//180./pi;
double rad2deg = 57.2957795131;//pi/180

double equationDuTemps () {
  //Cf. http://fr.wikipedia.org/wiki/%C3%89quation_du_temps#Version_simplifi.C3.A9e_de_l.27.C3.A9quation_temps
  //Le résultat est en minutes
  double b = 2*pi*(numJour-81)/365;
  return 7.678*sin(b+1.374)-9.87*sin(2*b);
}

double heureLocale () {
  return (numJour-floor(numJour))*24;
}

double tempsSolaire() {
	//Retourne le temps solaire en heures
	//un degré de longitude est égal à 4 minutes de temps solaire
	return heureLocale() - fuseauHoraire + (longitudeDeg*4 - equationDuTemps())/60;
}

double declinaison () {
  double va = 2*pi/365.25;
	//printf("Déclinaison: %g°\n",23.45*sin(va*(284+numJour)));
	return deg2rad*23.45*sin(va*(284+numJour));
}

double angleHoraire () {
  return pi * (tempsSolaire()-12)/12;
}

double hauteur (double ah, double dec) {
	/*
	paramètres:
		ah : angle horaire (en radians)
		dec : déclinaison (en radians)
	retour:
		double hauteur (en radians)
	*/
	return asin(sin(latitudeRad)*sin(dec) + cos(latitudeRad)*cos(dec)*cos(ah));
}

double azimut (double ah, double dec, double h) {
	/*
	paramètres:
		ah : angle horaire (en radians)
		dec : déclinaison (en radians)
		h : hauteur (en radians)
	retour:
		double hauteur (en radians)


	Explication de la formule utilisée:
		http://perso.limsi.fr/Individu/bourdin/master/Calculs_astronomiques_simples.pdf
	Les deux orthographes (azimuth et azimut) sont correctes.
	*/
	double azimuth = asin( cos(dec)*sin(ah) / cos(h)); //azimuth compté à partir du sud
	azimuth += pi;//On compte l'azimuth à partir du nord
	return azimuth;
}

double getNumJour(int jour, int mois, int annee, int heure, int minutes, int secondes) {
	double num = 0; //num va contenir le nombre de jours depuis le début de l'année
	double jours_dans_mois[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (annee%4==0) jours_dans_mois[2] = 29; //Annee bissextile
	if (mois<1 || mois>12) return -1; //Le numéro de mois doit être entre 1 et 12

	int numMois;
	for (numMois=1; numMois<mois; numMois++) num += jours_dans_mois[numMois-1];

	num += jour;

	num += (heure + (float)(minutes)/60 + (float)(secondes)/3600)/24;
	return num;
}

#define char2num(c) (c-'0')
double numJourFromDate(char date[],char time[]) {
	// date is like "Feb 12 1996"
	// time is like "23:59:01"
	
	char months[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
	int numMois;
	for (numMois=0; numMois<12; numMois++) {
		if(months[3*numMois] == date[0] &&
			months[3*numMois+1] == date[1] &&
			months[3*numMois+2] == date[2]) {
				break;
			}
	}
	numMois++; //Le numéro de mois commence à 1
	
	int jour;
	jour = char2num(date[5]);
	if (date[4] != ' ') jour += 10*char2num(date[4]);
	
	int annee=0;
	for (int i=7; i<11; i++) annee = annee*10+char2num(date[i]);

	int heure = 10*char2num(time[0]) + char2num(time[1]);
	int minutes = 10*char2num(time[3]) + char2num(time[4]);
	int secondes = 10*char2num(time[6]) +char2num(time[7]);

	return getNumJour(jour, numMois, annee, heure, minutes, secondes);
}


#ifndef ARDUINO
unsigned long millis() {
	//Simulation
	return 0;
}
#endif

void MAJnumJour () {
  numJour = numJourInit + (double)millis()/(24*60*60*1000);
}

void calculAzimuthEtHauteur (void) {
	//n est le numéro du jour dans l'année
	double ah = angleHoraire(); printf("ah:%g\n",ah);
	double dec = declinaison();printf("dec:%g\n",dec);
	double h = hauteur(ah, dec);
	double alpha = azimut(ah, dec, h);
	
	printf("hauteur : %g°\nazimuth : %g°\n\n", rad2deg*h, rad2deg*alpha);
}

void init (void) {
	latitudeRad = latitudeDeg * deg2rad;
	numJourInit = numJourFromDate(__DATE__, __TIME__);
	numJour = numJourInit;
	printf("numJour compilation: %g\n", numJour);
}

	
int main (void) {
	init();

	char dateTimeStr[22] = "Feb 12 1996 23:59:01"; // 22 = 12 + 1 + 8 + 1 ("Feb 12 1996 23:59:01\0")

	char *date = dateTimeStr;
	char *time = dateTimeStr+12;

	while (1) {
		printf("\nEntrez une date et une heure au format suivant: Feb 12 1996 23:59:01\n>");
		fgets(dateTimeStr, 22, stdin);
		dateTimeStr[11] = '\0';
		numJour = numJourFromDate(date, time);
		printf("Le numéro du jour choisi est: %g (la partie après la virgule indique l'heure)\n", numJour);
		calculAzimuthEtHauteur();
	}

	printf(" \n");
	return 0;
}
