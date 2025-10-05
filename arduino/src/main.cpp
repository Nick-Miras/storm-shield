//
// Created by Nick Anthony Miras on 10/5/25.
//
#include <Arduino.h>
#include <Servo.h>

#define WIND_SENSOR A0
#define MOISTURE_SENSOR A1
#define TOP_SERVO 3
#define TOP_SERVO_POWER 4
#define BOTTOM_SERVO 5

Servo topServo;
Servo bottomServo;

constexpr int threshold = 500;

/*
 * This function tells a servo to complete its task under n milliseconds.
 */
void writeDelay(const int targetAngle, const int milliSeconds, Servo *servo) {
    const int initialAngle = servo->read();
    const int distance = targetAngle - initialAngle;
    const int incrementalDelay = milliSeconds / abs(distance);
    for (int i = 1; i < abs(distance) + 1; i = i + 2) {
        int writeAngle;
        if (distance > 0) {
            writeAngle = initialAngle + i;
        } else {
            writeAngle = initialAngle - i;
        }
        servo->write(writeAngle);
        delay(incrementalDelay);
    }
}

/* This function is for controlling two servos with a write delay.
 * TODO: Read both initial angles and turn each servo with a different angleIncrement.
 */
void writeDelay(const int targetAngle, const int milliSeconds, Servo *servo1, Servo *servo2) {
    const int initialAngle = servo1->read();
    const int distance = targetAngle - initialAngle;
    const int incrementalDelay = milliSeconds / abs(distance);
    for (int i = 1; i < abs(distance) + 1; i = i + 2) {
        int writeAngle;
        if (distance > 0) {
            writeAngle = initialAngle + i;
        } else {
            writeAngle = initialAngle - i;
        }
        servo1->write(writeAngle);
        servo2->write(writeAngle);
        delay(incrementalDelay);
    }
}

/*
 *  TODO: This function informs the Wemos device to publish precipitation times.
 */
void informWemos() {}

int readSensor() {
    const int value = analogRead(MOISTURE_SENSOR);  // Read the analog value form sensor
    return value;                       // Return analog moisture value
}

void _runOnce() {
    delay(3000);
    writeDelay(90, 10000, &topServo);
}

void setup() {
    Serial.begin(9600);
    topServo.attach(TOP_SERVO);
    bottomServo.attach(BOTTOM_SERVO);
    topServo.write(0);
    bottomServo.write(0);

    _runOnce();
}

// void loop() {
//     //get the reading from the function below and print it
//     Serial.print("Analog output: ");
//     Serial.println(readSensor());
//     delay(1000);
// }

void loop() {
    if (readSensor() > threshold) {
        writeDelay(90, 10000, &topServo, &bottomServo);
        informWemos();
    } else {
        writeDelay(0, 10000, &topServo, &bottomServo);
    }
    delay(1000);
}
