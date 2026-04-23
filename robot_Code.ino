#include "MatrixMiniR4.h"

// =====================================
// Robot settings
// =====================================
const int OBSTACLE_DIST    = 250;   // 20 cm = 200 mm
const int FORWARD_SPEED    =  40;
const int BACKWARD_SPEED   = -20;

// Servo angles
const int CENTER_ANGLE = 88;
const int RIGHT_ANGLE  = 55;
const int LEFT_ANGLE   = 120;

// =====================================
// I2C2 line counting settings
// =====================================
const int           TARGET_COUNT          = 12;
const unsigned long STOP_DELAY_AFTER_12   = 8000;   // ms
const unsigned long COLOR_DEBOUNCE_MS     =  250;   // ms

// =====================================
// I2C3 special object settings
// =====================================
const unsigned long SPECIAL_HOLD_MS = 700;   // ms

// =====================================
// Enums
// =====================================
enum TrackColor  { TRACK_NONE, TRACK_ORANGE, TRACK_BLUE };
enum SpecialObject { OBJ_NONE, OBJ_RED, OBJ_GREEN, OBJ_MAGENTA };

// =====================================
// Global state
// =====================================
TrackColor   selectedColor     = TRACK_NONE;
TrackColor   lastDetectedColor = TRACK_NONE;

int orangeCount = 0;
int blueCount   = 0;

bool          finished           = false;
bool          finishTimerStarted = false;
unsigned long finishStartTime    = 0;
unsigned long lastCountTime      = 0;

// Special-object hold: during hold period the robot simply keeps
// moving forward — no repeated steering action.
SpecialObject heldObject     = OBJ_NONE;
unsigned long specialHoldUntil = 0;

// =====================================
// Laser helper
// =====================================
int readLaserMM()
{
    int d = MiniR4.I2C4.MXLaserV2.getDistance();
    if (d == 8190 || d <= 0) return 9999;   // treat invalid as "very far"
    return d;
}

// =====================================
// Motor helpers
// =====================================
void stopRobot()       { MiniR4.M1.setSpeed(0); }
void moveForward()     { MiniR4.M1.setSpeed(FORWARD_SPEED); }

// Move backward for a given duration then stop
void moveBackward(unsigned long ms)
{
    MiniR4.M1.setSpeed(BACKWARD_SPEED);
    delay(ms);
    MiniR4.M1.setSpeed(0);
}

// =====================================
// OLED helpers
// =====================================
void updateOLED(int r, int g, int b, int c)
{
    MiniR4.OLED.clearDisplay();
    MiniR4.OLED.setTextSize(1);
    MiniR4.OLED.setTextColor(SSD1306_WHITE);

    MiniR4.OLED.setCursor(0, 0);
    MiniR4.OLED.print("Mode: ");
    if      (selectedColor == TRACK_ORANGE) MiniR4.OLED.print("ORANGE");
    else if (selectedColor == TRACK_BLUE)   MiniR4.OLED.print("BLUE");
    else                                    MiniR4.OLED.print("WAIT");

    MiniR4.OLED.setCursor(0, 12);
    MiniR4.OLED.print("Orange: ");
    MiniR4.OLED.print(orangeCount);

    MiniR4.OLED.setCursor(0, 24);
    MiniR4.OLED.print("Blue: ");
    MiniR4.OLED.print(blueCount);

    MiniR4.OLED.setCursor(0, 36);
    MiniR4.OLED.print("R:"); MiniR4.OLED.print(r);
    MiniR4.OLED.print(" G:"); MiniR4.OLED.print(g);

    MiniR4.OLED.setCursor(0, 48);
    MiniR4.OLED.print("B:"); MiniR4.OLED.print(b);
    MiniR4.OLED.print(" C:"); MiniR4.OLED.print(c);

    MiniR4.OLED.display();
}

void showFinalOLED()
{
    MiniR4.OLED.clearDisplay();
    MiniR4.OLED.setTextSize(2);
    MiniR4.OLED.setTextColor(SSD1306_WHITE);
    MiniR4.OLED.setCursor(0, 0);

    if (selectedColor == TRACK_ORANGE)
    {
        MiniR4.OLED.print("Orange");
        MiniR4.OLED.setCursor(0, 24);
        MiniR4.OLED.print(orangeCount);
    }
    else if (selectedColor == TRACK_BLUE)
    {
        MiniR4.OLED.print("Blue");
        MiniR4.OLED.setCursor(0, 24);
        MiniR4.OLED.print(blueCount);
    }
    else
    {
        MiniR4.OLED.print("Stop");
    }

    MiniR4.OLED.display();
}

// =====================================
// I2C2 — line color detection
// ORANGE: CMYK(0,60,100,0) → R high, G medium, B low
// BLUE:   CMYK(100,80,0,0) → B dominant
// =====================================
TrackColor detectTrackColor(int r, int g, int b, int c)
{
    if (c < 30) return TRACK_NONE;

    // ORANGE: red strongest, green medium, blue clearly lowest
    if (r > 80 && g > 40 && b < 40 && r > g && g > b)
        return TRACK_ORANGE;

    // BLUE: blue clearly dominant over both red and green
    if (b > 80 && b > r + 30 && b > g + 20)
        return TRACK_BLUE;

    return TRACK_NONE;
}

// Returns true if the robot has finished (reached 12 and waited 8s).
// Does NOT call stopRobot() — that is handled in loop().
bool handleColorCounting()
{
    int r = MiniR4.I2C2.MXColorV3.getR();
    int g = MiniR4.I2C2.MXColorV3.getG();
    int b = MiniR4.I2C2.MXColorV3.getB();
    int c = MiniR4.I2C2.MXColorV3.getC();

    TrackColor currentColor = detectTrackColor(r, g, b, c);
    unsigned long now = millis();

    // First valid color seen sets the counting mode permanently
    if (selectedColor == TRACK_NONE)
    {
        if      (currentColor == TRACK_ORANGE) selectedColor = TRACK_ORANGE;
        else if (currentColor == TRACK_BLUE)   selectedColor = TRACK_BLUE;
    }

    // Count only the selected color; use rising-edge + debounce
    if (!finishTimerStarted && (now - lastCountTime) > COLOR_DEBOUNCE_MS)
    {
        if (selectedColor == TRACK_ORANGE &&
            currentColor  == TRACK_ORANGE &&
            lastDetectedColor != TRACK_ORANGE &&
            orangeCount < TARGET_COUNT)
        {
            orangeCount++;
            lastCountTime = now;
        }
        else if (selectedColor == TRACK_BLUE &&
                 currentColor  == TRACK_BLUE &&
                 lastDetectedColor != TRACK_BLUE &&
                 blueCount < TARGET_COUNT)
        {
            blueCount++;
            lastCountTime = now;
        }
    }

    lastDetectedColor = currentColor;

    // Start the 8-second finish timer once count reaches 12
    if (!finishTimerStarted)
    {
        if ((selectedColor == TRACK_ORANGE && orangeCount >= TARGET_COUNT) ||
            (selectedColor == TRACK_BLUE   && blueCount   >= TARGET_COUNT))
        {
            finishTimerStarted = true;
            finishStartTime    = now;
        }
    }

    updateOLED(r, g, b, c);

    // Signal finished after 8 seconds have elapsed
    if (finishTimerStarted && (now - finishStartTime >= STOP_DELAY_AFTER_12))
        return true;

    return false;
}

// =====================================
// I2C3 — special object detection
// RED     (238,39,55)
// GREEN   (68,214,44)
// MAGENTA (255,0,255)
// =====================================
SpecialObject detectSpecialObject()
{
    int r = MiniR4.I2C3.MXColorV3.getR();
    int g = MiniR4.I2C3.MXColorV3.getG();
    int b = MiniR4.I2C3.MXColorV3.getB();
    int c = MiniR4.I2C3.MXColorV3.getC();

    Serial.print("I2C3 R:"); Serial.print(r);
    Serial.print(" G:");      Serial.print(g);
    Serial.print(" B:");      Serial.print(b);
    Serial.print(" C:");      Serial.println(c);

    if (c < 30) return OBJ_NONE;

    // MAGENTA first — must precede RED so r+b dominant doesn't match RED
    if (r > 70 && b > 70 && g < 50 && r > g + 20 && b > g + 20)
        return OBJ_MAGENTA;

    // RED: r clearly dominant
    if (r > 80 && r > g + 30 && r > b + 20)
        return OBJ_RED;

    // GREEN: g clearly dominant
    if (g > 80 && g > r + 30 && g > b + 30)
        return OBJ_GREEN;

    return OBJ_NONE;
}

// =====================================
// Steering actions (for special objects)
// =====================================
void goRightOfObject()
{
    stopRobot();
    delay(100);
    MiniR4.RC4.setAngle(RIGHT_ANGLE);
    delay(300);
    moveForward();
    delay(900);
    stopRobot();
    MiniR4.RC4.setAngle(CENTER_ANGLE);
    delay(150);
}

void goLeftOfObject()
{
    stopRobot();
    delay(100);
    MiniR4.RC4.setAngle(LEFT_ANGLE);
    delay(300);
    moveForward();
    delay(900);
    stopRobot();
    MiniR4.RC4.setAngle(CENTER_ANGLE);
    delay(150);
}

// Scan right+left with laser, then steer toward the freer side
void servoScanAndGo()
{
    stopRobot();
    delay(150);

    MiniR4.RC4.setAngle(RIGHT_ANGLE);
    delay(500);
    int rightDist = readLaserMM();

    MiniR4.RC4.setAngle(LEFT_ANGLE);
    delay(500);
    int leftDist = readLaserMM();

    MiniR4.RC4.setAngle(CENTER_ANGLE);
    delay(250);

    if (rightDist >= leftDist)
    {
        MiniR4.RC4.setAngle(RIGHT_ANGLE);
        delay(250);
        moveForward();
        delay(700);
        stopRobot();
        MiniR4.RC4.setAngle(CENTER_ANGLE);
        delay(150);
    }
    else
    {
        MiniR4.RC4.setAngle(LEFT_ANGLE);
        delay(250);
        moveForward();
        delay(700);
        stopRobot();
        MiniR4.RC4.setAngle(CENTER_ANGLE);
        delay(150);
    }
}

// Returns true if a special object was handled (or is within its hold window).
// During the hold window the robot simply keeps moving — no re-executing action.
bool handleSpecialObject()
{
    unsigned long now = millis();

    // Inside hold window → robot just keeps moving forward; skip laser logic
    if (heldObject != OBJ_NONE && now < specialHoldUntil)
    {
        moveForward();
        return true;
    }

    // Hold expired — clear it
    heldObject = OBJ_NONE;

    SpecialObject obj = detectSpecialObject();
    if (obj == OBJ_NONE) return false;

    // Start hold timer
    heldObject       = obj;
    specialHoldUntil = now + SPECIAL_HOLD_MS;

    // Execute the action once
    if      (obj == OBJ_RED)     goRightOfObject();
    else if (obj == OBJ_GREEN)   goLeftOfObject();
    else if (obj == OBJ_MAGENTA) servoScanAndGo();

    return true;
}

// =====================================
// Normal laser obstacle avoidance
// =====================================
void avoidObstacle()
{
    int frontDist = readLaserMM();

    // No obstacle — keep going
    if (frontDist > OBSTACLE_DIST)
    {
        moveForward();
        return;
    }

    // Obstacle detected — stop and scan
    stopRobot();
    delay(200);

    MiniR4.RC4.setAngle(RIGHT_ANGLE);
    delay(500);
    int rightDist = readLaserMM();

    MiniR4.RC4.setAngle(LEFT_ANGLE);
    delay(500);
    int leftDist = readLaserMM();

    MiniR4.RC4.setAngle(CENTER_ANGLE);
    delay(300);

    if (rightDist > OBSTACLE_DIST && rightDist >= leftDist)
    {
        MiniR4.RC4.setAngle(RIGHT_ANGLE);
        delay(300);
        moveForward();
        delay(700);
        stopRobot();
        MiniR4.RC4.setAngle(CENTER_ANGLE);
        delay(200);
    }
    else if (leftDist > OBSTACLE_DIST && leftDist > rightDist)
    {
        MiniR4.RC4.setAngle(LEFT_ANGLE);
        delay(300);
        moveForward();
        delay(700);
        stopRobot();
        MiniR4.RC4.setAngle(CENTER_ANGLE);
        delay(200);
    }
    else
    {
        // Both sides blocked — back up further so the servo can scan a wider arc
        moveBackward(600);
    }
}

// =====================================
// Setup
// =====================================
void setup()
{
    MiniR4.begin();
    MiniR4.PWR.setBattCell(2);
    Serial.begin(9600);

    // Sensors
    MiniR4.I2C4.MXLaserV2.begin();
    MiniR4.I2C2.MXColorV3.begin();
    MiniR4.I2C3.MXColorV3.begin();

    // Motor and servo
    MiniR4.M1.setReverse(false);
    MiniR4.RC4.setAngle(CENTER_ANGLE);

    // OLED startup message
    MiniR4.OLED.clearDisplay();
    MiniR4.OLED.setTextSize(1);
    MiniR4.OLED.setTextColor(SSD1306_WHITE);
    MiniR4.OLED.setCursor(0, 0);
    MiniR4.OLED.print("Checking front...");
    MiniR4.OLED.display();

    // ── Initial obstacle check before the robot starts moving ──
    // If something is already within 20 cm, wait here until it clears.
    while (readLaserMM() <= OBSTACLE_DIST)
    {
        MiniR4.OLED.clearDisplay();
        MiniR4.OLED.setCursor(0, 0);
        MiniR4.OLED.print("BLOCKED - waiting");
        MiniR4.OLED.display();
        delay(200);
    }

    MiniR4.OLED.clearDisplay();
    MiniR4.OLED.setCursor(0, 0);
    MiniR4.OLED.print("Robot Ready");
    MiniR4.OLED.display();
    delay(500);
}

// =====================================
// Main loop
// Priority:
//   1. Line counting   (I2C2)  — updates OLED; sets finished flag
//   2. Special objects (I2C3)  — overrides obstacle logic
//   3. Laser obstacle avoidance
// =====================================
void loop()
{
    // If finished flag is set, keep robot stopped indefinitely
    if (finished)
    {
        stopRobot();
        return;
    }

    // ── Priority 1: Line counting ──
    if (handleColorCounting())
    {
        finished = true;
        stopRobot();
        showFinalOLED();
        return;
    }

    // ── Priority 2: Special object detection (overrides laser) ──
    if (handleSpecialObject())
    {
        delay(50);
        return;
    }

    // ── Priority 3: Normal laser obstacle avoidance ──
    avoidObstacle();

    delay(50);
}
