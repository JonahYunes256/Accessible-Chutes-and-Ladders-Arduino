#include <FastLED.h>

int button[4] = {9,2,12,7};


#define LED_STRIP_PIN_1 8
#define LED_STRIP_PIN_2 10
#define LED_STRIP_PIN_3 13
#define NUM_LEDS 70 
#define NUM_SECTIONS 6
#define LED_TYPE    WS2812B

CRGB leds1[149];
CRGB leds2[30];
CRGB leds3[30];

// ------------die-------------
CRGB sectionColors[NUM_SECTIONS] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Orange, CRGB::Violet};
int recordedNumber = 0;
//----------------------------------

//-------Game--------
int player[4] ={0,1,2,3};
int player1 = 0;
int player2 = 1;
int player3 = 2;
int player4 = 3;

int player_t = 1;

int playerp[4] = { 0, 0, 0, 0};
CRGB playerc[4] = {CRGB::Blue, CRGB::Red, CRGB::Green, CRGB::Magenta};


int numTiles1 = 0;
int numTiles2 = 0;
int numTiles3 = 0;
int numTiles4 = 0;


//int chutet[10] = {98,95,93,64,62,56,49,48,87,16};
//int chuteb[10]= {78,75,73,60,19,53,11,26,24,6};
//int ladderb[9] {1,4,9,21,28,36,51,71,80};
//int laddert[9]= {38,14,31,42,84,44,67,91,100};

int chutet[10] = {116,113,111,103,74,76,66,56,57,18};
int chuteb[10] = {92,89,87,28,21,70,63,30,13,6};
int ladderb[9] = {1,4,9,25,32,42,61,85,94};
int laddert[9] = {44,16,37,50,100,52,79,109,118};

int rTiles[100] = {1,2,3,4,5,6,7,8,9,10,
           13,14,15,16,17,18,19,20,21,22,
           25,26,27,28,29,30,31,32,33,34,
           37,38,39,40,41,42,43,44,45,46,
           49,50,51,52,53,54,55,56,57,58,
           61,62,63,64,65,66,67,68,69,70,
           73,74,75,76,77,78,79,80,81,82,
           85,86,87,88,89,90,91,92,93,94,
           97,98,99,100,101,102,103,104,105,106,
           109,110,111,112,113,114,115,116,117,118};
           
           





const int blendDuration = 1000;  // Time to blend colors in milliseconds
const int blendSteps = 50;       // Number of steps for blending

int skip[9] = {11, 21, 31, 41, 51, 61, 71, 81, 91};
//int use[13,26,39,43,53,63,73,83,93];
//-----------------------



void setup() {
  //powerup saftey
  delay(3000);
  
  //for testing values
  Serial.begin(9600);
  
   //Initialize the first LED strip
  FastLED.addLeds<NEOPIXEL, LED_STRIP_PIN_1>(leds1, 150);
  FastLED.setBrightness(60);  // Adjust brightness as needed
  

  // Initialize the second LED strip
  FastLED.addLeds<NEOPIXEL, LED_STRIP_PIN_2>(leds2, 30);
  // Set additional configuration for the second strip if needed
  FastLED.setBrightness(60); 
  

  // Initialize the third LED strip
  FastLED.addLeds<NEOPIXEL, LED_STRIP_PIN_3>(leds3, 30);
  
  // Set additional configuration for the third strip if needed
  FastLED.setBrightness(60);
  pinMode(button[0], INPUT_PULLUP);
  pinMode(button[1], INPUT_PULLUP);
  pinMode(button[2], INPUT_PULLUP);
  pinMode(button[3], INPUT_PULLUP);
  
  // Use analogRead() on an unconnected pin to generate entropy for randomSeed()
  randomSeed(analogRead(0));

  // Display the initial state of all LED strips
  FastLED.show();
}



//----------------------------------------------------------------- Die --------------------------------------------------------

int die(CRGB ledx[], CRGB ledy[])
{
  fill_solid(ledx, 30, CRGB::Black);
  fill_solid(ledy, 30, CRGB::Black);

    int ledsPerSection = 30 / NUM_SECTIONS;
    // for first led
    for (int i = 0; i < NUM_SECTIONS; ++i) 
    {
      // Turn on the LEDs in the current section with the assigned color
      for (int j = 0; j < ledsPerSection; ++j) 
      {
        int ledIndex = i * ledsPerSection + j;
        ledx[ledIndex] = sectionColors[i];
        ledy[ledIndex] = sectionColors[i];
      }
      // for second led
    
    for (int i = 0; i < NUM_SECTIONS; ++i) 
    {
      // Turn on the LEDs in the current section with the assigned color
      for (int j = 0; j < ledsPerSection; ++j) 
      {
        int ledIndex = i * ledsPerSection + j;
        ledx[ledIndex] = sectionColors[i];
        ledy[ledIndex] = sectionColors[i];
      }
      
      FastLED.show();
      delay(500);

      // Turn off the LEDs in the current section
      fill_solid(ledx, 30, CRGB::Black);
      fill_solid(ledy, 30, CRGB::Black);
      FastLED.show();
      delay(200);
    }

    // Randomly choose a set of LEDs to keep on
    int randomSet = random(NUM_SECTIONS);
    for (int j = randomSet * ledsPerSection; j < (randomSet + 1) * ledsPerSection; ++j) {
      ledx[j] = sectionColors[randomSet];
      ledy[j] = sectionColors[randomSet];
    }
    
    FastLED.show();

    // Record the random number (1-6)
    //recordedNumber = random(1, 7);
    Serial.println("Recorded Number: " + String(randomSet));


    // Reset recordedNumber for the next round
    return randomSet;
    
  }
  delay(2000);
  
}

//------------------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------- Debounce -------------------------------------------------------------

// Function to debounce a button
bool debounce(int buttonPin) 
{
  static unsigned long lastDebounceTime = 0;
  static unsigned long debounceDelay = 50;

  unsigned long currentMillis = millis();

  if (currentMillis - lastDebounceTime > debounceDelay) 
  {
    if (digitalRead(buttonPin) == LOW) 
    {
      lastDebounceTime = currentMillis;
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------------------------------------------------------------






//---------------------------------------------------------- Move ----------------------------------------------------------------
void move(CRGB leds[], int player, int numLeds, int currentPos, int targetPos, int playerp[], CRGB playerc[]) 
{


  // Ensure target position is within the valid range
  targetPos = constrain(targetPos, 0, numLeds - 1);

  
  // Skip two LEDs every ten LEDs
  //int skipOffset = (targetPos / 10) * 2;

  // Apply the skip offset to the target position
  //targetPos += skipOffset;

  targetPos = rTiles[targetPos-1];
  
  


  // Check if other players are still at the current position
  bool otherPlayersAtCurrentPos = false;
  for (int b = 0; b < 4; b++) {
    if (b != player && playerp[b] == currentPos) {
      otherPlayersAtCurrentPos = true;
      break;
    }
  }

 // Flag to indicate whether the ladder or chute animation has been completed
  bool ladderChuteAnimationComplete = false;
  int sequenceStep = 0;
  // Check for special positions (chutes and ladders)
  switch (targetPos) {
    case 100:
      // Handle the case when the targetPos is a special position (e.g., winning position)
      leds[1] = playerc[player];
      FastLED.show();
      delay(50);
      ladderChuteAnimationComplete = true; // Set the flag to true after winning position animation
      break;
    default:
      for (int i = 0; i < 10; i++) {
        if (targetPos == chutet[i] || targetPos == ladderb[i]) {
          
          // Handle chutes and ladders
          int sequencePos = targetPos;
          // int sequenceStep = (targetPos > currentPos) ? 1 : -1; // Determine the direction of the sequence

          //Move the player to target position before animation
          leds[targetPos] = playerc[player];
          FastLED.show();
          delay(500);

          
        if (targetPos == ladderb[i])
        {
          sequenceStep = 1;
          while (sequencePos != laddert[i]) 
          {
            // Update the position in the sequence
            sequencePos += sequenceStep;

            // Blink the LEDs to show the change in position
            if (leds[sequencePos] == CRGB::Black) {
              leds[sequencePos] = playerc[player];
            }
            FastLED.show();
            delay(250);

            leds[sequencePos] = CRGB::Black;
            FastLED.show();
            delay(50);
          }
        }
        
        if (targetPos == chutet[i])
        {
          sequenceStep = -1;
          while (sequencePos != chuteb[i]) 
          {
            // Update the position in the sequence
            sequencePos += sequenceStep;

            // Blink the LEDs to show the change in position
            if (leds[sequencePos] == CRGB::Black) {
              leds[sequencePos] = playerc[player];
            }
            FastLED.show();
            delay(250);

            leds[sequencePos] = CRGB::Black;
            FastLED.show();
            delay(50);
          }
           
        }

        
        if (ladderChuteAnimationComplete && !otherPlayersAtCurrentPos) 
        {
          leds[currentPos] = CRGB::Black;
          currentPos = sequencePos;
        }
          
          // At the end of the sequence, set the ladderChuteAnimationComplete flag to true
          ladderChuteAnimationComplete = true;
          break;
        }
        else
          {
            // No other players at the target position, set it directly
            leds1[targetPos] = playerc[player];
            FastLED.show();
            delay(50);
          }
      }
      
     
  }

  // Check if other players are still at the target position
  bool otherPlayersAtTargetPos = false;
  for (int j = 0; j < 4; j++) {
    if (j != player && playerp[j] == targetPos) {
      otherPlayersAtTargetPos = true;
      break;
    }
  }

    


  // Turn off LEDs at the previous position only if ladder or chute animation is complete
  if (ladderChuteAnimationComplete && !otherPlayersAtCurrentPos) {
    leds1[currentPos] = CRGB::Black;
  }

  // Update the player's position
  currentPos = targetPos;
}
//------------------------------------------------------------------------------------------------------------------------
//
//void move(CRGB leds[], int player, int numLeds, int currentPos, int targetPos, int playerp[], CRGB playerc[]) 
//{
//  int chutet[10] = {16,48,49,56,62,64,87,93,95,98};
//  int chuteb[10] = {6,26,11,53,19,60,24,73,75,78};
//  int ladderb[9] = {1,4,9,21,28,36,51,71,80};
//  int laddert[9] = {38,14,31,42,84,44,67,91,100};
//
//
//  // Ensure target position is within the valid range
//  targetPos = constrain(targetPos, 0, numLeds - 1);
//
//  
//  // Skip two LEDs every ten LEDs
//  //int skipOffset = (targetPos / 10) * 2;
//
//  // Apply the skip offset to the target position
//  //targetPos += skipOffset;
//
//  targetPos = rTiles[targetPos];
//
//
//  // Check if other players are still at the current position
//  bool otherPlayersAtCurrentPos = false;
//  for (int b = 0; b < 4; b++) {
//    if (b != player && playerp[b] == currentPos) {
//      otherPlayersAtCurrentPos = true;
//      break;
//    }
//  }
//
// // Flag to indicate whether the ladder or chute animation has been completed
//  bool ladderChuteAnimationComplete = false;
//  int sequenceStep = 0;
//  // Check for special positions (chutes and ladders)
//  switch (targetPos) {
//    case 100:
//      // Handle the case when the targetPos is a special position (e.g., winning position)
//      leds1[1] = playerc[player];
//      FastLED.show();
//      delay(50);
//      ladderChuteAnimationComplete = true; // Set the flag to true after winning position animation
//      break;
//    default:
//      for (int i = 0; i < 10; i++) {
//        if (targetPos == chutet[i] || targetPos == ladderb[i]) {
//          int sequencePos = targetPos;
//          int sequenceStep = (targetPos > currentPos) ? 1 : -1; // Determine the direction of the sequence
//
//          // Move the player to target position before animation
//          leds1[targetPos] = playerc[player];
//          FastLED.show();
//          delay(500);
//
//          while (sequencePos != (targetPos == ladderb[i] ? laddert[i] : chuteb[i])) {
//            // Update the position in the sequence
//            sequencePos += sequenceStep;
//
//            // Blink the LEDs to show the change in position
//            if (leds1[sequencePos] == CRGB::Black) {
//              leds1[sequencePos] = playerc[player];
//            }
//            FastLED.show();
//            delay(250);
//
//            leds1[sequencePos] = CRGB::Black;
//            FastLED.show();
//            delay(50);
//          }
//
//          // At the end of the sequence, set the ladderChuteAnimationComplete flag to true
//          ladderChuteAnimationComplete = true;
//          break;
//        }
//      }
//  }
//  // Check if other players are still at the target position
//  bool otherPlayersAtTargetPos = false;
//  for (int j = 0; j < 4; j++) {
//    if (j != player && playerp[j] == targetPos) {
//      otherPlayersAtTargetPos = true;
//      break;
//    }
//  }
//
//   // Blend logic
//  if (otherPlayersAtTargetPos) {
//    CRGB targetColor = CRGB::Black;
//
//    // Blend the colors of all players at the target position
//    for (int j = 0; j < 4; j++) {
//      if (j != player && playerp[j] == targetPos) {
//        nblend(targetColor, playerc[j], 128); // Adjust the blend value as needed
//      }
//    }
//
//    // Smoothly blend the colors at the target position
//    for (int step = 0; step < blendSteps; step++) {
//      nblend(leds1[targetPos], targetColor, 256 / blendSteps);  // Adjust the blend value as needed
//      FastLED.show();
//      delay(blendDuration / blendSteps);
//    }
//  } else {
//    // No other players at the target position, set it directly
//    leds1[targetPos] = playerc[player];
//    FastLED.show();
//    delay(50);
//  }
//
//  // Turn off LEDs at the previous position only if ladder or chute animation is complete
//  if (ladderChuteAnimationComplete && !otherPlayersAtCurrentPos) {
//    leds1[currentPos] = CRGB::Black;
//  }
//
//  // Update the player's position
//  currentPos = targetPos;
//}



//---------------------------------------------------------- checking and fixing colors -----------------------------------------------
void checkAndFixColors(CRGB leds[], int numLeds, int playerp[], CRGB playerc[]) {
  for (int i = 0; i < numLeds; i++) {
    // Check if the LED has a blended color
    if (leds[i].r != playerc[playerp[i]].r || leds[i].g != playerc[playerp[i]].g || leds[i].b != playerc[playerp[i]].b) {
      // Check if there's only one player on this spot
      int playerCount = 0;
      int currentPlayer = -1;

      for (int j = 0; j < 4; j++) {
        if (playerp[j] == i) {
          playerCount++;
          currentPlayer = j;
        }
      }

      // If there's only one player, set the LED to that player's color
      if (playerCount == 1) {
        leds[i] = playerc[currentPlayer];
      }
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------

////-------------------------------------------------------- Blending functions ------------------------------------------------------------
//void blendPlayersAtPosition(CRGB leds[], int playerp[], CRGB playerc[], int numPlayers, int numLeds, int blendSteps, int blendDuration) {
//    // Iterate over each LED position
//    for (int pos = 0; pos < numLeds; pos++) {
//        // Find players at the current position
//        int playersAtPos[4];  // Assuming a maximum of 4 players
//        int numPlayersAtPos = 0;
//
//        // Check each player's position
//        for (int player = 0; player < numPlayers; player++) {
//            if (playerp[player] == pos) {
//                playersAtPos[numPlayersAtPos++] = player;
//            }
//        }
//
//        // If more than one player is at the current position, blend their colors
//        if (numPlayersAtPos > 1) {
//            CRGB targetColor = CRGB::Black;
//
//            // Blend the colors of all players at the target position
//            for (int i = 0; i < numPlayersAtPos; i++) {
//                nblend(targetColor, playerc[playersAtPos[i]], 128); // Adjust the blend value as needed
//            }
//
//            // Smoothly blend the colors at the target position
//            for (int step = 0; step < blendSteps; step++) {
//                nblend(leds[pos], targetColor, 256 / blendSteps);  // Adjust the blend value as needed
//                FastLED.show();
//                delay(blendDuration / blendSteps);
//            }
//        }
//    }
//}
////--------------------------------------------------------------------------------------------------------------------------------------
//


//-------------------------------------------------------- Updating player positions ------------------------------------------------------
void updatePlayerLEDs(CRGB leds[], int numLeds, int playerp[], CRGB playerc[]) {
  // Clear all LEDs to black before updating
  fill_solid(leds, numLeds, CRGB::Black);

  // Update LEDs based on player positions and colors
  for (int i = 0; i < 4; i++) {
    int playerPos = playerp[i];
    if (playerPos >= 0 && playerPos < numLeds) {
      leds[playerPos] = playerc[i];
    }
  }

  // Show the updated LED configuration
  FastLED.show();
}
//------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------- Number of Players Check ---------------------------------------
//int numPlayers = 0;
//int numPlayer()
//{
//  if (digitalRead(button[0]=LOW) && numPlayers == 0)
//  {
//    numPlayers++;
//  }
//  if (digitalRead(button[1]=LOW) && numPlayers == 1)
//  {
//    numPlayers++;
//  }
//  if (digitalRead(button[2]=LOW) && numPlayers == 2)
//  {
//    numPlayers++;
//  }
//  if (digitalRead(button[3]=LOW) && numPlayers == 3)
//  {
//    numPlayers++;
//  }
//  return numPlayers;
//}
//---------------------------------------------------------------------------------------------------------------------------------


int count = 0;
int loop1 = 0;


//-------------------------------------------------------------------- Game -------------------------------------------------------------
void loop() {

  // Delay or do other things as needed
  //delay(1000);  // Adjust this delay as needed
  
  //updating the number of players playing
//  if (loop1 == 0)
//  {
//    //count = numPlayer();
//  }
//    for (int i = 0; i < NUM_LEDS; i++) {
//    leds1[i] = CRGB::Blue;
//  }
//  FastLED.show();
//
//  // Wait for 5 seconds
//  delay(5000);
//
//  // Turn off all LEDs using a for loop
//  for (int i = 0; i < NUM_LEDS; i++) {
//    leds1[i] = CRGB::Black;
//  }
//  delay(1000);
//  FastLED.show();
    
  
  
  //checkAndFixColors(leds1, 100, playerp, playerc);

  
  
  if (player_t == 1 && debounce(button[0])) {
    
    numTiles1 = die(leds3, leds2) + 1;
    move(leds1, player1, 149, playerp[0], playerp[0] + numTiles1, playerp, playerc);
    playerp[0] += numTiles1;
   for (int i =0; i < 10; i++)
      {
        if (playerp[0] == chutet[i])
        {
          playerp[0] = chuteb[i];
          //break;
        }
      }
      
    for (int v =0; v < 9; v++)
      {
      if (playerp[0] == ladderb[v])
        {
          playerp[0] = laddert[v];
        }
      }
    
    numTiles1 = 0;
    
    //checkAndFixColors(leds1, 100, playerp, playerc);

    // checking how many players are there
    player_t = 2;
  }
  

  if (player_t == 2 && debounce(button[1])) {
    numTiles2 = die(leds3, leds2) + 1;
    move(leds1, player2, 149, playerp[1], playerp[1] + numTiles2, playerp, playerc);
    playerp[1] += numTiles2;
    for (int i =0; i < 10; i++)
      {
        if (playerp[1] == chutet[i])
        {
          playerp[1] = chuteb[i];
          //break;
        }
      }
      for (int v =0; v < 9; v++)
      {
      if (playerp[1] == ladderb[v])
        {
          playerp[1] = laddert[v];
        }
      }
    numTiles2 = 0;
    
    player_t = 3;

    // checking for the number of players playing
//    if (count > 2)
//    { 
      player_t = 3;
//    }
//    else
//    {
//      player_t = 1;
//    }
    
    //checkAndFixColors(leds1, 100, playerp, playerc);
  }

  

  if (player_t == 3 && debounce(button[2])) {
    
    numTiles3 = die(leds3, leds2) + 1;
    //delay(1500);
    move(leds1, player3, 149, playerp[2], playerp[2] + numTiles3, playerp, playerc);
    playerp[2] += numTiles3;
    for (int i =0; i < 10; i++)
      {
        if (playerp[2] == chutet[i])
        {
          playerp[2] = chuteb[i];
          //break;
        }
      }
      for (int v =0; v < 9; v++)
      {
      if (playerp[2] == ladderb[v])
        {
          playerp[2] = laddert[v];
        }
      }
    numTiles3 = 0;

    // checking for number of players playing
//    if (count > 3)
//    {
      player_t = 4;
    //}
 //    else
//    {
//      player_t = 1;
//    }

    
    
    //checkAndFixColors(leds1, 100, playerp, playerc);
  }

  

  if (player_t == 4 && debounce(button[3])) {
    numTiles4 = die(leds3, leds2) + 1;
    move(leds1, player4, 149, playerp[3], playerp[3] + numTiles4, playerp, playerc);
    playerp[3] += numTiles4;
    for (int i =0; i < 10; i++)
      {
        if (playerp[3] == chutet[i])
        {
          playerp[3] = chuteb[i];
          //break;
        }
      }
      for (int v =0; v < 9; v++)
      {
      if (playerp[3] == ladderb[v])
        {
          playerp[3] = laddert[v];
        }
      }
    numTiles4 = 0;
    player_t = 1;
    //checkAndFixColors(leds1, 100, playerp, playerc);
  }

  updatePlayerLEDs(leds1, 149, playerp, playerc);




 //blendPlayersAtPosition(leds1, playerp, playerc, 4, 100, 4, 1000);

  // Additional loop to fade out LEDs for each player's previous position
//  for (int i = 0; i < 4; i++) 
//  {
//    fadeToBlackBy(leds1 + playerp[i], 1, 20);
//  }




  FastLED.show();
}
