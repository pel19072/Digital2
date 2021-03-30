#include <SPI.h>
#include <SD.h>
#include <stdint.h>
File myFile;
File root;
unsigned char eleccion = 0x00;

void show(void);
void choice(void);

void setup()
{
  Serial.begin(9600);
  SPI.setModule(0);

  Serial.print("Initializing SD card...");
  pinMode(PA_3, OUTPUT);

  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed");
    return;
  }
  Serial.println("initialization done.");
  show();
}

void loop()
{
  eleccion = Serial.read();
  delay(200);
  if (eleccion == 48 || eleccion == 49 || eleccion == 50 || eleccion == 51) {
    choice();
  }
}

void show(void) {
  root = SD.open("/");
  printDirectory(root, 0);
  root.close();
  Serial.println("Escoja el documento que desea ver:\n1:Yoshi\n2:Peach\n3:Toad");
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void choice(void) {
  switch (eleccion) {
    case 48:
      Serial.println("Escoja el documento que desea ver:\n1:Yoshi\n2:Peach\n3:Toad");
      break;
    case 49:
      myFile = SD.open("yoshi.txt");
      if (myFile) {
        // read from the file until there's nothing else in it:
        while (myFile.available()) {
          Serial.write(myFile.read());
        }
        // close the file:
        myFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening document");
      }
      Serial.println("Escoja el documento que desea ver:\n1:Yoshi\n2:Peach\n3:Toad");
      break;
    case 50:
      myFile = SD.open("peach.txt");
      if (myFile) {
        // read from the file until there's nothing else in it:
        while (myFile.available()) {
          Serial.write(myFile.read());
        }
        // close the file:
        myFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening document");
      }
      Serial.println("Escoja el documento que desea ver:\n1:Yoshi\n2:Peach\n3:Toad");
      break;
    case 51:
      myFile = SD.open("toad.txt");
      if (myFile) {
        // read from the file until there's nothing else in it:
        while (myFile.available()) {
          Serial.write(myFile.read());
        }
        // close the file:
        myFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening document");
      }
      Serial.println("Escoja el documento que desea ver:\n1:Yoshi\n2:Peach\n3:Toad");
      break;
  }

}
