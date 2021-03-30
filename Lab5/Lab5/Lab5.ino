#include <SPI.h>
#include <SD.h>
File myFile;
File root;
String document = "hola";
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
  Serial.println(eleccion);
  //choice();
}

void show(void) {
  root = SD.open("/");
  printDirectory(root, 0);
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
    case 0:
      break;
    case 1:
      document = "yoshi.txt";
      Serial.println(document);
      eleccion = 0;
      break;
    case 2:
      document = "yoshi.txt";
      Serial.println(document);
      eleccion = 0;
      break;
    case 3:
      document = "yoshi.txt";
      Serial.println(document);
      eleccion = 0;
      break;
  }
  myFile = SD.open(document);
  if (myFile) {
    Serial.println(document);

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
}
