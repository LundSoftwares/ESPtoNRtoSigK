/*
ESPtoNRtoSigK v1.0b
"A simpler way to send data wirelessly to SignalK"
by Markus Lund
 */
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <OneWire.h>
#include <DallasTemperature.h>

ESP8266WiFiMulti WiFiMulti;


//-----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//------------------------------CONFIG !!-------------------------------------------------------
//Wifi Settings
const char * wlan_ssid = "Replace_With_SSID_Here";
const char * wlan_pass = "Replace_With_Wlan_Pass_Here";

//SignalK Server IP
const char * signalk_ip = "Replace_With_IP_Here";

//Set Digital In Pin and Path for SignalK
//Go to line 67-71 and change setting to INPUT_PULLUP if necessary for you DI's
int DI1pin = 15;        
String DI1path = "Some.SignalK.Path.Here";
int DI2pin = 13;
String DI2path = "Some.SignalK.Path.Here";
int DI3pin = 12;
String DI3path = "Some.SignalK.Path.Here";
int DI4pin = 14;
String DI4path = "Some.SignalK.Path.Here";
int DI5pin = 16;
String DI5path = "Some.SignalK.Path.Here";

//Set a Digital Out Pin here
int DO1pin = 5;

//Set pin for 1Wire Bus
#define ONE_WIRE_BUS 2   

//--------------------------- END OF CONFIG !! --------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------





String DeviceName;
int port = 3101;  //Port number
WiFiServer server(port);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// Number of temperature devices found
int numberOfDevices;
// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress;

void setup() {
    Serial.begin(9600);
    // Start up the library

    pinMode(DI1pin, INPUT);
    pinMode(DI2pin, INPUT);
    pinMode(DI3pin, INPUT);
    pinMode(DI4pin, INPUT);
    pinMode(DI5pin, INPUT_PULLUP);
    pinMode(DO1pin, OUTPUT);

    // We start by connecting to a WiFi network
    const char * ssid = wlan_ssid;      
    const char * password = wlan_pass;  
    WiFiMulti.addAP(ssid, password);

    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();

    sensors.begin();
    numberOfDevices = sensors.getDeviceCount();
  
    // locate devices on the bus
    Serial.print("Locating 1wire devices...");
    Serial.print("Found ");
    Serial.print(numberOfDevices, DEC);
    Serial.println(" devices.");

    // Loop through each device, print out address
    for(int i=0;i<numberOfDevices; i++){
      // Search the wire for address
      if(sensors.getAddress(tempDeviceAddress, i)){
        Serial.print("Found device ");
        Serial.print(i, DEC);
        Serial.print(" with address: ");
        printAddress(tempDeviceAddress);
        Serial.println();
      } else {
        Serial.print("Found ghost device at ");
        Serial.print(i, DEC);
        Serial.print(" but could not detect address. Check power and cabling");
      }
    }
    delay(500);
}

void loop() {
    const uint16_t portout = 3100;    
    const char * host = signalk_ip;   
    String msg;
    int sensorVal;
    
//---------------------------------------------------------------------------------   
//Handles Digital Out
    WiFiClient client2 = server.available();
    if (client2) {

    if (!client2.connected()) {
      Serial.println("Connection Failed!");
    }
 if (client2.available()){
      while(client2.available()){
         char c = client2.read();
         String a = String(c);
         if(a == "1"){
          digitalWrite(DO1pin, true);
          Serial.println("Input Received: Turn Output ON");
         }
         else
         {
           digitalWrite(DO1pin, false);
           Serial.println("Input Received: Turn Output OFF");
         }
      }
     }
    }

//---------------------------------------------------------------------------------   
// Handles Digital In
    WiFiClient client;

    if (!client.connect(host, portout)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }

    sensorVal = digitalRead(DI1pin);
    msg = String(sensorVal);
    msg = "DI," + DI1path + "," + msg;
    client.print(msg);
    Serial.print("Sent : ");
    Serial.println(msg);
    delay(500);

    sensorVal = digitalRead(DI2pin);
    msg = String(sensorVal);
    msg = "DI," + DI2path + "," + msg;
    client.print(msg);
    Serial.print("Sent : ");
    Serial.println(msg);
    delay(500);

    sensorVal = digitalRead(DI3pin);
    msg = String(sensorVal);
    msg = "DI," + DI3path + "," + msg;
    client.print(msg);
    Serial.print("Sent : ");
    Serial.println(msg);
    delay(500);

    sensorVal = digitalRead(DI4pin);
    msg = String(sensorVal);
    msg = "DI," + DI4path + "," + msg;
    client.print(msg);
    Serial.print("Sent : ");
    Serial.println(msg);
    delay(500);

    sensorVal = digitalRead(DI5pin);
    msg = String(sensorVal);
    msg = "DI," + DI5path + "," + msg;
    client.print(msg);
    Serial.print("Sent : ");
    Serial.println(msg);
    delay(500);
//---------------------------------------------------------------------------------   
//Handles 1Wire sensors

  sensors.requestTemperatures();
  
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){
      // Output the device ID
      Serial.print("Temperature for device: ");
      Serial.print(i,DEC);
      Serial.print(", ");
      printAddress(tempDeviceAddress);
      Serial.println("");
      // Print the data
      float tempC = sensors.getTempC(tempDeviceAddress);
      Serial.print("Temp C: ");
      Serial.println(tempC);
      msg = "1w," + DeviceName + "," + tempC;
      client.print(msg);
      delay(500);
    }
  }
    client.stop();
    delay(1000);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  DeviceName = "";
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
      if (deviceAddress[i] < 9) DeviceName = DeviceName+"0";
      DeviceName = DeviceName+String(deviceAddress[i], HEX);
  }
}
