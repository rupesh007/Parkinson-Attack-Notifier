/*Simple Medical Application for Parkinson's Patient*/

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin_X = A3; // Analog pin for X
const int analogInPin_Y = A2;  // Analog Pin for Y
const int analogInPin_Z= A5;  //Analog pin for Z

const int buzz_pin=5;
//const int callibration_switch=10;
const int sample_window=10;
const int motionTime_thershold=10;

int rawX_min=512;
int rawX_max=512;

int rawY_min=512;
int rawY_max=512;

int rawZ_min=512;
int rawZ_max=512;


int sensorValue_X=0;      
int sensorValue_Y=0; 
int sensorValue_Z=0;

bool movement=false;

int time1=0;

//Moving average Filter for the Sampling the inputs

int readAxis(int axisPin,int sampleSize)
{
  long reading = 0;
  //analogRead(axisPin);
  
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
    delay(1);
  }
  return reading/sampleSize;
}

/*This method senses if there is anychange in the input*/
bool senseChange(int a, int b, int c){
 

  bool senseFlag=false;
  
  if(a < rawX_min || a > rawX_max) senseFlag=true;
  if(a < rawY_min || a > rawY_max) senseFlag=true;
  if(a < rawZ_min || a > rawZ_max) senseFlag=true;
  
return senseFlag;


}


/*This method callibrates the accelerometer...It is Simply called in setup method here. However UI could be provided for sesor callibration*/

void Calibrate(int xRaw, int yRaw, int zRaw)
{
  Serial.println("Calibration in Progress...Please Wait!!");
  if (xRaw < rawX_min){
    rawX_min = xRaw;
  }
  
  if (xRaw > rawX_max){
    rawX_max = xRaw;
  }
  
  if (yRaw < rawY_min){
    rawY_min = yRaw;
  }
  
  if (yRaw > rawY_max){
    rawY_max = yRaw;
  }
 
  if (zRaw < rawZ_min){
    rawZ_min = zRaw;
  }
  
  if (zRaw > rawZ_max){
   rawZ_max = zRaw;
  }
}


void setup() {
  int x_value=0;
  int y_value=0;
  int z_value=0;
  
  analogReference(EXTERNAL); //Analag reference external voltage
  // initialize serial communications at 115200 bps:
  Serial.begin(9600);
  Serial.println("Motion detection Started!!!");
  
  pinMode(analogInPin_X, INPUT);
  pinMode(analogInPin_Y, INPUT);
  pinMode(analogInPin_Z, INPUT);
  
  pinMode(buzz_pin,OUTPUT); //output pin for the buzzer


   //raw value reading
  x_value = readAxis(analogInPin_X,100);
  y_value = readAxis(analogInPin_Y,100);
  z_value = readAxis(analogInPin_Z,100);

  delay(2000);

  Calibrate(sensorValue_X,sensorValue_Y,sensorValue_Z);
  Serial.println("Calibration Done!!");

 /* Serial.print("X_min = ");
  Serial.print(rawX_min);
  Serial.print("\n");

  Serial.print("X_max = ");
  Serial.print(rawX_max);
  Serial.print("\n");

  Serial.print("Y_min = ");
  Serial.print(rawY_min);
  Serial.print("\n");

  Serial.print("Y_max = ");
  Serial.print(rawY_max);
  Serial.print("\n");

  Serial.print("Z_min = ");
  Serial.print(rawX_min);
  Serial.print("\n");

  Serial.print("Z_max = ");
  Serial.print(rawX_max);
  Serial.print("\n");
  
  Serial.print("\n\n\n"); */

  delay(100);
  
 
}

void loop() {
  
  //moving average value from the input
  sensorValue_X = readAxis(analogInPin_X,sample_window);
  sensorValue_Y = readAxis(analogInPin_Y,sample_window);
  sensorValue_Z = readAxis(analogInPin_Z,sample_window);
 
  // print the results to the serial monitor:
 /* Serial.print("Rawsensor_X = ");
  Serial.print(sensorValue_X);
  Serial.print("\n");
  
  Serial.print("Rawsensor_Y = ");
  Serial.print(sensorValue_Y);
  Serial.print("\n");

  Serial.print("Rawsensor_Z = ");
  Serial.print(sensorValue_Z);
  Serial.print("\n\n\n"); */

Serial.println("Motion Stable!!!");

movement=senseChange(sensorValue_X,sensorValue_Y,sensorValue_Z);

while(movement){
  time1=time1+1;
  
  Serial.println("Movement Detected...Timer1 Triggered!!");
  
  sensorValue_X = readAxis(analogInPin_X,sample_window);
  sensorValue_Y = readAxis(analogInPin_Y,sample_window);
  sensorValue_Z = readAxis(analogInPin_Z,sample_window);
  delay(100);
  movement=senseChange(sensorValue_X,sensorValue_Y,sensorValue_Z);
  if(!movement) {
    time1=0;
    break;  //breaks the while 
  } else { 
   
      delay(100);
      Serial.println("Movement still Detected...Timer2 Triggered!!");
        if(time1 > motionTime_thershold){
          // Do the Beep
          tone(buzz_pin,1000,1000);  //Square wave in the buzzer pin
          movement=false;
          time1=0;
          Serial.println("Actuation in the Buzzer");
         
        }
  
   }

  }
delay(1000);
}
