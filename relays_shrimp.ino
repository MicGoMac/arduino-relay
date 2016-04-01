/* 
michael's relay with serial for use with nas - shrimp
pins assignment so that all pins on one side

phase 2 2016-03-26
add manaul mode.
  4 button potential divider, to make same calls to output 1-4

corresponding voltage
1.5, 2.5, 3.5, 4.5
  
 */
unsigned long last_trigger_time = 0;
const int ledPin = 4;      //status led
const int controlPin = A0;
const int r1 = 5;
const int r2 = 6;
const int r3 = 7;
const int r4 = 8;

String inputString = ""; 
boolean stringComplete = false;
boolean r1_state = true;
boolean r2_state = true;
boolean r3_state = true;
boolean r4_state = true;

int ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
  
  Serial.begin(9600);       // use the serial port
  //inputString.reserve(200); // reserve 200 bytes for the inputString:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  for (int thisPin = r1; thisPin <= r4; thisPin++) {
          digitalWrite(thisPin, HIGH);
        }
        
pinMode(r1, OUTPUT);
pinMode(r2, OUTPUT);
pinMode(r3, OUTPUT);
pinMode(r4, OUTPUT);

Serial.println("relay_started");  
}

void loop() {
  //read control analogue in, 1024 steps
  //input be arranged 4 levels, 1v to >4v, under 1 volt ignored to avoid false trigger
  int sensorValue = analogRead(controlPin);
  
  //int sensorValue = random(1000); //test use
  //Serial.println(sensorValue);
  
  if (sensorValue>200 ){
     //check if recent trigger
    unsigned long currtime = millis();
    if ( (currtime - last_trigger_time) > 3000 ){
      char senseChar = convertLevel(sensorValue);
      Serial.println(senseChar);
      updateRelay(senseChar);
      delay(200); //add extra delay for valid input, runs like a new loop
    }
    //last_trigger_time = 1;
  }
  

  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    // Serial.println(inChar);
    updateRelay(inChar);
  }
  delay(200);
}

char convertLevel( int v){
  int fall_in_step = 4;
  //if high value v pass the loop, fall_in_step = 4
  //else will output 0-4
  for (int i=1; i <= 5; i++){
    if ( v < (i*204) ){
      fall_in_step = i-1;
      break;
    }
  }
  //convert fall_in_step to char
  String t= String(fall_in_step);
  return t[0];
}

void updateRelay(char inChar){
switch (inChar) {
      case '1':
        r1_state = !r1_state;
        //Serial.println(inChar);
        digitalWrite(r1, r1_state);
        gotit(inChar);
        break;
      case '2':
        r2_state = !r2_state;
        //Serial.println(inChar);
        digitalWrite(r2, r2_state);
        gotit(inChar);
        break;
      case '3':
        r3_state = !r3_state;
        //Serial.println(inChar);
        digitalWrite(r3, r3_state);
        gotit(inChar);
        break;
      case '4':
      r4_state = !r4_state;
       //Serial.println(inChar);
        digitalWrite(r4, r4_state);
        gotit(inChar);
        break; 
      case '0':
        for (int thisPin = r1; thisPin <= r4; thisPin++) {
          digitalWrite(thisPin, HIGH);
        }
        gotit(inChar);
        break; 
        
      default:
         Serial.println("non action char");
         
        break;
    }
  
}

void gotit( char n) {
  Serial.println( n);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(300);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(300);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(300);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
}

/*
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    
    if ( inChar != '1' && inChar != '2' && inChar != '3' && inChar != '4'){
    	//damn str cannot compare with char+
     	//inputString = inChar;
    	stringComplete = true;
    }
    
     
  }
  
}*/

