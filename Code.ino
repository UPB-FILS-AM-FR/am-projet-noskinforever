#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

const int buzzerPin = 15;

float prevGyroX, prevGyroY;

String lastMovement = "";

void setup() {
  Serial.begin(9600);
  
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  pinMode(buzzerPin, OUTPUT);

  digitalWrite(buzzerPin, HIGH);

  delay(1000);

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  prevGyroX = g.gyro.x;
  prevGyroY = g.gyro.y;
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float gyroX = g.gyro.x;
  float gyroY = g.gyro.y;

  if (abs(gyroX - prevGyroX) > 1.0 || abs(gyroY - prevGyroY) > 1.0) {
    lastMovement = detectMovement(gyroX, gyroY);

    digitalWrite(buzzerPin, LOW);
    delay(100);  // Beep duration
    digitalWrite(buzzerPin, HIGH);
  }

  displayMovement(lastMovement);

  prevGyroX = gyroX;
  prevGyroY = gyroY;

  delay(100);  // 100ms delay for readability
}

String detectMovement(float gyroX, float gyroY) {
  if (abs(gyroX) < 1.0 && abs(gyroY) < 1.0) {
    return lastMovement;  // No significant movement, retain last movement
  } else if (abs(gyroX) > abs(gyroY)) {
    if (gyroX > 0) {
      return "Tilting Right";
    } else {
      return "Tilting Left";
    }
  } else {
    if (gyroY > 0) {
      return "Tilting Up";
    } else {
      return "Tilting Down";
    }
  }
}

void displayMovement(String movement) {
  if (movement == "Tilting Right") {
    Serial.println("Tilting Right:");
    Serial.println("    _  ");
    Serial.println("   /       \\ ");
    Serial.println("  /         \\");
    Serial.println(" /   O   O   |");
    Serial.println("|     \\_/    |");
    Serial.println(" \\_/ ");
  } else if (movement == "Tilting Left") {
    Serial.println("Tilting Left:");
    Serial.println(" _  ");
    Serial.println("/       \\ ");
    Serial.println("\\         \\");
    Serial.println(" |   O   O \\ ");
    Serial.println(" |    \\_/   |");
    Serial.println(" \\_/ ");
  } else if (movement == "Tilting Up") {
    Serial.println("Tilting Up:");
    Serial.println("  _  ");
    Serial.println(" /       \\ ");
    Serial.println("/  O   O  \\");
    Serial.println("|   \\_/   |");
    Serial.println("|         |");
    Serial.println(" \\_/ ");
  } else if (movement == "Tilting Down") {
    Serial.println("Tilting Down:");
    Serial.println("  _  ");
    Serial.println(" /       \\ ");
    Serial.println("|   O   O |");
    Serial.println("|    \\_/  \\");
    Serial.println("\\         /");
    Serial.println(" \\_/ ");
  }
  Serial.println();
}