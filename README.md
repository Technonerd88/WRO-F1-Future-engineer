# WRO-F1-Future-engineer
Welcome to the official repository for Team F1 representing Kuwait!. This project focuses on the development of an autonomous RC vehicle designed to navigate a complex track using computer vision and sensor fusion.
Our goal is to build a robot that can:

Autonomously navigate a track marked by lines and walls.

Identify and avoid obstacles (Red and Green pillars) using real-time logic.

Optimize lap times through efficient steering geometry (Ackerman Steering) and PID speed control.
The Robot: Max Verstappen 
Brain: Matrix/Arduino

Vision/Sensors: 2X colour sensor 1X laser sensor

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
