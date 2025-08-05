#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial bluetoothSerial(10, 11);

const int buttonPin = 2;
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;
const int potPin = A0;
const int pirPin = 7;
const int servoPin = 9;

Servo myServo;

int mode = 0;
int lastButtonState = HIGH;
bool systemStarted = false;
bool lightOn = false;
int lastPirState = LOW;
bool lastPotDirection = false;

unsigned long lastPirTriggerTime = 0;
const unsigned long pirDebounceDelay = 1000;

String receivedBluetoothData = "";

int bluetoothBrightness = 0;
int currentServoAngle = 90;

// Odebrán mód 5
int manualModes[] = {0, 1, 2, 4};
int currentManualModeIndex = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pirPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(90);
  Serial.begin(9600);
  bluetoothSerial.begin(9600);
  setColor(0, 0, 0);
}

void loop() {
  // Bluetooth příkazy
  if (bluetoothSerial.available()) {
    char incomingChar = bluetoothSerial.read();
    if (incomingChar == '\n') {
      receivedBluetoothData.trim();

      if (mode != 99) {
        mode = 99;
        setColor(0, 0, 0);
        myServo.write(90);
        currentServoAngle = 90;
      }

      if (receivedBluetoothData == "led on") {
        bluetoothBrightness = 255;
        setColor(bluetoothBrightness, bluetoothBrightness * 0.7, 0);
        lightOn = true;
      } else if (receivedBluetoothData == "led off") {
        setColor(0, 0, 0);
        lightOn = false;
        bluetoothBrightness = 0;
      } else if (receivedBluetoothData == "jas+") {
        bluetoothBrightness = min(255, bluetoothBrightness + 25);
        setColor(bluetoothBrightness, bluetoothBrightness * 0.7, 0);
      } else if (receivedBluetoothData == "jas-") {
        bluetoothBrightness = max(0, bluetoothBrightness - 25);
        setColor(bluetoothBrightness, bluetoothBrightness * 0.7, 0);
      } else if (receivedBluetoothData == "servo_pos2") {
        myServo.write(0);
        currentServoAngle = 0;
      } else if (receivedBluetoothData == "servo_pos1") {
        myServo.write(90);
        currentServoAngle = 90;
      } else if (receivedBluetoothData == "servo_toggle") {
        currentServoAngle = (currentServoAngle == 0) ? 90 : 0;
        myServo.write(currentServoAngle);
      }

      receivedBluetoothData = "";
    } else {
      receivedBluetoothData += incomingChar;
    }
  }

  // Přepínání módů tlačítkem
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    if (!systemStarted) {
      systemStarted = true;
      mode = manualModes[currentManualModeIndex];
      bluetoothBrightness = 255;
      setColor(bluetoothBrightness, bluetoothBrightness * 0.7, 0);
    } else if (mode == 99) {
      mode = manualModes[currentManualModeIndex];
    } else {
      currentManualModeIndex = (currentManualModeIndex + 1) % (sizeof(manualModes) / sizeof(manualModes[0]));
      mode = manualModes[currentManualModeIndex];
    }
  }
  lastButtonState = buttonState;

  if (!systemStarted || mode == 99) return;

  // Módy
  if (mode == 0) {
    int potValue = analogRead(potPin);
    int brightness = map(potValue, 0, 1023, 0, 255);
    setColor(brightness, brightness * 0.7, 0);
    bluetoothBrightness = brightness;
  } else if (mode == 1) {
    int potValue = analogRead(potPin);
    bool potDirection = potValue >= 512;
    if (potDirection != lastPotDirection) {
      myServo.write(potDirection ? 90 : 0);
      currentServoAngle = potDirection ? 90 : 0;
      lastPotDirection = potDirection;
    }
    setColor(0, 0, 0);
  } else if (mode == 2) {
    int pirState = digitalRead(pirPin);
    unsigned long now = millis();
    if (pirState == HIGH && lastPirState == LOW && (now - lastPirTriggerTime > pirDebounceDelay)) {
      lightOn = !lightOn;
      lastPirTriggerTime = now;
    }
    lastPirState = pirState;
    setColor(lightOn ? 255 : 0, lightOn ? 180 : 0, 0);
    bluetoothBrightness = lightOn ? 255 : 0;
  } else if (mode == 4) {
    static int lastPotValue = -1;
    int currentPotValue = analogRead(potPin);
    if (abs(currentPotValue - lastPotValue) > 5) {
      int brightnessValue = map(currentPotValue, 0, 1023, 0, 255);
      bluetoothSerial.print("J:");
      bluetoothSerial.print(brightnessValue);
      bluetoothSerial.print("\n");
      lastPotValue = currentPotValue;
    }
    setColor(0, 0, 0);
  }
}

void setColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}