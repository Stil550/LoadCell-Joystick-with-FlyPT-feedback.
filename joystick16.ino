#include <Joystick.h>
#include <GyverHX711.h>
Joystick_ Joystick;
const unsigned char potPin = A4;
const unsigned char dataPinY = A0;
const unsigned char clockPinY = A1;
const unsigned char dataPinX = A2;
const unsigned char clockPinX = A3;
const unsigned short serialByte = 255;
const unsigned short joyMinRange = 0;
const unsigned short joyMaxRange = 1023;
const unsigned short forceMultX = 1.5; // 1 = no multiplier...
const unsigned short forceMultY = 5;


byte  serialVal; 
float serialMult;

float serialStepX;
float serialStepY;
float speedMultY;
float speedMultX;

long  joyRawY;
long  joyRawX;
int  joystickY;
int  joystickX;
int joyRange;
int joyRangeMid;
long cellYrange;
long cellXrange;
long cellYmin = -700000;
long cellYmax = 700000;
long cellXmin = -500000;
long cellXmax = 500000;
float joyXStep;
float joyYStep;


  

//int button1;int button2;int button3;int button4;int button5;int button6;int button7;int button8;

GyverHX711 scaleY(dataPinY,clockPinY, HX_GAIN64_A);
GyverHX711 scaleX(dataPinX,clockPinX, HX_GAIN64_A);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int pinToButtonMap = 2;
int lastButtonState[8] = {0,0,0,0,0,0,0,0};



void setup() {
Serial.begin(115200);
Serial.setTimeout(2);
scaleY.tare(); 
scaleX.tare();
Joystick.begin();

///// joystick setup 
Joystick.setXAxisRange(joyMinRange, joyMaxRange);
Joystick.setYAxisRange(joyMinRange, joyMaxRange);
joyRange = abs(joyMaxRange-joyMinRange);
joyRangeMid = joyRange/2; // simply divides the joystick range by 2 to find the mid point.  Works only with positive ranges.
cellYrange=abs(cellYmax-cellYmin);
cellXrange=abs(cellXmax-cellXmin);

///// button setup
pinMode(2, INPUT_PULLUP);  
pinMode(3, INPUT_PULLUP);  
pinMode(4, INPUT_PULLUP);  
pinMode(5, INPUT_PULLUP);  
pinMode(6, INPUT_PULLUP);  
pinMode(7, INPUT_PULLUP);  
pinMode(8, INPUT_PULLUP);  
pinMode(9, INPUT_PULLUP);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
long unsigned currentTime;
long static unsigned buttonTimer;
long static unsigned joyTimer;
short buttonRefresh = 100;
short joyRefresh = 50;
currentTime = millis();

//if (currentTime - joyTimer > joyRefresh)
//{
if (Serial.available()>0){serialVal = Serial.read();}else{serialVal=127;}  
if (scaleY.available()) {joyRawY = scaleY.read();} 
if (scaleX.available()) {joyRawX = scaleX.read();} 

joyYStep = abs(float(cellYrange)/float(joyRange)); //calculates a ratio between cell range (1,000,000) and joystick rangge (1024). 
joyXStep = abs(float(cellXrange)/float(joyRange));

// pure joystick mapping, without speed multiplier....
//joystickX = joyRangeMid-(joyRawX/joyXStep);
//joystickY = joyRangeMid-(joyRawY/joyYStep);

// joistick mapping with speed multiplier.....

serialStepX = float(forceMultX)/float(serialByte);  //calculates a ratio between forceMultipler (2) and serial Byte (255).
serialStepY = float(forceMultY)/float(serialByte);

speedMultX = 0.5+(float(serialVal)*serialStepX); // calculates a multilplier, min shoudl be 1,   max whatever is caluclated from forceMult
speedMultY = 0.5+(float(serialVal)*serialStepY);

joystickX = joyRangeMid-((joyRawX/joyXStep)/speedMultX); // takes joystick's middle of the range (512) and subtracts the calculation of cell input/ratio of ranges / speed mult.
joystickY = joyRangeMid-((joyRawY/joyYStep)/speedMultY); // this is controled from FlyPT.  FlyPT will be passing 0-255 via serial. The faster you fly, more force it is required to move the joystick.

Joystick.setYAxis(joystickY);
Joystick.setXAxis(joystickX);

joyTimer = currentTime;
//}

if (currentTime - buttonTimer > buttonRefresh)
{
  // Read pin values
  for (int index = 0; index < 8; index++)
  {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index])
    {
      Joystick.setButton(index, currentButtonState);
      lastButtonState[index] = currentButtonState;
    }
  }
//centers joystic with button in index 0 of the button array, button2 on joystick in my case.
if (lastButtonState[0]==1)
{
scaleY.tare(); 
scaleX.tare();
}
buttonTimer = currentTime;
}

//Serial.print("timeStart: ");Serial.println(timeStart);
//Serial.print("timeEnd: ");Serial.println(timeEnd);
//Serial.print("timeDelta: ");Serial.println(timeDelta);
//Serial.print("joystickX: ");Serial.println(joystickX);
//Serial.print("joystickY: ");Serial.println(joystickY);
//Serial.print("serialMult: ");Serial.println(serialMult,5);
//Serial.print("joyRawXY: ");Serial.println(joyRawX);
//Serial.print("joyXStep: ");Serial.println(joyXStep,6);
//Serial.print("joystickY: ");Serial.println(joystickY);
//Serial.print("cellYrange: ");Serial.println(cellYrange);


}
