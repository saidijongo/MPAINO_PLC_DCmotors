#include <Arduino.h>

// Define relay pins
int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};

// Define pump runtime
const float FLOW_RATE = 1.5; // in ml/minute

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void activateMotor(int pin, int runTime) {
  // Activate the specified motor relay for the given run time
  digitalWrite(pin, HIGH);
  delay(runTime * 1000);
  digitalWrite(pin, LOW);
}

void mixCocktail(String cocktailName, String ingredientName, int pumpNumber, float volume, int pumpRunTime) {
  Serial.print("Mixing cocktail: ");
  Serial.println(cocktailName);

  // Calculate run time based on volume and flow rate
  int runTime = int(volume / FLOW_RATE * 60); // in seconds

  // Activate the specified pump motor
  activateMotor(motorPins[pumpNumber - 1], runTime);

  Serial.print(cocktailName);
  Serial.print(" - ");
  Serial.print(ingredientName);
  Serial.println(" Completed");
}

void inputCmd(String commandStr) {
  commandStr.trim();

  String cocktailName = getValue(commandStr, ',', 0);
  String ingredientName = getValue(commandStr, ',', 1);
  int pumpNumber = getValue(commandStr, ',', 2).toInt();
  float volume = getValue(commandStr, ',', 3).toFloat();
  int pumpRunTime = getValue(commandStr, ',', 4).toInt();

  mixCocktail(cocktailName, ingredientName, pumpNumber, volume, pumpRunTime);
}

void setup() {
  // Setup relay pins as OUTPUT
  for (int i = 0; i < sizeof(motorPins) / sizeof(motorPins[0]); i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    inputCmd(data);
  }
}
