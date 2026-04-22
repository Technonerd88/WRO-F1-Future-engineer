# WRO-F1-Future-engineer
Welcome to the official repository for Team F1 representing Kuwait!. This project focuses on the development of an autonomous RC vehicle designed to navigate a complex track using computer vision and sensor fusion.
Our goal is to build a robot that can:

Autonomously navigate a track marked by lines and walls.

Identify and avoid obstacles (Red and Green pillars) using real-time logic.

Optimize lap times through efficient steering geometry (Ackerman Steering) and PID speed control.
The Robot: Max Verstappen 
Brain: Matrix/Arduino

Vision/Sensors: 2X colour sensor 1X laser sensor

Hardware Configuration
Our robotic platform utilizes a three-point sensing array designed for high-speed navigation and obstacle management:

Dual Color Sensing Array: Two downward-facing sensors positioned at the vehicle’s leading edge.

Laser Time-of-Flight (ToF) Sensor: A single long-range distance sensor mounted on the central axis.

The Challenge: Environmental Variable Interference
In a dynamic competition environment, two primary technical risks exist:

Ambient Light Noise: Variations in venue lighting can cause "False Positives" in color detection, leading to navigation failure.

Object Ambiguity: A distance sensor can detect an object but cannot determine its "meaning" (e.g., distinguishing between a track boundary and a traffic signal).

The Solution: Integrated Logic ControlA. Differential Steering Logic (The Color Sensors)Instead of basic "On/Off" detection, we implemented Differential Logic. By
comparing the values between the left and right color sensors, the robot calculates a steering error $e$:$$e = \text{Sensor}_{\text{left}} - \text{Sensor}_{\text{right}}$$This allows for Proportional Control, ensuring the robot makes smooth, micro-adjustments rather than sharp, jerky movements, which preserves momentum and stability.B. Predictive Braking & Object Classification (The Laser Sensor)We use the Laser ToF sensor to create a "Safety Buffer." The robot follows a tiered response logic:Discovery Phase: The laser scans for objects up to 80cm away.Deceleration Phase: Upon detection, the robot reduces motor PWM (power) proportionally to the distance, ensuring it doesn't overshoot the target.Verification Phase: Once the robot is within 5cm, the sensor fusion kicks in. The robot uses the color sensors to "classify" the object (e.g., Red Pillar vs. Green Pillar) to decide the next navigational maneuver.C. Dynamic Calibration ProtocolTo solve the problem of venue lighting, we developed a Pre-Run Calibration Script. Before the race begins, the robot samples the "Reflected Light Intensity" of the specific track surface. This sets a baseline "Zero," allowing the software to filter out ambient light noise automatically. . Engineering ConclusionBy fusing the high-speed distance data from the Laser Sensor with the precise alignment data from the Dual Color Sensors, Team F1 has created a robust navigation system that is both fast and adaptable to changing competition conditions.
Drive: DC Motors 

Steering: Servo 

Repository Structure
/src: The core C++ source code and control logic.

/schemes: Wiring diagrams and electronic circuit layouts.

/models: 3D design files (.STL) for custom chassis components.

/docs: Engineering journals and performance calibration logs.

Our Engineering Approach
We believe in an iterative design process. Our development focused on three main pillars:

Reliability: Ensuring the sensor data is filtered (Kalman/Median filters) to handle different lighting conditions.

Speed: Balancing the center of gravity to allow for high-speed cornering without drifting.

Reproducibility: Documentation that allows any other engineer to rebuild our robot from scratch.

"Engineering is not just about building a robot; it's about solving the problems that arise when the robot meets the real world."
