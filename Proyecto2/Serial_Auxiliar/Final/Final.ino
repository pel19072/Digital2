#include "config.h"

/************************ Example Starts Here *******************************/
#define RXD2 16
#define TXD2 17
#define IO_LOOP_DELAY 5000

char sensor[19];
char sensor1 = 0;
int count = 0;
char led_blanca = 0x00;
char led_anaranjada = 0x00;
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *leds = io.feed("LEDs");
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

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  leds->get();
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  if (Serial2.available() > 0) {
    Serial2.write(led_blanca);
    Serial2.write(0x0A);
    Serial2.readBytesUntil(10, sensor, 17);
    Serial.println(sensor);
    //prueba = ~prueba;
  }
  secs->save(sensor);
  delay(2500);
}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  String LED = data->toString();
  Serial.println(LED);
  if (LED == "I"){
    led_blanca = 0x01;
  }
  else{
    led_blanca = 0x00;
  }
}
