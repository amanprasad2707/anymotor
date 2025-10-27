# 🌀 AnyMotor Library

**Universal Motor Control Library for Arduino**  
Drive any DC motor through any H-bridge driver — cleanly, safely, and efficiently.

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-green.svg)](https://www.arduino.cc/)
[![Version](https://img.shields.io/badge/version-1.0.0-orange.svg)](https://github.com/amanprasad2707/anymotor)

---

## 🚀 Overview

`AnyMotor` is a **universal DC motor driver library** for Arduino that simplifies motor control using any standard H-bridge driver such as **L298N**, **L293D**, **TB6612**, or **MX1508**.  

It offers high-level functions for **direction**, **speed**, **acceleration**, **deadband compensation**, and even **dual-motor movement** — all wrapped in a clean, object-oriented API.

---

## ⚙️ Features

✅ Simple API for forward, backward, stop, and brake  
✅ Supports both **single** and **dual motor** setups  
✅ **PWM speed control** (0–255)  
✅ **Deadband compensation** for smooth starts  
✅ **Acceleration & deceleration** functions  
✅ **Timed motion** (move for specific duration)  
✅ Optional **inversion** for reversed wiring  
✅ Compatible with most **H-bridge motor drivers**  
✅ Lightweight and portable — perfect for embedded use  

---

## 🧩 Supported Motor Drivers

| Driver  | Channels | Logic Voltage | Notes |
|----------|-----------|----------------|--------|
| L298N    | 2         | 5V             | Classic, robust H-bridge |
| L293D    | 2         | 5V             | Popular for small motors |
| TB6612FNG| 2         | 3.3V–5V        | Efficient, low heat |
| MX1508   | 2         | 3.3V–5V        | Compact and cheap |

---

## 📦 Installation

1. Download this repository as a ZIP.
2. In Arduino IDE, go to **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded ZIP file.
