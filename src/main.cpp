#include <Arduino.h>
#include "MotionDetect.h"

MotionDetect::Camera camera;
MotionDetect::Motion motion;

#define stop delay(5000); while(1);

void setup() {
    Serial.begin(115200);
    delay(3000);
    ESP_LOGI("setup", "Start");

    camera.wrover();
    ESP_LOGI("setup", "Camera Model set to WROVER");

    /* Initialize the camera */
    if (!camera.begin()) {
        ESP_LOGE("Setup", "camera begin failure");
        stop;
    }
    else {
        ESP_LOGI("SUCCESS", "Camera OK");
    } 

    Serial.println("Camera initialized");

}

void loop() {

    if (!camera.capture()) {
        ESP_LOGW("loop", "camera capture Failure");
        stop;
    }

    delay(5000); // wait 5 seconds before next capture

}