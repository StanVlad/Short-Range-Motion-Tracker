#include <Stepper.h>
#include <SharpIR.h>

#define STG A0 //pin pentru senzorul stang
#define DR A1 //pin pentru senzorul drept
#define model 430 //modelul de senzor

const int pasiPerRotatie = 130; //numarul de impulsuri magnetice dintr-o miscare
int distanta1 = 0; //in aceste variabile vom retine distantele masurate
int distanta2 = 0;
int nrStanga = 0; //in aceste variabile vom retine deciziile luate
int nrStop = 0;
int nrDreapta = 0;

Stepper motor(pasiPerRotatie, 8, 10, 9, 11);// declararea motorului
SharpIR stanga(STG, model); //declararea senzorilor
SharpIR dreapta(DR, model);

int modul(int a)
{
  if (a > 0)
    return a;
  else
    return -a;
}

int maxim(int a, int b, int c)
{
  if (a > b)
  {
    if (a > c)
      return a;
    return c;
  }
  else
  {
    if (b > c)
      return b;
    return c;
  }
}

int look()
{
  distanta1 = stanga.distance() + 2; //se fac masuratorile cu ambii senzori
  distanta2 = dreapta.distance() + 2;
  
  Serial.print("D1: "); //pentru usurinta testarii
  Serial.println(distanta1);
  Serial.print("D2: ");
  Serial.println(distanta2);

  if ((distanta1 < 10) && (distanta2 < 10)) //obiectul e localizat
    return 0;
  if ((distanta1 >= 10) && (distanta2 < 10)) //obiectul s-a miscat spre dreapta robotului
    return 1;
  if ((distanta1 < 10) && (distanta2 >= 10)) //obiectul s-a miscat spre stanga robotului
    return -1;
}

void reinitializare()
{
  nrStanga = 0;
  nrStop = 0;
  nrDreapta = 0;
}

void setup() {
  motor.setSpeed(90); //viteza cu care se misca robotul
  Serial.begin(9600);
}

void loop() {

  for (int i = 0; i < 10; i++) //pentru a evita valori eronate, procedeul se executa de 10 ori
  {
    int decizie = look();
    switch(decizie) //se incrementeaza unul din cazuri
    {
      case -1:
        nrStanga++;
        break; 
      case 0:
        nrStop++;
        break;
      case 1:
        nrDreapta++;
        break;
    }
    delay(50);
  }
  if (maxim(nrStanga, nrStop, nrDreapta) == nrStanga) //se efectueaza rotatie la stanga
    motor.step(-pasiPerRotatie);
  else
    if (maxim(nrStanga, nrStop, nrDreapta) == nrDreapta) //se efectueaza rotatie la dreapta
      motor.step(pasiPerRotatie);

  reinitializare();
  
}
