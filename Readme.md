# R2D2 OS

## Connection

### Arduino
- GND -> Syren 0V
- 14 (TX1) -> Body Master RX
- 16 (TX2) -> Syren S1
- 23 (Software TX)-> Sound module RX

### Syren
- 0V -> Sabertooth 0V
- S1 -> Sabertooth S1 

### Body master

Body master connects SCL, SDA, GND and VCC to corresponding PCA9685 pins

| Motor Name          | PCA9685 Pin Number |
|-------------------|------------|
| INTERFACE_DOORS   | 0          |
| INTERFACE_CONTROL | 1          |
| INTERFACE_ARM     | 2          |
| GRIPPER_DOORS     | 3          |
| GRIPPER_ARM       | 4          |
| GRIPPER_CONTROL   | 5          |
| UTILITY_ARM_1     | 6          |
| UTILITY_ARM_2     | 7          |

## Control

### Body - Right controller

- Cross - toggle interface arm
- Circle - toggle gripper arm
- Up arrow:
    - Toggle upper utility arm
    - Dome L1: Doodoo Sound
    - Body L1: Alarm sound
    - Body L2: Scream sound
- Right arrow:
    - Control gripper arm
    - Body L1: "Sent" sound
    - Body L2: Wolf sound
- Down arrow:
    - Toggle down utility arm
    - Dome L1: Patrol sound
    - Body L1: "Ooh" sound
    - Body L2: Annoyed sound
- Left arrow:
    - Control interface arm
    - Dome L1: Short sound
    - Body L1: Miscellaneous sound
    - Body L2: Chortle sound


### Dome - Left controller

## Commands

### Body master commands

| Command                | Code   | Description                |
|------------------------|--------|----------------------------|
| TOGGLE_INTERFACE_ARM     | BM_TIA | Toggles the interface arm    |
| ROTATE_INTERFACE_ARM     | BM_SIA | Rotates the interface arm    |
| TOGGLE_GRIPPER_ARM       | BM_TGA  | Toggles the gripper arm      |
| CONTROL_GRIPPER_ARM     | BM_CGA  | Controls (open/close) the gripper arm    |
| TOGGLE_UTILITY_ARM_1   | BM_TU1 | Toggles utility arm 1      |
| TOGGLE_UTILITY_ARM_2   | BM_TU2 | Toggles utility arm 2      |


### Sound commands

Commands sent from Ardiuno to sound module with DFPlayer

| Command         | Code|Description|Sound no|
|-----------------|-----|-----------|--------|
| START_SOUND     | P1  |Sound played when module starts| 14
| LEFT_PAD_CON    | P2  |Left controller connected | 32
| RIGHT_PAD_CON   | P3  |Right controller connected| 33
| ALARM_SOUND     | P4  |Alarm sound | 13-16
| MISC_SOUND   | P5  |Miscalleounus | 17-24
| OOH_SOUND   | P6  |"Ooh" sound | 25-31
| SENT_SOUND  | P7  |"Sent" sound | 32-53
| SCREAM_SOUND     | P8  |Scream sound|1
| CHORTLE_SOUND   | P9  |Chortle Sound|2
| WOLF_SOUND  | P10 |Wolf sound|4
| ANNOYED_SOUND   | P11 |Annoyed sound|8
| DOO_DOO_SOUND        | P12 |DooDoo|3
| SHORT_SOUND      | P13 |Short|6
| PATROL_SOUND     | P14 |Patrol|7
