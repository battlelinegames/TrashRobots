#include <Bluepad32.h>

#define MC1_IN1 32
#define MC1_IN2 33
#define MC1_IN3 14
#define MC1_IN4 13
#define MC1_ENA 25
#define MC1_ENB 26

#define SERVO_ARM 18
#define SERVO_SMALL 19

/*
#define MC2_IN1 21
#define MC2_IN2 19
#define MC2_IN3 18
#define MC2_IN4 5
#define MC2_ENA 23
#define MC2_ENB 22
*/
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
    Serial.printf("Battery: %d\n", ctl->battery());
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}
/*
MC2_IN1 21
MC2_IN2 19
MC2_IN3 18
MC2_IN4 5
MC2_ENA 23
MC2_ENB 22
*/
void setLeftMotor(ControllerPtr ctl) {
    int y_val = ctl->axisY();
    
    Serial.printf(
        "left motor y_val: %4d\n",
        y_val       
    );
    
    if( y_val > 10 ) {
        digitalWrite(MC1_IN1, HIGH);
        digitalWrite(MC1_IN2, LOW);
        int pwmValue = map(y_val, 0, 512, 0, 255);  // Adjust this based on your needs
        analogWrite(MC1_ENA, pwmValue);

    }
    else if( y_val < -10 ) {
        digitalWrite(MC1_IN2, HIGH);
        digitalWrite(MC1_IN1, LOW);
        int pwmValue = map(-y_val, 0, 512, 0, 255);  // Adjust this based on your needs
        analogWrite(MC1_ENA, pwmValue);
    }
    else {
        digitalWrite(MC1_IN1, LOW);
        digitalWrite(MC1_IN2, LOW);
        analogWrite(MC1_ENA, 0);
    }
}

void setRightMotor(ControllerPtr ctl) {
    int y_val = ctl->axisRY();
    Serial.printf(
        "right motor y_val: %4d\n",
        y_val       
    );
    if( y_val > 10 ) {
        digitalWrite(MC1_IN3, LOW);
        digitalWrite(MC1_IN4, HIGH);
        int pwmValue = map(y_val, 0, 512, 0, 255);  // Adjust this based on your needs
        analogWrite(MC1_ENB, pwmValue);

    }
    else if( y_val < -10 ) {
        digitalWrite(MC1_IN3, HIGH);
        digitalWrite(MC1_IN4, LOW);
        int pwmValue = map(-y_val, 0, 512, 0, 255);  // Adjust this based on your needs
        analogWrite(MC1_ENB, pwmValue);
    }
    else {
        digitalWrite(MC1_IN3, LOW);
        digitalWrite(MC1_IN4, LOW);
        analogWrite(MC1_ENB, 0);
    }
}

/*
void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "axis L: %4d, %4d, axis R: %4d, %4d\n",
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY()       // (-511 - 512) right Y axis
    );
}
*/

void processGamepad(ControllerPtr ctl) {
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...
    int bat = ctl->battery();
    ctl->setColorLED(255 - bat, bat, 0);

    if( ctl->l1() ) {
        analogWrite(SERVO_ARM, 250);
    }
    if( ctl->l2()) {
        analogWrite(SERVO_SMALL, 250);
    }

    if( ctl->r1()) {
        analogWrite(SERVO_ARM, 50);
    }
    if( ctl->r2()) {
        analogWrite(SERVO_SMALL, 50);
    }

    if( ctl->throttle() ) { // l2 10 bit analog

    }
    if( ctl->brake() ) { // r2 10 bit analog

    }

    if( ctl->thumbL() ) { 
        // PRESS IN LEFT STICK
    }
    if( ctl->thumbR() ) {
        // PRESS ON RIGHT STICK
    }

    if (ctl->a()) { // CROSS ON PS
        Serial.printf("PRESS A\n");
        /*
        static int colorIdx = 0;
        // Some gamepads like DS4 and DualSense support changing the color LED.
        // It is possible to change it by calling:
        switch (colorIdx % 3) {
            case 0:
                // Red
                ctl->setColorLED(255, 0, 0);
                break;
            case 1:
                // Green
                ctl->setColorLED(0, 255, 0);
                break;
            case 2:
                // Blue
                ctl->setColorLED(0, 0, 255);
                break;
        }
        colorIdx++;
        */
    }

    if (ctl->b()) { // CIRCLE ON PS
        /*
        static int led = 0;
        led++;
        ctl->setPlayerLEDs(led & 0x0f);
        */
        //analogWrite(SERVO_ARM, 0);
        Serial.printf("PRESS B\n");
        

    }

    if (ctl->x()) { // SQUARE ON PS
        //ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
        //                  0x40 /* strongMagnitude */);
        //analogWrite(SERVO_ARM, 250);
        Serial.printf("PRESS X\n");

    }

    if (ctl->y()) { // TRIANGLE ON PS
        //ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
        //                  0x40 /* strongMagnitude */);
        //analogWrite(SERVO_ARM, 250);
        Serial.printf("PRESS Y\n");

    }

    // Another way to query controller data is by getting the buttons() function.
    // See how the different "dump*" functions dump the Controller info.
    //dumpGamepad(ctl);
    setLeftMotor(ctl);
    setRightMotor(ctl);
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else {
                Serial.println("Unsupported controller");
            }
        }
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Serial.begin(115200);
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedController, &onDisconnectedController);

    int motorPins[] = {
        MC1_IN1, MC1_IN2, MC1_IN3, MC1_IN4, MC1_ENA, MC1_ENB, SERVO_ARM, SERVO_SMALL

    };

    for (int i = 0; i < sizeof(motorPins)/sizeof(motorPins[0]); i++) {
        pinMode(motorPins[i], OUTPUT);
        digitalWrite(motorPins[i], LOW);   // start with everything off
    }


    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    // BP32.forgetBluetoothKeys();

    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    //     vTaskDelay(1);
    delay(100);
}
