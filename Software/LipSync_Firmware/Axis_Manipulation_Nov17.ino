/*
 * 11/17/17: Earliest stage of axis manipulation - a user is asked to move the joystick "Up" as high as possible,
 *           and an angle is returned to be used for rotating the axis.
 */

const float pi = 3.14159;
const int MAX_RADIUS = 1023;
float theta;
float xh_stretch, xl_stretch, yh_stretch, yl_stretch;
float magnitude_up, magnitude_right, magnitude_down, magnitude_left;
int x_UP, y_UP, x_RIGHT, y_RIGHT, x_DOWN, y_DOWN, x_LEFT, y_LEFT;

void Axis_Calibration(void);
void theta_calculation(int x_UP, int y_UP);
void stretch_calculation(int x_UP, int x_RIGHT, int x_DOWN, int x_LEFT, int y_UP, int y_RIGHT, int y_DOWN, int y_LEFT);

void setup() {
  
  Serial.begin(9600);
  /*pinMode(PRESSURE_CURSOR, INPUT);                // 
  pinMode(X_DIR_HIGH, INPUT);                     // 
  pinMode(X_DIR_LOW, INPUT);                      // joystick locations
  pinMode(Y_DIR_HIGH, INPUT);                     // 
  pinMode(Y_DIR_LOW, INPUT); */                     // 

}

void loop(){
  x_UP = 100;
  x_RIGHT = 800;
  x_DOWN = -50;
  x_LEFT = -950;
  y_UP = 900;
  y_RIGHT = 0;
  y_DOWN = -900;
  y_LEFT = -100;

  theta_calculation(x_UP, y_UP);
  stretch_calculation(x_UP, x_RIGHT, x_DOWN, x_LEFT, y_UP, y_RIGHT, y_DOWN, y_LEFT);

  Serial.println(theta);
}

/*void Axis_Calibration(void){    //gets the x/y coordinates from the furthest "up/right/down/left" positions

  Serial.println("Prepare for axis calibration!");
  Serial.println(" ");
  blink(4, 300, 3);

  Serial.println("Move the mouthpiece to the furthest comfortable 'UP' position and hold it there until the LED turns SOLID RED, then release the mouthpiece.");
  blink(6, 500, 1);
  if (analogRead(Y_DIR_HIGH) > 0){
    y_UP = analogRead(Y_DIR_HIGH);
  }
  else
    y_UP = analogRead(Y_DIR_LOW);
    
  if (analogRead(X_DIR_HIGH) > 0){
    x_UP = analogRead(X_DIR_HIGH);
  }
  else
    x_UP = analogRead(X_DIR_LOW);

  Serial.println("Next, move the mouthpiece to the furthest comfortable 'RIGHT' position and hold it there until the LED turns SOLID RED, then release the mouthpiece.");
  blink(6, 500, 1);
  if (analogRead(Y_DIR_HIGH) > 0){
    y_RIGHT = analogRead(Y_DIR_HIGH);
  }
  else
    y_RIGHT = analogRead(Y_DIR_LOW);
    
  if (analogRead(X_DIR_HIGH) > 0){
    x_RIGHT = analogRead(X_DIR_HIGH);
  }
  else
    x_RIGHT = analogRead(X_DIR_LOW);

  Serial.println("Next, move the mouthpiece to the furthest comfortable 'DOWN' position and hold it there until the LED turns SOLID RED, then release the mouthpiece.");
  blink(6, 500, 1);
  if (analogRead(Y_DIR_HIGH) > 0){
    y_DOWN = analogRead(Y_DIR_HIGH);
  }
  else
    y_DOWN = analogRead(Y_DIR_LOW);
    
  if (analogRead(X_DIR_HIGH) > 0){
    x_DOWN = analogRead(X_DIR_HIGH);
  }
  else
    x_DOWN = analogRead(X_DIR_LOW);

  Serial.println("Next, move the mouthpiece to the furthest comfortable 'LEFT' position and hold it there until the LED turns SOLID RED, then release the mouthpiece.");
  blink(6, 500, 1);
  if (analogRead(Y_DIR_HIGH) > 0){
    y_LEFT = analogRead(Y_DIR_HIGH);
  }
  else
    y_LEFT = analogRead(Y_DIR_LOW);
    
  if (analogRead(X_DIR_HIGH) > 0){
    x_LEFT = analogRead(X_DIR_HIGH);
  }
  else
    x_LEFT = analogRead(X_DIR_LOW);
}*/

void theta_calculation(int x_UP, int y_UP) {                             //calculates angle 'theta' to rotate axis by, based on the comfortable 'UP' position

  if(((double)y_UP > 0) && ((double)x_UP < 0)){         //quadrant 2
    theta = atan((double)x_UP/(double)y_UP);
  }
  
  else if(((double)y_UP < 0) && ((double)x_UP < 0)){    //quadrant 3
    theta = atan((double)y_UP/(double)x_UP) + pi/2;
  }

  else if(((double)y_UP < 0) && ((double)x_UP > 0)){    //quadrant 4
    theta = atan((double)y_UP/(double)x_UP) + pi;
  }

  else if(((double)y_UP > 0) && ((double)x_UP > 0)){    //quadrant 1
    theta = 2*pi - atan((double)x_UP/(double)y_UP);
  }


}

void stretch_calculation(int x_UP, int x_RIGHT, int x_DOWN, int x_LEFT, int y_UP, int y_RIGHT, int y_DOWN, int y_LEFT){
  double magnitude_up_funct, magnitude_right_funct, magnitude_down_funct, magnitude_left_funct, yh_stretch_funct, xh_stretch_funct, yl_stretch_funct, xl_stretch_funct;

  magnitude_up_funct = sqrt(pow(x_UP, 2) + pow(y_UP, 2));
  magnitude_right_funct = sqrt(pow(x_RIGHT,2) + pow(y_RIGHT,2));
  magnitude_down_funct = sqrt(pow(x_DOWN,2) + pow(y_DOWN,2));
  magnitude_left_funct = sqrt(pow(x_LEFT,2) + pow(y_LEFT,2));

  yh_stretch_funct = MAX_RADIUS/magnitude_up_funct;
  xh_stretch_funct = MAX_RADIUS/magnitude_right_funct;
  yl_stretch_funct = MAX_RADIUS/magnitude_down_funct;
  xl_stretch_funct = MAX_RADIUS/magnitude_left_funct;
  
}



