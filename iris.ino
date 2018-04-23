#include <Servo.h>

int servoPin1 = 6;
int servoPin2 = 9;
int servoPin3 = 10;
int potPin1 = 2;
int potPin2 = 3;
int potPin3 = 4;
 
Servo servo1, servo2, servo3;  

void setup()
{
  Serial.begin(9600);  
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
}
 
 
void loop()
{
   int val1 = analogRead(potPin1);
   Serial.println(val1);
   int mval1 = map(val1, 0,1024,0,360);
   servo1.write(mval1);

   int val2 = analogRead(potPin2);
   Serial.println(val2);
   int mval2 = map(val2, 0,1024,0,360);
   servo2.write(mval2);

   int val3 = analogRead(potPin3);
   Serial.println(val3);
   int mval3= map(val3, 0,1024,0,360);
   servo3.write(mval3);
   }
   
