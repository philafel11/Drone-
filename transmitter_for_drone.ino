// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>

// Include dependant SPI Library 
#include <SPI.h>

// Define Joystick Connections
#define joyVert    A0 
#define joyUP      A1
#define slider     A2

//#define joyVert2    A2 
//#define joyHorz2    A3

// Define Joystick Values - Start at 0 (middle position)
int joyposVert = 0;
int joyposUP = 0;
int sliderpos = 0;

//int joyposVert2 = 0;
//int joyposHorz2 = 0;

// Define addresses for radio channels
#define CLIENT_ADDRESS 1   
#define SERVER_ADDRESS 2

// Create an instance of the radio driver
RH_NRF24 RadioDriver;

// Sets the radio driver to NRF24 and the client address to 1
RHReliableDatagram RadioManager(RadioDriver, CLIENT_ADDRESS);

// Declare unsigned 8-bit motorcontrol array
uint8_t motorcontrol[5]; 

// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);
 
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!RadioManager.init())
    Serial.println("init failed");
  
  // Set initial motor direction as forward
  

}

void loop()
{
  
  // Read the right Joystick X and Y positions
  joyposVert = analogRead(joyVert); 
  joyposUP = analogRead(joyUP);
  sliderpos = analogRead(slider);

  motorcontrol[4] = map(sliderpos, 0, 1023, 0, 255); //ELEVATION

 if (joyposUP > 530)
 {
   motorcontrol[1] = map(joyposUP, 530, 1023, 0, 255);//FORWARD
 }

else if (joyposUP < 490) 
{
  motorcontrol[3] = map(joyposUP, 490, 0, 0, 255);//BACK  
}

else 
{
  motorcontrol[1] = 0;
  motorcontrol[3] = 0;
}
 if (joyposVert > 530)
 {
   motorcontrol[0] = map(joyposVert, 530, 1023, 0, 255);//right
 }
else if (joyposVert < 490) 
{
  motorcontrol[2] = map(joyposVert, 490, 0, 0, 255);//BACK  
}

else 
{
  motorcontrol[0] = 0;
  motorcontrol[2] = 0;
}

  
  Serial.print("FORWARD = ");
  Serial.print(motorcontrol[1]*4+1000);
  Serial.print("       BACK = ");
  Serial.print(motorcontrol[3]*4+1000);
  Serial.print("       RIGHT = ");
  Serial.print(motorcontrol[0]*4+1000);
  Serial.print("       LEFT = ");
  Serial.print(motorcontrol[2]*4+1000);
  Serial.print("       ELEVATION = ");
  Serial.println(motorcontrol[4]*4+1000);
  
  

  
//left joystick 
  //motorcontrol[4] = map(joyposVert2, 0, 1023, 0, 255);//UP
  //motorcontrol[5] = map(joyposHorz2, 0, 1023, 0, 255);//LEFT
  //motorcontrol[6] = map(joyposHorz2, 0, 1023, 255, 0);//RIGHT

  
  //Send a message containing Motor Control data to manager_server
  if (RadioManager.sendtoWait(motorcontrol, sizeof(motorcontrol), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    

  delay(100);  // Wait a bit before next transmission
}
}
