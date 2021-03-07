#include "config.h"

/************************ Example Starts Here *******************************/
#define RXD2 16
#define TXD2 17
#define IO_LOOP_DELAY 5000

char sensor[19];  //Buffer que se enviará a Adafruit
char sensor1 = 0;
int count = 0;
char led_blanca = 0x00;
char led_anaranjada = 0x00;
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *leds = io.feed("LEDs");
AdafruitIO_Feed *leds2 = io.feed("LEDs2");
AdafruitIO_Feed *secs = io.feed("RTC");

void setup() {

  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  leds->onMessage(handleMessage);
  leds2->onMessage(handleMessage2);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  leds->get();
  leds2->get();
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  if (Serial2.available() > 0) {
    //Mando los dos estados de las leds y
    //un indicador (en este caso un enter)
    //para que el PIC comience a enviar.
    Serial2.write(led_blanca);
    Serial2.write(led_anaranjada);
    Serial2.write(0x0A);
    //Leo HASTA encontrar el enter, 
    //el cual es el indicador desde el PIC
    Serial2.readBytesUntil(10, sensor, 17);
    Serial.println(sensor);
  }
  //Programación defensiva para evitar desfases
  if (sensor[2] == '/' && sensor[5] == '/') {
    secs->save(sensor);
    //Para cumplir con no más de 30 datos por minuto.
    delay(3000);
  }

}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  String LED = data->toString();
  Serial.println(LED);
  //Según el valor que se mande, 
  //cambio el estado de las LEDs
  if (LED == "I") {
    led_blanca = 0x01;
  }
  else {
    led_blanca = 0x00;
  }
}

void handleMessage2(AdafruitIO_Data *data) {
  String LED = data->toString();
  Serial.println(LED);
  if (LED == "I") {
    led_anaranjada = 0x01;
  }
  else {
    led_anaranjada = 0x00;
  }
}
