# WRO-F1-Future-engineer

<img src="https://github.com/user-attachments/assets/ebbca2d5-5954-4dd9-914f-37c204f79c7a" width="400"/>

Welcome to the official repository for Team F1 representing Kuwait!

## About Us

On the right is Mahdi Khalid. He is 20 years old, and this is his second year participating in WRO.
On the left is Abdulkareem. He is 15 years old, and this is also his second year participating in the WRO competition.

## Our Project

This project focuses on developing an autonomous RC vehicle designed to navigate a complex track using sensor fusion and real-time decision logic.

The robot uses the MATRIX Mini R4 controller, a DC motor for movement, a servo motor for steering, a laser distance sensor for obstacle detection, a color sensor for line counting, and a UART camera for object detection.

---

## 🧠 System Overview

The robot is built using the MATRIX Mini R4 and uses a combination of sensors and structured logic to operate autonomously.

### Hardware and Pin Mapping

<img width="400" height="400" alt="pins" src="https://github.com/user-attachments/assets/7d11625a-ca79-41cc-adc8-6bda2675bb43" />

| Component | Port / Pin | Purpose |
|---|---:|---|
| MATRIX Mini R4 | Main controller | Controls the full robot system |
| DC Motor | M1 | Main drive motor for forward/backward movement |
| Servo Motor | RC4 | Steering and scanning direction |
| Laser Sensor V2 | I2C4 | Distance and obstacle detection |
| Color Sensor V3 | I2C2 | Line detection and line counting |
| Camera | UART, Baud 9600 | Object detection |
| Built-in OLED Display | Built-in | Displays mode, sensor values, and count |

---

## ⚙️ Block Setup Configuration

The robot program should be configured with the following setup blocks:

```text
Mini Begin: UART On, Baud 9600
DC Motor: M1, Enable Reverse: No
Servo Motor: RC4, Set Position: 85
Clear Screen
Set Screen Text Size: 3
Set Screen Text Color: White
Laser Sensor V2: I2C4, Initialize
Color Sensor V3: I2C2, Initialize
Set Lines = 0
```

### Correct Main Loop Hardware Use

```text
DC Encoder Motor M1 Speed 50
Display Laser Sensor V2 I2C4 Distance on OLED

Check Color Sensor V3 on I2C2 for line counting
Check Camera on UART for object detection
Use Laser Sensor V2 on I2C4 for obstacle distance
Use Servo RC4 for steering
Use DC Motor M1 for movement
```

---

## 🚗 Core Behavior

### 1. Movement

The robot checks the distance in front using the laser sensor.

- If **distance > 20 cm**, the robot moves forward.
- If **distance ≤ 20 cm**, the robot:
- Stops
- Scans right using the servo
- Scans left using the servo
- Compares both distances
- Moves toward the direction with more free space
- Moves backward and retries if both sides are blocked

---

### 2. Line Detection and Counting

Line detection uses:

```text
Color Sensor V3 → I2C2
```

The robot detects only the selected line color.

Detected line examples:

- **Blue line**: Blue value is high, while red and green are low.
- **Orange line**: Red is high, green is medium, and blue is low.

Example blue-line condition:

```text
IF Color Sensor V3 I2C2 Blue > 100
AND Color Sensor V3 I2C2 Green < 90
AND Color Sensor V3 I2C2 Red < 90
THEN
Change Lines by 1
Stop motor briefly
Wait 1000 ms
Turn servo to 120
Move forward
Wait 3300 ms
ELSE
Servo RC4 Set Position 85
DC Encoder Motor M1 Speed 50
```

Behavior:

- The robot counts detected lines.
- A debounce delay is used to avoid double-counting the same line.
- After **12 detections**, the robot stops.

Stop condition:

```text
IF Lines = 12
THEN
Servo RC4 Set Position 85
DC Encoder Motor M1 Speed 0
Clear Screen
```

OLED displays:

- Current mode
- Line count
- Laser distance
- Color sensor RGB values

---

### 3. Object Detection

<img width="500" height="350" alt="CAMERA" src="https://github.com/user-attachments/assets/ac967a33-7a92-42fd-9de2-5645175492f1" />



Object detection uses:

```text
Camera → UART, Baud 9600
```

The camera detects colored objects and sends results through UART.

Behavior:

- **Red object** → turn right
- **Green object** → turn left
- **Magenta object** → scan and choose the best direction using the laser sensor

Example object logic:

```text
IF Camera detects Red
THEN
Servo RC4 Set Position 120
DC Motor M1 Power 50

IF Camera detects Green
THEN
Servo RC4 Set Position 50
DC Motor M1 Power 50

IF Camera detects Magenta
THEN
DC Motor M1 Power 0
Scan right and left using Servo RC4
Compare distances using Laser Sensor V2 I2C4
Move toward the clearer direction
```

---

## 4. Priority System

The robot follows this priority order:

1. Line counting using Color Sensor V3 on I2C2
2. Object detection using Camera on UART
3. Obstacle avoidance using Laser Sensor V2 on I2C4

This priority system keeps the behavior stable and predictable.

---

## 🔁 Logic Flowchart

```text
START
↓
Initialize hardware
↓
M1 motor ready
RC4 servo centered at 85
Laser Sensor V2 initialized on I2C4
Color Sensor V3 initialized on I2C2
Camera enabled on UART
↓
LOOP
↓
Check line using Color Sensor I2C2
↓
Reached 12 lines?
├── Yes → Stop motor → Center servo → Clear screen → END
└── No
↓
Check camera object through UART
├── Red → Turn right
├── Green → Turn left
├── Magenta → Scan and choose best direction
└── None
↓
Check distance using Laser Sensor I2C4
├── Distance > 20 cm → Move forward
└── Distance ≤ 20 cm
↓
Stop
↓
Scan right
Scan left
↓
Choose clearer direction
↓
Continue loop
```

---

## 🧩 System Diagram

```text
+----------------------+
| MATRIX Mini R4 |
| Main Controller |
+----------+-----------+
|
---------------------------------------------
| | | |
↓ ↓ ↓ ↓
Laser Sensor Color Sensor Camera OLED
I2C4 I2C2 UART Built-in
Distance Line Count Object Detect Display

|
↓
Decision Logic
|
-----------------------
| |
↓ ↓
DC Motor M1 Servo Motor RC4
Forward/Backward Steering/Scan
```

---

## 🔧 Sensors

<img src="https://github.com/user-attachments/assets/f88c67a5-0e04-4c20-a7a6-9e7a0775d9d3" width="200"/>
<img src="https://github.com/user-attachments/assets/cdc63020-4011-422b-8ccc-a9e8b67aabc5" width="200"/>

- Laser Sensor V2 provides accurate distance detection.
- Color Sensor V3 detects track lines for counting.
- Camera detects colored objects through UART communication.
- Separating line detection and object detection avoids confusion between tasks.

---

## 🚗 Steering and Drive

<img src="https://github.com/user-attachments/assets/a3352adb-ee73-48d4-903c-28b13340c422" width="220"/>

- The DC motor on **M1** drives the robot forward and backward.
- The servo motor on **RC4** controls steering.
- The neutral steering position is **85**.
- Right turn example: **120**.
- Left turn example: **50**.

---

## 🏗️ Robot Structure

<img src="https://github.com/user-attachments/assets/217caa82-9cdf-411b-beb2-da5486a6c8aa" width="300"/>
<img src="https://github.com/user-attachments/assets/5ebbe0f4-016c-4e6c-b297-e9b5a891e75e" width="300"/>
<img src="https://github.com/user-attachments/assets/a1488723-81ef-4f4b-a7a2-a9538a5e68d4" width="300"/>

Design features:

- Balanced center of mass
- Front-mounted sensors for early detection
- Stable chassis for consistent performance
- Clear wiring layout based on the corrected MATRIX Mini R4 pin mapping

---

## 🎯 Engineering Approach

### Reliability

- Simple detection rules
- Correct sensor-to-port assignment
- Clear priority system
- Reduced noise and false detection

### Speed

- Fast response using the laser sensor
- Efficient turning using the servo motor
- Camera-based object detection for quick reactions

### Simplicity

- Modular code structure
- Easy debugging and tuning
- Separate systems for line counting, object detection, and obstacle avoidance

---

## 🏁 Conclusion

Team F1 developed a robot that is:

- Fast
- Stable
- Predictable

By combining laser distance sensing, color-based line counting, camera-based object detection, and structured decision logic, the system performs consistently under competition conditions.
