# R2D2 OS

## Connection

#### Arduino
- GND -> Syren 0V
- 16 (TX2) -> Syren S1
- 23 (Software TX)-> Sound module RX

#### Syren
- 0V -> Sabertooth 0V
- S1 -> Sabertooth S1 


### Sound commands

Commands sent from Ardiuno to sound module with DFPlayer

| Command         | Code|Description|Sound no|
|-----------------|-----|-----------|--------|
| START_SOUND     | P1  |Sound played when module starts| 14
| LEFT_PAD_CON    | P2  |Left controller connected | 32
| RIGHT_PAD_CON   | P3  |Right controller connected| 33
| L1_UP_ARROW     | P4  |Alarm sound | 13-16
| L1_LEFT_ARROW   | P5  |Miscalleounus | 17-24
| L1_DOWN_ARROW   | P6  |"Ooh" sound | 25-31
| L1_RIGHT_ARROW  | P7  |"Sent" sound | 32-53
| L2_UP_ARROW     | P8  |Scream sound|1
| L2_LEFT_ARROW   | P9  |Chortle Sound|2
| L2_RIGHT_ARROW  | P10 |Wolf sound|4
| L2_DOWN_ARROW   | P11 |Annoyed sound|8
| UP_ARROW        | P12 |DooDoo|3
| LEFT_ARROW      | P13 |Short|6
| RIGHT_ARROW     | P14 |Patrol|7
