//code for making your own accelerometer
//module MPU6050 has gyroscope too, but i'm currently reading only accln
//you can make changes for measuring maximum/minimum acceleration in "processAccelData"
//demonstration of finally finished circuit is here: https://youtu.be/YG_fWjylvV4
//you may publish this code anywhere you want, BUT give credits to me
//_______________________________________________________________________________________________

#include<Wire.h>
#include<Servo.h>
long accelX, accelY, accelZ;    //variables for measuring acceleration
float gForceX, gForceY, gForceZ, maxX = 0.0, maxY = 0.0, maxZ = 0.0;  //variables for storing g-forces 

long gyroX, gyroY, gyroZ;   //variables for measuring angle rotations
float rotX, rotY, rotZ;

float pitch, roll, prevP, prevR;
float errorP, errorR;

int writeVal1, writeVal2, i=0;
int *pitchCorrect, *rollCorrect;

Servo myServo1;
Servo myServo2;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
  myServo1.attach(11);      //myServo1, 11 => roll
  myServo2.attach(10);      //myServo2, 10 => pitch
}

//function for setting up the module's communication

void setupMPU(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);
  Wire.write(0x00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();
}

//function for reading acceleration data

void recordAccelRegisters(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);
  while(Wire.available()<6);
  accelX = Wire.read()<<8|Wire.read();
  accelY = Wire.read()<<8|Wire.read();
  accelZ = Wire.read()<<8|Wire.read();
  processAccelData();
}

//function for processing acceleration data;

void processAccelData(){
  gForceX = accelX/16384.0;
  gForceY = accelY/16384.0;
  gForceZ = accelZ/16384.0;
  pitch = (atan(gForceX/(sqrt(pow(gForceY, 2)+pow(gForceZ, 2)))))*(2000.0/PI)+1500;
  roll = (atan(-gForceY/gForceZ))*(2000.0/PI)+1500;
}

void loop(){
  recordAccelRegisters();
  printData();
  pitchCor(pitch);
  rollCor(roll);
  delay(10);
}

//function for printing the acceleration data

void printData(){
  Serial.print("accel X =");
  Serial.print(gForceX);
  Serial.print("g ");
  Serial.print(" accel Y =");
  Serial.print(gForceY);
  Serial.print("g ");
  Serial.print(" accel Z =");
  Serial.print(gForceZ);
  Serial.print("g ");
  Serial.print("pitch = ");
  Serial.print(pitch);
  Serial.print(" roll = ");
  Serial.print(roll);
  Serial.println("");
}

  void pitchCor( float pitch){
    
    if(pitch>1505.0 || pitch<1500){
      myServo2.writeMicroseconds(3000-pitch);
      delay(10);
    }
}

  void rollCor( float roll){
  if(roll>1502.0 || roll<1500){
      myServo1.writeMicroseconds(1500+(1500-roll));
      delay(10);
  }
}