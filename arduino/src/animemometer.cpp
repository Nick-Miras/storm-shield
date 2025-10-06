//
// Created by Nick Anthony Miras on 10/6/25.
//

#include <Arduino.h>
#include "animemometer.h"

volatile unsigned long pulseCount = 0;
unsigned long lastMeasurementTime = 0;
float windSpeed = 0.0;

constexpr float PULSES_PER_MS = 0.0875;

void mainLoop() {
    unsigned long currentTime = millis();

    if (currentTime - lastMeasurementTime >= 1000) {
        noInterrupts();
        unsigned long count = pulseCount;
        pulseCount = 0;
        interrupts();

        float frequency = count / 1.0;  // 1-second window
        windSpeed = frequency / PULSES_PER_MS;

        Serial.print("Frequency: ");
        Serial.print(frequency);
        Serial.print(" Hz | Wind speed: ");
        Serial.print(windSpeed);
        Serial.println(" m/s");

        lastMeasurementTime = currentTime;
    }
}

void countPulse() {
    if (digitalRead(WIND_SENSOR) == LOW) {
        pulseCount++;
    }
}