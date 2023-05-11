// Crame Arm - Control a robotic crane arm using an IR remote and an Arduino
// NANO 33 IoT The arm has 3 movements: opening/closing the gripper(claw),
// moving the arm up/down, and rotating the base clockwise/counterclockwise

#include <IRremote.h>  // Library for IR remote
#include <Servo.h>     // Library for Servo

// Servo variables
Servo arm, rightClaw, leftClaw, base;

// Buzzer variable
int buzzer = 7;

// IR Receiver variable
const byte IR_RECEIVE_PIN = 14;

// Arm and Claw speed variables
const int ARM_SPEED = 2;
const int CLAW_SPEED = 3;

// Arm and Claw position variable
int armPosition = 10;
int clawPosition = 0;

// Define IR Remote buttons
#define CH_MINUS 69
#define LEFT_ARROW 68
#define CH 70
#define RIGHT_ARROW 64
#define CH_PLUS 71
#define PAUSE 67

void setup() {
  base.attach(10);  // Attach base servo to pin 10
  base.write(90);   // Set position to 90 degrees

  arm.attach(8);  // Attach arm servo to pin 8
  arm.write(10);  // Set position to 10 degrees

  rightClaw.attach(3);  // Attach right claw servo to pin 3
  rightClaw.write(0);   // Set position to 0 degrees

  leftClaw.attach(2);  // Attach left claw servo to pin 2
  leftClaw.write(0);   // Set position to 0 degrees

  Serial.begin(9600);  // Set baud rate to 9600 bits per second

  IrReceiver.begin(IR_RECEIVE_PIN,
                   ENABLE_LED_FEEDBACK);  // Initialize IR Receiver

  pinMode(buzzer, OUTPUT);  // Set buzzer pin to output

  tone(buzzer, 2000);  // Play startup sound
  delay(100);          // Wait 100 ms
  noTone(buzzer);      // Stop startup sound
}

void loop() {
  if (IrReceiver.decode())  // Get button command from remote
  {
    switch (IrReceiver.decodedIRData.command)  // Decode button command (code)
    {
      case CH_MINUS: {
        armPosition += ARM_SPEED;  // Increase arm position by arm speed
        arm.write(armPosition);    // Move arm servo to arm position
        Serial.print("Arm moving up. Position: ");  // Print arm position
        Serial.println(armPosition);
        break;
      }
      case LEFT_ARROW: {
        if (armPosition > 10) {
          armPosition -= ARM_SPEED;  // Decrease arm position by arm speed
        }
        arm.write(armPosition);  // Move arm servo to arm position
        Serial.print("Arm moving down. Position: ");  // Print arm position
        Serial.println(armPosition);
        break;
      }
      case CH: {
        base.write(80);  // Rotate base servo counterclockwise
        Serial.println("Rotating counterclockwise.");  // Print claw position
        delay(75);                                     // Wait 75 ms
        base.write(90);  // Stop base servo rotation
        break;
      }
      case RIGHT_ARROW: {
        base.write(103);                        // Rotate base servo clockwise
        Serial.println("Rotating clockwise.");  // Print claw position
        delay(75);                              // Wait 75 ms
        base.write(90);                         // Stop base servo rotation
        break;
      }
      case CH_PLUS: {
        clawPosition += CLAW_SPEED;  // Increase claw position by claw speed
        rightClaw.write(180 - clawPosition);  // Move right claw inwards
        leftClaw.write(clawPosition);         // Move left claw inwards

        Serial.print("Claw closing. Position: ");  // Print claw position
        Serial.println(clawPosition);
        break;
      }
      case PAUSE: {
        clawPosition -= CLAW_SPEED;  // Decrease claw position by claw speed
        rightClaw.write(180 - clawPosition);       // Move right claw outwards
        leftClaw.write(clawPosition);              // Move left claw outwards
        Serial.print("Claw opening. Position: ");  // Print claw position
        Serial.println(clawPosition);
        break;
      }
      default: {
        base.write(90);  // Stop base rotation
        break;
      }
    }
    IrReceiver.resume();  // Reset and check for new button command
  }
}