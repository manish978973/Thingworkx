#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define DHTPIN D2 

#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "MDCC_00997327504092";
const char* password = "ndoaktum";

int sensorPin = A0; 
int sensorValue = 0;
int motion = D6; 

char* server = "http://pp-20062800129z.devportal.ptc.io:8080";
char* APIKEY = "bfdc572d-ce10-4b40-9e2e-4dba8d7862f1";

char ThingName[] = "EnvStrobe";
char ServiceName[] = "SetValue";

char Property1[]= "Humidity";
char Property2[]= "Light";
char Property3[]= "Temperature";
char Property4[]= "Motion";

#define ACCEPT_TYPE "text/csv"


void setup() {
 Serial.begin(115200);
 WiFi.begin(ssid, password);
 pinMode(motion, INPUT);

 while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print("Attempting to reconnect");
      dht.begin();
}

}

void loop() {
  // put your main code here, to run repeatedly:
   if(WiFi.status() == WL_CONNECTED){

       float h = dht.readHumidity();
       float tempo = dht.readTemperature();

       if (isnan(h) || isnan(tempo)) {
       Serial.println(F("Failed to read from DHT sensor!"));
       return;
      }

      int motionval = 0;

       if (digitalRead(motion) == HIGH){
       motionval = 1;
     }
      


      sensorValue = analogRead(sensorPin);
      
      delay(1000);

      HTTPClient http;

      String d = "{\"Humidity\":\""+String(h)+"\"}";
      Serial.println(d);

       String fullRequestURL = String(server) + "/Thingworx/Things/" + ThingName + "/Properties/" + Property1 + "?appKey=" + String(APIKEY);
       http.begin(fullRequestURL);
       http.addHeader("Accept",ACCEPT_TYPE,false,false);
       http.addHeader("Content-Type","application/json",false,false);
   
      Serial.println(fullRequestURL);
      int httpCode = http.sendRequest("PUT",d);

      if (httpCode > 0) {
         String payload = http.getString();
         Serial.println(payload);

         String k = "{\"Light\":\""+String(sensorValue)+"\"}";
         Serial.println(k);

         String fullRequestURL2 = String(server) + "/Thingworx/Things/" + ThingName + "/Properties/" + Property2 + "?appKey=" + String(APIKEY);
         http.begin(fullRequestURL2);
         http.addHeader("Accept",ACCEPT_TYPE,false,false);
         http.addHeader("Content-Type","application/json",false,false);
   
         Serial.println(fullRequestURL2);
        int httpCode = http.sendRequest("PUT",k);

          if (httpCode > 0) {

            String payload = http.getString();
            Serial.println(payload);

            String t = "{\"Temperature\":\""+String(tempo)+"\"}";
            Serial.println(t);

            String fullRequestURL3 = String(server) + "/Thingworx/Things/" + ThingName + "/Properties/" + Property3 + "?appKey=" + String(APIKEY);

           http.begin(fullRequestURL3);
           http.addHeader("Accept",ACCEPT_TYPE,false,false);
           http.addHeader("Content-Type","application/json",false,false);
   
           Serial.println(fullRequestURL3);
            int httpCode = http.sendRequest("PUT",t);

             if (httpCode > 0) {

              String payload = http.getString();
              Serial.println(payload);

            String motionmes = "{\"Motion\":\""+String(motionval)+"\"}";
            Serial.println(motionmes);

            String fullRequestURLmot = String(server) + "/Thingworx/Things/" + ThingName + "/Properties/" + Property4 + "?appKey=" + String(APIKEY);
         http.begin(fullRequestURLmot);
         http.addHeader("Accept",ACCEPT_TYPE,false,false);
         http.addHeader("Content-Type","application/json",false,false);
   
         Serial.println(fullRequestURLmot);
        int httpCode = http.sendRequest("PUT",motionmes);

        if (httpCode > 0) {
         String payload = http.getString();
         Serial.println(payload);
        }


                   http.end();

              
              
             }

           
            

            
          }
        

         
        
      }

     delay(3000);
    
   }

}
