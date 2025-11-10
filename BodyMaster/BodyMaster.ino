#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>

#include "R2D2Commands.h"

/** Marcduino port mapping
 * ATMEGA -> ARDUINO -> MARCDUINO
 * 4 -> 2 -> 1
 * 5 -> 3 -> 2
 * 6 -> 4 -> 3
 * 11 -> 5 -> 4
 * 12 -> 6 -> 5
 * 13 -> 7 -> 11
 * 14 -> 8 -> RC IN S
 * 15 -> 9 -> 6
 * 16 -> 10 -> 7
 * 17 -> 11 -> 8
 * 18 -> 12 -> 9
 * 19 -> 13 -> 10
 * 23 -> A0 -> Slave/MP S
 * 24 -> A1 -> Teeces/mp3 S
 * 25 -> A2 ->
 * 26 -> A3 ->
 * *
 *
 * ISP
 * 3 2 1
 * 6 5 4
 *
 * RST SCK  MISO
 * GND MOSI VCC
 *
 * 1 - MISO
 * 2 - SCK
 * 3 - RST
 * 4 - VCC
 * 5 - MOSI
 * 6 - GND
 */

// Commands

Adafruit_PWMServoDriver pcaBoard = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 480 // this is the 'maximum' pulse length count (out of 4096)

// PCA9685 board addresses
#pragma region "PCA9685 board addresses"

#define INTERFACE_DOORS 0
#define INTERFACE_CONTROL 1
#define INTERFACE_ARM 2

#define GRIPPER_DOORS 3
#define GRIPPER_CONTROL 4
#define GRIPPER_ARM 5

#define UTILITY_ARM_TOP 6
#define UTILITY_ARM_DOWN 7

#define INT_DOORS_CLOSE 132 // angle for interface doors close
#define INT_DOORS_OPEN 0    // angle for interface doors open

#define GRIP_DOORS_CLOSE 25 // angle for interface doors close
#define GRIP_DOORS_OPEN 130 // angle for interface doors open

#define ARM_DOWN 0 // angle for arm down
#define ARM_UP 180 // angle for arm up

#define INTERFACE_ARM_MIN 15  // angle for min interface arm spin
#define INTERFACE_ARM_MAX 180 // angle for max interface arm spin

#define GRIPPER_ARM_CLOSED 110 // angle for gripper arm closed
#define GRIPPER_ARM_OPEN 50    // angle for gripper arm open

#define UTILITY_ARM_CLOSED 0 // angle for utility arm closed
#define UTILITY_ARM_OPEN 120 // angle for utility arm open

#define DOORS_DELAY 1000 // delay for door close

#pragma endregion

bool isTopUtilityArmOpen = false;
bool isBottomUtilityArmOpen = false;
bool isGripperArmShown = false; // flag for gripper arm
bool isInterfaceArmShown = false;
bool isGripperOpen = false; // flag for gripper state - closed or open
bool isInterfaceRotated = false;

Servo servo;

void setup()
{
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");
  pcaBoard.begin();
  pcaBoard.setPWMFreq(50); // Analog servos run at ~60 Hz updates

  hideInterfaceArm();
  hideGripperArm();
  closeBottomUtilityArm();
  closeTopUtilityArm();

  setMotorAngle(INTERFACE_DOORS, INT_DOORS_OPEN);
}

void loop()
{
  // while (Serial.available() > 0)
  {
    String command = Serial.readString();

    if (command == TOGGLE_GRIPPER_ARM)
    {
      handleToggleGripperArmCommand();
    }
    else if (command == TOGGLE_INTERFACE_ARM)
    {
      handleToggleInterfaceArmCommand();
    }
    else if (command == TOGGLE_UTILITY_ARM_TOP)
    {
      handleToggleUtilityArm1Command();
    }
    else if (command == TOGGLE_UTILITY_ARM_DOWN)
    {
      handleToggleUtilityArm2Command();
    }
    else if (command == CONTROL_GRIPPER)
    {
      handleControlGripperCommand();
    }
    else if (command == ROTATE_INTERFACE_ARM)
    {
      handleControlInterfaceCommand();
    }
  }
}

void handleToggleGripperArmCommand()
{
  if (isGripperArmShown)
  {
    hideGripperArm();
  }
  else
  {
    showGripperArm();
  }
}

void handleToggleInterfaceArmCommand()
{
  if (isInterfaceArmShown)
  {
    hideInterfaceArm();
  }
  else
  {
    showInterfaceArm();
  }
}

void handleToggleUtilityArm1Command()
{
  if (isTopUtilityArmOpen)
  {
    closeTopUtilityArm();
  }
  else
  {
    openTopUtilityArm();
  }
}

void handleToggleUtilityArm2Command()
{
  if (isBottomUtilityArmOpen)
  {
    closeBottomUtilityArm();
  }
  else
  {
    openBottomUtilityArm();
  }
}

void handleControlGripperCommand()
{
  if (!isGripperArmShown)
  {
    return;
  }
  if (isGripperOpen)
  {
    closeGripper();
  }
  else
  {
    openGripper();
  }
}

void handleControlInterfaceCommand()
{
  if (!isInterfaceArmShown)
  {
    return;
  }

  if (isInterfaceRotated)
  {
    rotateInInterfaceArm();
  }
  else
  {
    rotateOutInterfaceArm();
  }
}

int angleToPulse(int ang) // gets angle in degree and returns the pulse width
{
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX); // map angle of 0 to 180 to Servo min and Servo max
  return pulse;
}

void setMotorAngle(int motor, int angle)
{
  pcaBoard.setPWM(motor, 0, angleToPulse(angle));
}

void hideInterfaceArm()
{
  isInterfaceArmShown = false;
  rotateInInterfaceArm();
  setMotorAngle(INTERFACE_ARM, ARM_DOWN);
  delay(DOORS_DELAY);
  setMotorAngle(INTERFACE_DOORS, INT_DOORS_CLOSE);
}

void showInterfaceArm()
{
  isInterfaceArmShown = true;
  setMotorAngle(INTERFACE_DOORS, INT_DOORS_OPEN);
  delay(DOORS_DELAY);
  setMotorAngle(INTERFACE_ARM, ARM_UP);
}

void rotateOutInterfaceArm()
{
  isInterfaceRotated = true;
  setMotorAngle(INTERFACE_CONTROL, INTERFACE_ARM_MAX);
}

void rotateInInterfaceArm()
{
  isInterfaceRotated = false;
  setMotorAngle(INTERFACE_CONTROL, INTERFACE_ARM_MIN);
}

void showGripperArm()
{
  isGripperArmShown = true;
  setMotorAngle(GRIPPER_DOORS, GRIP_DOORS_OPEN);
  delay(DOORS_DELAY);
  setMotorAngle(GRIPPER_ARM, ARM_UP);
}

void hideGripperArm()
{
  isGripperArmShown = false;
  setMotorAngle(GRIPPER_ARM, ARM_DOWN);
  closeGripper();
  delay(DOORS_DELAY);
  setMotorAngle(GRIPPER_DOORS, GRIP_DOORS_CLOSE);
}

void openGripper()
{
  isGripperOpen = true;
  setMotorAngle(GRIPPER_CONTROL, GRIPPER_ARM_OPEN);
}

void closeGripper()
{
  isGripperOpen = false;
  setMotorAngle(GRIPPER_CONTROL, GRIPPER_ARM_CLOSED);
}

void openTopUtilityArm()
{
  isTopUtilityArmOpen = true;
  setMotorAngle(UTILITY_ARM_TOP, UTILITY_ARM_OPEN);
}

void closeTopUtilityArm()
{
  isTopUtilityArmOpen = false;
  setMotorAngle(UTILITY_ARM_TOP, UTILITY_ARM_CLOSED);
}

void openBottomUtilityArm()
{
  isBottomUtilityArmOpen = true;
  setMotorAngle(UTILITY_ARM_DOWN, UTILITY_ARM_OPEN);
}

void closeBottomUtilityArm()
{
  isBottomUtilityArmOpen = false;
  setMotorAngle(UTILITY_ARM_DOWN, UTILITY_ARM_CLOSED);
}