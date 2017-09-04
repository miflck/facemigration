
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


unsigned long firstTime; // how long since the button was first pressed
long millis_held;    // How long the button was held (milliseconds)


const int outpin = 4;     // the number of the pushbutton pin


void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  pinMode(outpin, OUTPUT);
  digitalWrite(outpin, LOW);
//  delay(1000);
  digitalWrite(outpin, HIGH);
  //delay(5000);
  //Serial.println("-1");
  
  
    Serial.println(F("----------SETUP----------"));

  
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      // turn LED on:
      digitalWrite(ledPin, HIGH);
      Serial.println(buttonState);
      firstTime = millis();
    } else {
      // turn LED off:
      digitalWrite(ledPin, LOW);
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }

  if (buttonState == HIGH) {
    millis_held = (millis() - firstTime);
    if (millis_held > 5000) {
     Serial.println("-1");
      firstTime = millis();
    }
  }

  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

}

