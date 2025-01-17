// Pilotage des phases (sorties)
#define Hin_U 13
#define Lin_U 12
#define Hin_V 11
#define Lin_V 10
#define Hin_W 9
#define Lin_W 8

// Acq Hall
#define capteur_a 7
#define capteur_b 6
#define capteur_c 5

// pilotage alpha
#define pin_alpha 4


#define PAS_ALPHA 5

bool a, b, c;
bool K1, K2, K3, K4, K5, K6;

long timestamp;
void setup() {
// put your setup code here, to run once:
// COnfiguration des phases à piloter
pinMode(Hin_U, OUTPUT);
pinMode(Lin_U, OUTPUT);
pinMode(Hin_V, OUTPUT);
pinMode(Lin_V, OUTPUT);
pinMode(Hin_W, OUTPUT);
pinMode(Lin_W, OUTPUT);

// Configuration de l'acq. des capteurs Hall
pinMode(capteur_a, INPUT);
pinMode(capteur_b, INPUT);
pinMode(capteur_c, INPUT);

// Configuration
pinMode(pin_alpha, OUTPUT);


Serial.begin(2000000);

timestamp = micros();
  // put your main code here, to run repeatedly:

}



bool lasta, lastb, lastc;
int i = 0;
double moyenne = 0;
int alpha = 128;


int valeur_actuelle = 0;

  int test = 0;
void loop() {

    long begintimestamp = micros();
    
    a = digitalRead(capteur_a);
    b = digitalRead(capteur_b);
    c = digitalRead(capteur_c);
  
    
    K1 = not(a) and (b); //K1 sur preparation
    if (K1==1) {
        digitalWrite(Lin_U, 0);
        digitalWrite(Hin_U, K1);
    }
    K2 = a and not(b); //K2 sur preparation
    if (K2==1) {
        digitalWrite(Hin_U, 0);
        digitalWrite(Lin_U, K2);
    }
    K3 = c and not(b); //K3 sur preparation
    if (K3==1) {
        digitalWrite(Lin_V, 0);
        digitalWrite(Hin_V, K3);
    }
    K4 = not(c) and (b); //K4 sur preparation
    if (K4==1) {
        digitalWrite(Hin_V, 0);
        digitalWrite(Lin_V, K4);
    }
    K5 = a and not(c); //K5 sur preparation
    if (K5==1) {
        digitalWrite(Lin_W, 0);
        digitalWrite(Hin_W, K5);
    }
    K6 = not(a) and c; //K6 sur preparation
    if (K6==1) {
        digitalWrite(Hin_W, 0);
        digitalWrite(Lin_W, K6);
    }

    
    analogWrite(pin_alpha, alpha);
    
    if(lasta!=a || lastb!=b || lastc!=c){
        lasta = a;
        lastb = b;
        lastc = c;
     
        valeur_actuelle = 2500000/(begintimestamp - timestamp);
       
        moyenne = (moyenne * 99 + valeur_actuelle)/100.;  // (1000000 / delai) / 24 * 60
                                                                               // (1s en us / delai en us) / 24 poles * 60 (conversion rpm)
        if(i++ >= 50){
             Serial.print(0);  // échelle de dessin (0 rpm min)
            Serial.print(" ");
            
            Serial.print(moyenne);
            Serial.print(" ");
            
            Serial.print(alpha);
            Serial.print(" ");
            Serial.print(1970); // échelle de dessin (10000 rpm max)
            Serial.print(" ");
            Serial.print(2030); // échelle de dessin (10000 rpm max)
            Serial.print(" ");
            Serial.print(4000); // échelle de dessin (10000 rpm max)
            Serial.println();
            i=0;
        }

        if(moyenne > 2030)
            alpha = (alpha < 5) ? 0 : (alpha-5);
        else if(moyenne < 1970)
            alpha = (alpha > (254-5)) ? (255) : (alpha+5);
                
        timestamp = begintimestamp;
    }

  
  
  // Serial.print("\r\n");
}
