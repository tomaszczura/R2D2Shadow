#include "SoftwareSerial.h"

#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00 // Returns info with command 0x41 [0x01: info, 0x00: no info]

SoftwareSerial mySerial(9, 10);

// Commands from arduino
#define START_SOUND "P1";
#define LEFT_PAD_CON "P2"
#define RIGHT_PAD_CON "P3"
#define L1_UP_ARROW "P4"
#define L1_LEFT_ARROW "P5"
#define L1_DOWN_ARROW "P6"
#define L1_RIGHT_ARROW "P7"
#define L2_UP_ARROW "P8"
#define L2_LEFT_ARROW "P9"
#define L2_RIGHT_ARROW "P10"
#define L2_DOWN_ARROW "P11"
#define UP_ARROW "P12"
#define LEFT_ARROW "P13"
#define RIGHT_ARROW "P14"

#define CONTROL_LED 3 // pin 5 on ATMEGA

// - Start - 14 msc
// - Left controller setup - 32
// - Right controller setup - 33

// - l1 + arrow
//  - up - alarm - 13-16
//  - left - misc - 17-24
//  - right - ooh - 25-31
//  - down - sent - 32-53
// - l2 + arrow
//   - up  - 1 scream
//  - left - 2 chortle
//  - right - 4 wolf
//  - down - 8 annoyed

//  - up -  3 - doodoo
//  - left - 6 short
//  - riught - 7 - patrol

// DFPlayer commands
#define INITIALIZE_PLAYER 0x3F
#define PLAY_TRACK 0x03
#define PAUSE 0x0E
#define PLAY_NEXT 0x01
#define SET_VOLUME 0x06

// there is a weird offset in the track numbers - 5th track is 1st track
#define OFFSET 5

void setup()
{
  pinMode(CONTROL_LED, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  initializePlayer();
  playTrack(14 + OFFSET); // play initial sound
}

void loop()
{
  digitalWrite(CONTROL_LED, HIGH);

  setVolumeFromPotentiometer();

  if (Serial.available() > 0)
  {
    String command = Serial.readString();

    if (command == LEFT_PAD_CON)
    {
      playTrack(32 + OFFSET);
    }
    else if (command == RIGHT_PAD_CON)
    {
      playTrack(33 + OFFSET);
    }
    else if (command == L1_UP_ARROW)
    {
      playTrack(random(13, 17) + OFFSET);
    }
    else if (command == L1_LEFT_ARROW)
    {
      playTrack(random(17, 25) + OFFSET);
    }
    else if (command == L1_DOWN_ARROW)
    {
      playTrack(random(25, 32) + OFFSET);
    }
    else if (command == L1_RIGHT_ARROW)
    {
      playTrack(random(32, 54) + OFFSET);
    }
    else if (command == L2_UP_ARROW)
    {
      playTrack(1 + OFFSET);
    }
    else if (command == L2_LEFT_ARROW)
    {
      playTrack(2 + OFFSET);
    }
    else if (command == L2_RIGHT_ARROW)
    {
      playTrack(4 + OFFSET);
    }
    else if (command == L2_DOWN_ARROW)
    {
      playTrack(8 + OFFSET);
    }
    else if (command == UP_ARROW)
    {
      playTrack(3 + OFFSET);
    }
    else if (command == LEFT_ARROW)
    {
      playTrack(6 + OFFSET);
    }
    else if (command == RIGHT_ARROW)
    {
      playTrack(7 + OFFSET);
    }
  }
}

int currentVolume = 0;

void setVolumeFromPotentiometer()
{
  int volResistance = analogRead(A5);
  int vol = map(volResistance, 0, 1023, 0, 30);
  if (vol != currentVolume)
  {
    currentVolume = vol;
    setVolume(vol);
  }
}

void initializePlayer()
{
  execute_CMD(INITIALIZE_PLAYER, 0, 0);
  delay(200);
  setVolumeFromPotentiometer();
  delay(200);
}

void playTrack(int track)
{
  execute_CMD(PLAY_TRACK, 0, track);
  delay(200);
}

void pause()
{
  execute_CMD(PAUSE, 0, 0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D, 0, 1);
  delay(500);
}

void setVolume(int volume)
{
  execute_CMD(SET_VOLUME, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}

// Excecute the command and parameters
void execute_CMD(byte CMD, byte Par1, byte Par2)
{
  // Calculate the checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  // Build the command line
  byte Command_line[10] = {
      Start_Byte,
      Version_Byte,
      Command_Length,
      CMD,
      Acknowledge,
      Par1,
      Par2,
      highByte(checksum),
      lowByte(checksum), End_Byte};
  // Send the command line to the module
  for (byte k = 0; k < 10; k++)
  {
    mySerial.write(Command_line[k]);
  }
}
