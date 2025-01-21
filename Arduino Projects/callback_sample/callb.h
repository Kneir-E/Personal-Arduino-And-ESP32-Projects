#ifndef CALLB_H
#define CALLB_H

typedef void (*CallbackFunction)();

// Function that accepts a callback
void registerCallback(CallbackFunction callback);

CallbackFunction registeredCallback = nullptr;

void registerCallback(CallbackFunction callback) {
    registeredCallback = callback;
}

// Function to call the registered callback
void triggerCallback() {
    if (registeredCallback != nullptr) {
        registeredCallback();
    }
}

#endif