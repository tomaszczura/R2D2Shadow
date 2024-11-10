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
#define CHANGE_HOLO_PROJECTOR "CH"

#define SERVO_1A 0
#define SERVO_1B 1
#define LED_1_BLUE 2
#define LED_1_WHITE 3

#define SERVO_2A 4
#define SERVO_2B 5
#define LED_2_BLUE 6
#define LED_2_WHITE 7

#define SERVO_3A 8
#define SERVO_3B 9
#define LED_3_BLUE 10
#define LED_3_WHITE 11

Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 588 // this is the 'maximum' pulse length count (out of 4096)

int activeHoloProjector = 1;

void setup()
{
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");
  board1.begin();
  board1.setPWMFreq(60); // Analog servos run at ~60 Hz updates
}

void loop()
{
  turnOnLedsForActiveHoloProjector();

  board1.setPWM(SERVO_1A, 0, angleToPulse(0));
  board1.setPWM(LED_1_WHITE, 0, 4095);
  delay(2000);
  board1.setPWM(0, 0, angleToPulse(45));
  board1.setPWM(LED_1_WHITE, 0, 0);
  delay(2000);
}

int angleToPulse(int ang) // gets angle in degree and returns the pulse width
{
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX); // map angle of 0 to 180 to Servo min and Servo max
  Serial.print("Angle: ");
  Serial.print(ang);
  Serial.print(" pulse: ");
  Serial.println(pulse);
  return pulse;
}

int turnOnLed(int led)
{
  board1.setPWM(led, 0, 4095);
}

int turnOffLed(int led)
{
  board1.setPWM(led, 0, 0);
}

int activateNextHoloProjector()
{
  activeHoloProjector++;
  if (activeHoloProjector > 3)
  {
    activeHoloProjector = 1;
  }
  return activeHoloProjector;
}

int turnOnLedsForActiveHoloProjector()
{
  if (activeHoloProjector == 1)
  {
    turnOnLed(LED_1_BLUE);
    turnOffLed(LED_1_WHITE);

    turnOffLed(LED_2_BLUE);
    turnOnLed(LED_2_WHITE);

    turnOffLed(LED_3_BLUE);
    turnOnLed(LED_3_WHITE);
  }
  else if (activeHoloProjector == 2)
  {
    turnOnLed(LED_2_BLUE);
    turnOffLed(LED_2_WHITE);

    turnOffLed(LED_1_BLUE);
    turnOnLed(LED_1_WHITE);

    turnOffLed(LED_3_BLUE);
    turnOnLed(LED_3_WHITE);
  }
  else if (activeHoloProjector == 3)
  {
    turnOnLed(LED_3_BLUE);
    turnOffLed(LED_3_WHITE);

    turnOffLed(LED_1_BLUE);
    turnOnLed(LED_1_WHITE);

    turnOffLed(LED_2_BLUE);
    turnOnLed(LED_2_WHITE);
  }
}