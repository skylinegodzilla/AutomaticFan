/*
  Automatic Arduino Lilypad Fan with adjustable Temperature
  By Ben Cawley
  
  Using:
    USB Arduino Lilypad
    5V Fan
    DHT11 Sensor
    MotorDriver to controll the fan (I used a DRV8833 but you can use any one realey)
    5V USB power supply
    2 Tactile Switchs
    2 10k ohm resistors

  
  Code was written by Ben Cawley
  Except for the DHT Sensor Library written by Adafruit.
   
*/
//==========My Classes==========
class Button 
{
  private:
    byte down;
  public:
    Button(int pin);
    int pin; 
    bool isDown();    //Check to see if the button has allredey been pressed
    bool isUp();      //Check to see if the button is not been pressed
    void On();        //Call this when the buttion has been pressed but isUP is true
    void Off();       //Call this when the buttion is not pressed but isDown is true

};
// init Button
Button::Button(int pin)
{
 this->down = 0;    //Value to say the button is down
 this->pin = pin;   //What pin the button is assinged to
 
}
//Check if the button is down
bool Button::isDown()
{
 return (bool)this->down;
}

//Check to see if the button is up
bool Button::isUp()
{
 return (bool)!this->down;
}

//Set the button as down
void Button::On()
{
 if (!this->down)
 {
   this->down = 1;
 }
 return;
}

//Set button as up
void Button::Off()
{
 if (this->down)
 {
   this->down = 0;
 }
 return;
}

//==========Global Vars==========
//=Setting up DHT= 
#include "DHT.h"          //Adafruit DHT sensor Library

#define DHTPIN 3          //The DHT is on pin3
#define DHTTYPE DHT11     //Letting the libray know that im using a DHT 11
DHT dht(DHTPIN, DHTTYPE); //Creating a new DHT object

//=Setting up pins pointers= 
const int fanPin = 2;       //Fan Pin
int tempSetting = 21;       //setting a defalt temp

//=Creating button objects=
Button dwnBtn(9);
Button upBtn(10);


//=DHT output infomation= 
 float h;       //Humidity
 float t;       //Temperature 
 float hi;     //Heat index


void setup()
{
 //=Set pins mode=
  pinMode(fanPin, OUTPUT);
  pinMode(dwnBtn.pin, INPUT);
  pinMode(upBtn.pin, INPUT);
 
 //=Begin serial console=
  Serial.begin(9600);
 
 //=start the DHT object
  dht.begin();
}

//==========Main Loop==========
void loop()
{
  //=Checks to see if the buttions have changed state since the last loop=
  if(digitalRead(upBtn.pin))  //if the up button is on
  {
    if(upBtn.isUp())  //check to see if the Button has just been pressed
    {
      upBtn.On();     //Yes it was just pressed so set the buttion as isdown
      tempUp();       //Call the function for this button
    }
  }
  else
  {
    upBtn.Off();    //the buttion is not being pressed so checkthat it is set as isup
  }

  if(digitalRead(dwnBtn.pin)) 
  {
    if(dwnBtn.isUp())
    {
      dwnBtn.On();
      tempDown();
    }
  }
  else
  {
    dwnBtn.Off();
  }
   
  //=Updates sensor info=
  updateInfo();     //I put this in to a function because I keeped moving it for testing

  //Turn the fan on or off based on the tempture
  if(t>tempSetting)
  {
    digitalWrite(fanPin, HIGH);      //turns Fan on
  }
  else
  {
    digitalWrite(fanPin, LOW);      //turns Fan off
  }
  
}
void printInfo ()
{
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.print(" *C ");
  Serial.print("\n");

  Serial.print("Current temp settings: ");
  Serial.print(tempSetting);
  Serial.print(" *C ");
  Serial.print("\n");
}

void tempUp()
{
  tempSetting++;    //increase temp setting
  Serial.print("Pressed up \n");
  printInfo();
}
void tempDown()
{
  tempSetting--;  //decresse temp setting
  Serial.print("Pressed down \n");
  printInfo();
}

void updateInfo()
{
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  
  t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  
  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius 
  hi = dht.computeHeatIndex(t, h, false);
}


