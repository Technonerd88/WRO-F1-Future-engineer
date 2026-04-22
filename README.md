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


The Solution: Integrated Logic ControlA. Differential Steering Logic (The Color Sensors)Instead of basic "On/Off" detection, we implemented Differential Logic. By
comparing the values between the left and right color sensors, the robot calculates a steering error $e$:$$e = \text{Sensor}_{\text{left}} - \text{Sensor}_{\text{right}}$$This allows for Proportional Control, ensuring the robot makes smooth, micro-adjustments rather than sharp, jerky movements, which preserves momentum and stability.B. Predictive Braking & Object Classification (The Laser Sensor)We use the Laser ToF sensor to create a "Safety Buffer." The robot follows a tiered response logic:Discovery Phase: The laser scans for objects up to 80cm away.Deceleration Phase: Upon detection, the robot reduces motor PWM (power) proportionally to the distance, ensuring it doesn't overshoot the target.Verification Phase: Once the robot is within 5cm, the sensor fusion kicks in. The robot uses the color sensors to "classify" the object (e.g., Red Pillar vs. Green Pillar) to decide the next navigational maneuver.C. Dynamic Calibration ProtocolTo solve the problem of venue lighting, we developed a Pre-Run Calibration Script. Before the race begins, the robot samples the "Reflected Light Intensity" of the specific track surface. This sets a baseline "Zero," allowing the software to filter out ambient light noise automatically. . Engineering ConclusionBy fusing the high-speed distance data from the Laser Sensor with the precise alignment data from the Dual Color Sensors, Team F1 has created a robust navigation system that is both fast and adaptable to changing competition conditions.
Drive: DC Motors 

Steering: Servo 
Steering System: Proportional Servo Control1. Mechanical Configuration: Ackermann Steering GeometryTeam F1 utilizes a front-axle steering geometry driven by a 
high-torque digital servo motor. Unlike "tank steering" (differential drive), our vehicle uses a car-like steering system. This provides several 
advantages:Directional Stability: The robot maintains its heading more effectively at high speeds.Energy Efficiency: Turning requires less energy as the wheels
roll through the arc rather than skidding.Predictable Pathing: The relationship between the servo angle and the turning radius is linear, making our navigation 
algorithms more precise.2. Technical Implementation: The PWM InterfaceThe steering is controlled via Pulse Width Modulation (PWM). By mapping the degree of the 
servo (typically 0° to 180°) to our software's steering variables, we achieved a "Virtual Steering Rack."Center Calibration: We implemented a software offset to ensure the "Zero Point" results in a perfectly straight trajectory, compensating for any mechanical tolerances in the linkage.Angular Limits: To protect the hardware, we programmed Electronic Soft-Limits. This prevents the servo from attempting to turn beyond the mechanical capability of the suspension, preventing motor burnout and gear stripping.3. Software Logic: Proportional Steering (P-Control)The servo does not just move "Left" or "Right." It responds proportionally to the data received from our Dual Color Sensors and Laser Sensor:$$Steering\_Angle = Center + (Error \times K_p)$$Error: The difference between our intended path and our current position.$K_p$ (Proportional Gain): A tuned constant that determines how "aggressively" the servo reacts.Dynamic Centering: When no obstacles or markers are detected, the servo automatically returns to the neutral position ($90^\circ$), ensuring the robot exits turns cleanly.

<img width="363" height="600" alt="Servo steering" src="https://github.com/user-attachments/assets/a3352adb-ee73-48d4-903c-28b13340c422" />


DC Motors:

Drive System: DC Propulsion and Dynamic Actuation
Hardware Overview: High-Torque DC Power
Team F1 utilizes a high-output DC Motor coupled with an integrated drive assembly to provide the necessary propulsion for the competition.

The DC Motor: Serves as the primary power plant, converting electrical energy from our battery into mechanical rotation.

The Actuator/Drivetrain: Transmits power from the motor shaft to the drive wheels, providing the torque required to reach top speed quickly while maintaining stability during low-speed maneuvers.

 Bidirectional Control (H-Bridge Logic)
To navigate the complex WRO environment, our robot must be capable of both forward and reverse motion. We implemented an H-Bridge Motor Driver circuit which allows us to:

Forward Propulsion: Rotate the motor clockwise for maximum speed during straightaways.

Reverse Actuation: Quickly reverse the polarity to back away from obstacles or perform three-point turns if the vehicle becomes trapped.

Active Braking: By shorting the motor leads through the driver, we can perform an "Electronic Brake," stopping the robot instantly when the Laser Sensor detects a collision risk.

 Software Implementation: PWM Speed Regulation
We do not simply turn the motor "On" or "Off." We use Pulse Width Modulation (PWM) to control the velocity. This allows for:

Smooth Acceleration: Gradually increasing the PWM duty cycle to prevent the wheels from spinning out (losing traction) at the start of the race.

Controlled Deceleration: Reducing power as we approach a Red/Green obstacle, ensuring the sensors have enough time to process color data.

Battery Efficiency: By optimizing the power output, we ensure the robot maintains consistent performance throughout the entire 12-marker run.

 Technical Synergy: The "F1" Integration
The DC Motor works in perfect synchronization with our other components:

Laser Sensor calculates the distance to the wall.

Processor calculates the required speed (PWM).

DC Motor executes the speed, while the Servo handles the direction.

 Engineering Conclusion
"By utilizing a PWM-controlled DC motor with bidirectional actuation, Team F1 achieved a balance between raw speed and precise positioning. The ability to modulate power allows our vehicle to adapt its velocity based on real-time sensor feedback, ensuring we navigate the 12 milestones with both speed and safety."

The brain:Control System: The Neural Hub (Matrix & Arduino)
 Hardware Architecture: Matrix Robotics + Arduino Duo
Team F1’s vehicle is powered by the Arduino-based Matrix Controller. This system acts as the central processor, managing the high-speed communication between our input sensors and output actuators.

The Brain (Arduino): Handles the complex mathematical calculations for the PID steering and the 12-marker color logic.

The Powerhouse (Matrix): Provides the robust electrical interface needed to drive the high-torque DC motors and the precision steering servo, ensuring they receive steady voltage even during high-drain maneuvers.

 Software Logic: Multithreaded Processing Emulation
Our code is structured to handle multiple data streams simultaneously. The "Brain" processes a continuous loop of three critical tasks:

Sensing: Constantly polling the I2C and Analog ports for data from the 2X Color Sensors and the Laser ToF Sensor.

Decision Making: Comparing real-time data against our "12-Marker Milestone" counter and obstacle detection logic.

Commanding: Sending updated PWM signals to the DC Motor (for speed) and the Servo (for heading).

 Reliability and Communication
In a competitive robotics environment, electrical noise can be a major issue. We chose the Matrix system because of its:

Integrated Motor Drivers: Reduces wiring complexity, which minimizes the chance of a loose connection during the run.

Regulated Power Rails: Ensures that when the DC motor draws a lot of power to accelerate, the sensors do not "brown out" or lose their calibration.

 Technical Synergy: The "F1" Operating System
The "Brain" is programmed with a specialized logic flow for the Future Engineers category:

Input Filter: Our Arduino code filters out "noise" from the color sensors to ensure we only count legitimate markers.

Feedback Loop: It creates a closed-loop system where the Laser Sensor tells the brain how far the wall is, and the brain immediately tells the Servo how much to turn.

 Engineering Conclusion
"The integration of the Matrix Robotics System with an Arduino core provides Team F1 with a flexible yet powerful control platform. By centralizing our sensor fusion and motor control into one synchronized hub, we have created a robot capable of millisecond-level reactions, ensuring a fast, autonomous, and reliable performance in the WRO competition." 
Repository Structure
/src: The core C++ source code and control logic.

/schemes: Wiring diagrams and electronic circuit layouts.

/models: 3D design files (.STL) for custom chassis components.

The challenge: 
Mission Execution & Algorithmic Strategy1. Open Challenge: Adaptive PathfindingObjective: Navigate 3 laps in a randomized corridor without traffic signs.Dynamic Corridor Adaptation: Since the inner wall configuration is unknown until the start, Team F1 uses a Reactive Wall-Following Algorithm. The Laser Sensor acts as a rangefinder, maintaining a "Safety Envelope" from the outer boundary.Mathematical Steering Control: We implemented a proportional control loop where the steering angle $\theta$ is adjusted based on the distance $d$ from the wall:$$\theta = K_p \times (d_{target} - d_{actual})$$Lap Mastery: Color Sensor 1 is programmed to ignore all noise and strictly identify the Start/Finish line color. Once the internal counter hits 3, the robot transitions from "Speed Mode" to "Precision Braking" to secure the finish zone bonus.2. Obstacle Challenge: Cognitive NavigationObjective: Navigate 3 laps while obeying Red/Green traffic pillars and performing a parallel park.Obstacle Perception Layer: The robot utilizes the Laser Sensor to identify a "Point of Interest" (a pillar) up to 30cm ahead. This triggers a state-change in the software.Color Classification & Maneuvering:Red Signal: Upon detection via Color Sensor 2, the Arduino executes a "Keep Right" maneuver, calculating a steering arc that ensures the vehicle stays outside the 85mm penalty zone.Green Signal: The vehicle executes a "Keep Left" maneuver.Trajectory Recovery: After passing the pillar, the robot uses the Dual Color Sensors to quickly re-align itself with the center of the track, preventing a collision with the next wall.3. The Finale: Precision Parallel ParkingAfter the third lap is confirmed by the lap-counter, the robot enters its Parking Sub-Routine:Search Phase: The vehicle slows to 30% power, using the Laser Sensor to find a gap in the wall (the parking lot).Execution Phase: Once the gap is found, the robot performs a synchronized multi-step move:Reverse-Yaw: The DC Motor reverses while the Servo turns to a 45° angle.Counter-Steer: The Servo flips to the opposite angle to bring the front of the car into the space.Final Alignment: The robot straightens its wheels and stops, completing the mission
The battery that we used for our robot: 

<img width="259" height="194" alt="Battery" src="https://github.com/user-attachments/assets/6198caca-8863-428c-85c2-f596614bd539" />
Vehicle Overview: The Matrix Engineering Philosophy: 

Front View Analysis: Sensor Array and Ackerman Alignment
The front view of the vehicle highlights the mission-critical systems required for the Obstacle and Open challenges.

Key Technical Features:
A. Dual Sensor Mount: We engineered a rigid, forward-facing bracket using two Matrix chassis plates to mount our primary Dual Color Sensors. This spacing is carefully calibrated to match the track markers.

Color Sensor 1 (Left): Programmed to detect Blue/Orange floor milestones (Lap Tracker).

Color Sensor 2 (Right): Programmed to classify Red/Green traffic pillars (Dodge Logic).

B. Micro Servo Integration: Positioned directly behind the sensor array is the primary Steering Servo. This location provides immediate and balanced mechanical linkage to the front knuckles, reducing steering "play" for faster response times in obstacle dodge maneuvers.

C. Ackermann Geometry: The front wheels utilize a car-like Ackermann steering mechanism (rather than differential drive). This engineering choice provides vastly superior directional stability at higher velocities and prevents wheel skid during sharp turns, which is critical for precision pathing in the dynamic corridor challenge.

D. Track-Optimized Wheels: We utilized the Matrix kit's large-diameter, soft-rubber tires on the front axle. The tread pattern and soft compound provide the necessary traction for rapid steering adjustments.
<img width="3024" height="4032" alt="Front View" src="https://github.com/user-attachments/assets/8d6500ec-42c1-42c7-93e4-a27c2452a2b0" />


Our Engineering Approach
We believe in an iterative design process. Our development focused on three main pillars:

Reliability: Ensuring the sensor data is filtered (Kalman/Median filters) to handle different lighting conditions.

Speed: Balancing the center of gravity to allow for high-speed cornering without drifting.

Reproducibility: Documentation that allows any other engineer to rebuild our robot from scratch.

"Engineering is not just about building a robot; it's about solving the problems that arise when the robot meets the real world."
