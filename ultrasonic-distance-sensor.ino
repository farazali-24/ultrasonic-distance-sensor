#include <NewPing.h>   // Library for ultrasonic distance measurement

// -------------------- Pin Definitions --------------------
#define TRIG_PIN 32      // Trigger pin of HC-SR04
#define ECHO_PIN 33      // Echo pin of HC-SR04 (use voltage divider to 3.3V)
#define LED_ULTRA 25     // LED pin (with series resistor)

// -------------------- Constants --------------------
#define MAX_DISTANCE 400     // Maximum distance to measure (cm)
#define FILTER_SAMPLES 5     // Number of samples for moving average

// Create ultrasonic sensor object
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);       // Start serial monitor for debugging
  pinMode(LED_ULTRA, OUTPUT); // Set LED pin as output
}

void loop() {
  unsigned int distance = 0;          // Stores raw distance readings
  unsigned int filteredDistance = 0;  // Stores averaged (filtered) distance

  // -------------------- Moving Average Filter --------------------
  // Take multiple readings and average them to reduce noise
  for (int i = 0; i < FILTER_SAMPLES; i++) {
    unsigned int d = sonar.ping_cm();        // Read distance in cm
    if (d > 0 && d <= MAX_DISTANCE) {        // Accept only valid readings
      distance += d;
    }
    delay(10);  // Small delay between samples
  }

  if (distance > 0) {
    filteredDistance = distance / FILTER_SAMPLES; // Compute average
  }

  // -------------------- LED Brightness Control --------------------
  if (filteredDistance > 0 && filteredDistance <= 10) {
    digitalWrite(LED_ULTRA, HIGH);  // Very close → LED fully ON
  } 
  else if (filteredDistance > 10 && filteredDistance <= 100) {
    // Map distance to LED brightness (closer = brighter)
    int brightness = map(filteredDistance, 10, 100, 255, 0);
    analogWrite(LED_ULTRA, brightness);
  } 
  else {
    digitalWrite(LED_ULTRA, LOW);   // Too far / no reading → LED OFF
  }

  // -------------------- Serial Debugging --------------------
  Serial.printf("Filtered Distance: %d cm\n", filteredDistance);
}
