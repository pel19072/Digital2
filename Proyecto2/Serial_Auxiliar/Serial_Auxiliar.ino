#define RXD2 16
#define TXD2 17
char sensor = 0;
void setup() {
  // Start Serial port
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  if (Serial2.available() > 0) {
    // read the incoming byte:
    sensor = Serial2.read();
    Serial.print(sensor);
    //Serial.println(color);
    //Serial2.write(color);
  }
}
