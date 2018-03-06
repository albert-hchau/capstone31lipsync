// ---- PIN ASSIGNMENTS ----
#include <EEPROM.h>
#include <Mouse.h>
#include <math.h>


//***PIN ASSIGNMENTS***//

#define MODE_SELECT 12                            // LipSync Mode Select - USB mode (comm_mode = 0; jumper on) or Bluetooth mode (comm_mode = 1; jumper off) - digital input pin 12 (internally pulled-up)
#define PUSH_BUTTON_UP 10                          // Cursor Control Button 1: UP - digital input pin 8 (internally pulled-up)4
#define PUSH_BUTTON_DOWN 9                        // Cursor Control Button 2: DOWN - digital input pin 7 (internally pulled-up)
#define LED_1 11                                   // LipSync LED Color1 : GREEN - digital output pin 5
#define LED_2 5                                   // LipSync LED Color2 : RED - digital outputpin 4

#define TRANS_CONTROL A3                          // Bluetooth Transistor Control Pin - digital output pin A3
#define PIO4 A4                                   // Bluetooth PIO4 Command Pin - digital output pin A4

#define PRESSURE_CURSOR A4                        // Sip & Puff Pressure Transducer Pin - analog input pin A5
#define X_DIR_HIGH A0                             // X Direction High (Cartesian positive x : right) - analog input pin A0
#define X_DIR_LOW A1                              // X Direction Low (Cartesian negative x : left) - digital output pin A1
#define Y_DIR_HIGH A2                             // Y Direction High (Cartesian positive y : up) - analog input pin A2
#define Y_DIR_LOW A3                             // Y Direction Low (Cartesian negative y : down) - analog input pin A10

int xh, yh, xl, yl; 
float xh_yh, xh_yl, xl_yl, xl_yh;
float x_right, x_left, y_up, y_down;
int default_cursor_speed = 5;
int cursor_speed;
float yh_comp = 1.0;
float yl_comp = 1.0;
float xh_comp = 1.0;
float xl_comp = 1.0;
float xh_max = 756;
float xl_max = 750;
float yh_max = 798;
float yl_max = 746;

float radius_threshold = 50.0;
int cursor_delay = 20;

bool bluetooth_mode;                       // Bluetooth mode toggle, if true then bluetooth mode is initialized

void setup() {
 digitalWrite(11, HIGH);
 delay(500);
 digitalWrite(11, LOW);

 digitalWrite(5, HIGH);
 delay(500);
 digitalWrite(5,LOW);

  bluetooth_mode = false;                         // initialize into wired or bluetooth mode

  pinMode(PUSH_BUTTON_UP, INPUT);                 // initialize the two buttons for mouse speed sensitivity
  pinMode(PUSH_BUTTON_DOWN, INPUT);

  if( (EEPROM.get(0, cursor_speed) < 1) || (EEPROM.get(0, cursor_speed) > 15) ){
    cursor_speed = default_cursor_speed;
    EEPROM.put(0, cursor_speed);
  }
  else{
    EEPROM.get(0, cursor_speed);
  }
    
  Mouse.begin();
  Serial.begin(115200);                           // setting baud rate for serial coms for diagnostic data return from Bluetooth and microcontroller ***MAY REMOVE LATER***
  pinMode(X_DIR_HIGH, INPUT);                     // redefine the pins when all has been finalized
  pinMode(X_DIR_LOW, INPUT);                      // ditto above
  pinMode(Y_DIR_HIGH, INPUT);                     // ditto above
  pinMode(Y_DIR_LOW, INPUT);                      // ditto above

  // Find Neutral Positions At Startup
  x_right = analogRead(X_DIR_HIGH);            // Initial neutral x-high value of joystick
  delay(10);

  x_left = analogRead(X_DIR_LOW);             // Initial neutral x-low value of joystick
  delay(10);

  y_up = analogRead(Y_DIR_HIGH);            // Initial neutral y-high value of joystick
  delay(10);

  y_down = analogRead(Y_DIR_LOW);             // Initial neutral y-low value of joystick
}

void loop() {
  
  xh = analogRead(X_DIR_HIGH);                    // A0 :: NOT CORRECT MAPPINGS
  xl = analogRead(X_DIR_LOW);                     // A1
  yh = analogRead(Y_DIR_HIGH);                    // A2
  yl = analogRead(Y_DIR_LOW);                     // A10

  /*Serial.print("xh:");
  Serial.println(xh);

  Serial.print("xl:");
  Serial.println(xl);

  Serial.print("yh:");
  Serial.println(yh);

  Serial.print("yl:");
  Serial.println(yl);*/

  Serial.println("-----");
  
  Serial.print("Speed: ");
  Serial.println(cursor_speed);
  
  xh_yh = sqrt(sq(((xh - x_right) > 0) ? (float)(xh - x_right) : 0.0) + sq(((yh - y_up) > 0) ? (float)(yh - y_up) : 0.0));     // sq() function raises input to power of 2, returning the same data type int->int ...
  xh_yl = sqrt(sq(((xh - x_right) > 0) ? (float)(xh - x_right) : 0.0) + sq(((yl - y_down) > 0) ? (float)(yl - y_down) : 0.0));   // the sqrt() function raises input to power 1/2, returning a float type
  xl_yh = sqrt(sq(((xl - x_left) > 0) ? (float)(xl - x_left) : 0.0) + sq(((yh - y_up) > 0) ? (float)(yh - y_up) : 0.0));      // These are the vector magnitudes of each quadrant 1-4. Since the FSRs all register
  xl_yl = sqrt(sq(((xl - x_left) > 0) ? (float)(xl - x_left) : 0.0) + sq(((yl - y_down) > 0) ? (float)(yl - y_down) : 0.0));    // a larger digital value with a positive application force, a large negative difference

  if ((xh_yh > radius_threshold) || (xh_yl > radius_threshold) || (xl_yl > radius_threshold) || (xl_yh > radius_threshold)) {
  
    if (bluetooth_mode == false){
      if ((xh_yh >= xh_yl) && (xh_yh >= xl_yh) && (xh_yh >= xl_yl)) {
              //Serial.println("quad1");
              Serial.print("xh_yh:");
              Mouse.move(x_cursor_high(xh), y_cursor_high(yh), 0);
              //Serial.print(xh);
              Serial.print(x_cursor_high(xh));
              Serial.print("  y:");
              //Serial.println(yh);
              Serial.println(y_cursor_high(yh));
              delay(cursor_delay);
              //poll_counter = 0;
            } else if ((xh_yl > xh_yh) && (xh_yl > xl_yl) && (xh_yl > xl_yh)) {
              //Serial.println("quad4");
              Serial.print("xh_yl:");
              //Serial.println(xh_yl);
              Mouse.move(x_cursor_high(xh), y_cursor_low(yl), 0);
              //Serial.print(xh);
              Serial.print(x_cursor_high(xh));
              Serial.print("  y:");
              //Serial.println(yl);
              Serial.println(y_cursor_high(yl));
              delay(cursor_delay);
              //poll_counter = 0;
            } else if ((xl_yl >= xh_yh) && (xl_yl >= xh_yl) && (xl_yl >= xl_yh)) {
              //Serial.println("quad3");
              Serial.print("xl_yl:");
              //Serial.println(xl_yl);
              Mouse.move(x_cursor_low(xl), y_cursor_low(yl), 0);
              //Serial.print(xl);
              Serial.print(x_cursor_high(xl));
              Serial.print("  y:");
              //Serial.println(yl);
              Serial.println(y_cursor_high(yl));
              delay(cursor_delay);
              //poll_counter = 0;
            } else if ((xl_yh > xh_yh) && (xl_yh >= xh_yl) && (xl_yh >= xl_yl)) {
              //Serial.println("quad2");
              Serial.print("xl_yh:");
              //Serial.println(xl_yh);
              Mouse.move(x_cursor_low(xl), y_cursor_high(yh), 0);
              //Serial.print(xl);
              Serial.print(x_cursor_high(xl));
              Serial.print("  y:");
              //Serial.println(yh);
              Serial.println(y_cursor_high(yh));
              delay(cursor_delay);
              //poll_counter = 0;
            }
    }
  }
  else if (bluetooth_mode == true){
  
  }

  // Cursor speed control push button - pushed down to change
  if(analogRead(PUSH_BUTTON_UP) == 0){
    delay(250);
    Serial.println("PUSH_BUTTON_UP Pressed");
    if(analogRead(PUSH_BUTTON_UP) == 0){
      // JoystickCalibration();  // Uses EEPROM to save values, not possible on the Featherboard
    }
    else{
      Serial.println("PUSH_BUTTON_UP Pressed");
      if(cursor_speed <= 15){          // Can't increase more than 15 
        cursor_speed += 1;
        EEPROM.put(0,cursor_speed);
      }
    }
  }

  if(analogRead(PUSH_BUTTON_DOWN) == 0){
    delay(250);
    if(analogRead(PUSH_BUTTON_DOWN) == 0){
      // JoystickCalibration();  // Uses EEPROM to save values, not possible on the Featherboard
    }
    else{
      Serial.println("PUSH_BUTTON_DOWN Pressed");
      if(cursor_speed >= 1){          // Can't decrease less than 1
        cursor_speed -= 1;
        EEPROM.put(0,cursor_speed);
      }
    }
  }


  //delay(200);
}


//***HID MOUSE CURSOR MOVEMENT FUNCTIONS***//

int y_cursor_high(int j) {

  if (j > y_up) {

    float y_up_factor = 1.25 * (yh_comp * (((float)(j - y_up)) / (yh_max - y_up)));

    int k = (int)(round(-1.0 * pow(cursor_speed, y_up_factor)) - 1.0);

    if (k <= (-1 * cursor_speed) ) {
      k = -1 * cursor_speed;
      return k;
    } else if ( (k < 0) && (k > (-1 * cursor_speed))) {
      return k;
    } else {
      k = 0;
      return k;
    }
  } else {
    return 0;
  }
}

int y_cursor_low(int j) {

  if (j > y_down) {

    float y_down_factor = 1.25 * (yl_comp * (((float)(j - y_down)) / (yl_max - y_down)));

    int k = (int)(round(1.0 * pow(cursor_speed, y_down_factor)) - 1.0);

    if (k >= cursor_speed) {

      k = cursor_speed;
      return k;
    } else if ((k > 0) && (k < cursor_speed)) {
      return k;
    } else {
      k = 0;
      return k;
    }
  } else {
    return 0;
  }
}

int x_cursor_high(int j) {

  if (j > x_right) {

    float x_right_factor = 1.25 * (xh_comp * (((float)(j - x_right)) / (xh_max - x_right)));

    int k = (int)(round(1.0 * pow(cursor_speed, x_right_factor)) - 1.0);

    if (k >= cursor_speed) {

      k = cursor_speed;
      return k;
    } else if ((k > 0) && (k < cursor_speed)) {
      return k;
    } else {
      k = 0;
      return k;
    }
  } else {
    return 0;
  }
}

int x_cursor_low(int j) {

  if (j > x_left) {

    float x_left_factor = 1.25 * (xl_comp * (((float)(j - x_left)) / (xl_max - x_left)));

    int k = (int)(round(-1.0 * pow(cursor_speed, x_left_factor)) - 1.0);

    if ( k <= (-1 * cursor_speed) ) {
      k = -1 * cursor_speed;
      return k;
    } else if ( (k < 0) && (k > -1 * cursor_speed)) {
      return k;
    } else {
      k = 0;
      return k;
    }
  } else {
    return 0;
  }
}
