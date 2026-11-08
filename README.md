# Rock Paper Scissors SHOOOT!! 
This robot is a mechanical hand that will always beat you at rock paper scissors with a special little trick up their sleeves (a camera?)

![Images/Screenshot 2026-11-08 205758_edited.png](https://github.com/manula5454/Rock-Paper-Scissors-SHOOT/blob/153f294fad6eae972a9b807f018260d4f4bc5df1/Images/Screenshot%202026-11-08%20205758_edited.png)

### This project uses an ESP32 camera module with WIFI and hardware including just servo motors. Data collected from the camera is sent to a webpage which displays it in graphs to evaluate your environment in the morning. It can also act as an alarm. 

* ESP32
* Mechanical hand
* rock paper scissors


**MainCodeFun IS THE FINAL CODE**

## Using this repo:
1. Code is located in [Arduino](https://github.com/manula5454/Sleep-Helper/blob/80c0facc8ba70d53b0e343af6b2a82c639735664/MainCodeFun/MainCodeFun.ino) and can easily be uploaded to ESP32 with the ESP32 board library 
2. Other code can be used to test some sensors and see early iterations of my code for hardware [Other Code](https://github.com/manula5454/Sleep-Helper/tree/8dfda3dc41291881e5782dcd2b5fa8ecebd2d8ed/Other%20Code)
3. Get a breadboard and start wiring according to the table below.
4. Download the webpage and copy the path for the .html file and copy link onto web browser of your choice [Webpage](https://github.com/manula5454/Sleep-Helper/tree/8dfda3dc41291881e5782dcd2b5fa8ecebd2d8ed/Webpage)
5. Connect your board to the webpage (make sure bluetooth on your device is enabled)
6. Edit Arduino code for your sensors sensitivity.
7. If you have a 3D printer download the CAD files and enhance your nightstand experience! [CAD](CAD)
8. Place it on your night stand and good night!

CAD --> No printable files are available as they should be tweaked for your own 3D printer and your own components.

## heres how I made the project :)
1. Started by displaying an OLED screen with data from DHT11 module (temp & humidity)
2. Added other modules to add sound and light sensing and some LEDs
3. Created some alarm clock functionality with code and displayed on OLED (this includes current time and alarm)
4. Polished code up for every function
5. Started building a webpage which displays data in a graph
6. Designed CAD exterior for project.

![CAD design](https://github.com/manula5454/Sleep-Helper/blob/21d42190051f7ac357d3748c608b7ce5381138d3/Images/CAD.png)

## Roadblocks
* Trying to use Arduino UNO as it had too little SRAM for my program
* Connecting faulty sensors from unverified sources (e.g. my DHT11 did not work and needed another)
* Wires loosening on motor 

## Code - Arduino IDE
Libraries : 

```
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
```
(can be found in IDE)

```void EnvironmentShow(float t, float h) ``` -- used to position temp and humidity data on OLED
```void displayTime()``` -- Displays time on OLED screen
```vvoid NoiseAlert()``` -- Displays a noise alert if sound sensor detects noise over a specific threshold
```void updateClock()``` -- updates current time on OLED 

## Hardware 

Parts List: 

* ESP32
* DHT11 Module
* Big sound sensor module
* Rotary encoder module w/ switch
* LDR
* LED
* DC motor
* 10uF Capacitor (connect to power rail)
* Resistors
* Active Buzzer (can be module)
* Pushbutton

### Wiring table

| Component | Component pin | ESP32 pin | Notes |
|---|---|---|---|
| OLED (SSD1306, I2C) | SDA | GPIO21 | I2C data |
| OLED (SSD1306, I2C) | SCL | GPIO22 | I2C clock |
| OLED (SSD1306, I2C) | VCC | 3V3 | |
| OLED (SSD1306, I2C) | GND | GND | |
| DHT11 | DATA | GPIO4 | Temperature and humidity |
| DHT11 | VCC | 3V3 | |
| DHT11 | GND | GND | |
| Rotary encoder | CLK | GPIO32 | |
| Rotary encoder | DT | GPIO33 | |
| Rotary encoder | SW | GPIO23 | `INPUT_PULLUP` |
| Rotary encoder | + | 3V3 | |
| Rotary encoder | GND | GND | |
| Dismiss button | Leg 1 | GPIO5 | `INPUT_PULLUP` |
| Dismiss button | Leg 2 | GND | |
| Sound sensor | AO | GPIO36 | ADC1, input only |
| Sound sensor | VCC | 3V3 | Keeps output within 3.3 V |
| Sound sensor | GND | GND | |
| LDR + 10 kΩ divider | Midpoint | GPIO39 | ADC1, input only |
| LDR + 10 kΩ divider | LDR top leg | 3V3 | 10 kΩ resistor from midpoint to GND |
| Buzzer (active) | + | GPIO18 | |
| Buzzer (active) | − | GND | |
| Status LED | Anode via 220 Ω | GPIO13 | Cathode to GND |
| Relay module | IN | GPIO19 | |
| Relay module | VCC | 5V | |
| Relay module | GND | GND | |
| 5V supply | Relay COM | Same supply that powers the ESP32 |
| Relay NO | Motor wire 1 | Switched leg |
| Motor wire 2 | GND | Common ground |
| Relay NC | Not connected | |
| 470 µF capacitor (optional) | Across 5V and GND near the relay | Long leg to 5V |
## Webpage

To connect your ESP32 to the webpage:

1. Click Connect to device
2. Select your device
3. The text on the top left should say Connected
4. To double check wiring and connection is good toggle the fan button on the webpage and see if the fan turns on

Clear Saved Data --> deletes all data so far

A **baseline** exists for each graph to help understand what your environment should be, can be changed in code.

**IMPORTANT: As the webpage is local only it can store data continously BUT if the site is refreshed it will disconnect from device and thus not continue reading data untilit is manually connected again (but still keep the data collected so far)**
