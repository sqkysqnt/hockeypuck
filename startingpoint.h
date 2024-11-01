#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "SensorQMI8658.hpp"
#include <TFT_eSPI.h>
#include "utilities.h"


SensorQMI8658 qmi;
IMUdata acc;
IMUdata gyr;

TFT_eSPI tft = TFT_eSPI();


void displayData(float ax, float ay, float az, float gx, float gy, float gz) {
    tft.fillScreen(TFT_BLACK); // Clear screen each time for new data
    // Display accelerometer data
    tft.setCursor(0, 0);
    tft.print("ACCEL:");
    tft.setCursor(0, 20);
    tft.printf("X: %.2f", ax);
    tft.setCursor(0, 40);
    tft.printf("Y: %.2f", ay);
    tft.setCursor(0, 60);
    tft.printf("Z: %.2f", az);
    // Display gyroscope data
    tft.setCursor(0, 100);
    tft.print("GYRO:");
    tft.setCursor(0, 120);
    tft.printf("X: %.2f", gx);
    tft.setCursor(0, 140);
    tft.printf("Y: %.2f", gy);
    tft.setCursor(0, 160);
    tft.printf("Z: %.2f", gz);
}


void setup()
{
    Serial.begin(115200);
    while (!Serial);

#if IMU_INT > 0
    qmi.setPins(IMU_INT);
#endif

#ifdef USE_WIRE
    //Using WIRE !!
    if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL)) {
        Serial.println("Failed to find QMI8658 - check your wiring!");
        while (1) {
            delay(1000);
        }
    }
#endif

    /* Get chip id*/
    Serial.print("Device ID:");
    Serial.println(qmi.getChipID(), HEX);


    if (qmi.selfTestAccel()) {
        Serial.println("Accelerometer self-test successful");
    } else {
        Serial.println("Accelerometer self-test failed!");
    }

    if (qmi.selfTestGyro()) {
        Serial.println("Gyroscope self-test successful");
    } else {
        Serial.println("Gyroscope self-test failed!");
    }


    qmi.configAccelerometer(
        /*
         * ACC_RANGE_2G
         * ACC_RANGE_4G
         * ACC_RANGE_8G
         * ACC_RANGE_16G
         * */
        SensorQMI8658::ACC_RANGE_4G,
        /*
         * ACC_ODR_1000H
         * ACC_ODR_500Hz
         * ACC_ODR_250Hz
         * ACC_ODR_125Hz
         * ACC_ODR_62_5Hz
         * ACC_ODR_31_25Hz
         * ACC_ODR_LOWPOWER_128Hz
         * ACC_ODR_LOWPOWER_21Hz
         * ACC_ODR_LOWPOWER_11Hz
         * ACC_ODR_LOWPOWER_3H
        * */
        SensorQMI8658::ACC_ODR_1000Hz,
        /*
        *  LPF_MODE_0     //2.66% of ODR
        *  LPF_MODE_1     //3.63% of ODR
        *  LPF_MODE_2     //5.39% of ODR
        *  LPF_MODE_3     //13.37% of ODR
        *  LPF_OFF        // OFF Low-Pass Fitter
        * */
        SensorQMI8658::LPF_MODE_0);




    qmi.configGyroscope(
        /*
        * GYR_RANGE_16DPS
        * GYR_RANGE_32DPS
        * GYR_RANGE_64DPS
        * GYR_RANGE_128DPS
        * GYR_RANGE_256DPS
        * GYR_RANGE_512DPS
        * GYR_RANGE_1024DPS
        * */
        SensorQMI8658::GYR_RANGE_64DPS,
        /*
         * GYR_ODR_7174_4Hz
         * GYR_ODR_3587_2Hz
         * GYR_ODR_1793_6Hz
         * GYR_ODR_896_8Hz
         * GYR_ODR_448_4Hz
         * GYR_ODR_224_2Hz
         * GYR_ODR_112_1Hz
         * GYR_ODR_56_05Hz
         * GYR_ODR_28_025H
         * */
        SensorQMI8658::GYR_ODR_896_8Hz,
        /*
        *  LPF_MODE_0     //2.66% of ODR
        *  LPF_MODE_1     //3.63% of ODR
        *  LPF_MODE_2     //5.39% of ODR
        *  LPF_MODE_3     //13.37% of ODR
        *  LPF_OFF        // OFF Low-Pass Fitter
        * */
        SensorQMI8658::LPF_MODE_3);




    /*
    * If both the accelerometer and gyroscope sensors are turned on at the same time,
    * the output frequency will be based on the gyroscope output frequency.
    * The example configuration is 896.8HZ output frequency,
    * so the acceleration output frequency is also limited to 896.8HZ
    * */
    qmi.enableGyroscope();
    qmi.enableAccelerometer();
    qmi.dumpCtrlRegister();

    Serial.println("Ready to Read data");

    
    // Initialize TFT display - uncommenting the following 6 lines puts the device in a boot loop    
    //tft.init();
    //tft.setRotation(0);
    //tft.fillScreen(TFT_BLACK);
    //tft.setTextColor(TFT_WHITE, TFT_BLACK);
    //tft.setTextSize(1);
    //Serial.println("Display initialized");

}


void loop()
{
    // When the interrupt pin is passed in through setPin,
    // the GPIO will be read to see if the data is ready.
    if (qmi.getDataReady()) {

        // Serial.print("Timestamp:");
        // Serial.print(qmi.getTimestamp());

        if (qmi.getAccelerometer(acc.x, acc.y, acc.z)) {

            // Print to serial plotter
            //Serial.print("ACCEL.x:"); Serial.print(acc.x); Serial.print(",");
            //Serial.print("ACCEL.y:"); Serial.print(acc.y); Serial.print(",");
            //Serial.print("ACCEL.z:"); Serial.print(acc.z); Serial.println();

            /*
            m2/s to mg
            Serial.print(" ACCEL.x:"); Serial.print(acc.x * 1000); Serial.println(" mg");
            Serial.print(",ACCEL.y:"); Serial.print(acc.y * 1000); Serial.println(" mg");
            Serial.print(",ACCEL.z:"); Serial.print(acc.z * 1000); Serial.println(" mg");
            */

        }

        if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z)) {


            // Print to serial plotter
            //Serial.print("GYRO.x:"); Serial.print(gyr.x); Serial.print(",");
            //Serial.print("GYRO.y:"); Serial.print(gyr.y); Serial.print(",");
            //Serial.print("GYRO.z:"); Serial.print(gyr.z); Serial.println();


            // Serial.print(" GYRO.x:"); Serial.print(gyr.x); Serial.println(" degrees/sec");
            // Serial.print(",GYRO.y:"); Serial.print(gyr.y); Serial.println(" degrees/sec");
            // Serial.print(",GYRO.z:"); Serial.print(gyr.z); Serial.println(" degrees/sec");

        }

        //if (qmi.getAccelerometer(acc.x, acc.y, acc.z) && qmi.getGyroscope(gyr.x, gyr.y, gyr.z)) {
        //    displayData(acc.x, acc.y, acc.z, gyr.x, gyr.y, gyr.z); // Show data on TFT display
        //}
        // Serial.print("Temperature:");
        // Serial.print(qmi.getTemperature_C());
        // Serial.println(" degrees C");

    }
    delay(100);
}

