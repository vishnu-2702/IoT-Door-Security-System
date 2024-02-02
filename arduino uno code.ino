const int doorStatus = 2; // Pin connected to door status sensor
const int buzzerPin = 3; // Pin connected to buzzer
const int greenLed = 4; // Pin connected to green LED
const int redLed = 5; // Pin connected to red LED

void setup() {
  pinMode(doorStatus, INPUT); // Set door status pin as input
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  pinMode(greenLed, OUTPUT); // Set green LED pin as output
  pinMode(redLed, OUTPUT); // Set red LED pin as output
}

void loop() {
  int doorClosed = digitalRead(doorStatus); // Read the door status sensor

  if (doorClosed == LOW) { // If door is open
    digitalWrite(greenLed, LOW); // Turn off green LED
    digitalWrite(redLed, HIGH); // Turn on red LED
    for (int i = 0; i < 5; i++) { // Beep 5 times
      digitalWrite(buzzerPin, HIGH); // Make a beep sound for 500ms at 1kHz
      delay(500); // Wait 500ms between beeps
    }
  } else {
    digitalWrite(greenLed, HIGH); // Turn on green LED
    digitalWrite(redLed, LOW); // Turn off red LED
    noTone(buzzerPin); // Turn off the buzzer if the door is closed
  }
  
  delay(100); // Wait a short time before checking the door status again
}

