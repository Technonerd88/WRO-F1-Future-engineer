# WRO-F1-Future-engineer



<img width="2316" height="3088" alt="unnamed" src="https://github.com/user-attachments/assets/ebbca2d5-5954-4dd9-914f-37c204f79c7a" />

Welcome to the official repository for Team F1 representing Kuwait!. About Us: On the right is me mahdi khalid! I am 20 years old and this is my 2nd year participating in WRO. On the left is my friend Abdulkareem! He is 15 years old and this is also his 2nd year participating in WRO competition Our Project: This project focuses on the development of an autonomous RC vehicle designed to navigate a complex track using computer vision and sensor fusion.
Our goal is to build a robot that can:

Autonomously navigate a track marked by lines and walls.

Identify and avoid obstacles (Red and Green pillars) using real-time logic.

Optimize lap times through efficient steering geometry (Ackerman Steering) and PID speed control.
The Robot: Max Verstappen 
Brain: Matrix/Arduino

Vision/Sensors: 2X colour sensor:
Sensor 1: The "Counter & Lap Tracker"
Task: Detect Blue and Orange colors (12 times total).
Location: Usually mounted very low, pointing directly at the floor or at the specific height of the track markers.

How it works: This sensor acts as your Progress Monitor. Instead of just looking for lines, it is looking for "milestones."

The Logic: * It uses a Trigger Variable (e.g., Color_Count = 0).

Every time it sees Blue or Orange, it adds 1 to the count.

The Problem it Solves: It prevents the robot from getting "lost" in the race. If the race requires 3 laps and there are 4 markers per lap, the robot knows exactly when it has finished the 12th marker and should stop or enter its "Finish" routine.

Engineering Benefit: This allows your robot to perform different actions based on where it is in the race (e.g., "On count 4, speed up; on count 12, park").

Sensor 2: The "Safety & Navigation Guard"
Task: Detect Green and Red obstacles.
Location: Usually mounted slightly higher or angled forward to "see" the colored pillars or blocks before the robot hits them.

How it works: This sensor acts as your Decision Maker. In the "Future Engineers" category, Green and Red usually tell the robot which side of the obstacle it must pass on.

The Logic:

If Red detected: The robot executes a "Steer Left" command to avoid the obstacle.

If Green detected: The robot executes a "Steer Right" command.

The Problem it Solves: This separates your navigation from your counting. By having a separate sensor for obstacles, the robot won't accidentally count a "Red Pillar" as a "Blue Floor Marker." It keeps your data clean.
<img width="320" height="584" alt="Laser sensor" src="https://github.com/user-attachments/assets/f88c67a5-0e04-4c20-a7a6-9e7a0775d9d3" />



 1X laser sensor
High-Speed Decision Making (Seeing the Future)Color sensors only see what is directly in front of or beneath them (usually a few millimeters or centimeters). If your robot is moving fast, by the time a color sensor sees a Red or Green obstacle, it might be too late to turn without crashing.The Laser Advantage: It can "see" obstacles up to 2 meters away.The Benefit: Your robot can begin to slow down or prepare its turn before it even reaches the obstacle. This allows you to maintain a much higher average speed throughout the track.2. Precise Distance Measurement ($1\text{mm}$ Accuracy)Unlike ultrasonic sensors (which use sound and can be "fuzzy"), laser sensors use light. Light doesn't spread out as much, meaning it can pinpoint exactly where an object is.The Benefit: You can program your robot to maintain a perfect distance from a wall or to stop exactly 10cm away from a pillar every single time. This consistency is what wins competitions.3. Solving the "Wall-Following" ProblemIn the Future Engineers category, you often need to navigate through a "corridor" or stay a certain distance from a boundary.The Logic: You can use the laser sensor to measure the distance to the side wall. If the distance increases, the robot steered too far away; if it decreases, the robot is getting too close.The Result: You get a robot that drives in a perfectly straight line relative to the track walls, which is much faster than "hunting" for lines on the floor.4. Overcoming "Color Blindness"Sometimes, the lighting in a competition hall is so bad that color sensors struggle to tell the difference between a dark wall and the dark floor.The Laser Advantage: Laser sensors don't care about color or light levels. They measure physical presence.The Benefit: Even if the room is dark or the floor is reflective, the laser will always tell the truth: "There is a solid object 30cm in front of us." It acts as the ultimate "Fail-Safe."
<img width="287" height="574" alt="Colour sensor" src="https://github.com/user-attachments/assets/cdc63020-4011-422b-8ccc-a9e8b67aabc5" />




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

<img width="363" height="600" alt="Servo steering" src="https://github.com/user-attachments/assets/a3352adb-ee73-48d4-903c-28b13340c422" />





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
