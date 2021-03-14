#define AUDIO_IN_PIN 35

int analogValue;
unsigned long newTime;

void setup() {
  Serial.begin(115200);
}

void loop() {
  newTime = micros();
  
  for (int i = 0; i < 100000; i++) {
    analogValue = analogRead(AUDIO_IN_PIN);
  }

  float conversionTime = (micros() - newTime) / 1000000.0/ 100000;

  Serial.print("Max sampling frequency: ");
  Serial.print((1.0 / conversionTime));
  Serial.println(" Hz");
}
