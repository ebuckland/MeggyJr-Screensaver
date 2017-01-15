#include <MeggyJrSimple.h>
#include <stdlib.h> 
#include <time.h>

// total number of boxes, reccomended less than 5
#define NUM_BOXES 3

struct boxkit 
{
  double yvelo[NUM_BOXES];
  double xvelo[NUM_BOXES];
  int color[NUM_BOXES];
  double boxX[NUM_BOXES];
  double boxY[NUM_BOXES];
};

struct boxkit boxes;

void setup() {
  // sets up console printing and meggy jr functions
  Serial.begin(9600);
  MeggyJrSimpleSetup();

  // sets up the bouncing boxes struct
  setupBoxesStruct();
}

void setupBoxesStruct(void) {
  for (int i = 0; i < NUM_BOXES; i++) 
  {
    // calculate different initial movement values or colors for each box
    boxes.yvelo[i] = (i == 0 ? .2 : boxes.xvelo[i-1] - .3);
    boxes.xvelo[i] = (i == 0 ? .15 : boxes.xvelo[i-1] + .08);
    boxes.color[i] = (i == 0 ? 1 : boxes.color[i-1] + 5);
    if (boxes.color[i] > 14) {boxes.color[i] = boxes.color[0] + 2;}
    boxes.boxX[i] = (i == 0 ? 2 : boxes.boxX[i-1] + 3);
    if (boxes.boxX[i] > 7) {boxes.boxX[i] -= 5;} 
    boxes.boxY[i] = 8 - i - 1;

    // prints out the values for each box for debugging
    Serial.print("Box ");Serial.print(i);
    Serial.print(" - y velo: ");Serial.print(boxes.yvelo[i]);
    Serial.print("  x velo: ");Serial.print(boxes.xvelo[i]);
    Serial.print("  color: ");Serial.print(boxes.color[i]);
    Serial.print("  box x: ");Serial.print(boxes.boxX[i]);
    Serial.print("  box y: ");Serial.println(boxes.boxX[i]);
  }
}

void updateBoxes(void) 
{
  // updates each box's movement
  for (int i = 0; i < NUM_BOXES; i++) 
  {
    // moves the box with the velocity it has
    boxes.boxY[i] += boxes.yvelo[i];
    boxes.boxX[i] += boxes.xvelo[i];

    // checks to see if the box has hit the bottom
    if (boxes.boxY[i] <= 1) 
    {
      boxes.yvelo[i] = -1 * boxes.yvelo[i];
      boxes.color[i] = (boxes.color[i] == 15? 1: boxes.color[i] + 1);
    } else {
      boxes.yvelo[i] -= .15;
    }
    
    // checks to see if the box has hit the right
    if (boxes.boxX[i] >= 6 && boxes.xvelo[i] > 0) 
    {
      boxes.xvelo[i] = -1 * boxes.xvelo[i];
      boxes.boxX[i] = 6.7;
    }

    // checks to see if the box has hit the left
    if (boxes.boxX[i] <= 1 && boxes.xvelo[i] < 0) 
    {
      boxes.xvelo[i] = -1 * boxes.xvelo[i];
      boxes.boxX[i] = .3;
    }
  }
}

void drawBoxes(void) {
  // draws each box in the struct
  for (int i = 0; i < NUM_BOXES; i++) 
  {
    DrawPx((int)boxes.boxX[i],(int)boxes.boxY[i],boxes.color[i]);
    DrawPx((int)boxes.boxX[i]+1,(int)boxes.boxY[i],boxes.color[i]);
    DrawPx((int)boxes.boxX[i],(int)boxes.boxY[i]-1,boxes.color[i]);
    DrawPx((int)boxes.boxX[i]+1,(int)boxes.boxY[i]-1,boxes.color[i]);
  }
}

void loop() 
{
  // updates the location of the box
  updateBoxes();

  // clears the picture
  ClearSlate();

  // draws the new boxes
  drawBoxes();

  // displays the drawn boxes
  DisplaySlate(); 

  //delay to decrease unneccesarily high framerate
  delay(90);
}
