#include <Arduino.h>
#include "MotionDetect.h"
#include <SD_MMC.h>

MotionDetect::Camera camera;
MotionDetect::Motion motion;


#define SD_MMC_D0         40           //Hardwired to SD card
#define SD_MMC_CLK        39           //Hardwired to SD card
#define SD_MMC_CMD        38           //Hardwired to SD card

#define stop delay(5000); while(1);

void setup() {
    Serial.begin(115200);
    delay(3000);
    ESP_LOGI("setup", "Start");

    camera.s3wroom();
    ESP_LOGI("setup", "Camera Model set to WROVER");
    camera.setXclkFreq(22000000);                          // Seems to work better at 22 HHz

    /* Initialize the camera */
    if (!camera.begin()) {
        ESP_LOGE("Setup", "camera begin failure");
        stop;
    }
    else {
        ESP_LOGI("SUCCESS", "Camera OK");
    } 

    sensor_t * s = esp_camera_sensor_get();
        s->set_vflip(s, 1); // flip it back
        s->set_brightness(s, 1); // up the brightness just a bit
        s->set_saturation(s, 0); // lower the saturation 

    Serial.println("Camera initialized");
    SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
    if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
      ESP_LOGE("SD", "Micro SD Card Mount Failed #####");
      stop;
    }
    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE){
        ESP_LOGE("SD", "No Micro SD Card attached #####");
        stop;
    }
  
    ESP_LOGI("SD", "SD_MMC Initialization Success! Free Space: %llu MB", (SD_MMC.totalBytes() - SD_MMC.usedBytes()) / (1024 * 1024));

    if (!camera.capture()) {
        ESP_LOGW("loop", "camera capture Failure");
        stop;
    }
    const char* filename = "/test1.jpg";
    File file = SD_MMC.open(filename, FILE_WRITE);
    if(!file){
        ESP_LOGE("SD", "Failed to open file %s for writing  \n", filename);
        stop;
    }
    file.write(camera.getFrameBufptr(), camera.getFrameSize());
    file.close();
    ESP_LOGI("SD", "File saved: %s", filename);

}

void loop() {
}