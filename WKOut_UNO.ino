#include <Arduino.h>
#include <TM1637Display.h> //Display
#include <Encoder.h>
#include <IRremote.hpp> // Library for TL1838 IR Receiver

#define ENC_A 2
#define ENC_B 3
#define CLK 11
#define DIO 12
#define BUTTON 7
#define COUNT_FIVE_MINS 1280
#define BASE_FIVE_MINS 300
#define BUZZER_PIN 6
#define IR_PIN 9
#define IR_UP 0xF7087F80
#define IR_DOWN 0xEF107F80
#define IR_OK 0xF30C7F80
#define IR_POWER 0xFC037F80


long counter = 0;
long old_counter = 0;
int state = 0;
int seconds = 0;
unsigned long millis_start = 0;
uint32_t ir_data = 0;

Encoder myEnc(ENC_B, ENC_A);

/// state = 0 => init
/// state = 1 => set up time
/// state = 10 => countdown
/// state = 20 => buzz/light

TM1637Display display(CLK, DIO);

void setup() {
  display.setBrightness(0x0f);
  display.showNumberDecEx(8888,0b01000000);
  delay(500);
  // Set encoder pins as inputs
  pinMode(BUTTON, INPUT_PULLUP);
  myEnc.write(0);
  // Setup Serial Monitor
  Serial.begin(9600);
  
  
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  state = 1;
  display.showNumberDecEx(0,0b01000000,true,4,0);
}

void loop(){
  switch (state) {
    case 1:
      state1();
      break;
    case 10:
      state10();
      break;
    case 20:
      state20();
      break;
    default:
      break;

  }
}

void state1(){
  // reads from the counter
  counter = myEnc.read();
  //changes from the IR
  if (IrReceiver.decode()) {
    ir_data = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();
    //Serial.println(ir_data, HEX);
    
    if (ir_data == IR_UP)
    {
      counter += 64;
      myEnc.write(counter);
    }
    if (ir_data == IR_DOWN)
    {
      counter -= 64;
      myEnc.write(counter);
    }
  }

  if (counter < 0) {myEnc.write(0);counter = 0;}
  if (counter != old_counter) {
    old_counter = counter;
    
    if (counter < COUNT_FIVE_MINS) {
      seconds = (counter >> 6) * 15;
    }
    else {
      seconds = BASE_FIVE_MINS + ((counter - COUNT_FIVE_MINS) >> 6) * 60;
    }
  }


  show_time(seconds);
  if (digitalRead(BUTTON) == LOW || ir_data == IR_OK){
    ir_data = 0;
    millis_start = millis();
    delay(950);
    state = 10;
  }
}


void show_time(int secs) {
  int time = (secs / 60)*100 + (secs % 60);
  display.showNumberDecEx(time,0b01000000,true,4,0);
}

void state10() {
  int minus = seconds - ((millis() - millis_start) / 1000);
  int time = (minus / 60)*100 + (minus % 60);
  if (minus %2 > 0) 
  display.showNumberDecEx(time,0b01000000,true,4,0);
  else
  display.showNumberDec(time,true,4,0);
  if (IrReceiver.decode()) {
    ir_data = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();
  }
  if (digitalRead(BUTTON) == LOW || ir_data == IR_POWER){
    display.showNumberDecEx(8888,0b01000000);
    delay(1000);
    ir_data = 0;
    state = 1;
  }
  if (time <= 0) {delay(1000);state = 20;}
}

void state20(){
  
  for(int i = 0; i < 3 ; i++){
    digitalWrite(LED_BUILTIN, HIGH);  // change state of the LED by setting the pin to the HIGH voltage level
    tone(BUZZER_PIN, 440, 500);
    delay(500);                      // wait for a second
    noTone(BUZZER_PIN);
    digitalWrite(LED_BUILTIN, LOW);   // change state of the LED by setting the pin to the LOW voltage level
    delay(500);                      // wait for a second
  }
  state = 1;
}