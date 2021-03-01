#define RXD2 16
#define TXD2 17
char sensor = 0;
const int Boton = 23;
int flag = 0;
int counter = 0;
void setup() {
  // Start Serial port
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(Boton, INPUT);
}

void loop() {
  if (Serial2.available() > 0) {
    sensor = Serial2.read();
    Serial.print(sensor);
  }
//  if (digitalRead(Boton) == 0) {
//    flag = 1;
//  }
//  else {
//    if (flag == 1) {
//      flag = 0;
//      if (counter == 0) {
//        counter = 1;
//      }
//      else {
//        counter = 0;
//      }
//    }
//  }
//  if (counter = 0) {
//    Serial2.write(48);
//    Serial2.write(10);
//  }
//  else {
//    Serial2.write(49);
//    Serial2.write(10);
//  }

}
