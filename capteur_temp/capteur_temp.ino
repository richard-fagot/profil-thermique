// C'est le code source à utiliser 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h> // pour le service REST donnant la température
#include <ESP8266HTTPClient.h> // pour envoyer la température à thingspeak
#include <ESP8266mDNS.h> // multicast DNS pour accéder au module par son nom

// Temperature
#include <OneWire.h>
#include <DallasTemperature.h>

// OLED Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//#define DEVICE_NAME "exterieur"
//#define FIELD_NAME "field7"

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 OLED(OLED_RESET);


#define ONE_WIRE_BUS 2

#define INTERVAL 60 // une minute entre deux prises de mesures

// Nom du module, qui devient accessible via http://DEVICE_NAME.local/
const char* DEVICE_NAME = "salon";
const char* FIELD_NAME = "field1";

// Paramètres d'authentification sur le réseau
const char* ssid = "TP-Link_4E6C";
const char* password = "24914589";

//const char* ssid = "Hotel_Biney";
//const char* password = "biney.12";


// Configuration du capteur de température en 1-wire.
//OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire(D3);
DallasTemperature sensors(&oneWire);

// Le serveur tcp écoute sur le port 80 (http)
ESP8266WebServer server(80);

const int led = 13;
String IP;

///////////////////////////////////////////////
//
//  Initialise l'écran OLED.
//
///////////////////////////////////////////////
void setupDisplay() {
  OLED.begin();
  OLED.clearDisplay();
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
}

///////////////////////////////////////////////
//
//  Service REST renvoyant la température
//  mesurée par le capteur.
//
///////////////////////////////////////////////
void handleTemp() {
  float temp = getTemp();
  String response = "{temperature:";
  response += temp;
  response += "}";
  server.send(200, "text/plain", response);
}

///////////////////////////////////////////////
//
//  Renvoi la température mesurée par le 
//  capteur.
//
///////////////////////////////////////////////
float getTemp() {
  // Lit la température sur tous les capteurs  (une seul en fait).
 sensors.requestTemperatures();
 // renvoi la température du premier (et seul) capteur.
 return sensors.getTempCByIndex(0);
}

///////////////////////////////////////////////
//
//  Affiche une température en gros sur 
//  l'écran OLED.
//
///////////////////////////////////////////////
void displayTemp(float temp) {
  float t = temp;
  OLED.clearDisplay();
  OLED.setTextSize(3);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.print(t);
  OLED.setCursor(100,0);
  OLED.print("C");
  OLED.drawCircle(93, 3, 3, WHITE);
  OLED.display();  
}

///////////////////////////////////////////////
//
//  Affiche une erreur.
//
///////////////////////////////////////////////
void displayError(String msg) {
  OLED.clearDisplay();
  OLED.setTextSize(1);
  OLED.setTextColor(BLACK, WHITE);
  OLED.setCursor(0,0);
  OLED.print(msg);
}

///////////////////////////////////////////////
//
//  Affiche les paramètres réseaux.
//
///////////////////////////////////////////////
void displayInfos() {
  OLED.println();
  OLED.print("SSID: ");
  OLED.println(ssid);
  OLED.print("IP: ");
  OLED.print(WiFi.localIP());
  OLED.display();
}

///////////////////////////////////////////////
//
// Affiche le mode d'emploi.
//
///////////////////////////////////////////////
void handleNotFound(){
  digitalWrite(led, 1);
  String message = "This device returns temperature in Kelvin\n\nUsage:\n\n";
  message += "http://"+IP+"temperature\n\n";
  message += "Returns : {temperature: <kelvin>}";
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setupmDNS() {
  
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin(DEVICE_NAME)) {
    Serial.println("Erreur mDNS");
    displayError("mDNS erreur");
    while(1) { 
      delay(1000);
    }
  }

  Serial.println("DNS OK");
}

///////////////////////////////////////////////
//
// Envoi de la température sur Thingspeak.
//
///////////////////////////////////////////////
void sendTempToThingspeak(float temp) {
  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=3GRD3ZIXSZTBCLOI&";
  url += FIELD_NAME;
  url += "=";
  url += temp;
  http.begin(url);  //Specify request destination
  int httpCode = http.GET();                                                          //Send the request

  Serial.printf("htttpCode : %i\n", httpCode);
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();   //Get the request response payload
    Serial.println("payload : " + payload);
  }
  
  http.end();   //Close connection
}

///////////////////////////////////////////////
// Initialisation du programme.
///////////////////////////////////////////////
void setup(void){
 Serial.begin(115200);
  setupDisplay();
  sensors.begin();
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  WiFi.begin(ssid, password);

  // Attend la connexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    OLED.print(".");
    OLED.display();
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
  displayInfos();

  setupmDNS();
  
  server.on("/temperature", handleTemp);
  
  server.onNotFound(handleNotFound);

  server.begin();
}

///////////////////////////////////////////////
// Initialisation des variables pour
// l'intervalle de temps entre deux mesures
///////////////////////////////////////////////

// 1min. entre chaque prise de mesure
int intervall = INTERVAL*1000;
int old_time = millis();
int current_time;


void loop(void){
  // Surveille la présence d'une requête client
  // et appelle la méthode ::handleTemp().
  server.handleClient();
  
  // Lit et affiche la température à intervalle régulier
  current_time = millis();
  if(intervall < (current_time - old_time)) {
     float temp = getTemp();

     
     displayTemp(temp);
     
     sendTempToThingspeak(temp);
     old_time = current_time;
  }
}
