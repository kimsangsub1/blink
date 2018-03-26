// ESP8266 DS18B20 ArduinoIDE Thingspeak IoT Example code
// http://vaasa.hacklab.fi
//
// https://github.com/milesburton/Arduino-Temperature-Control-Library
// https://gist.github.com/jeje/57091acf138a92c4176a

#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DallasTemperature.h>
#include <Time.h>
#include <TimeLib.h>

/*
  list of function. 
  You can only one function of this. 
  How you can use this?? Just remove the annotation mark '//'
*/
//#define THINGSPEAK 1
#define IFTTT 1

/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*
  Function THINGSPEAK. 
  We will send the temperature to my thingspeak account.
  Set the flag to THINGSPEAK. then the code will be activated.
*/
#ifdef THINGSPEAK
#define ONE_WIRE_BUS D3

const char* host = "api.thingspeak.com"; // Your domain
String ApiKey = "A4LFK4QL386QAQ0U";
String path = "/update?key=" + ApiKey + "&field1=";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

const char* ssid = "olleh_WiFi_DC69";
const char* pass = "0000002020";

char temperatureString[6];

void setup(void){
  Serial.begin(115200);
  Serial.println("");
  
  WiFi.begin(ssid, pass);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  DS18B20.begin();
}

float getTemperature() {
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}

void loop() {
  float temperature = getTemperature();
  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial.println(temperatureString);
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET ") + path + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
  delay(500);
}
#endif

/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*
  Function IFTTT.
  This fucntion is that we will use IFTTT 
  to send the temperature to my line acccount.
*/

#ifdef IFTTT /*send the temperature to my line using IFTTT*/

//define parameters
#define WIFISID "olleh_WiFi_DC69"
#define WIFIPASS "0000002020"

//define the parameters for the IFTTT
#define HOSTIFTTT "maker.ifttt.com"
#define EVENTO "temperature_to_line"
#define IFTTTKEY "bPbSyEul3US4lR4tYeaHnb"
#define ONE_WIRE_BUS D3

//ESP8266WiFiMulti ;
// use the SSL connection of the ESP8266
WiFiClientSecure client;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
// Variable that allows the function to be executed once
bool ejecutado = false;
char temperatureString[6];
void setup() {
  Serial.begin(115200);
  delay(10);
  // Conectamos a la red WiFi
  WiFi.begin(WIFISID, WIFIPASS);
  Serial.println("");
  Serial.print("Waiting to connect to WiFi... ");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFISID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  DS18B20.begin();
}
float temp;
float getTemperature() {
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(500);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}

float temperature;
void loop() {
  temperature = getTemperature();
  dtostrf(temperature, 2, 2, temperatureString);
  // close any previous connection
  if (client.connected())
  {
    client.stop();
  }
  // wait until all the data has been sent
  client.flush();
  
  if (client.connect(HOSTIFTTT,443)) {/*if it doesn't work, change the value to 80*/
    Serial.println("Connected");
    // build the HTTP request
    String toSend = "GET /trigger/";
    toSend += EVENTO;
    toSend += "/with/key/";
    toSend += IFTTTKEY;
    toSend += "?value1=";
    toSend += temperatureString;
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += HOSTIFTTT;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";
    client.print(toSend);
  }
/*
  // wait until all the data has been sent
  client.flush();
  //  disconnect from the client
  client.stop();
*/  
  delay(500);
}
#endif
