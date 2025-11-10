#include <Arduino.h>
#include "MotionDetect.h"
#include <esp_log.h>

MotionDetect::Camera camera;
MotionDetect::Motion motion;

void setup() {
    Serial.begin(115200);
    delay(3000);
    ESP_LOGI("setup", "Start");

    camera.ESP32_Cam();
    
    /* Initialize the camera */
    if (!camera.begin()) {
        ESP_LOGE("Setup", "camera begin failure");
    }
    else {
        ESP_LOGI("SUCCESS", "Camera OK");
    }            
    sensor_t * s = esp_camera_sensor_get();
        s->set_framesize(s, FRAMESIZE_QVGA);      //Sets the frame size to QVGA 320x240
        s->set_exposure_ctrl(s, 0);               // 0 = disable , 1 = enable Enables or disables automatic exposure control (AEC)
        s->set_aec2(s, 0);                        // 0 = disable , 1 = enable Enables or disables AEC 2, an enhanced AEC algorithm

    for (int i = 0; i < 10; i++) {
        if (!camera.capture()) {
            ESP_LOGW("loop", "camera capture Failure");
            delay(1000);
            esp_restart();
        }
        motion.baseline(camera.getFrameBufptr(), camera.getFrameSize());   // set a baseline frame to compare motion against
    }
}

void loop() {
    if (!camera.capture()) {
        ESP_LOGW("loop", "camera capture Failure");
        delay(1000);
        return;
    }
    motion.baseline(camera.getFrameBufptr(), camera.getFrameSize());
    Serial.printf("Baseline change. Light Level: %.2f pixel change: %.2f\n", motion.getlightLevel(), motion.getPercentDiff());
    delay (5000); // wait 5 seconds before next capture
}