//define constants etc.
#define size 5

#define LED_LOSE 13
#define LEDR 12
#define LEDY 11
#define LEDB 10
#define LEDG 9
#define LED_WIN 8

#define pb_red 7
#define pb_yellow 6
#define pb_blue 5
#define pb_green 4

//initialization for global variables
int RandomSeriesArray[size]; //The array that will hold the random numbers of sequence of four colour LEDS
int Pressed_Button = 8; //The button that is pressed (it's initialized as 8 because 8 means 'not pressed'.
int NumberOfRounds = 5; //Number of rounds
int size_sequence = 0; //for each round of sequence of random array
bool Start = false; //for start of the round 
bool AlreadyClicked = false; //use for 'early pressed buttons'
int data = 0; //use for 'early pressed buttons'
unsigned long measure_time = 0; //A variable that will measure time 
unsigned long TIMEOUT = 240000; //Timeout for the game, it'll turn off after 4 minutes

void setup() {
  //configure pin modes (inputs, outputs) etc.
  
  //Output: (Six LEDS, one for losing, one for winning and four for user to memorize)
  pinMode(LED_LOSE, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LED_WIN, OUTPUT);
  
  //Input: (Four Button that will correspond to the four colour LEDs (LEDR, LEDY, LEDB, LEDG)
  pinMode(pb_red, INPUT);
  pinMode(pb_yellow, INPUT);
  pinMode(pb_blue, INPUT);
  pinMode(pb_green, INPUT);
  
  Serial.begin(9600);
  //This is to show when the game starts and restarts within the terminal
}

//This function goes through all the LEDs for colour usage and turns them off to LOW
void TurnOffEverything(void) {
  for(int i = 9; i < 14; i++) {
    digitalWrite(i, LOW);
  }
}

/* This function is for when the user presses a wrong button where 
it flashes the LED_LOSE three times with delay of 200ms between */
void YouLose(void) {
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_LOSE, HIGH);
    delay(200);
    digitalWrite(LED_LOSE, LOW);
    delay(200);
  }
}

/* This function is for when the user presses the right buttons through all the 
rounds and it flashes the LED_WIN three times with delay of 200ms between */
void YouWin(void) {
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_WIN, HIGH);
    delay(200);
    digitalWrite(LED_WIN, LOW);
    delay(200);
  }
}

/* This function is for making a random sequence for the user 
to play with the lights
*/
void RandomSequence(void) {
  
  //To initialize (seed) the random number generator
  randomSeed(analogRead(A0));
  
  for(int i = 0; i < NumberOfRounds; i++) {
    RandomSeriesArray[i] = random(9,13);
    /* This for loop goes through the random numbers of the colour LEDs
    (red, yellow, blue, green) and assigns it to the array of RandomSeriesArray[] */
    
  }
}

// This function checks if the button is pressed, if it hasn't it just passes the function with no code
int ButtonCheck(void) {
  if (digitalRead(pb_red) == HIGH) {
    /* And when it's verified that the button is checked, it goes through a while loop that waits until 
    the user takes a finger off from the button. and When the button is read OFF or LOW, it returns that
    button's corresponding number to the LED lights. So it can read off which button is pressed, and waits 
    until the user has lift it up. It also turns on the corresponding light which the user presses on also, 
    so it's easier to track which button the user has pressed. */
    
    while(true) {
      digitalWrite(LEDR, HIGH); 
      if(digitalRead(pb_red) == LOW) {
        digitalWrite(LEDR, LOW);
        measure_time = millis();
        //measure time here when he lifts his button up and starts counting. This is true for all buttons.
        
        return 12;
      }
    }
  } else if (digitalRead(pb_yellow) == HIGH) {
      while(true) {
        digitalWrite(LEDY, HIGH); 
        if(digitalRead(pb_yellow) == LOW) {
          digitalWrite(LEDY, LOW); 
          measure_time = millis();
          return 11;
        }
      }
  } else if (digitalRead(pb_blue) == HIGH) {
      while(true) {
        digitalWrite(LEDB, HIGH); 
      if(digitalRead(pb_blue) == LOW) {
        digitalWrite(LEDB, LOW); 
        measure_time = millis();
        return 10;
      }
    }
  } else if (digitalRead(pb_green) == HIGH) {
    while(true) {
      digitalWrite(LEDG, HIGH); 
      if(digitalRead(pb_green) == LOW) {
        digitalWrite(LEDG, LOW); 
        measure_time = millis();
        return 9;
      }
    }
  } else {
    return 8;
    /* But when there has been no button pressed initially, it just returns 8 which isn't a number that corresponds to any 
    of the LED colour lights. Down in the code, there is an while loop where it waits until a number isn't 8, meaning it 
    waits until the ButtonCheck() function returns one of the number that corresponds to one of the LED colour lights. */
  }
}

/* This function goes through while one of the LED lights turn on or turn off unlike the function ButtonCheck() checks if 
the button is pressed after the LEDs have been showed. So this function is mainly for responding to the 'early press' for 
buttons. (size_sequence will be explained later down in the code) */
void ChecksThrough_HIGH_OFF(void) {
  
  for(int i = 0; i <= size_sequence; i++) {
    Pressed_Button = 8;
    //it resets the pressed button to be 8, meaning a button has not been pressed, so it doesn't use the past button value through the if then statements 
    
    Pressed_Button = ButtonCheck();
    /* Here I check if the user has pressed button and returns a value of the corresponding LED lights that have been 
    pressed through the for loop. */

    if(Pressed_Button != 8) {
      /* When pressed Button is not returned as 8 from ButtonCheck() function (which i explained earlier), it goes through 
      the code to check if the pressed button is the correct one. */
      
      if(Pressed_Button == RandomSeriesArray[data]) {
        /* Here it checks if the pressed button is the correct button that corresponds to the lights that were turned HIGH for
        the user to see the sequence. If it matches, it increments the variable data, which is initialized as 0. */
        data++;
        break;
        
      } else {
        YouLose();
        //When you lose, it goes through the lose sequence, will be explained later with its function
        
        Start = false;
        //sets boolean start = false so the game restarts
        
        AlreadyClicked == true;
        break;
        /* sets boolean AlreadyClicked to true so it passes through the loop() function 
        without going through it so it restarts after breaking out of the loop. */
      }
    }
    if (size_sequence == i) {
      AlreadyClicked == true;
      /* This will be explained down in the code again but, here it checks the boolean AlreadyClicked to true so the 
        code for the button that is pressed after the LED lights have turned on and off doesn't have to go through the code
        (because it already showed the results from here for all buttons in the size_sequence, if not all of the buttons are pressed, 
        the remaining button(s) that have to be pressed will be moved on to the next function loop() with the variable data that stores how 
        many steps it has gone through) */
    }
  }
}

//main program loop
void loop() {  
  if(Start == false) {
    int size_sequence = 0;
    //initialize size_sequence to be 0 so it starts with round one every time the game starts/restarts
    
    RandomSequence();
    //initialize the random sequence array
    
    Start = true;
    //make boolean Start = true so it doesn't go through it again unless the user lost
    
    Serial.println("Game Start");
    //Prints out "Game Start" to the terminal as the game keeps starts or restarts
    
    for(int i = 0; i < 3; i++) {
      //repeats this for loop 3 times
      
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDY, HIGH);
      digitalWrite(LEDB, HIGH);
      digitalWrite(LEDG, HIGH);
      delay(100);
      //turn on all the colour LEDs for 100ms
      
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDY, LOW);
      digitalWrite(LEDB, LOW);
      digitalWrite(LEDG, LOW);
      delay(100);
      //turn off all the colour LEDs for 100ms

    }
    
    delay(600);
    //delay for a user to distinguish the colour that starts with
    
  }
  AlreadyClicked = false;
  //Make sure this resets every round, or else, it'll keep skipping the main loop() function for buttons pressed. 
  data = 0;
  //initialize data to be 0, will be changed through the for next for loops if the buttons have been pressed during the LEDs turned on or turning off
  
  for(int i = 0; i <= size_sequence; i++) {
    //repeat this by the number of size_sequence (number of rounds)
    
    for(int j = 0; j < 50; j++) {
      
      if(Start == true) {
        digitalWrite(RandomSeriesArray[i], HIGH);
        //turn on the first random colour of the array
      
        ChecksThrough_HIGH_OFF();
        //Check through if the button was pressed through the colours being turned on or off
      
        delay(10);
        //this repeats for 50 times so it goes through 10ms x 50 = 500ms and it checks between the delays
      } else {
        break;
        //If the game is already lost, no need to keep showing the sequence
      }
      
      if(data > 0) {
        break;
      }
    }
    
    for(int i = 0; i < 40; i++) {
      TurnOffEverything();
      //Make sure to turn off every colour LEDs now
      
      ChecksThrough_HIGH_OFF();
      //Check through if the button was pressed through the colours being turned on or off

      
      delay(10);
      //this repeats for 40 times so it goes through 10ms x 40 = 400ms and it checks between the delays
      
      if(Start == false) {
        break;
      }
      
      if(data > 0) {
        break;
      }
      /* if some button were pressed, break out of the loop so the person can't just be clicking the colours that show up, 
      if the user distrupts the sequence, turn if off so the user guesses the next colours of sequence */
    } 
    if(data > 0) {
        break;
    }
  }
  
  //If the correct sequence were all clicked, it turns 
  if(AlreadyClicked == false) {
    for(int i = data; i <= size_sequence; i++) {
      //run through by the number of rounds
      
      measure_time = millis();
      //Start measuring time from here
        
      while(true) {
        //go through this loop until one of the if then statements goes true (meaning until when the user either presses a button or goes timeout [4min])
        
        Pressed_Button = ButtonCheck();
        //Check button throughout the loop
      
        if(Pressed_Button != 8) {
          //if pressed button is pressed, go through the next if then statement
          
          if(Pressed_Button == RandomSeriesArray[i]) {
            
              delay(250); //This delay lets the arudino wait for the next button
              break; //then break out of this while loop so it goes to the next sequence in the for loop
          } else {
            
            YouLose(); //Go through the lose sequence
            Start = false; //make it so it restarts
            break; //Go out of the while loop
          }
        }
        if (Start == false) {
          
          break; //Go out of the for loop when lost 
        }
        if (millis() - measure_time > TIMEOUT) {
          
          YouLose();
          Start = false;
          break;
          //Go out of the for loop when time out 
        }
      }
    }
  }
  
  if (Start == true) {
    
    size_sequence++; //increment size_sequence by 1 to go through the next round
  } else {
    size_sequence = 0; //or else, set it to 0 so it restarts from round 1(this is when lost)
  }
  
  if (size_sequence == NumberOfRounds) {
    
    YouWin(); //if you reached the last round, make the user know he won with the YouWin() function
    Start = false; //Let the game restart
    size_sequence = 0; //set it to 0 so it restarts from round 1
  }
  
  delay(500);
  //some delay for the user to distinguish that a new game has started
}
