#include <Servo.h>
Servo servoX;
Servo servoY;
void setup() {
    Serial.begin(9600);
    servoX.attach(3);
    servoY.attach(5);
    servoX.write(90);
    servoY.write(90);


}
int angleX;
int angleY;
unsigned int integerValueX=0;
unsigned int integerValueY=0;  
char incomingByte;
void loop()
{
 
  
  if (Serial.available() > 0) {   
    integerValueX = 0;         
    while(1) {            
      incomingByte = Serial.read();
      if (incomingByte == 'X')
      {
          integerValueY = 0;
        while(1)
        {
            incomingByte = Serial.read();
            if(incomingByte=='\n') break;
            if (incomingByte == -1) continue;  
             integerValueY *= 10;  
            integerValueY = ((incomingByte - 48) + integerValueY);

        }
      }
      if(incomingByte=='\n') break;
      if (incomingByte == -1) continue;  
      integerValueX *= 10; 
      integerValueX = ((incomingByte - 48) + integerValueX);
    }
    angleX = map(integerValueX, 600, 100, 30, 179);
    angleY=map(integerValueY,400,100,30,179);
    angleX = min(angleX, 179);
    angleX = max(angleX, 30); 
    angleY=min(angleY,179);
    angleY=max(angleY,30);
    servoX.write(angleX);
    servoY.write(angleY);
}
}
