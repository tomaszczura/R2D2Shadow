#include <Adafruit_PWMServoDriver.h>

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
#define SERVOMAX 588 // this is the 'maximum' pulse length count (out of 4096)

// PCA9685 board addresses
#pragma region "PCA9685 board addresses"

#define INTERFACE_DOORS 0
#define INTERFACE_CONTROL 1
#define INTERFACE_ARM 2

#define GRIPPER_DOORS 3
#define GRIPPER_ARM 4
#define GRIPPER_CONTROL 5

#define UTILITY_ARM_1 6
#define UTILITY_ARM_2 7

#define DOORS_CLOSE 90 // angle for doors close
#define DOORS_OPEN 0   // angle for doors open

#define ARM_DOWN 0 // angle for arm down
#define ARM_UP 120 // angle for arm up

#define INTERFACE_ARM_MIN 90 // angle for min interface arm spin
#define INTERFACE_ARM_MAX 0  // angle for max interface arm spin

#define GRIPPER_ARM_CLOSED 0 // angle for gripper arm closed
#define GRIPPER_ARM_OPEN 180 // angle for gripper arm open

#define UTILITY_ARM_CLOSED 0 // angle for utility arm closed
#define UTILITY_ARM_OPEN 180 // angle for utility arm open

#define DOORS_CLOSE_DELAY 1000 // delay for door close

#pragma endregion

void setup()
{
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");
  pcaBoard.begin();
  pcaBoard.setPWMFreq(60); // Analog servos run at ~60 Hz updates

  closeInterfaceArm();
}

void loop()
{
  delay(2000);

  // setMotorAngle(INTERFACE_CONTROL, INTERFACE_ARM_MAX);
  // setMotorAngle(INTERFACE_ARM, ARM_UP);
  // delay(DOORS_CLOSE_DELAY);
  // setMotorAngle(INTERFACE_DOORS, DOORS_OPEN);

  // openInterfaceArm();
  // delay(2000);
  closeInterfaceArm();
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

void closeInterfaceArm()
{
  setMotorAngle(INTERFACE_CONTROL, INTERFACE_ARM_MIN);
  setMotorAngle(INTERFACE_ARM, ARM_DOWN);
  delay(DOORS_CLOSE_DELAY);
  setMotorAngle(INTERFACE_DOORS, DOORS_CLOSE);
}

void openInterfaceArm()
{
  setMotorAngle(INTERFACE_DOORS, DOORS_OPEN);
  delay(DOORS_CLOSE_DELAY);
  setMotorAngle(INTERFACE_ARM, ARM_UP);
}