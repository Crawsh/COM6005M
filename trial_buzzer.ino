#include <Pushsafer.h>
#include <ThingSpeak.h>
#include <WiFiNINA.h>

//wifi variables setup
const char ssid[] = "VM2E5A18";
const char password[] = "xsGar5pgbbnn";
WiFiClient client;

//thingSpeak variables setup
const int channel_id = 2407058;
const char write_api_key[] = "0ED29B56LNP6NCP8";

//pins set up
const int pinTemp = A0;
const int B = 3975; //used for temperature conversion
const int speakerPin = 3;  
const int pinButton = 7;   


//pushSafer set up
#define PushsaferKey "ahk6IL22Jmtv6jPDibUE"
Pushsafer pushsafer(PushsaferKey, client);

float toCelcius(int val){

    float resistance = (float)(1023-val)*10000/val;
    float temperature = 1/(log(resistance/10000)/B+1/298.15)-273.15;
    return temperature;
}

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(pinButton, INPUT);

  //connect to wifi session
  Serial.begin(9600);
  Serial.print("Attempting to connect to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println("WiFi connection has been established!");
  delay(60);
  ThingSpeak.begin(client);
}

void loop() {
  

  if(digitalRead(pinButton))
    {

      pushsafer.debug = true;

        struct PushSaferInput input;
          input.message = "Your animal may be requesting food";
          input.title = "Food button pushed!";
          input.sound = "8";
          input.vibration = "1";
          input.icon = "1";
          input.iconcolor = "#FFCCCC";
          input.priority = "1";
          input.device = "a";
          input.url = "https://www.pushsafer.com";
          input.urlTitle = "Open Pushsafer.com";

  Serial.println(pushsafer.sendEvent(input));
  Serial.println("Notification sent");

        ThingSpeak.setField(1, 1);
        int thingSpeakWriteStatus = ThingSpeak.writeFields(channel_id, write_api_key);
        if(thingSpeakWriteStatus == 200){
          Serial.println("Successfully written to thingSpeak");
        }
        else {
          Serial.println("Error writing to thingSpeak - Error code:");
          Serial.println(thingSpeakWriteStatus);
        }
  
      
        digitalWrite(speakerPin, HIGH);

        

    }
    else
    {
        digitalWrite(speakerPin, LOW);
    }

    ThingSpeak.setField(2, toCelcius(analogRead(pinTemp)));
    
    delay(10);
 
}
