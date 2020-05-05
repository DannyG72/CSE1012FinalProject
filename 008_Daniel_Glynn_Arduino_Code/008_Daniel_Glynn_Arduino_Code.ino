#include <SevSeg.h>
#include <Adafruit_NeoPixel.h>
#include <math.h> 

//Change default maximum score by editing the line below
int maximumScore = 15;               //initial maximum score


SevSeg sevseg;
int ledPinRGB = 19; // Initialize LED pin
Adafruit_NeoPixel strip(1, ledPinRGB, NEO_RGB); // Initialize Adafruit LED object

String stringFromSerial; // initialize empty string that will be used to assign strings from Serial.
int numberFromSerial; // variable that will be used to take number from serial

int displayTimeRemainingLeft = 1000; //timer variable that will be counte down for displaying Left information
int displayTimeRemainingRight = 0;   //timer variable that will be counte down for displaying Right information
int displayTimeRemainingMaximum = 0; //timer variable that will be counte down for displaying maximum score information
int timeSinceLastScoreDecrease = 0;  //timer variable that will be counted Up for determining if maximum score should be displayed
int currentNumberToDisplay = 0;      //variable that will be displayed 
int tensPlaceScore = 0;              //initial tens place

int buttonPinLeft = 14; // define button pin //
int buttonStateLeft = 0;// button read state initialized to zero
int buttonStateOldLeft = 2; // assert button state old as something that is impossible to begin with
int currentNumberLeft = 0; // current number displayed on the scoreboard
int ledPinLeft = 17;

int buttonPinRight = 15; // define button pin
int buttonStateRight = 0;// button read state initialized to zero
int buttonStateOldRight = 2; // assert button state old as something that is impossible to begin with
int currentNumberRight = 0; // current number displayed on the scoreboard
int ledPinRight = 18;

int buttonPinMiddle = 16; // define button pin
int buttonStateMiddle = 0;// button read state initialized to zero
int buttonStateOldMiddle = 2; // assert button state old as something that is impossible to begin with
int ScoreCountingTo=0; // what should the middle button cause to display on the screen, this is the score being counted to


void setup(){

 Serial.begin(9600);
 while (! Serial); // Wait until Serial is ready
 
 Serial.println("Enter the following commands to set scores:");
 Serial.println("'Max #' to set the maximum score 0-79");
 Serial.println("'Left #' to set the current Left score 0-79");
 Serial.println("'Right #' to set the current Right score 0-79");
 Serial.println("Current Scores are now: ");
 Serial.print("Max Score: ");
 Serial.println(maximumScore);
 Serial.print("Left Score: ");
 Serial.println(currentNumberLeft);
 Serial.print("Right Score: ");
 Serial.println(currentNumberRight);

 byte numDigits = 1; // SevSeg package
 byte digitPins[] = {}; // SevSeg package
 byte segmentPins[] = {10, 9, 5, 7, 8, 2, 3, 6}; // SevSeg package - define each segment is on which arduino pin
 bool resistorsOnSegments = true;
 byte hardwareConfig = COMMON_ANODE; // SevSeg package
 sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments); // SevSeg package
 sevseg.setBrightness(10); // SevSeg package
 pinMode(buttonPinLeft, INPUT); // Initialize left button pin
 pinMode(buttonPinRight, INPUT); // Initialize right button pin
 pinMode(buttonPinMiddle, INPUT); // Initialize middle button pin
 pinMode(ledPinLeft, OUTPUT); // Initialize left LED pin
 pinMode(ledPinRight, OUTPUT); // Initialize right LED pin
 strip.begin();
 strip.show(); // Initialize all pixels to 'off'
}

void loop(){
  
  if(Serial.available() > 0)
    { 
    stringFromSerial = Serial.readStringUntil('\n'); //This will wait to see words sent to serial.
    stringFromSerial.toLowerCase(); // convert serial to lowercase
    Serial.print("From Serial:" );  
    Serial.println(stringFromSerial);
    numberFromSerial = getValue(stringFromSerial, ' ', 1).toInt(); //use a user defined getValue function to split the string on the space character and take the second value.
  
    if (stringFromSerial.startsWith("m")) { //Check if string starts with m to set max score
      maximumScore = numberFromSerial;
    } else if (stringFromSerial.startsWith("l")) { //Check if string starts with l to set left score
      currentNumberLeft = numberFromSerial;
    } else if (stringFromSerial.startsWith("r")) { //Check if string starts with r to set right score
      currentNumberRight = numberFromSerial;
    } else {}
    Serial.println("Current Scores are now: ");
    Serial.print("Max Score: ");
    Serial.println(maximumScore);
    Serial.print("Left Score: ");
    Serial.println(currentNumberLeft);
    Serial.print("Right Score: ");
    Serial.println(currentNumberRight);
    }

 buttonStateLeft = digitalRead(buttonPinLeft);    // read button state from left button pin
 buttonStateRight = digitalRead(buttonPinRight);  // read button state from right button pin
 buttonStateMiddle = digitalRead(buttonPinMiddle);// read button state from middle button pin
 
 if ((buttonStateOldLeft != buttonStateLeft) && (buttonStateLeft == 1)&& (buttonStateMiddle == 0)) { //Check to see if left button is pressed and the middle button is not pressed.
 currentNumberLeft += 1; // increase left score by one if left button is pressed
 displayTimeRemainingLeft = 1000; // reset left timer to have it displayed for 1 second
 displayTimeRemainingRight = 0;
 displayTimeRemainingMaximum = 0;
 Serial.println("Current Scores are now: ");
 Serial.print("Max Score: ");
 Serial.println(maximumScore);
 Serial.print("Left Score: ");
 Serial.println(currentNumberLeft);
 Serial.print("Right Score: ");
 Serial.println(currentNumberRight);
 
 } else if ((buttonStateOldLeft != buttonStateLeft) && (buttonStateLeft == 1)&& (buttonStateMiddle == 1)) { // check if right button is pressed and middle button is being held
 currentNumberLeft -= 1; // decrease left score by one if left button is pressed while middle button is beind held
 displayTimeRemainingLeft = 1000; // reset left timer to have it displayed for 1 second
 displayTimeRemainingRight = 0;
 displayTimeRemainingMaximum = 0;
 timeSinceLastScoreDecrease = 0;
 Serial.println("Current Scores are now: ");
 Serial.print("Max Score: ");
 Serial.println(maximumScore);
 Serial.print("Left Score: ");
 Serial.println(currentNumberLeft);
 Serial.print("Right Score: ");
 Serial.println(currentNumberRight);
 
 } else if ((buttonStateOldRight != buttonStateRight) && (buttonStateRight == 1)&& (buttonStateMiddle == 0)) { // check if right button is pressed and middle button is not being held
 currentNumberRight += 1; // increase right score by one if button is pressed
 displayTimeRemainingLeft = 0;
 displayTimeRemainingRight = 1000; // reset right timer to have it displayed for 1 second
 displayTimeRemainingMaximum = 0;
 Serial.println("Current Scores are now: ");
 Serial.print("Max Score: ");
 Serial.println(maximumScore);
 Serial.print("Left Score: ");
 Serial.println(currentNumberLeft);
 Serial.print("Right Score: ");
 Serial.println(currentNumberRight);

 } else if ((buttonStateOldRight != buttonStateRight) && (buttonStateRight == 1)&& (buttonStateMiddle == 1)) { // check if right button is pressed and middle button is being held
 currentNumberRight -= 1; // decrease right score by one if left button is pressed while middle button is beind held
 displayTimeRemainingLeft = 0;
 displayTimeRemainingRight = 1000; // reset right timer to have it displayed for 1 second
 displayTimeRemainingMaximum = 0;
 timeSinceLastScoreDecrease = 0;
 Serial.println("Current Scores are now: ");
 Serial.print("Max Score: ");
 Serial.println(maximumScore);
 Serial.print("Left Score: ");
 Serial.println(currentNumberLeft);
 Serial.print("Right Score: ");
 Serial.println(currentNumberRight);
 
 } else if ((buttonStateLeft == 0 ) && (buttonStateRight == 0 ) && (buttonStateMiddle == 1) && (timeSinceLastScoreDecrease >= 1000)) { // check if neither side buttons are pressed, middle button is being held, and timeSinceLastScoreDecrease is greater than 1000 as to not instantly hide a score thats been decreased.
 displayTimeRemainingLeft = 0;
 displayTimeRemainingRight = 0;
 displayTimeRemainingMaximum = 1000;
 } else {}

 if (displayTimeRemainingLeft > 0) { // Left is being displayed right now if displayTimeRemainingLeft is greater than 0.
  currentNumberToDisplay = currentNumberLeft;
  if (currentNumberLeft >= maximumScore) { //Check if left score is greater than or equal to maximum score.
    if (displayTimeRemainingLeft%200>100) { //Cause LED to blink.
      digitalWrite(ledPinLeft, HIGH);
    } else {
      digitalWrite(ledPinLeft, LOW);
    }
  } else {digitalWrite(ledPinLeft, HIGH);
  }
  digitalWrite(ledPinRight, LOW);
  if (displayTimeRemainingLeft == 1) {
    digitalWrite(ledPinLeft, LOW);
    displayTimeRemainingRight = 1000;
  }
  displayTimeRemainingLeft -= 1;

  
 } else if (displayTimeRemainingRight > 0) { Right is being displayed right now if displayTimeRemainingRight is greater than 0.
  currentNumberToDisplay = currentNumberRight;
  if (currentNumberRight >= maximumScore) { //Check if right score is greater than or equal to maximum score.
    if (displayTimeRemainingRight%200>100) { //Cause LED to blink.
      digitalWrite(ledPinRight, HIGH);
    } else {
      digitalWrite(ledPinRight, LOW);
    }
  } else {digitalWrite(ledPinRight, HIGH);
  }
  digitalWrite(ledPinLeft, LOW);
  if (displayTimeRemainingRight == 1) {
    digitalWrite(ledPinRight, LOW);
    displayTimeRemainingLeft = 1000;
  }
  displayTimeRemainingRight -= 1;

  
 } else if (displayTimeRemainingMaximum > 0) { // Middle max score is being displayed right now if displayTimeRemainingMaximum is greater than 0.
  currentNumberToDisplay = maximumScore;
  digitalWrite(ledPinLeft, HIGH);
  digitalWrite(ledPinRight, HIGH);
  if (displayTimeRemainingMaximum == 1) {
    digitalWrite(ledPinLeft, LOW);
    digitalWrite(ledPinRight, LOW);
    displayTimeRemainingLeft = 1000;
  }
  displayTimeRemainingMaximum -= 1;
  
 } else {}
 delay(1);

 buttonStateOldLeft = buttonStateLeft; //define old left button state before going back to the beginning of the loop
 buttonStateOldRight = buttonStateRight; //define old right button state before going back to the beginning of the loop
 buttonStateOldMiddle = buttonStateMiddle; //define old middle button state before going back to the beginning of the loop
 timeSinceLastScoreDecrease += 1; // increase time since last score decrease as this variable is checking if the score was decreased recently before displaying maximum score

 int currentOnesPlace = returnOnes(currentNumberToDisplay); //User defined funciton, return ones place value from a number.
 int currentTensPlace = returnTens(currentNumberToDisplay); //User defined function, return tens place value from a number.
 
 if (currentTensPlace == 0) { // set NeoPixel color depending on the tens value.
  strip.setPixelColor(0, 0, 0, 0);
 } else if (currentTensPlace == 1) {
  strip.setPixelColor(0, 255, 0, 0);
 } else if (currentTensPlace == 2) {
  strip.setPixelColor(0, 255, 127, 0);
 } else if (currentTensPlace == 3) {
  strip.setPixelColor(0, 255, 255, 0);
 } else if (currentTensPlace == 4) {
  strip.setPixelColor(0, 0, 255, 0);
 } else if (currentTensPlace == 5) {
  strip.setPixelColor(0, 0, 0, 255);
 } else if (currentTensPlace == 6) {
  strip.setPixelColor(0, 46, 43, 95);
 } else if (currentTensPlace == 7) {
  strip.setPixelColor(0, 139, 0, 255);
 }
 strip.show(); // strip.show() updates the LED
 
 
 sevseg.setNumber(currentOnesPlace); // updating the display
 sevseg.refreshDisplay(); // updating display so new number is shown
 
}

int returnTens(int inputNumber){ // Take an integer number then return the tens value.
  int tens = 0;
  if ((inputNumber >9) && inputNumber <= 79){
    tens = abs(floor(inputNumber/10));
  } else {
    tens = 0;
  }
  int returnItem = tens;
  return returnItem;
  }

int returnOnes(int inputNumber){ // Take an integer number then return the ones value.
  int ones = 0;
  if ((abs(inputNumber >9)) && abs(inputNumber <= 79)){
    ones = abs(inputNumber%10);
  } else if (inputNumber <10) {
    ones = abs(inputNumber);
  } else {
    ones = 0;
  }
  int returnItem = ones;
  return returnItem;
  }

String getValue(String data, char separator, int index) 
// Essentially a split function, code is written by Odis Harkins from Stack Overflow
// https://arduino.stackexchange.com/a/1237
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
