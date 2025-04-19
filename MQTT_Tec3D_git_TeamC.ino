/*********

*********/

#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>    //Automatically selected when using ESP32
#endif

#include <PubSubClient.h>

// Wifi security

const char* ssid = "ciberfisicos";
const char* password = "12345678";


// MQTT Broker IP address
const char* mqtt_server = "xx.xx.xx.xx";
//const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
float value = 0;
float Dato_Enviar = 0;


//Define
int pushButton = 4;
int LED = 23;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  pinMode(pushButton, INPUT);
  pinMode(LED, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();


  // If topic = "casa", check message is either "Uno" or "Dos". 
  // Changes the output state according to the message
  if (String(topic) == "TeamC_sub") {
    Serial.print("Changing output to ");
    if(messageTemp == "Uno"){
      Serial.println("Uno");
      digitalWrite(LED, HIGH);
    }
    else if(messageTemp == "Dos"){
      Serial.println("off");
      digitalWrite(LED, LOW);
    }
  }
}

void reconnect() {
  // Reconnect
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client_TeamC")) { //"ESPClient_3" represent the client name that connects to broker
      Serial.println("connected");
      // Subscribe
      client.subscribe("TeamC_sub");          //SUBSCRIBE topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
   
    // read the input pin:
    int buttonState = digitalRead(pushButton);

    // print out the state of the button:
    Serial.println(buttonState);
    
    // Check if input button has been pressed
    if (buttonState > 0){
      digitalWrite(LED, HIGH);
      delay(500);               // delay
      digitalWrite(LED, LOW);
    }
      // Convert the value to a char array
      char tempString[2]; // Changed size to 2.  
      tempString[0] = (buttonState == HIGH) ? '1' : '0';  //Button state
      tempString[1] = '\0';         // Null-terminate the string. 
    
      Serial.print("Estado: ");
      Serial.println(tempString);
      client.publish("TeamC", tempString);      //PUBLISH topic

    delay(1);  // delay in between reads for stability

    }
}
