# Access control system
It’s an access control system which checks if there is a car at the gate using an **IR sensor**.
If yes, the system checks the driver card ID by using an **RFID scanner**. If it is authorized, the gate will open using a **servo motor** and the car will pass. 
Also an **LCD** will print “pass” and a **green LED** will be on. If it isn’t authorized, the gate will not open and the LCD will print “failed” and a **red LED** will be on.
## List of used components:
- IR sensor
- RFID scanner
- Servo motor
- LCD
- Arduino Uno
- Green LED
- Red LED

## Design models:
### 1. Wiring diagram
![alt](https://drive.google.com/uc?export=view&id=1lh4NTBGpeUcTVLEoWNqAvCe0FE8Q0pfC)
### 2. Finite State Machine
![alt](https://drive.google.com/uc?export=view&id=1-fh69O4-rr26Y6tQpgS831l4HruOTz-G)
### 3. Petri-nets Diagram
![alt](https://drive.google.com/uc?export=view&id=1zgbLQtsZFtKLakEwusHlyBdoGGM32Ksk)
- The system starts with one token in IDLE state, and we have 6 input states, that they can have tokens or not depending on user actions or sensor reads.
- The 6 input states are ( IR1, IR2, Card True, Card False, Not IR1, Not IR2)

## Test Cases Description
#### 1. The system is in an idle state (Gate is off, Leds are off, LCD “Welcome”). 
  - When a car arrives in front of the gate the LCD displays “Scan Your Card”. 
#### 2. The System is in a car state (Gate is off, Leds are off, LCD “Scan Your Card”). 
  - If the car returns and leaves the gate the system returns to idle state again and the LCD displays “Welcome”. 
  - If the user scans an unauthenticated card the LCD displays “Fail” and a red led becomes on for three seconds then returns to the car state to try scanning the card again. 
  - If the user scans an authenticated card the LCD displays “Pass” and a Green led becomes on and the motor opens the gate. 
#### 3. The system is in a pass state (Gate is On, red led is off,green led is on, LCD “pass”). 
  - If the outer IR catches the car leaving the gate the system closes the gate and goes to Busy state. 
#### 4. The system is in a busy state (Gate is off, Leds are off) 
  - If the car stays in front of the outer IR the LCD displays “Busy”. 
  - If there is a car in front of the gate the system returns to the car state. 
  - If there is not a car in front of the gate the system returns to the idle state
  
### References: 
- https://store.fut-electronics.com/products/arduino-uno-r3-latest-revision 
- https://create.arduino.cc/projecthub/akshayjoseph666/interface-16x2-lcd-parallel-interface-with-arduino-uno-2e87e2 
- https://lastminuteengineers.com/how-rfid-works-rc522-arduino-tutorial/ 
- https://www.skyfilabs.com/online-courses/access-control-with-rfid
