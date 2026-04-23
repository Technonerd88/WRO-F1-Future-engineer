# WRO-F1-Future-engineer

<img src="https://github.com/user-attachments/assets/ebbca2d5-5954-4dd9-914f-37c204f79c7a" width="400"/>

Welcome to the official repository for Team F1 representing Kuwait!

**About Us:**  
On the right is me, Mahdi Khalid. I am 20 years old, and this is my second year participating in WRO.  
On the left is my teammate Abdulkareem. He is 15 years old, and this is also his second year participating in the WRO competition.

**Our Project:**  
This project focuses on developing an autonomous RC vehicle designed to navigate a complex track using sensor fusion and real-time decision logic.

---

## 🧠 System Overview

The robot is built using the MATRIX Mini R4 and uses a combination of sensors and structured logic to operate autonomously.

### Hardware:
- MATRIX Mini R4 (controller)
- 1 DC motor (M1) for movement
- 1 servo motor (RC4) for steering and scanning
- Laser Sensor V2 (I2C4) for distance detection
- Color Sensor V3 (I2C2) for line counting
- Color Sensor V3 (I2C3) for object detection
- Built-in OLED display

---

## ⚙️ Core Behavior

### 1. Movement
- The robot checks the distance in front using the laser sensor.
- If **distance > 20 cm → move forward**
- If **distance ≤ 20 cm**:
  - Stop
  - Scan right → measure distance
  - Scan left → measure distance
  - Move toward the direction with more free space
  - If both sides are blocked → move backward further and retry

---

### 2. Line Detection and Counting (I2C2)
The robot detects only:
- **Orange** → red high, green medium, blue low  
- **Blue** → blue dominant  

Behavior:
- First detected color selects the mode
- Count only that color
- Uses debounce to avoid double counting
- Stops after **12 detections**
- Waits **8 seconds**, then stops completely

OLED displays:
- Mode (ORANGE / BLUE / WAIT)
- Orange count
- Blue count
- Live RGBC values

---

### 3. Object Detection (I2C3)
The robot detects:
- **Red → go right**
- **Green → go left**
- **Magenta → scan and choose best direction**

Behavior:
- Overrides normal obstacle avoidance
- Uses short hold time to ensure stable reaction

---

### 4. Priority System
1. Line counting  
2. Object detection  
3. Obstacle avoidance  

This ensures stable and predictable behavior.

---

## 🔁 Logic Flowchart

```text
START
  ↓
Check Line (I2C2)
  ↓
Reached 12?
  ├── Yes → Wait 8s → STOP
  └── No
        ↓
Check Object (I2C3)
  ├── Red → Turn Right
  ├── Green → Turn Left
  ├── Magenta → Scan & Decide
  └── None
        ↓
Check Distance (Laser)
  ├── >20cm → Move Forward
  └── ≤20cm
        ↓
     Stop
        ↓
 Scan Right → Distance
 Scan Left  → Distance
        ↓
Choose Best Direction
        ↓
Move
        ↓
LOOP


```

---

## 🧩 System Diagram

```text
           +----------------------+
           |   MATRIX Mini R4     |
           |  (Main Controller)   |
           +----------+-----------+
                      |
     -----------------------------------------
     |             |            |            |
     ↓             ↓            ↓            ↓
 Laser Sensor   Color Sensor   Color Sensor  OLED
   (I2C4)         (I2C2)         (I2C3)
 Distance       Line Count      Object Detect

                      |
                      ↓
               Decision Logic
                      |
          -----------------------
          |                     |
          ↓                     ↓
     DC Motor (M1)        Servo (RC4)
    Forward/Backward     Steering/Scan

```

## 🔧 Sensors

<img src="https://github.com/user-attachments/assets/f88c67a5-0e04-4c20-a7a6-9e7a0775d9d3" width="200"/>
<img src="https://github.com/user-attachments/assets/cdc63020-4011-422b-8ccc-a9e8b67aabc5" width="200"/>

- Laser sensor provides accurate distance detection  
- Color sensors allow reliable classification of lines and objects  
- Using two separate color sensors avoids confusion between tasks  

---

## 🚗 Steering & Drive

<img src="https://github.com/user-attachments/assets/a3352adb-ee73-48d4-903c-28b13340c422" width="220"/>

- Ackermann-style steering for smooth turns  
- Servo-controlled direction changes  
- Single motor simplifies forward motion control  

---

## 🏗️ Robot Structure

<img src="https://github.com/user-attachments/assets/217caa82-9cdf-411b-beb2-da5486a6c8aa" width="300"/>
<img src="https://github.com/user-attachments/assets/5ebbe0f4-016c-4e6c-b297-e9b5a891e75e" width="300"/>
<img src="https://github.com/user-attachments/assets/a1488723-81ef-4f4b-a7a2-a9538a5e68d4" width="300"/>

**Design features:**
- Balanced center of mass  
- Front-mounted sensors for early detection  
- Stable chassis for consistent performance  

---

## 🎯 Engineering Approach

### Reliability
- Simple detection rules  
- Clear priority system  
- Reduced noise and false detection  

### Speed
- Fast response using laser sensor  
- Efficient turning with servo  

### Simplicity
- Modular code structure  
- Easy debugging and tuning  

---

## 🏁 Conclusion

Team F1 developed a robot that is:

- Fast  
- Stable  
- Predictable  

By combining laser sensing, dual color detection, and structured decision logic, the system performs consistently under real competition conditions.
