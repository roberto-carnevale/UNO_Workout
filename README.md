# Workout clock
## Why another a workout clock?
A decent Workout clock comes about 50 euros.
This is minimal, but at home you do not need huge stuff:
* a remote
* a countdown
* a buffer to tell you to stop/start
easy and friendly!

## What do you need?
An old Arduino UNO or similar board... and you you are smarter than me you can use the ATMega alone.
* a display TM1736
* a ir receiver LM1838
* a standard rotor encoder
* a passive buzzer
and a button to start.

Globally is no more than a few euros and an Arduino UNO you probably do not use anymore...

## PIN Out
Connect the encoder to pin 2 and 3 to use Arduino UNO interrupts.

The 7 segment 4 digit is connected to 11 (CLK) and 12 (DIO).

I set up a pull up button at pin 7 and the pull up buzzer at pin 6.

finally the IR at pin 8 (do not use a PWM pin).

## An old remote
If you have an old remote from a broken "whatever" stuff, use it... record the sequence and substitute them on define

| Constant    | Description      |
| ----------- | ---------------- |
| IR_UP       | +15" or +1'      |
| IR_DOWN     | -15" or -1'      |
| IR_OK       | Start countdown  |
| IR_POWER    | Stop countdown   |

Each 64 steps of the rotary encoder the clock gets +/-15" till 5:00 and 1' after til 60 minutes.
## IRremote lib and tune()
IRremote library and `tune()` command share the same timer in the Arduino UNO platform.
The `#define IR_USE_AVR_TIMER1` instruction moves the IRremote lib on another internal timer.

## Code logic
The system is a state machine.
| State    | Description      |
| -------- | ---------------- |
|   1      | Set up           |
|  10      | Countdown        |
|  20      | Buzzing          |



