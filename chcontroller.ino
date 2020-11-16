/* 
 * Author: 
 *   Joshua Cummings
 *   Miranda Hanson
 * License: 
 *   MIT
 * Description:
 *   A custom musical input device for keyboard based interpretation.
 *   For example:
 *    https://www.musicca.com/guitar
 * Last Updated:
 *   2020-11-16
 */

 
#include "Keyboard.h"

#define NUM_BUTTONS 6
#define KEY_LEFT_SHIFT 0x81
#define KEY_PRESS_DURATION 3

int pot_pin = A0;
int pot_state = 0;
/*
 * TODO: Put button properties in a class,
 * then create InputButton array
 */
int button_pins[NUM_BUTTONS] = { 7,  2,  3,  4,  5,  6  };
char button_chars[NUM_BUTTONS][13] = {
  {'1','2','3','4','5','6','7','8','9','0', NULL, NULL, NULL},
  {'q','w','e','r','t','y','u','i','o','p','[',   ']',  '\\'},
  {'1','2','3','4','5','6','7','8','9','0', NULL, NULL, NULL},
  {'q','w','e','r','t','y','u','i','o','p','[',   ']',  '\\'},
  {'a','s','d','f','g','h','j','k','l',';','\'',  NULL, NULL},
  {'z','x','c','v','b','n','m',',','.','/', NULL, NULL, NULL},
};
bool button_states[NUM_BUTTONS] = { 0,  0,  0,  0,  0,  0  };
bool button_last_states[NUM_BUTTONS] = { 0,  0,  0,  0,  0,  0  };
char button_mods[NUM_BUTTONS] = {
  KEY_LEFT_SHIFT,
  KEY_LEFT_SHIFT,
  NULL,
  NULL,
  NULL,
  NULL,
};


void setup() {
  // Configure button pin inputs
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(button_pins[i], INPUT_PULLUP);
  }

  // Begin processes
  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  // Handle potentiometer
  pot_state = analogRead(pot_pin);
  pot_state = constrain(pot_state, 0, 1024);
  pot_state = map(pot_state, 0, 1024, 0, 14); 
  
  // Handle buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    
    // Update state
    button_last_states[i] = button_states[i];
    button_states[i] = digitalRead(button_pins[i]);

    // Send key command if passes debounce check and there's a mapped char
    if (button_states[i] && !button_last_states[i] && button_chars[i][pot_state] != NULL) {
      
      // Handle modifier
      if (button_mods[i]) {
        Keyboard.press(button_mods[i]);
        delay(1);
        Keyboard.press(button_chars[i][pot_state]);
        delay(KEY_PRESS_DURATION);        
        Keyboard.release(button_chars[i][pot_state]);
        delay(1);
        Keyboard.release(button_mods[i]);
      } else {
        Keyboard.press(button_chars[i][pot_state]);
        delay(KEY_PRESS_DURATION);        
        Keyboard.release(button_chars[i][pot_state]);       
      }
      
      Serial.print("Pin:"); Serial.print(button_pins[i]);
      Serial.print(" Mod:"); Serial.print(button_mods[i]);
      Serial.print(" Char:"); Serial.println(button_chars[i][pot_state]);
      
    }
  }

  delay(1);
}
