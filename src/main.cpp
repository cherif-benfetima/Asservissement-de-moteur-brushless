#include <Arduino.h>

#define Ha 10
#define Hb 9
#define Hc 8

#define tLin_U 5
#define TLin_V 6
#define TLin_W 7

#define THin_U 2
#define THin_V 3
#define THin_W 4
#define BUCK 12

bool a, b, c;

bool kl_U, kh_U, kl_V, kh_V, kl_W, kh_W;

long timestamp;


int alpha = 100;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(2000000);
    pinMode(Ha, INPUT);
    pinMode(Hb, INPUT);
    pinMode(Hc, INPUT);
    pinMode(BUCK, OUTPUT);

    pinMode(tLin_U, OUTPUT);
    pinMode(TLin_V, OUTPUT);
    pinMode(TLin_W, OUTPUT);

    pinMode(THin_U, OUTPUT);
    pinMode(THin_V, OUTPUT);
    pinMode(THin_W, OUTPUT);
    

    timestamp = micros();
}






bool lasta, lastb, lastc;
int i = 0;
float consigne = 1500.f;//tr/min
float vitesse_actuelle = 0.f;//tr/min
float Kp = 0.1f, Ki = 0.1f, Kd = 0.1f;
float erreur = 0.f, erreur_precedente = 0.f, somme_erreur = 0.f, delta_erreur = 0.f;
unsigned long dt = 0, dt_prec = 0;

void loop()
{
    long begintimestamp = micros();

    a = digitalRead(Ha);
    b = digitalRead(Hb);
    c = digitalRead(Hc);

    kl_U = not(a) and b;
    kh_U = a and not(b);

    kl_V = not(b) and c;
    kh_V = b and not(b);

    kl_W = not(c) and a;
    kh_W = not(a) and c;

    digitalWrite(THin_U, kh_U);
    digitalWrite(tLin_U, kl_U);

    digitalWrite(THin_V, kh_V);
    digitalWrite(TLin_V, kl_V);

    digitalWrite(THin_W, kh_W);
    digitalWrite(TLin_W, kl_W);

    if (lasta != a || lastb != b || lastc != c)
    {
        lasta = a;
        lastb = b;
        lastc = c;

        vitesse_actuelle = (vitesse_actuelle * 99 + 2500000 / (begintimestamp - timestamp)) / 100.; // (1000000 / delai) / 24 * 60
                                                                                  // (1s en us / delai en us) / 24 poles * 60 (conversion rpm)
        if (i++ >= 50)
        {
            Serial.print(0); // échelle de dessin (0 rpm min)
            Serial.print(" ");
            Serial.print(vitesse_actuelle);
            Serial.print(" ");
            Serial.println(10000); // échelle de dessin (10000 rpm max)
            i = 0;
        }

        timestamp = begintimestamp;
    }


    dt = micros();

    erreur = consigne - vitesse_actuelle;
    somme_erreur += (erreur * (dt - dt_prec)) / 1000000.0f; // Integrate over time
    delta_erreur = (erreur - erreur_precedente) / ((dt - dt_prec) / 1000000.0f); // Differentiate over time

    alpha = Kp * (consigne - vitesse_actuelle) + Ki * somme_erreur + Kd * delta_erreur;

    if(alpha < 0) {alpha = 0;}
    if(alpha > 255) {alpha = 255;}
    analogWrite(BUCK, alpha);

    erreur_precedente = erreur;

    dt_prec = dt;
}
