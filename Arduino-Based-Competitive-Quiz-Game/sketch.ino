#include <Wire.h> 
#include <hd44780.h> 
#include <hd44780ioClass/hd44780_I2Cexp.h> 
 
hd44780_I2Cexp lcd;  // Create LCD object 
 
#define LCD_COLS 20 
#define LCD_ROWS 4 
 
// === Pin and score setup === 
const int buttons[] = { 2, 3, 4, 5 }; 
int scores[4] = { 0, 0, 0, 0 }; 
 
// === Questions and Answers === 
const char* questions[5] = { 
  // 012345678901234567890 
  "Inventor C language?", 
  "Select valid the one", 
  "Store positive nums?", 
  "Linux Kernal written?", 
  "Linux Kernal C STD?" 
}; 
 
const char* options_q1[4] = { "A) Ken Thompson", "B) Charles Babbage", "C) James Gosling", "D) Bjarne Stroustrup" }; 
const char* options_q2[4] = { "A) stringprint()", "B) sprintf()", "C) formatstr()", "D) stringformatter()" }; 
const char* options_q3[4] = { "A) unsigned short", "B) unsigned int", "C) unsigned long", "D) All are valid" }; 
const char* options_q4[4] = { "A) C", "B) C++", "C) Java", "D) Python" }; 
const char* options_q5[4] = { "A) C99", "B) C11", "C) C18", "D) C23" }; 
 
const char** allOptions[5] = { 
  options_q1, options_q2, options_q3, options_q4, options_q5 
}; 
 
const char correctAnswers[5] = { 'A', 'B', 'D', 'A', 'A' }; 
 
void showIntro() { 
  lcd.clear(); 
  lcd.setCursor(5, 1); 
  lcd.print("WELCOME TO"); 
  lcd.setCursor(2, 2); 
  lcd.print("ARDUINO QUIZ GAME"); 
  delay(3000); 
 
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("A competitive game"); 
  lcd.setCursor(0, 1); 
  lcd.print("Played between"); 
  lcd.setCursor(0, 2); 
  lcd.print("4 Players!"); 
  delay(3000); 
 
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("A question pops up"); 
  lcd.setCursor(0, 1); 
  lcd.print("4 Players Race to"); 
  lcd.setCursor(0, 2); 
  lcd.print("Get selected for"); 
  lcd.setCursor(0, 3); 
  lcd.print("Answering"); 
  delay(3000); 
 
  lcd.clear(); 
  lcd.setCursor(3, 1); 
  lcd.print("ARE YOU READY!"); 
  lcd.setCursor(6, 2); 
  lcd.print("GO GO GO!"); 
  delay(1000); 
} 
 
// === Setup === 
void setup() { 
  Serial.begin(9600); 
 
  // Initialize LCD 
  lcd.begin(LCD_COLS, LCD_ROWS); 
  lcd.backlight();  // Turn on backlight 
 
  showIntro(); 
 
  // Set up buttons 
  for (int i = 0; i < 4; i++) { 
    pinMode(buttons[i], INPUT_PULLUP); 
  } 
 
  pinMode(6, OUTPUT); 
} 
 
// === Main Game Loop === 
void loop() { 
  for (int q = 0; q < 5; q++) { 
    displayQuestion(q);                 // Display the question 
    int player = waitForBuzzIn();       // Wait for buzz-in 
    displayOptions(q);                  // Display options 
    char answer = getPlayerAnswer();    // Get the player's answer 
    evaluateAnswer(q, player, answer);  // Evaluate the answer 
 
    delay(2000);  // Small delay before showing next question 
  } 
 
  showScores();  // Show final scores 
  while (true);  // Halt the game after all questions 
} 
 
// === Helper Functions === 
 
// Function to display the question 
void displayQuestion(int index) { 
  lcd.clear();  // Clear screen 
  lcd.setCursor(0, 0); 
  lcd.print(questions[index]); 
 
  // Leave space for player buzz-in 
  lcd.setCursor(0, 2);  // Prepare for buzzing in text (bottom two lines) 
  lcd.print("Waiting for player"); 
 
  delay(1000);  // Wait for 1 second to show the question clearly 
} 
 
// Function to wait for a player to buzz in 
int waitForBuzzIn() { 
  while (true) { 
    for (int i = 0; i < 4; i++) { 
      if (digitalRead(buttons[i]) == LOW) { 
        // Display who buzzed in on bottom lines without clearing question 
        lcd.setCursor(0, 2);  // Set cursor to bottom lines 
        lcd.print("Player "); 
        lcd.print(i + 1); 
        lcd.print(" buzzed in!"); 
        digitalWrite(6, HIGH); 
        delay(1000);  // Wait for 1 second to show the player's buzz 
        digitalWrite(6, LOW); 
        return i;  // Return the player who buzzed in 
      } 
    } 
  } 
} 
 
// Function to display the options 
void displayOptions(int index) { 
  lcd.clear();  // Clear screen 
 
  for (int i = 0; i < 4; i++) { 
    lcd.setCursor(0, i);              // Set cursor to line i 
    lcd.print(allOptions[index][i]);  // Display option 
    delay(1000);                      // Delay before showing the next option 
  } 
} 
 
// Function to get the player's answer 
char getPlayerAnswer() { 
  while (true) { 
    for (int i = 0; i < 4; i++) { 
      if (digitalRead(buttons[i]) == LOW) { 
        lcd.clear(); 
        char answer = 'A' + i; 
        lcd.setCursor(0, 0); 
        lcd.print("You selected: "); 
        lcd.print(answer); 
        delay(1000);  // Wait a bit before returning answer 
        return answer; 
      } 
    } 
  } 
} 
 
// Function to evaluate the player's answer and update scores 
void evaluateAnswer(int questionIndex, int playerIndex, char selectedAnswer) { 
  lcd.setCursor(0, 2); 
 
  if (selectedAnswer == correctAnswers[questionIndex]) { 
    lcd.print("Correct!"); 
    scores[playerIndex]++; 
  } else { 
    lcd.print("Incorrect!"); 
    lcd.setCursor(0, 3); 
    lcd.print("Correct: "); 
    lcd.print(correctAnswers[questionIndex]); 
  } 
 
  delay(2000);  // Wait for 2 seconds before next question 
} 
 
// Function to display final scores 
void showScores() { 
  lcd.clear(); 
  for (int i = 0; i < 4; i++) { 
    lcd.setCursor(0, i); 
    lcd.print("Player "); 
    lcd.print(i + 1); 
    lcd.print(": "); 
    lcd.print(scores[i]); 
  } 
  while (true); 
}