#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial espSerial(9,10); // This makes pin 9 of Arduino as RX pin and pin 10 of Arduino as the TX pin
const int sensorPin= 0;
#include <LiquidCrystal.h> 
LiquidCrystal lcd(12,11, 5, 4, 3, 2);

String mySSID = "NETWORK";       // WiFi SSID
String myPWD = "*******"; // WiFi Password
String myAPI = "6L22OL5XC391UHWC";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 


void setup() 
{
    lcd.begin(16,2);
    lcd.setCursor (0,0);
    lcd.print ("   AIR QUALITY");
    lcd.setCursor (0,1);
    lcd.print ("DETECTION SYSTEM");
    delay(2000);
    Serial.begin(115200);
    espSerial.begin(115200);

    espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
    espData("AT+CWMODE=3", 1000, DEBUG);                 //Set the ESP mode as station mode
    espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
      
    pinMode(sensorPin, INPUT);        //Gas sensor will be an input to the arduino
    lcd.clear();
}

void loop() {

    float air_quality = analogRead(A0);
    Serial.println(air_quality);

    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(air_quality);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG); 
    espSerial.println(sendData);
    
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(1000);
   
    
    lcd.setCursor (0, 0);
    lcd.print("Air Quality");
    lcd.setCursor(0,1);
    
    lcd.print (air_quality);
    lcd.print (" PPM ");
}


String espData(String command, const int timeout, boolean debug)
{
       
    String response = "";
    espSerial.println(command);
    long int time = millis();
    while ( (time + timeout) > millis())
    
    {
      while (espSerial.available())
      {
        char c = espSerial.read();
        response += c;
      }
    }
    if (debug)
    {
      //Serial.print(response);
    }
    return response;
}
