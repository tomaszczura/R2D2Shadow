#include "SoftwareSerial.h"

#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

SoftwareSerial mySerial(9, 10);

#define CONTROL_LED 3 // pin 5 on ATMEGA

// DFPlayer commands
#define INITIALIZE_PLAYER 0x3F
#define PLAY_TRACK 0x03
#define PAUSE 0x0E
#define PLAY_NEXT 0x01
#define SET_VOLUME 0x06

void setup () {
  pinMode(CONTROL_LED, OUTPUT);
  mySerial.begin (9600);
  delay(1000);
  initializePlayer();
  playFirst();
}

int currentPlay = 1;

void loop () { 
  digitalWrite(CONTROL_LED, HIGH);
  playTrack(currentPlay);
  currentPlay++;
  delay(5000);
  digitalWrite(CONTROL_LED, LOW);
  playTrack(currentPlay);
  currentPlay++;
  delay(5000);
}

void initializePlayer() {
  execute_CMD(INITIALIZE_PLAYER, 0, 0);
  delay(500);
  setVolume(20);
  delay(500);
}

void playTrack(int track) {
  execute_CMD(PLAY_TRACK, 0, track);
  delay(500);
}

void playFirst() {
  execute_CMD(0x11,0,1); 
  delay(500);
}

void pause() {
  execute_CMD(PAUSE,0,0);
  delay(500);
}

void play() {
  execute_CMD(0x0D,0,1); 
  delay(500);
}

void playNext() {
  execute_CMD(PLAY_NEXT,0,1);
  delay(500);
}

void playPrevious() {
  execute_CMD(0x02,0,1);
  delay(500);
}

void setVolume(int volume) {
  execute_CMD(SET_VOLUME, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}

// Excecute the command and parameters
void execute_CMD(byte CMD, byte Par1, byte Par2){
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
    lowByte(checksum), End_Byte
  };
  //Send the command line to the module
  for (byte k=0; k<10; k++){
    mySerial.write(Command_line[k]);
  }
}

