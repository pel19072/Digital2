/*
  Blink
  The basic Energia example.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Change the LED define to blink other LEDs.

  Hardware Required:
    LaunchPad with an LED

  This example code is in the public domain.
*/
#include <stdint.h>
#include <TM4C123GH6PM.h>
// most launchpads have a red LED
#define LEDR RED_LED
#define LEDG GREEN_LED
#define LEDB BLUE_LED
#define PUSHA PUSH1
#define PUSHB PUSH2
#define PUSHS PA_2
//puertos
int PORTA[] = {PB_0, PB_1, PE_4, PE_5, PB_4, PA_5, PA_6, PA_7};
int PORTB[] = {PD_0, PD_1, PD_2, PD_3, PE_1, PE_2, PE_3, PD_7};
//banderas
uint8_t FLAG = 0;
uint8_t FLAG1 = 0;
uint8_t FLAG2 = 0;
uint8_t START = 0;
uint8_t i = 0;
uint8_t j = 0;
//prototipados
void semaforo(void);
void jugador1(void);
void jugador2(void);

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  // initialize the digital pin as an output.
  //semaforo
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  //pushes
  pinMode(PUSHA, INPUT_PULLUP);
  pinMode(PUSHB, INPUT_PULLUP);
  pinMode(PUSHS, INPUT_PULLUP);
  //puertos
  //PUERTO A
  pinMode(PB_0, OUTPUT);
  pinMode(PB_1, OUTPUT);
  pinMode(PE_4, OUTPUT);
  pinMode(PE_5, OUTPUT);
  pinMode(PB_4, OUTPUT);
  pinMode(PA_5, OUTPUT);
  pinMode(PA_6, OUTPUT);
  pinMode(PA_7, OUTPUT);
  //PUERTO B
  pinMode(PD_0, OUTPUT);
  pinMode(PD_1, OUTPUT);
  pinMode(PD_2, OUTPUT);
  pinMode(PD_3, OUTPUT);
  pinMode(PE_1, OUTPUT);
  pinMode(PE_2, OUTPUT);
  pinMode(PE_3, OUTPUT);
  pinMode(PD_7, OUTPUT);
  semaforo();
}

// the loop routine runs over and over again forever:
void loop() {
  //prueba();
  //ANTIREBOTE DEL BOTON DE INICIO
  if (digitalRead(PUSHS) == 0) {
    FLAG = 1;
    delay(50);
  }
  else {
    if (FLAG == 1) {
      FLAG = 0;
      semaforo();
      delay(50);
    }
  }
  // JUGADOR 1
  if (digitalRead(PUSHA) == 0 && START == 1) {
    FLAG1 = 1;
    delay(50);
  }
  else {
    if (FLAG1 == 1) {
      FLAG1 = 0;
      jugador1();
      delay(50);
    }
  }
  // JUGADOR 2
  //MISMA LOGICA QUE PARA EL JUGADOR 1
  if (digitalRead(PUSHB) == 0 && START == 1) {
    FLAG2 = 1;
    delay(50);
  }
  else {
    if (FLAG2 == 1) {
      FLAG2 = 0;
      jugador2();
      delay(50);
    }
  }
/*
  Serial.print("i: ");
  Serial.print(i);
  Serial.print("  j: ");
  Serial.print(j);
  Serial.print("  S: ");
  Serial.println(START);
  delay(200);
  */
}

void semaforo(void) {
  //inicializacion
  i = 0;
  j = 0;
  START = 1;
  for (int n = 0; n < 8; n++) {
    digitalWrite(PORTA[n], LOW);
    digitalWrite(PORTB[n], LOW);
  }
  //luces
  digitalWrite(LEDR, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDG, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDB, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(LEDR, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDG, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDB, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(LEDR, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDG, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDB, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(LEDR, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDG, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDB, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
}

void jugador1(void) {
  if (i < 8) {
    if (i > 0) {
      digitalWrite(PORTA[i - 1], LOW);
    }
    digitalWrite(PORTA[i], HIGH);
    i++;
  }
  else {
    digitalWrite(PORTA[7], LOW);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDG, HIGH);
    START = 0;
    i = 0;
  }
}

void jugador2(void) {
  if (j < 8) {
    if (j > 0) {
      digitalWrite(PORTB[j - 1], LOW);
    }
    digitalWrite(PORTB[j], HIGH);
    j++;
  }
  else {
    digitalWrite(PORTB[7], LOW);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, HIGH);
    START = 0;
    j = 0;
  }
}
