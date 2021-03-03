#define RXD2 16
#define TXD2 17

char sensor[17];
//const int Boton = 23;
char prueba = 0xFF;
//char enter = 0x00;
//int flag = 0;
//int counter = 0;
//int count = 0;
void setup() {
  // Start Serial port
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  //pinMode(Boton, INPUT);
}

void loop() {
  if (Serial2.available() > 0) {
    
    //        if (digitalRead(Boton) == 0) {
    //          flag = 1;
    //        }
    //        else {
    //          if (flag == 1) {
    //            flag = 0;
    //            if (counter == 0) {
    //              counter = 1;
    //              prueba = 0x00;
    //            }
    //            else {
    //              counter = 0;
    //              prueba = 0xFF;
    //            }
    //          }
    //        }
    Serial2.write(prueba);
    Serial2.write(0x0A);
    delay(20);
    Serial2.readBytesUntil(10, sensor, 17);
    //Serial2.flush();
    Serial.println(sensor);
    //    while (sensor1 != 0x0A) {
    //      sensor1 = Serial2.read();
    //      Serial.print(sensor1);
    //    }
  }
}
