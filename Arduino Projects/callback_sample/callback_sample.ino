#include "callb.h"

// Define the callback function
void myCallback() {
    Serial.println("Callback function called!");
}

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    
    // Register the callback function
    registerCallback(myCallback);
}

void loop() {
    // Call the triggerCallback function periodically
    triggerCallback();
    
    // Wait for a second before calling again
    delay(1000);
}
