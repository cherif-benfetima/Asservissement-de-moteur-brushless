#include <Arduino.h>

#define Ha 10
#define Hb 9
#define Hc 8

#define TLin_U 5
#define TLin_V 6
#define TLin_W 7

#define THin_U 2
#define THin_V 3
#define THin_W 4
#define BUCK 12

bool a, b, c;

bool kl_U, kh_U, kl_V, kh_V, kl_W, kh_W;

long timestamp;

int alpha = 128;  // Valeur initiale du rapport cyclique du buck (entre 0 et 255)
double vitesse_consigne = 3000;  // Vitesse de consigne en RPM (ajustez cette valeur selon vos besoins)

bool lasta, lastb, lastc;
int i = 0;
double moyenne = 0;  // Vitesse moyenne (RPM)
long dernierDelai = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(2000000);
    pinMode(Ha, INPUT);
    pinMode(Hb, INPUT);
    pinMode(Hc, INPUT);
    pinMode(BUCK, OUTPUT);

    pinMode(TLin_U, OUTPUT);
    pinMode(TLin_V, OUTPUT);
    pinMode(TLin_W, OUTPUT);

    pinMode(THin_U, OUTPUT);
    pinMode(THin_V, OUTPUT);
    pinMode(THin_W, OUTPUT);

    timestamp = micros();
}

void loop() {
    long begintimestamp = micros();

    // Lire les capteurs Hall
    a = digitalRead(Ha);
    b = digitalRead(Hb);
    c = digitalRead(Hc);

    // Logique des phases du moteur
    kl_U = not(a) and b;
    kh_U = a and not(b);

    kl_V = not(b) and c;
    kh_V = b and not(b);

    kl_W = not(c) and a;
    kh_W = not(a) and c;

    digitalWrite(THin_U, kh_U);
    digitalWrite(TLin_U, kl_U);
  
    digitalWrite(THin_V, kh_V);
    digitalWrite(TLin_V, kl_V);
  
    digitalWrite(THin_W, kh_W);
    digitalWrite(TLin_W, kl_W);

    // Calcul de la vitesse
    if (lasta != a || lastb != b || lastc != c) {
        lasta = a;
        lastb = b;
        lastc = c;

        double delai = (begintimestamp - timestamp);  // Calcul du délai en microsecondes
        if (delai > 0) {
            moyenne = (moyenne * 99 + 2500000.0 / delai) / 100.0;  // Calcul de la vitesse moyenne (RPM)
        }

        // Asservissement de la vitesse
        if (moyenne < vitesse_consigne) {
            // Si la vitesse est trop faible, augmenter le rapport cyclique
            alpha = min(alpha + 1, 255);  // Limiter à 255 (maximum PWM)
        } else if (moyenne > vitesse_consigne) {
            // Si la vitesse est trop grande, diminuer le rapport cyclique
            alpha = max(alpha - 1, 0);  // Limiter à 0 (minimum PWM)
        }

        // Appliquer la commande PWM au BUCK
        analogWrite(BUCK, alpha);

        if (i++ >= 50) {
            // Afficher les informations de débogage toutes les 50 itérations
            Serial.print(0);  // échelle de dessin (0 RPM min)
            Serial.print(" ");
            Serial.print(moyenne);  // Afficher la vitesse actuelle
            Serial.print(" ");
            Serial.print(10000);  // échelle de dessin (10000 RPM max)
            Serial.print(" alpha: ");
            Serial.println(alpha);  // Afficher la valeur actuelle du rapport cyclique (alpha)
            i = 0;
        }

        timestamp = begintimestamp;  // Mettre à jour le timestamp pour le prochain calcul
    }
}