#include <FastLED.h>

int button[4] = {9,12,2,7};


#define LED_STRIP_PIN_1 8
#define LED_STRIP_PIN_2 10
#define LED_STRIP_PIN_3 13
#define NUM_PLAYERS 4
#define NUM_LEDS 118 
#define NUM_LEDS_D 30
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
int player[4] = {0,1,2,3};
int player1 = 0;
int player2 = 1;
int player3 = 2;
int player4 = 3;

int player_t = 1;

int playerp[4] = { 0, 0, 0, 0};
CRGB playerc[4] = {CRGB::Blue, CRGB::Red, CRGB::Green, CRGB::Yellow};

int numTiles[4] = {0,0,0,0};
int numTiles1 = 0;
int numTiles2 = 0;
int numTiles3 = 0;
int numTiles4 = 0;


int chutet_[10] = {98,95,93,64,62,56,49,48,87,16};
int chuteb_[10]= {78,75,73,60,19,53,11,26,24,6};
int ladderb_[9] {1,4,9,21,28,36,51,71,80};
int laddert_[9]= {38,14,31,42,84,44,67,91,100};

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
  fill_solid(ledx, NUM_LEDS_D, CRGB::Black);
  fill_solid(ledy, NUM_LEDS_D, CRGB::Black);

    int ledsPerSection = 30 / NUM_SECTIONS;
    
    for (int i = 0; i < NUM_SECTIONS; ++i) 
    {
      // Turn on the LEDs in the current section with the assigned color
      for (int j = 0; j < ledsPerSection; ++j) 
      {
        int ledIndex = i * ledsPerSection + j;
        ledx[ledIndex] = sectionColors[i];
        ledy[ledIndex] = sectionColors[i];
      }
    
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
void move(int player, int currentPos, int targetPos) 
{
  // Ensure target position is within the valid range
  targetPos = constrain(targetPos, 0, NUM_LEDS - 1);

  
  // Skip two LEDs every ten LEDs
  //int skipOffset = (targetPos / 10) * 2;

  // Apply the skip offset to the target position
  //targetPos += skipOffset;
  

  int RCP =rTiles [currentPos - 1];
  int RTP = rTiles[targetPos - 1];
  
  


  // Check if other players are still at the current position
  bool otherPlayersAtCurrentPos = false;
  for (int b = 0; b < 4; b++) 
  {
    if (b != player && playerp[b] == currentPos) 
    {
      otherPlayersAtCurrentPos = true;
      break;
    }
  }

 // Flag to indicate whether the ladder or chute animation has been completed
  bool ladderChuteAnimationComplete = false;
  int sequenceStep = 0;
  int sequencePos = RTP;
  // Check for special positions (chutes and ladders)
  switch (RTP) {
    case 118:
      //Winning animation
      for (int s = 0; s<10; s++)
      {
        delay (250);
        leds1[118] = playerc[player];
        FastLED.show();
        delay(250);

        leds1[118] = CRGB::Black;
        FastLED.show();
        delay(250);
      }
      playerp[player] = 0;
      leds1[1] = playerc[player];
      FastLED.show();
      delay(50);
      ladderChuteAnimationComplete = true; // Set the flag to true after winning position animation
      //asm volatile ("  jmp 0");
      break;
      
    default:
      for (int i = 0; i < 10; i++) {
//-----------------chute---------------
        if (RTP == chutet[i]) {
          
          //Move the player to target position before animation
          leds1[RTP] = playerc[player];
          leds1[RCP] = CRGB::Black;
          FastLED.show();
          delay(500);

          // Squence ticks to be going down because it is a chute
          sequenceStep = -1;
          
          while (sequencePos != chuteb[i]) 
          {
            // Update the position in the sequence
            sequencePos += sequenceStep;

            // Blink the LEDs to show the change in position
            if (leds1[sequencePos] == CRGB::Black) {
              leds1[sequencePos] = playerc[player];
            }
            FastLED.show();
            delay(250);

            leds1[sequencePos] = CRGB::Black;
            FastLED.show();
            delay(50);
          }
          // At the end of the sequence, set the ladderChuteAnimationComplete flag to true
          leds1[RTP] = playerc[player];
          leds1[RCP] = CRGB::Black;
                FastLED.show();
                delay(50);
          ladderChuteAnimationComplete = true;
          break;
           if (ladderChuteAnimationComplete && !otherPlayersAtCurrentPos) 
            {
              leds1[RCP] = CRGB::Black;
            }
          
          
        
        }
        else
          {
            // No other players at the target position, set it directly
            leds1[RTP] = playerc[player];
            FastLED.show();
            delay(50);
          }
      }


//------------------Ladder-----------------
        for (int d = 0; d < 9; d++)
        {
          if (RTP == ladderb[d])
          {

            //Move the player to target position before animation
            leds1[RTP] = playerc[player];
            leds1[RCP] = CRGB::Black;
            FastLED.show();
            delay(500);
                sequenceStep = 1;
            while (sequencePos != laddert[d]) 
            {
              // Update the position in the sequence
              sequencePos += sequenceStep;
  
              // Blink the LEDs to show the change in position
              if (leds1[sequencePos] == CRGB::Black) {
                leds1[sequencePos] = playerc[player];
              }
              FastLED.show();
              delay(250);
  
              leds1[sequencePos] = CRGB::Black;
              FastLED.show();
              delay(50);
             }
              if (ladderChuteAnimationComplete && !otherPlayersAtCurrentPos) 
                {
                  leds1[RCP] = CRGB::Black;
                }
              
              // At the end of the sequence, set the ladderChuteAnimationComplete flag to true
              leds1[RTP] = playerc[player];
              leds1[RCP] = CRGB::Black;
                FastLED.show();
                delay(50);
              ladderChuteAnimationComplete = true;
              break;
            
            }
            else
              {
                // No other players at the target position, set it directly
                leds1[RTP] = playerc[player];
                leds1[RCP] = CRGB::Black;
                FastLED.show();
                delay(50);
              }
        }     
  }

  // Turn off LEDs at the previous position only if ladder or chute animation is complete
  if (ladderChuteAnimationComplete && !otherPlayersAtCurrentPos) 
  {
    leds1[currentPos] = CRGB::Black;
  }
}

//------------------------------------------------------------------------------------------------------------------------




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




//-------------------------------------------------------- Updating player positions ------------------------------------------------------
void updatePlayerLEDs(CRGB leds[], int numLeds, int playerp[], CRGB playerc[]) {
  int RPP[NUM_PLAYERS];
  
  // Clear all LEDs to black before updating
  fill_solid(leds, numLeds, CRGB::Black);
  
  for (int x = 0; x < NUM_PLAYERS; x++)
  {
    RPP[x] = rTiles[playerp[x]-1];
  }
  
  // Update LEDs based on player positions and colors
  for (int i = 0; i < NUM_PLAYERS; i++) {
    int playerPos = RPP[i];
    if (playerPos >= 0 && playerPos < numLeds) {
      leds[playerPos] = playerc[i];
    }
  }
  for (int q =0; q < 4; q++)
  {
    if (playerp[0] == playerp[q])
    {
      
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
  delay(100);  // Adjust this delay as needed
  
  //updating the number of players playing
//  if (loop1 == 0)
//  {
//    count = numPlayer();
//  }

//-------------------- Little Test --------------------- 
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
  // put count instead of NUM_PLAYERS
  for (int i = 0; i < NUM_PLAYERS; i++)
  {
   //if (debounce(button[i]))
   //{
      numTiles[i] = die(leds3, leds2) + 1;
      delay(100);
      move(player[i], playerp[i], playerp[i] + numTiles[i]);
      playerp[i] += numTiles[i];
      for (int j = 0; j < 10; j++)
      {
        if (playerp[i] == chutet_[j])
        {
          playerp[i] = chuteb_[j];
          break;
        }
      }
          
      for (int v =0; v < 9; v++)
        {
        if (playerp[i] == ladderb_[v])
          {
            playerp[i] = laddert_[v];
          }
        }

        // Reset the return from the die function
        numTiles[i] = 0;
    
        // Set the player turn to to the next player
        //player_t = 2;
     //}
     updatePlayerLEDs(leds1, 118, playerp, playerc);
     FastLED.show();

  }



  updatePlayerLEDs(leds1, 118, playerp, playerc);

 //blendPlayersAtPosition(leds1, playerp, playerc, 4, 100, 4, 1000);

  // Additional loop to fade out LEDs for each player's previous position
//  for (int i = 0; i < 4; i++) 
//  {
//    fadeToBlackBy(leds1 + playerp[i], 1, 20);
//  }




  FastLED.show();
}
