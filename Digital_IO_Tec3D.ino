/*
 Tec3D
 Authors: Eloina LdelReal, Ivo Ayala
 
*/

//Define
int pushButton = 4;
int LED = 23;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication
  Serial.begin(115200);

  // make the pushbutton's pin an input and LED pin as output:
  pinMode(pushButton, INPUT);
  pinMode(LED, OUTPUT);
  }

// the loop routine runs over and over again forever:
void loop() {

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
  
  delay(1);  // delay in between reads for stability

}
