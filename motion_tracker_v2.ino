#include <Stepper.h>
#include <SharpIR.h>

#define STG A0 //pin pentru senzorul stang
#define DR A1 //pin pentru senzorul drept
#define model 430 //modelul de senzor

const int stepsPerRevolution = 130; //numarul de impulsuri magnetice dintr-o miscare
int distanta1 = 0; //in aceste variabile vom retine distantele masurate
int distanta2 = 0;
int nrStanga = 0;
int nrStop = 0;
int nrDreapta = 0;

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);// declararea motorului
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
  distanta1 = stanga.distance() + 2;
  distanta2 = dreapta.distance() + 2;
  
  Serial.print("D1: ");
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
  myStepper.setSpeed(90); //viteza cu care se misca robotul
  Serial.begin(9600);
}

void loop() {

  for (int i = 0; i < 10; i++)
  {
      int decizie = look();
      switch(decizie)
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
  if (maxim(nrStanga, nrStop, nrDreapta) == nrStanga)
    myStepper.step(-stepsPerRevolution);
  else
    if (maxim(nrStanga, nrStop, nrDreapta) == nrDreapta)
      myStepper.step(stepsPerRevolution);

  reinitializare();
  
}
