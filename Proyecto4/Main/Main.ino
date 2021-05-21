/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
#define RXD2 16
#define TXD2 17

char sensor[5];
int temporal;
int temporal2 = 0;
// SSID & Password
const char* ssid = "CLARO1_B6ABFF";  // Enter your SSID here
const char* password = "527s2wiSGo";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void tabla(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g);
void number(void);

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  // Pines para display
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  //Inicialización de los pines
  digitalWrite(5, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8

  server.onNotFound(handle_NotFound);
  // Creación del servidor local
  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  if (Serial2.available() > 0) {
    // Lectura serial, lee los 5 datos y almacena los 4 que no son un enter
    Serial2.readBytesUntil(10, sensor, 4);
    delay(20);
  }
  temporal = 0;
  for (int i = 0; i < 4; i++) {
    // Cuento qué parqueos están disponibles
    if (sensor[i] == 0x31) {
      temporal++;
    }
  }
  // Se llama a la tabla de conversión del número de parqueos 
  // disponibles para que se muestre en el display
  number();
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  Serial.println("GPIO2 Status: OFF");
  // Crea el código de html
  server.send(200, "text/html", SendHTML());
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML() {
  // Creación del código HTML -Tomado del documento html Main en el directorio data-
  String ptr = "<!--Comentario-->\n";
  ptr += "<!doctype>\n";
  ptr += "\n";
  ptr += "<html lang=\"es\" <!--Para que reconozca el español-->\n";
  ptr += "\t<head>\n";
  ptr += "\t\t<title>Proyecto 4</title>  <!--Este título se guarda en la pestaña, no en la página como tal-->\n";
  ptr += "\t\t<meta charset=\"utf-8\"/>\n";
  ptr += "\t\t<style>\n";
  ptr += "\t\t\th1, h2{color:#ffffff;}\n";
  ptr += "\t\t\tdiv.uno{\n";
  ptr += "\t\t\t\ttext-align: center;\n";
  ptr += "\t\t\t\tcolor:black;\n";
  ptr += "\t\t\t\talign: center;\n";
  ptr += "\t\t\t\tpadding: 14px;\n";
  ptr += "\t\t\t\tbackground-color: #f7523b;\n";
  ptr += "\t\t\t\tfont-family: \"Montserrat\", sans-serif;\n";
  ptr += "\t\t\t\theight:20%;\n";
  ptr += "\t\t\t\twidth: 100%;\n";
  ptr += "\t\t\t}\n";
  ptr += "\t\t\tdiv.dos {\n";
  ptr += "\t\t\t\tfloat: left;\n";
  ptr += "\t\t\t\twidth: 100%;\n";
  ptr += "\t\t\t\theight: auto;\n";
  ptr += "\t\t\t\tpadding: 8px;\n";
  ptr += "\t\t\t\tbackground-color:#ffffff;\n";
  ptr += "\t\t\t}\n";
  ptr += "\t\t\tdiv.tres {\n";
  ptr += "\t\t\t\ttext-align: center;\n";
  ptr += "\t\t\t\tbackground-color: #f77b6a;\n";
  ptr += "\t\t\t\tfont-family:courier;\n";
  ptr += "\t\t\t\tfloat: center;\n";
  ptr += "\t\t\t\twidth: 100%;\n";
  ptr += "\t\t\t\theight: auto;\n";
  ptr += "\t\t\t}\n";
  ptr += "\t\t\ttable {\n";
  ptr += "\t\t\t\twidth: 100%;\n";
  ptr += "\t\t\t\tfloat: center;\n";
  ptr += "\t\t\t}\n";
  ptr += "\t\t\tth, td {\n";
  ptr += "\t\t\t  padding: 15px;\n";
  ptr += "\t\t\t  text-align: center;\n";
  ptr += "\t\t\t\tborder-bottom: 1px solid #ddd;\n";
  ptr += "\t\t\t}\n";
  ptr += "\t\t\ttr:nth-child(even) {background-color: #f2f2f2;}\n";
  ptr += "\t\t\ttr:hover {background-color: #f8f8f8;}\n";
  ptr += "\t\t\tth {\n";
  ptr += "\t\t\t  background-color: #f7523b;\n";
  ptr += "\t\t\t  color: white;\n";
  ptr += "\t\t\t}\n";
  ptr += "\t\t\ttd {\n";
  ptr += "\t\t\t  height: 50px;\n";
  ptr += "\t\t\t  vertical-align: center;\n";
  ptr += "\t\t\t}\n";
  ptr += "\t\t</style>";
  ptr += "\t</head>\n";
  ptr += "\n";
  ptr += "\t<body>\n";
  ptr += "\n";
  ptr += "\t<div class=\"uno\">\n";
  ptr += "\t<header>\n";
  ptr += "\t\t<h1>Parqueo-Matic</h1>\n";
  ptr += "    <h2>Control de Parqueos</h2>\n";
  ptr += "\t</header> <!--1-->\n";
  ptr += "\t</div>\n";
  ptr += "\n";
  ptr += "\n";
  ptr += "\t<br>\n";
  ptr += "\t<br>\n";
  ptr += "\n";
  ptr += "\t<div class=\"dos\">\n";
  ptr += "\t<section> <!--2-->\n";
  ptr += "    <table>\n";
  ptr += "      <tr>\n";
  ptr += "        <th>Número de parqueo</th>\n";
  ptr += "        <th>Disponibilidad</th>\n";
  ptr += "      </tr>\n";
  ptr += "      <tr>\n";
  ptr += "        <td>Parqueo 1 &#x1F693;</td>\n";
  if (sensor[0] == 0x31){
    ptr += "        <td>Disponible &#x2705;</td>";
  }
  else{
    ptr += "        <td>Ocupado &#9940;</td>";
  }
  ptr += "      </tr>\n";
  ptr += "      <tr>\n";
  ptr += "        <td>Parqueo 2 &#x1F695;</td>\n";
  if (sensor[1] == 0x31){
    ptr += "        <td>Disponible &#x2705;</td>";
  }
  else{
    ptr += "        <td>Ocupado &#9940;</td>";
  }
  ptr += "      </tr>\n";
  ptr += "      <tr>\n";
  ptr += "        <td>Parqueo 3 &#x1F697;</td>\n";
  if (sensor[2] == 0x31){
    ptr += "        <td>Disponible &#x2705;</td>";
  }
  else{
    ptr += "        <td>Ocupado &#9940;</td>";
  }
  ptr += "      </tr>\n";
  ptr += "      <tr>\n";
  ptr += "        <td>Parqueo 4 &#x1F699;</td>\n";
  if (sensor[3] == 0x31){
    ptr += "        <td>Disponible &#x2705;</td>";
  }
  else{
    ptr += "        <td>Ocupado &#9940;</td>";
  }
  ptr += "      </tr>\n";
  ptr += "    </table>\n";
  ptr += "\t</section>\n";
  ptr += "\t</div>\n";
  ptr += "\t<div class=\"tres\">\n";
  ptr += "\t<p>\n";
  ptr += "\t\t\tParqueos Disponibles:";
  ptr +=  String(temporal); 
  ptr += "<br>\n";
  ptr += "\t\t\tProyecto por Ricardo Pellecer <br>\n";
  ptr += "\t\t\tElectrónica Digital 2\n";
  ptr += "\t\t</p> <!--3-->\n";
  ptr += "\t</div>\n";
  ptr += "\n";
  ptr += "  <script>\n";
  ptr += "  function timedRefresh(timeoutPeriod) {\n";
  ptr += "  \tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n";
  ptr += "  }\n";
  ptr += "  window.onload = timedRefresh(5000);\n";
  ptr += "  </script>\n";
  ptr += "\n";
  ptr += "\t</body>\n";
  ptr += "</html>";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

// Conversión de número para colocar en display
void tabla(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g) {
  if (g == 1) {
    digitalWrite(12, HIGH);
  } else {
    digitalWrite(12, LOW);
  }
  if (f == 1) {
    digitalWrite(14, HIGH);
  } else {
    digitalWrite(14, LOW);
  }
  if (e == 1) {
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(5, LOW);
  }
  if (d == 1) {
    digitalWrite(18, HIGH);
  } else {
    digitalWrite(18, LOW);
  }
  if (c == 1) {
    digitalWrite(19, HIGH);
  } else {
    digitalWrite(19, LOW);
  }
  if (b == 1) {
    digitalWrite(26, HIGH);
  } else {
    digitalWrite(26, LOW);
  }
  if (a == 1) {
    digitalWrite(27, HIGH);
  } else {
    digitalWrite(27, LOW);
  }
}

// Coloca las 5 opciones posibles.
void number(void) {
  switch (temporal) {
    case 0:
      tabla(0,0,0,0,0,0,1);
      break;
    case 1:
      tabla(1,0,0,1,1,1,1);
      break;
    case 2:
      tabla(0,0,1,0,0,1,0);
      break;
    case 3:
      tabla(0,0,0,0,1,1,0);
      break;
    case 4:
      tabla(1,0,0,1,1,0,0);
      break;
  }
}
