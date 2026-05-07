# 🚗 Smart Reverse Parking Assist System

> An Arduino-based intelligent proximity detection system that guides drivers safely while reversing — using ultrasonic sensing, multi-level visual alerts, audio feedback, and a real-time 7-segment distance display.

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [Circuit Diagram & Pin Configuration](#-circuit-diagram--pin-configuration)
- [System Architecture](#-system-architecture)
- [Alert Level Reference](#-alert-level-reference)
- [7-Segment Display Logic](#-7-segment-display-logic)
- [Code Walkthrough](#-code-walkthrough)
- [How It Works — Step by Step](#-how-it-works--step-by-step)
- [Serial Monitor Output](#-serial-monitor-output)
- [Setup & Installation](#-setup--installation)
- [Customization Guide](#-customization-guide)
- [Troubleshooting](#-troubleshooting)
- [Future Improvements](#-future-improvements)
- [License](#-license)

---

## 🔍 Overview

The **Smart Reverse Parking Assist System** is an embedded electronics project built on the **Arduino Uno** platform. Inspired by real-world reverse parking sensors found in modern vehicles, this system uses an **HC-SR04 ultrasonic sensor** to measure the distance between the vehicle (or any moving object) and an obstacle in real time.

Based on the measured distance, the system classifies the situation into **5 distinct alert levels** — from completely safe to critically dangerous — and responds with:

- **Colored LED indicators** (Green / Yellow / Red)
- **Variable-frequency buzzer tones** that increase in urgency
- **A 7-segment display** showing the current alert level (1–5)
- **Serial Monitor logging** for debugging and observation

This project is ideal for:
- 🎓 Arduino beginners learning sensor integration
- 🚘 Hobbyists building DIY vehicle safety systems
- 🛠️ Makers prototyping proximity-aware automation
- 📚 Electronics students learning about PWM, GPIO, and timing

---

## ✨ Features

| Feature | Description |
|---|---|
| 🔊 **Adaptive Buzzer** | Beep frequency and interval speed up as distance decreases |
| 💡 **3-LED Traffic Light** | Intuitive green → yellow → red visual cues |
| 📟 **7-Segment Display** | Shows live alert level (1 = danger, 5 = safe) |
| 📡 **Ultrasonic Sensor** | Accurate distance measurement up to ~400 cm |
| 🖥️ **Serial Logging** | Real-time distance values printed to Serial Monitor |
| ⚡ **Non-blocking Design** | Fast 50ms refresh loop for responsive feedback |

---

## 🧰 Hardware Requirements

| Component | Quantity | Notes |
|---|---|---|
| Arduino Uno (or compatible) | 1 | ATmega328P based board |
| HC-SR04 Ultrasonic Sensor | 1 | 2 cm – 400 cm range |
| Green LED | 1 | 5mm standard |
| Yellow LED | 1 | 5mm standard |
| Red LED | 1 | 5mm standard |
| Passive/Active Buzzer | 1 | Works with `tone()` function |
| 7-Segment Display (Common Cathode) | 1 | Single digit |
| 220Ω Resistors | 3 | One per LED |
| 100Ω Resistors | 7 | One per segment pin |
| Breadboard | 1 | Full or half size |
| Jumper Wires | ~30 | Male-to-male |
| USB Cable (Type-A to Type-B) | 1 | For programming & power |

> ⚠️ **Important:** This project uses a **Common Cathode** 7-segment display. If you use a **Common Anode** display, you must invert all segment HIGH/LOW values in the `numbers` array.

---

## 🔌 Circuit Diagram & Pin Configuration

### Pin Mapping Table

| Arduino Pin | Component | Role |
|---|---|---|
| `Pin 9` | HC-SR04 — TRIG | Sends ultrasonic pulse |
| `Pin 10` | HC-SR04 — ECHO | Receives reflected pulse |
| `Pin 2` | Green LED (+) | Safe zone indicator |
| `Pin 3` | Yellow LED (+) | Caution indicator |
| `Pin 4` | Red LED (+) | Danger indicator |
| `Pin 5` | Buzzer (+) | Audio alert output |
| `Pin 6` | 7-Seg — Segment A | Horizontal top |
| `Pin 7` | 7-Seg — Segment B | Vertical top-right |
| `Pin 8` | 7-Seg — Segment C | Vertical bottom-right |
| `Pin 11` | 7-Seg — Segment D | Horizontal bottom |
| `Pin 12` | 7-Seg — Segment E | Vertical bottom-left |
| `Pin 13` | 7-Seg — Segment F | Vertical top-left |
| `Pin A0` | 7-Seg — Segment G | Horizontal middle |
| `GND` | All component GNDs | Common ground |
| `5V` | HC-SR04 VCC | Sensor power |

### Wiring Notes

- **LEDs:** Connect the **anode (+, longer leg)** to the Arduino pin through a **220Ω resistor**. Connect the **cathode (–, shorter leg)** to **GND**.
- **Buzzer:** Connect the **positive pin** to `Pin 5` and the **negative pin** to `GND`.
- **HC-SR04:** VCC → `5V`, GND → `GND`, TRIG → `Pin 9`, ECHO → `Pin 10`.
- **7-Segment Display (Common Cathode):** Connect the **common cathode pin(s)** to `GND`. Connect each segment pin through a **100Ω resistor** to the corresponding Arduino pin (A0, 6–8, 11–13).

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     ARDUINO UNO                             │
│                                                             │
│  ┌──────────────┐     ┌───────────────────────────────┐    │
│  │ HC-SR04      │────▶│  Distance Calculation Engine  │    │
│  │ Ultrasonic   │     │  distance = duration×0.034÷2  │    │
│  │ Sensor       │     └──────────────┬────────────────┘    │
│  └──────────────┘                    │                      │
│                            ┌─────────▼──────────┐          │
│                            │  Level Classifier  │          │
│                            │  L5: > 40 cm SAFE  │          │
│                            │  L4: > 30 cm SCAUT │          │
│                            │  L3: > 20 cm CAUT  │          │
│                            │  L2: > 10 cm DNGR  │          │
│                            │  L1: ≤ 10 cm CRIT  │          │
│                            └──┬──────┬──────┬───┘          │
│                               │      │      │               │
│                    ┌──────────▼──┐ ┌─▼──┐ ┌▼────────────┐ │
│                    │  LED Array  │ │BZR │ │ 7-Seg Disp  │ │
│                    │ GRN/YLW/RED │ │🔊  │ │  Level 1–5  │ │
│                    └─────────────┘ └────┘ └─────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

---

## 🚦 Alert Level Reference

The system defines **5 proximity levels**, each triggering a unique combination of LED, buzzer, and display behavior:

---

### 🟢 Level 5 — SAFE
```
Distance  :  > 40 cm
LED       :  GREEN ON
Buzzer    :  Silent
Display   :  5
```
> The path is clear. No obstacles detected within the danger zone. The vehicle may continue reversing freely.

---

### 🟢 Level 4 — SAFE CAUTION
```
Distance  :  30 cm – 40 cm
LED       :  GREEN ON
Buzzer    :  1000 Hz | ON 500ms / OFF 500ms (slow beep)
Display   :  4
```
> An obstacle has been detected within moderate range. The driver is alerted with a slow, calm beep to begin paying attention.

---

### 🟡 Level 3 — CAUTION
```
Distance  :  20 cm – 30 cm
LED       :  YELLOW ON
Buzzer    :  1200 Hz | ON 300ms / OFF 300ms (medium beep)
Display   :  3
```
> The obstacle is getting closer. The yellow LED activates and beeping speeds up to indicate increasing urgency.

---

### 🔴 Level 2 — DANGER
```
Distance  :  10 cm – 20 cm
LED       :  RED ON
Buzzer    :  1500 Hz | ON 150ms / OFF 150ms (fast beep)
Display   :  2
```
> The vehicle is dangerously close to the obstacle. Rapid beeping and red light urge the driver to slow down or stop immediately.

---

### 🔴 Level 1 — CRITICAL / VERY DANGEROUS
```
Distance  :  ≤ 10 cm
LED       :  RED ON
Buzzer    :  2000 Hz | Continuous (no break)
Display   :  1
```
> **STOP!** The obstacle is critically close. A continuous high-pitched alarm sounds without interruption. Immediate action is required.

---

### Summary Table

| Level | Label | Distance | LED | Buzzer | Delay (ms) |
|---|---|---|---|---|---|
| 5 | Safe | > 40 cm | 🟢 Green | Off | — |
| 4 | Safe Caution | 30–40 cm | 🟢 Green | 1000 Hz | 500 ON / 500 OFF |
| 3 | Caution | 20–30 cm | 🟡 Yellow | 1200 Hz | 300 ON / 300 OFF |
| 2 | Danger | 10–20 cm | 🔴 Red | 1500 Hz | 150 ON / 150 OFF |
| 1 | Critical | ≤ 10 cm | 🔴 Red | 2000 Hz | Continuous |

---

## 📟 7-Segment Display Logic

The 7-segment display shows the current **alert level (1–5)** to give the driver (or an observer) an immediate numeric reference.

### Segment Layout

```
 _
|_|   ←  Segments: a (top), b (top-right), c (bottom-right),
|_|          d (bottom), e (bottom-left), f (top-left), g (middle)
```

### Digit Encoding (Common Cathode — HIGH = segment ON)

| Digit | a | b | c | d | e | f | g |
|---|---|---|---|---|---|---|---|
| 0 | 1 | 1 | 1 | 1 | 1 | 1 | 0 |
| 1 | 0 | 1 | 1 | 0 | 0 | 0 | 0 |
| 2 | 1 | 1 | 0 | 1 | 1 | 0 | 1 |
| 3 | 1 | 1 | 1 | 1 | 0 | 0 | 1 |
| 4 | 0 | 1 | 1 | 0 | 0 | 1 | 1 |
| 5 | 1 | 0 | 1 | 1 | 0 | 1 | 1 |

> Only digits **1 through 5** are used by this system. The full 0–9 lookup table is defined in the code for extensibility.

### `displayDigit()` Function

```cpp
void displayDigit(int num) {
  for(int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], numbers[num][i]);
  }
}
```

This function iterates over all 7 segment pins and sets each one HIGH or LOW based on the pre-encoded pattern in `numbers[num]`. Calling `displayDigit(level)` at the end of every loop cycle ensures the display always reflects the current proximity level.

---

## 🧠 Code Walkthrough

### 1. Pin Definitions

```cpp
#define TRIG_PIN 9
#define ECHO_PIN 10
#define GREEN_LED 2
#define YELLOW_LED 3
#define RED_LED 4
#define BUZZER 5
int segmentPins[7] = {6, 7, 8, 11, 12, 13, A0};
```

All hardware connections are defined using `#define` macros and an integer array for easy modification. Using named constants instead of raw numbers makes the code readable and maintainable.

---

### 2. Distance Measurement

```cpp
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);

duration = pulseIn(ECHO_PIN, HIGH);
distance = duration * 0.034 / 2;
```

**How it works:**
1. A **2 µs LOW pulse** clears any residual signal on the TRIG pin.
2. A **10 µs HIGH pulse** triggers the sensor to emit 8 ultrasonic bursts at 40 kHz.
3. The sound waves travel outward, bounce off an obstacle, and return.
4. `pulseIn()` measures the time (in microseconds) the ECHO pin stays HIGH — this is the round-trip travel time of sound.
5. The formula `duration × 0.034 / 2` converts travel time to centimeters:
   - Speed of sound ≈ **0.034 cm/µs**
   - Divide by **2** because the signal travels *to* the obstacle and *back*

---

### 3. Level Classification (if-else Chain)

```cpp
if(distance > 40)       { level = 5; /* SAFE     */ }
else if(distance > 30)  { level = 4; /* S.CAUTION*/ }
else if(distance > 20)  { level = 3; /* CAUTION  */ }
else if(distance > 10)  { level = 2; /* DANGER   */ }
else                    { level = 1; /* CRITICAL */ }
```

The conditions are checked from least urgent to most urgent. Each block:
- Sets the `level` variable
- Turns the correct LED(s) ON/OFF
- Activates the buzzer with the appropriate frequency and timing

---

### 4. Buzzer Timing Strategy

The beep pattern uses **`delay()`** between `tone()` and `noTone()` calls:

```cpp
tone(BUZZER, 1000);   // play 1000 Hz
delay(500);           // hold for 500ms
noTone(BUZZER);       // silence
delay(500);           // pause for 500ms
```

As distance decreases, both the **frequency increases** (pitch gets higher) and the **delay shrinks** (beeps get faster), naturally mimicking real automotive parking sensors.

> ⚠️ **Note:** The `delay()` calls inside each level block cause the loop to pause during beeping. This is intentional for simple implementations, but means the sensor is *not* continuously sampled during the delay period. See [Future Improvements](#-future-improvements) for a non-blocking alternative.

---

### 5. Main Loop Flow

```
loop() runs every ~50ms (plus buzzer delay overhead)
  │
  ├── Trigger HC-SR04 → measure distance
  ├── Print to Serial Monitor
  ├── Evaluate level (1–5)
  ├── Set LED states
  ├── Activate buzzer pattern
  ├── Display level on 7-segment
  └── delay(50) → repeat
```

---

## 🖥️ Serial Monitor Output

Open the Arduino IDE Serial Monitor at **9600 baud** to see real-time distance readings:

```
Distance: 87 cm
Distance: 62 cm
Distance: 45 cm
Distance: 38 cm
Distance: 27 cm
Distance: 18 cm
Distance: 9 cm
```

This is invaluable for debugging, calibration, and verifying sensor placement.

---

## 🚀 Setup & Installation

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) (v1.8+ or v2.x)
- Arduino Uno board with USB cable
- All components listed in [Hardware Requirements](#-hardware-requirements)

### Steps

**1. Assemble the Circuit**

Wire all components to the Arduino following the [Pin Configuration table](#pin-mapping-table). Double-check polarity on LEDs and the buzzer before powering on.

**2. Clone or Download the Code**

```bash
git clone https://github.com/yourusername/smart-parking-assist.git
```
Or download the `.ino` file directly.

**3. Open in Arduino IDE**

```
File → Open → smart_parking_assist.ino
```

**4. Select Board and Port**

```
Tools → Board → Arduino Uno
Tools → Port → (select your COM port)
```

**5. Upload**

Click the **Upload** button (→) or press `Ctrl + U`.

**6. Test**

- Open `Tools → Serial Monitor` and set baud rate to **9600**.
- Move your hand toward the ultrasonic sensor and observe the LEDs, buzzer, and display responding to proximity changes.

---

## 🎛️ Customization Guide

### Changing Distance Thresholds

Modify the `if-else` conditions in `loop()`:

```cpp
// Example: stricter thresholds for a smaller vehicle
if(distance > 60)       { level = 5; }  // was > 40
else if(distance > 45)  { level = 4; }  // was > 30
else if(distance > 30)  { level = 3; }  // was > 20
else if(distance > 15)  { level = 2; }  // was > 10
else                    { level = 1; }
```

### Changing Buzzer Frequencies

Find the `tone()` calls and adjust the frequency argument (in Hz):

```cpp
tone(BUZZER, 800);   // lower, softer beep for Level 4
tone(BUZZER, 2500);  // higher, sharper beep for Level 1
```

Human hearing is most sensitive between 1000–3000 Hz — stay within this range for best audibility.

### Adding More LEDs

You can wire additional LEDs to unused pins and light them in relevant levels:

```cpp
#define BLUE_LED 6   // Example: flashing SOS at Level 1
```

### Using a Common Anode Display

If your 7-segment display is **Common Anode** (common pin connects to VCC), invert all `1`s and `0`s in the `numbers` array, and connect the common pin to **5V** instead of GND:

```cpp
// Common Anode version of digit "1"
{1, 0, 0, 1, 1, 1, 1}  // LOW = ON for Common Anode
```

---

## 🔧 Troubleshooting

| Problem | Possible Cause | Solution |
|---|---|---|
| Display shows wrong digits | Common Anode display used | Invert segment values in `numbers[]` |
| Sensor always reads 0 or 1 cm | TRIG/ECHO wires swapped | Double-check Pin 9 = TRIG, Pin 10 = ECHO |
| No buzzer sound | Wrong buzzer type (DC buzzer) | Use a passive buzzer that works with `tone()` |
| LEDs always ON or always OFF | Missing/wrong resistor | Verify 220Ω resistors on each LED anode |
| Erratic distance readings | Power supply noise | Add a 100µF capacitor across 5V and GND |
| Sensor reads through walls | HC-SR04 too sensitive | Reduce `pulseIn` timeout or add housing |
| Serial Monitor garbled text | Wrong baud rate | Set Serial Monitor to **9600 baud** |
| Buzzer beeps at wrong speed | `delay()` not matching level | Check that each level's delay values are correct |

---

## 🔮 Future Improvements

Here are some ideas to take this project further:

- [ ] **Non-blocking Buzzer** — Replace `delay()` with `millis()` timers so the sensor samples continuously without pausing
- [ ] **LCD Display** — Show the actual distance in cm instead of just the level number
- [ ] **Dual Sensors** — Add a second HC-SR04 for side-angle detection (blind spot monitoring)
- [ ] **Wireless Alert** — Use an HC-05 Bluetooth module to send proximity data to a smartphone app
- [ ] **PWM LED Brightness** — Fade the red LED intensity based on proximity (dim at Level 2, blinding at Level 1)
- [ ] **EEPROM Configuration** — Allow threshold values to be stored and updated without re-uploading code
- [ ] **Kalman Filter** — Smooth out sensor noise for more stable distance readings
- [ ] **PCB Design** — Design a custom PCB for a compact, vehicle-mountable form factor

---

## 📁 Project Structure

```
smart-parking-assist/
│
├── smart_parking_assist.ino   ← Main Arduino sketch
├── README.md                  ← This file
├── circuit/
│   ├── schematic.png          ← Wiring diagram image
│   └── breadboard_layout.png  ← Fritzing layout
└── docs/
    └── demo.gif               ← Demo animation (optional)
```

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).

Feel free to use, modify, and distribute with attribution.

---

## 🙌 Acknowledgements

- Arduino Community for extensive `tone()` and `pulseIn()` documentation
- HC-SR04 datasheet for timing specifications
- Open-source electronics community for 7-segment encoding references

---

<div align="center">

**Built with ❤️ using Arduino**

*If this project helped you, consider giving it a ⭐ on GitHub!*

</div>