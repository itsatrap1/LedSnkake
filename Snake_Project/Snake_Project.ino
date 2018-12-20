#include "LedControl.h"
#include <LiquidCrystal.h>
#include <string.h>

LedControl lc = LedControl(12, 11, 10, 1);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define JOY_X A0
#define JOY_Y A1
#define SW_PIN 13
#define V0_PIN 9
#define INIT_GAME_DELAY 200
#define WELCOME_MSG "Welcome to LedSnake !"
#define NEW_GAME_MSG " Press to play"

int sw = 0;
int snake_length;
int snake_x[64];
int snake_y[64];
int food_pos[2];
int pos_x = 0, pos_y = 0;
int score;
int high_score = 0;
int game_delay;
int x = 0;
bool an_g = false;
bool game_state = true; // False if game is over & true if game is not.
String move_direction;
const char welcome_message[21] = WELCOME_MSG;
const char new_game_message[14] = NEW_GAME_MSG;
int x_axis_reading = 0;

void initialize_game() {

  for ( int i = 0 ; i < 64 ; i++) { // Sets all the positions outside the matrix.

    snake_x[i] = -1;
    snake_y[i] = -1;
  }

  snake_length = 1;
  snake_x[0] = 5;
  snake_y[0] = 3;

  score = 0;
  game_state = false;
  game_delay = INIT_GAME_DELAY;
  spawn_food();
  lcd.setCursor(0, 0);
  //lcd.print("Score :  0");

}
void game_over() {

  game_state = false;
  game_over_msg();
  initialize_game();
}

void movement(String heading) {

  for (int i = snake_length - 1; i > 0; i--) {

    snake_x[i] = snake_x[i - 1];
    snake_y[i] = snake_y[i - 1];
  }

  if (heading == "up") {

    if (snake_y[0] == 0) {
      snake_y[0] = 7;
    }
    else {
      snake_y[0]--;
    }
  }

  if (heading == "down") {

    if (snake_y[0] == 7) {
      snake_y[0] = 0;
    }
    else {
      snake_y[0]++;
    }
  }

  if (heading == "right") {

    if (snake_x[0] == 7) {
      snake_x[0] = 0;
    }
    else {
      snake_x[0]++;
    }
  }

  if (heading == "left") {

    if (snake_x[0] == 0) {
      snake_x[0] = 7;
    }
    else {
      snake_x[0]--;
    }
  }
}
void draw_snake(void) {  // Draw me like one of your french snakes.

  for (int i = 0; i < snake_length; i++) {

    lc.setLed(0, snake_y[i], snake_x[i], 1);
  }
}

void spawn_food(void) {

  int newfood_x = random(0, 8);
  int newfood_y = random(0, 8);

  while (food_avoids_snake(newfood_x, newfood_y)) {

    newfood_x = random(0, 8);
    newfood_y = random(0, 8);
  }

  food_pos[0] = newfood_x;
  food_pos[1] = newfood_y;
}

void draw_food() {

  lc.setLed(0, food_pos[0], food_pos[1], 1);
  delay(60);                                 // Creates a blinking motion so that you can see the food better.
  lc.setLed(0, food_pos[0], food_pos[1], 0);

}

bool food_avoids_snake(int x, int y) {

  for (int i = 0;  i < snake_length ; i++)

    if ((snake_y[i] == x && snake_x[i] == y)) {
      return true;
    }
  return false;
}

void food_is_eaten(void) {

  if (snake_y[0] == food_pos[0] && snake_x[0] == food_pos[1]) {

    score += 10;
    if (score > high_score) {

      high_score = score;
    }
    snake_length++;
    spawn_food();
    decrease_delay();
    print_score(score);
  }
}

void snake_ate_itself(void) {

  for (int i = 1; i < snake_length; i++) {

    if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {

      game_over();
      an_g = true;
    }
  }

}

int treat_value(int data) {

  return (data * 9 / 1024);
}

void decrease_delay(void) { // Speeds up the game.

  if (game_delay != 10) {

    game_delay -= 10;
  }
}

void print_score(int curr_score) {

  lcd.clear();
  lcd.print("Score :");
  lcd.setCursor(8, 0);
  lcd.print(curr_score);
  lcd.setCursor(0, 0);
  delay(100);
}

void lcd_welcome_msg(void) {

  int count = 0;
  lcd.setCursor(15, 0);
  lcd.autoscroll();
  for (int s = 0; s < strlen(welcome_message); s++) {

    lcd.print(welcome_message[s]);
    count++;
    delay(230);

    if (count == 11) {
      lcd.noAutoscroll();
      lcd.setCursor(16, 1);
    }
  }
  delay(2000);
  lcd.clear();

}

void lcd_new_game(void) {

  int count = 0;
  lcd.clear();
  lcd.setCursor(14, 0);
  lcd.autoscroll();

  for (int s = 0; s < strlen(new_game_message); s++) {

    lcd.print(new_game_message[s]);
    count++;
    delay(230);

    if (count == 13) {
      lcd.noAutoscroll();

    }
  }

}

void meniu(void) {

  lcd.clear();
  lcd_welcome_msg();
  lcd_new_game();

}

void game_over_msg(void) {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Game Over! :(");
  delay(2000);
  lcd.clear();
  lcd.print("Your score: ");
  lcd.setCursor(13, 0);
  lcd.print(score);
  lcd.setCursor(0 , 1);
  lcd.print("HighScore :");
  lcd.setCursor(13, 1);
  lcd.print(high_score);


}

void another_game(void) {

  // lcd.clear(
  lcd.clear();
  lcd.print("Press to play"); // hit em with the rhyme.
  lcd.setCursor(0, 1);
  lcd.print("Go -> for no");
  lcd.clear();

  int button = digitalRead(SW_PIN);
  int right  = treat_value(analogRead(JOY_X));

  if (button != 1) {
    initialize_game();
  }

  if (right > 5) {

    end_game();
  }
} * /
void setup() {

  // Driver & Matrix setup :

  pinMode(SW_PIN, INPUT_PULLUP);
  digitalWrite(SW_PIN, HIGH);

  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  Serial.begin(9600);

  // Lcd setup :

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  pinMode(V0_PIN, OUTPUT);
  analogWrite(V0_PIN, 70);

  meniu();
  initialize_game();
  //  an_g = false;
}

void loop() {
  int y = digitalRead(SW_PIN);
  if (y != 1) {
    game_state = true;
  }
  if (game_state) {

    lc.clearDisplay(0);

    pos_x = treat_value(analogRead(JOY_X));
    pos_y = treat_value(analogRead(JOY_Y));

    if (pos_x > 5 && move_direction != "left") {

      move_direction = "right";
    }

    else if (pos_x < 3 && move_direction != "right") {

      move_direction = "left";
    }

    else if (pos_y > 5 && move_direction != "up") {

      move_direction = "down";
    }

    else if (pos_y < 3 && move_direction != "down") {

      move_direction = "up";
    }

    movement(move_direction);
    food_is_eaten();
    draw_snake();
    draw_food();
    snake_ate_itself();
    delay(game_delay);
  }
}
