#include <Servo.h>

Servo servo9;
Servo servo10;
Servo servo11;
Servo servo12;

Servo thrusterGroup1[] = { servo9, servo10 }; // groups of 2 Servos
Servo thrusterGroup2[] = { servo11, servo12 };
Servo allThrusters[] = { servo9, servo10, servo11, servo12 };

int thrusterStop = 1060;

int commands = {
  0, // pause thrusters
  1, // thruster forward
  2, // thruster back
  3, // left forward, right backward
  4, // left backward, right forward

  5, // increment forwardValue
  6, // decrement forwardValue
  7, // increment backwardValue
  8  // decrement backwardValue
}

int currentGroup = 0; // currently active group
int currentCommand = 0;
int forwardValue = 1500;
int backwardValue = 1500;


// we will interactively add or subtract to forward, backward values
int incrementOrDecrementValue = 20;

void setup() {
  setupServo(servo9,   9);
  setupServo(servo10, 10);
  setupServo(servo11, 11);
  setupServo(servo12, 12);
}

void setupServo(Servo servo, int pin) {
  // attach to pin, with small delay and send init status
  servo.attach(pin);
  delay(2);
  servo.writeMicroseconds(500); // init for ESC.
}

void loop() {
  if(Serial.available()) {
     // send the group to control and then the command.
     currentGroup = currentGroup(Serial.parseInt());
     currentCommand = Serial.parseInt();
  }
  
  // send the current group the current command.
  groupControl(currentGroup, currentCommand);
 
  pauseThrusters();
}

void groupControl(currentGroup, currentCommand) {
    swtich(currentCommand) {
    // thruster directions
    case commands[0]:
      pauseThrusters();
      break;
    case commands[1]:
      commandGroup(currentGroup, forwardValue, forwardValue);
      break;
    case commands[2]:
      commandGroup(currentGroup, backwardValue, backwardValue);
      break;
    case commands[3]:
      commandGroup(currentGroup, forwardValue, backwardValue);
      break;
    case commands[4]:
      commandGroup(currentGroup, backwardValue, forwardValue);
      break;

    // change thruster speed
    case commands[5]:
      forwardValue += incrementOrDecrementValue;
      break;
    case commands[6]:
      forwardValue -= incrementOrDecrementValue;
      break;
    case commands[7]:
      backwardValue += incrementOrDecrementValue;
      break;
    case commands[8]:
      backwardValue += incrementOrDecrementValue;
      break;
    default:
      pauseThrusters();
  }
}

void commandGroup(int theGroup, int value1, int value2) {
  // Send commands to a Servo group
  Servo group[] = getCurrentGroup(theGroup);
  group[0].writeMicroseconds(value1);
  group[1].writeMicroseconds(value2);
  delay(2);
}

void pauseThrusters() {
  // stop all thrusters!
  for(int i = 0; i < allThrusters.length(); i++) {
    allThrusters[i].writeMicroseconds(thrusterStop);
   }
   currentCommand = 0;
}

void runThroughRange(Servo myservo) {
  int pos = 0;
  for(pos = 1500; pos < 1900; pos += 1) {
    myservo.writeMicroseconds(pos);
    delay(15);
  }
  for(pos = 1900; pos>=1500; pos-=1) {
    myservo.writeMicroseconds(pos);
    delay(15);
  }
}

int[] getCurrentGroup(int group) {
  return group == 1 ? thrusterGroup1 : thrusterGroup2;
}

int setSerial3Buffer() {
  // FYI:
  // We can't replace this with serialPort.readString();
  // As much as I would like to...
  if(Serial3.available()) {
    int c; // a single character
    int i = 0;
    for(i = 0; i < 100; i++) {
      c = Serial3.read();
      if (c != '\n' && c != -1) {
        // This delay needs to be here or else garbage enters the buffer
        delay(2);
        buffer[i] = c;
      } else {
        buffer[i] = '\0';
      }
    }
  }
}
