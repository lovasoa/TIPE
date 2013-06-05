/*
Tout le code de ce fichier sert à obtenir un programme compilable
sur PC sans avoir la bibliothèque standard d'Arduino. Il contient notamment une définition
de certaines fonctions permettant d'exécuter le programme sur PC en loggant tout ce qu'il
fait, et en entrant des valeurs à la main à la place des valeurs mesurées par les composants réels
*/
#include <iostream>

using namespace std;

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

