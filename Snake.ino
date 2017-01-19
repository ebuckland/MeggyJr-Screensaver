#include <MeggyJrSimple.h>

struct 
{
  byte x;
  byte y;
  byte grid[8][8];
  byte Length;
} snake;

struct 
{
  byte x;
  byte y;
} apple;

// 0 - up, 1 - right, 2 - down, 3 - left
byte moveDirection = 1;

void setup() 
{
  Serial.begin(9600);
  MeggyJrSimpleSetup();
  // play a nice little turn on tune
  Tone_Start(12282,100);
  delay(100);
  Tone_Start(9282,200);

  // display nice little animation to get the user's focus
  displayAuxAnimation();

  
  // setup snake
  snake.x = 2;
  snake.y = 4;
  snake.Length = 2;
  
  // spawns the apple in a random location
  apple.x = random(8);
  apple.y = random(8);
  
  if ((apple.x == 3 && apple.y == 4) || 
      (apple.x == 4 && apple.y == 4)) 
  {
    apple.x = 2; apple.y = 1;
  }
}

// updates the game
int updateSnake() 
{
  // removes one from each segment of the snake
  for (int i = 0; i < 8; i++) 
  {
    for (int j = 0; j < 8; j++) 
    {
      if (snake.grid[i][j] != 0)
      {
        snake.grid[i][j] --;
      }
    }
  }
  
  // checks to see if there is any collision
  if (moveDirection == 1) 
  {
    if (snake.x == 7 || snake.grid[snake.x + 1][snake.y] != 0) 
    {
      return 0;
    }
    snake.x++;
  }
  else if (moveDirection == 3) 
  {
    if (snake.x == 0 || snake.grid[snake.x - 1][snake.y] != 0) 
    {
      return 0;
    }
    snake.x--;
  }
  else if (moveDirection == 2) 
  {
    if (snake.y == 0 || snake.grid[snake.x][snake.y - 1] != 0) 
    {
      return 0;
    }
    snake.y--;
  }
  else if (moveDirection == 0) 
  {
    if (snake.y == 7 || snake.grid[snake.x][snake.y + 1] != 0) 
    {
      return 0;
    }
    snake.y++;
  }

  if (snake.x == apple.x && snake.y == apple.y) 
  {
    Tone_Start(8282,200);
    do 
    {
      apple.x = random(8);
      apple.y = random(8);
      // repeats until the apple is in a place that is not in the snake
    } while (snake.grid[apple.x][apple.y] != 0);
      for (int i = 0; i < 8; i++) 
      {
        for (int j = 0; j < 8; j++) 
        {
          if (snake.grid[i][j] != 0)
          {
            snake.grid[i][j] ++;
          }
        }
      }
    snake.Length++;
  }
  snake.grid[snake.x][snake.y] = snake.Length;
  return 1;
}

// main loop
void loop() 
{

  // update the game
  if (!updateSnake()) 
  {
    // display game over animation, then stop
    gameOver();
    delay(900000);
  }
  
  // clear the previous frame
  ClearSlate();
  
  // draw the updated gamge
  drawSnake();
  
  // display the slate
  DisplaySlate();

  // check for user input while waiting
  
  unsigned long tempMillis = millis();
  while (tempMillis + 600 > millis()) 
  {
    // check to see if buttons are pressed during the wait time
    CheckButtonsPress();
    if (Button_Up) {
      moveDirection = 0;
    }
    else if (Button_Right)
    {
      moveDirection = 1;
    }
    else if (Button_Down)
    {
      moveDirection = 2;
    }
    else if (Button_Left)
    {
      moveDirection = 3;
    }
  }
}

// draws the snake
void drawSnake() 
{
  // draws the apple
  DrawPx(apple.x,apple.y,Green);
  
  // draws the snake from the snake grid
  for (int i = 0; i < 8; i++) 
  {
    for (int j = 0; j < 8; j++) 
    {
      if (snake.grid[i][j] > 0) 
      {
        DrawPx(i,j,Red);
      }
    }
  }
}

// nice little animation
void displayAuxAnimation()
{
  int displayNum[] = {1,3,6,12,24,48,96,192,128};
  for (int i = 0; i < 2; i ++) {
    for (int pos = 0; pos < 9; pos++) 
    { 
      SetAuxLEDs(displayNum[pos]);
      Serial.println(displayNum[pos]);
      delay(40);
    }
    for (int pos = 8; pos > -1; pos--) 
    { 
      SetAuxLEDs(displayNum[pos]);
      Serial.println(displayNum[pos]);
      delay(40);
    }
  }
  SetAuxLEDs(0);
}

// game over animation, flickers snake while playing sad sound
void gameOver()
{
  Tone_Start(9282,200);
  ClearSlate();
  DisplaySlate();
  delay (100);
  drawSnake();
  DisplaySlate();
  delay (150);
  Tone_Start(12282,200);
  delay (50);
  ClearSlate();
  DisplaySlate();
  delay (100);
  drawSnake();
  DisplaySlate();
  delay (100);
  ClearSlate();
  DisplaySlate();
  // displays the persons score in binary on the auxiliary leds
  SetAuxLEDs(snake.Length -2);
}
