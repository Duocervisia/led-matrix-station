# LED Matrix Station

![Station](station.jpg)

This project is an Arduino-based information station that uses an LED matrix to display the time, weather information and the departures of trains in the vbb sector. It connects to WiFi to fetch the data and displays it on an LED matrix. The project also includes a distance sensor to measure the distance to an object. If a person is closer than a specified amount of centimeters to the sensor, the weather and departure information will scroll across the LED matrix. If the sensor is not triggered, the station just shows the current time.

## Table of Contents

- [Features](#features)
- [Components](#components)
- [Setup](#setup)
- [License](#license)

## Features

- Weather Information: Fetches and displays weather data.
- Time Synchronization: Synchronizes time using NTP servers.
- LED Matrix Display: Displays the time and weather and public transport information

## Components

- ESP8266: Microcontroller with WiFi capabilities.
- SR04 Ultrasonic Sensor: Measures distance to an object.
- MAX7219 LED Matrix: Displays information.

## Setup

1. Connect the components according to the Schematic.png 

2. Install Libraries: Ensure the following libraries are installed in your Arduino IDE:
    - ArduinoJsonon
    - Timezone
    - TimeLib
    - HC-SR04 HC-SR04
    You can find the libraries in the libaries folder in this project.

3. Configure env.h: Rename "configure-env.h" to "env.h". Update the ssid and password variables in the env.h file with your WiFi credentials. Change the weatherKey to your API key from openweathermap. Other variables can also be adjusted in this file.

4. Upload Code: Upload the provided code to your ESP8266 board.


This project is an Arduino-based information station that uses an LED matrix to display the time, weather information and the departures of trains in the vbb sector. It connects to WiFi to fetch the data and displays it on an LED matrix. The project also includes a distance sensor to measure the distance to an object. If a person is closer than a specified amount of centimeters to the sensor, the weather and departure information will scroll across the LED matrix. If the sensor is not triggered, the station just shows the current time.

## Table of Contents

- [Features](#features)
- [Components](#components)
- [Setup](#setup)
- [License](#license)

## Features

- Weather Information: Fetches and displays weather data.
- Time Synchronization: Synchronizes time using NTP servers.
- LED Matrix Display: Displays the time and weather and public transport information

## Components

- ESP8266: Microcontroller with WiFi capabilities.
- SR04 Ultrasonic Sensor: Measures distance to an object.
- MAX7219 LED Matrix: Displays information.

## Setup

1. Connect the components according to the Schematic.png 

2. Install Libraries: Ensure the following libraries are installed in your Arduino IDE:
    - ArduinoJsonon
    - Timezone
    - TimeLib
    - HC-SR04 HC-SR04
    You can find the libraries in the libaries folder in this project.

3. Configure env.h: Rename "configure-env.h" to "env.h". Update the ssid and password variables in the env.h file with your WiFi credentials. Change the weatherKey to your API key from openweathermap. Other variables can also be adjusted in this file.

4. Upload Code: Upload the provided code to your ESP8266 board.