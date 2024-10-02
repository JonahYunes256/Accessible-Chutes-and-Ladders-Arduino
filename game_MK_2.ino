#include <Adafruit_NeoPixel.h>

#define NUM_LEDS_STRIP 10  // Number of LEDs in each strip
#define NUM_STRIPS 3       // Number of LED strips
#define LED_PIN_1 6        // Data pin for the first LED strip
#define LED_PIN_2 7        // Data pin for the second LED strip
#define LED_PIN_3 8        // Data pin for the third LED strip

#define BUTTON_PIN_1 2     // Button pin for player 1
#define BUTTON_PIN_2 3     // Button pin for player 2
#define BUTTON_PIN_3 4     // Button pin for player 3
#define BUTTON_PIN_4 5     // Button pin for player 4

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS_STRIP, LED_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS_STRIP, LED_PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS_STRIP, LED_PIN_3, NEO_GRB + NEO_KHZ800);

int playerPositions[4] = {0, 0, 0, 0};  // Player positions on the board
int diceValue;

void setup() {
  strip1.begin();
  strip2.begin();
  strip3.begin();
  
  strip1.show();
  strip2.show();
  strip3.show();

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(BUTTON_PIN_4, INPUT_PULLUP);
}

void loop() {
  // Check for button presses
  checkButtonPress(BUTTON_PIN_1, 0);
  checkButtonPress(BUTTON_PIN_2, 1);
  checkButtonPress(BUTTON_PIN_3, 2);
  checkButtonPress(BUTTON_PIN_4, 3);
}

void checkButtonPress(int buttonPin, int player) {
  if (digitalRead(buttonPin) == LOW) {
    // Button is pressed, roll the dice
    diceValue = random(1, 7);  // Generates a random number between 1 and 6

    // Update player position
    playerPositions[player] += diceValue;

    // Update LED strips based on player positions
    updateLEDs();

    // Check for chutes and ladders
    checkChutesAndLadders();

    // Delay for visual effect (you can adjust this based on your preference)
    delay(1000);
  }
}

void updateLEDs() {
  // Clear all LEDs
  clearLEDs();

  // Update LEDs based on player positions
  for (int i = 0; i < 4; i++) {
    int stripNum = playerPositions[i] / NUM_LEDS_STRIP;
    int ledNum = playerPositions[i] % NUM_LEDS_STRIP;
    
    if (stripNum == 0) {
      strip1.setPixelColor(ledNum, 255, 0, 0);  // Red for player 1
    } else if (stripNum == 1) {
      strip2.setPixelColor(ledNum, 0, 255, 0);  // Green for player 2
    } else if (stripNum == 2) {
      strip3.setPixelColor(ledNum, 0, 0, 255);  // Blue for player 3
    }
  }

  // Show the updated LEDs
  strip1.show();
  strip2.show();
  strip3.show();
}

void clearLEDs() {
  // Turn off all LEDs
  strip1.clear();
  strip2.clear();
  strip3.clear();
}

void checkChutesAndLadders() {
  // Implement the logic for chutes and ladders here
  // Update player positions accordingly
  // You may need a lookup table or specific rules based on the game board
}
