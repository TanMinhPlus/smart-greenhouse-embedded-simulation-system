# Smart greenhouse embedded simulation system
# 🌱 Smart Greenhouse Embedded System

## 📌 Overview

This project presents a **Smart Greenhouse Control System** built using Arduino, designed to automatically monitor and regulate environmental conditions for optimal plant growth.

The system integrates multiple sensors and actuators to maintain:

* Temperature
* Humidity
* Light intensity
* Soil moisture
* Rain conditions

It was fully designed and validated using **Proteus simulation**.

---

## 🧠 Key Features

* 🌧️ Rain detection with **priority-based control**
* 🌞 Automatic curtain control based on light intensity
* 🌡️ Temperature-based fan & heater control
* 💧 Smart irrigation using soil moisture sensing
* 🖥️ Real-time monitoring via LCD display
* ⚡ Safety-focused design (relay isolation, fuse indicators)

---

## 🏗️ System Architecture

The system is divided into 3 main subsystems:

### 1. Weather & Curtain Control

* Rain sensor
* LDR (light sensor)
* Servo motors (curtain control)

### 2. Climate Control

* DHT11 sensor
* Fan
* Heater (lamp)

### 3. Irrigation Control

* Soil moisture sensor
* Water pump

---

## ⚙️ Hardware Components

* Arduino Uno
* DHT11 (Temperature & Humidity)
* LDR (Light sensor)
* Rain sensor module
* Soil moisture sensor
* Servo motors (curtain)
* Relay modules
* Water pump
* Fan system
* LCD 20x4 (I2C)

---

## 🔌 System Logic

The system uses a **threshold-based + priority logic control**:

* Rain has highest priority → close curtain immediately
* Temperature controls fan/heater
* Soil moisture controls irrigation
* Light controls curtain (only when no rain)

---

## 📂 Project Structure

```id="struct123"
docs/        → Report, diagrams
hardware/    → Proteus schematic
firmware/    → Arduino source code
```

---

## ▶️ Simulation

* Tool: Proteus Design Suite
* Microcontroller: Arduino Uno
* All scenarios tested:

  * High temperature
  * Rain condition
  * Dry soil
  * Light variation
  * Fault detection (blown fuse)

---

## 📊 Results

* Accurate sensor reading ✔
* Correct actuator response ✔
* Priority logic works ✔
* Stable system behavior ✔

---

## ⚠️ Limitations

* No real hardware implementation yet
* Fixed threshold (not adaptive)
* No IoT / remote monitoring

---

## 🚀 Future Improvements

* IoT integration (ESP32)
* Cloud monitoring dashboard
* AI-based control (adaptive logic)
* Energy optimization (solar)

---

## 📸 Demo

(Add your Proteus screenshot here)

---

## 👨‍💻 Authors

* Doan Duy Long
* Pham Tan Minh
* Nguyen Tran Hai Phong

---

## 📄 License

MIT License

