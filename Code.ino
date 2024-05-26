#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Create an MPU6050 object
Adafruit_MPU6050 mpu;

// Pin definitions
const int buzzerPin = 15;

// Variables to store previous gyroscope readings
float prevGyroX, prevGyroY;

// Variable to store the last detected movement
String lastMovement = "";

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize I2C
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  // Initialize the buzzer pin
  pinMode(buzzerPin, OUTPUT);

  // Ensure the buzzer is off at the beginning
  digitalWrite(buzzerPin, HIGH);

  // Give some time to stabilize
  delay(1000);

  // Get initial gyro values
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  prevGyroX = g.gyro.x;
  prevGyroY = g.gyro.y;
}

void loop() {
  // Read current gyro values
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float gyroX = g.gyro.x;
  float gyroY = g.gyro.y;

  // Check for change in direction
  if (abs(gyroX - prevGyroX) > 1.0 || abs(gyroY - prevGyroY) > 1.0) {
    // Update last movement
    lastMovement = detectMovement(gyroX, gyroY);

    // Trigger a beep on the buzzer for direction change
    digitalWrite(buzzerPin, LOW);
    delay(100);  // Beep duration
    digitalWrite(buzzerPin, HIGH);
  }

  // Display the last detected movement in the Serial Monitor
  displayMovement(lastMovement);

  // Update previous gyro values
  prevGyroX = gyroX;
  prevGyroY = gyroY;

  // Small delay to avoid flooding the serial monitor
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