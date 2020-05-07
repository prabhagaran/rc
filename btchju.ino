//***************************************************************L293 Connection*******************************************************************//
const int DIR1 = 5;      //
const int M1 = 6;       //
const int M2 = 9;      //
const int DIR2 = 10;  //

//**************************************Leds connected to Arduino UNO Pin 12********************************************************************//

const int Frontlights  = 12;
const int BackLights  = 11;
const int FogLights  = 4;
const int RI  = 8;       // Pin of Right Indicator
const int LI  = 13;      //  Pin of Left Indicator
const int light  = 7;

//****************************************Buzzer / Speaker to Arduino UNO Pin 3****************************************************************//

const int buzzer = 3 ; // Pin of Buzzer

//*****************Bluetooth (HC-06 JY-MCU) State pin on pin 2 of Arduino*********************************************************************//

const int BTState = 2;

//*******************************************************Calculate Battery Level**************************************************************//

const float maxBattery = 8.0;// Change value to your max battery voltage level!
int perVolt;                 // Percentage variable
float voltage = 0.0;         // Read battery voltage
int level;
// *******************************************************Use it to make a delay... without delay() function!*********************************//

long previousMillis = -1000 * 10; // -1000*10=-10sec. to read the first value. If you use 0 then you will take the first value after 10sec.
long interval = 1000 * 10;     // interval at which to read battery voltage, change it if you want! (10*1000=10sec)
unsigned long currentMillis;   //unsigned long currentMillis;

//*********************************************************Useful Variables***********************************************************//

byte indicator = 0, timercountstatus = 0, signal1 = 0, timerenable = 0, timerenable2 = 0, timerenable3 = 0;
unsigned int timercount = 0, timer = 0, timer2 = 0, timer3 = 0 ;
int gear = 1;
const int osctimer = 16000, deosctimer = 16000, LITimer = 16000, RITimer = 16000 ;


int i = 0;
int j = 0;
int y = 0;
int z = 0;
int state;
int l = 0;
int r = 0, p = 0, q = 0;
int vSpeed = 200;   // Default speed, from 0 to 255

void setup() {
  // Set pins as outputs:
  pinMode(DIR1, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(DIR2, OUTPUT);

  pinMode(Frontlights, OUTPUT);
  pinMode(BackLights, OUTPUT);
  pinMode(FogLights, OUTPUT);
  pinMode(BTState, INPUT);
  pinMode(RI, OUTPUT);
  pinMode(LI, OUTPUT);
  pinMode(light, OUTPUT);

  Serial.begin(9600);
  Serial3.begin(9600);// Initialize Serial3 communication at 9600 bits per second:
}
void backlight()
{
  if ((r == 1) || (q == 1))
  {
    digitalWrite(BackLights, HIGH);
    r = 0;
    q = 0;
  }
  else
  {
    digitalWrite(BackLights, LOW);
  }
}



void Acceleration()
{
  if (timerenable == 1)
  {
    timer = timer + 1;
    if (timer >= osctimer)
    {
      gear = gear + 1;
      timer = 0;
      Serial.println(gear);

    }
    if (gear > 5)
    {
      gear = 5;

    }
  }
}
void deAcceleration()
{
  if (timerenable == 1)
  {
    timer = timer + 1;
    if (timer >= deosctimer)
    {
      gear = gear - 1;
      timer = 0;
      if (gear <= 1)
      {
        gear = 0;
        timerenable = 0;
      }
      analogWrite(DIR1, (gear * 31) + 100);
      analogWrite(M1, 0);
      analogWrite(M2, (gear * 31) + 100);
      analogWrite(DIR2, 0);
    }
  }
}

void Indicator(byte signal2)
{
  if (signal2 == 0)
  {
    digitalWrite(RI, LOW);
    digitalWrite(LI, LOW);
    noTone(buzzer);
    timercountstatus = 0;

  }
  else
  {
    timercountstatus = 1;

  }

}
void loop()
{

  if (timercount >= 6500)
  {
    timercount = 0;
  }

  if (timercountstatus == 1)
  {
    timercount = timercount + 1;
    if (timercount <= 3200)
    {
      if (signal1 == 1)
      {
        digitalWrite(RI, HIGH);
        digitalWrite(LI, LOW);
        tone(buzzer, 1000);
      }
      if (signal1 == 2)
      {
        digitalWrite(RI, LOW);
        digitalWrite(LI, HIGH);
        tone(buzzer, 1000);
      }
      if (signal1 == 3)
      {
        digitalWrite(RI, HIGH);
        digitalWrite(LI, HIGH);
        tone(buzzer, 1000);

      }
    }
    else
    {
      digitalWrite(RI, LOW);
      digitalWrite(LI, LOW);
      noTone(buzzer);
    }
  }

  if (timerenable2 == 1)
  {
    timer2 = timer2 + 1;
    if (timer2 >= LITimer)

    {
      signal1 = 2;
      Indicator(signal1);

      timer2 = 0;  timerenable2 = 0;

    }

  }
  if (timerenable3 == 1)
  {
    timer3 = timer3 + 1;
    if (timer3 >= RITimer)

    {
      signal1 = 1;
      Indicator(signal1);

      timer3 = 0;  timerenable3 = 0;

    }

  }

  //Stop car when connection lost or bluetooth disconnected

  if (digitalRead(BTState) == LOW)
  {
    state = 'S';
  }

  //Save income data to variable 'state'
  if (Serial3.available() > 0)
  {
    state = Serial3.read();

  }

  //Change speed if state is equal from 0 to 4. Values must be from 0 to 255 (PWM)
  if (state == '0') {
    vSpeed = 0;
  }
  else if (state == '1') {
    vSpeed = 100;
  }
  else if (state == '2') {
    vSpeed = 180;
  }
  else if (state == '3') {
    vSpeed = 200;
  }
  else if (state == '4') {
    vSpeed = 255;
  }

  /***********************Forward****************************/
  //If state is equal with letter 'F', car will go forward!
  if (state == 'F')
  {
    signal1 = 0;
    Indicator(signal1);
    timerenable = 1;
    Acceleration();
    digitalWrite(LI, LOW);
    analogWrite(M1, (gear * 31) + 100);
    analogWrite(DIR1, 255);
    analogWrite(DIR2, (gear * 31) + 100);
    analogWrite(M2, 255);
    //Serial.println("f");
    //Serial.println(gear);

  }
  /**********************Forward Left************************/
  //If state is equal with letter 'G', car will go forward left
  else if (state == 'G')
  {

    timerenable2 = 1;

    analogWrite(DIR1, 255);
    analogWrite(M1, 0);
    analogWrite(M2, 0);
    analogWrite(DIR2, 255);
  }
  /**********************Forward Right************************/
  //If state is equal with letter 'I', car will go forward right

  else if (state == 'I')
  {
    timerenable3 = 1;
    analogWrite(DIR1, 0);
    analogWrite(M1, 255);
    analogWrite(M2, 255);
    analogWrite(DIR2, 0);
  }

  /***********************Backward****************************/

  //If state is equal with letter 'B', car will go backward

  else if (state == 'B')
  {
    signal1 = 0;
    Indicator(signal1);
    r = 1;
    analogWrite(DIR1, 0);
    analogWrite(M1, 255);
    analogWrite(M2, 0);
    analogWrite(DIR2, 255);

  }

  /**********************Backward Left************************/
  //If state is equal with letter 'H', car will go backward left

  else if (state == 'H')
  {

    timerenable2 = 1;
    analogWrite(DIR1, 0);
    analogWrite(M1, 255);
    analogWrite(M2, 255);
    analogWrite(DIR2, 0);
  }

  /**********************Backward Right************************/

  //If state is equal with letter 'J', car will go backward right

  else if (state == 'J')
  {

    timerenable3 = 1;
    analogWrite(DIR1, 255);
    analogWrite(M1, 0);
    analogWrite(M2, 0);
    analogWrite(DIR2, 255);

  }
  /***************************Left*****************************/

  //If state is equal with letter 'L', wheels will turn left

  else if (state == 'L')
  {
    timerenable2 = 1;
    analogWrite(DIR1, 255);
    analogWrite(M1, 0);
    analogWrite(M2, 0);
    analogWrite(DIR2, 255);
  }

  /***************************Right*****************************/

  //If state is equal with letter 'R', wheels will turn right

  else if (state == 'R')
  {
    timerenable3 = 1;
    analogWrite(DIR1, 0);
    analogWrite(M1, 255);
    analogWrite(M2, 255);
    analogWrite(DIR2, 0);

  }

  /************************Lights*****************************/

  //If state is equal with letter 'W', turn leds on or of off

  else if (state == 'W')
  {
    if (i == 0)
    {
      analogWrite(Frontlights, 50);
      analogWrite(light, 50);
      i = 1;
    }

    else if (i == 1)
    {
      p = 1;
      analogWrite(Frontlights, 200);
      analogWrite(light, 50);
      i = 2;
    }
    else if (i == 2)
    {
      analogWrite(Frontlights, 0);
      analogWrite(light, 0);
      i = 0;
    }
    state = 'n';
  }
  else if (state == 'U')
  {
    if (y == 0)
    {
      analogWrite(FogLights, 50);
      y = 1;
    }
    else if (y == 1)
    {
      analogWrite(FogLights, 0);
      y = 0;

    }
    state = 'n';
  }

  /**********************Horn sound***************************/
  //If state is equal with letter 'V', play (or stop) horn sound
  else if (state == 'V')
  {
    if (j == 0)
    {
      tone(buzzer, 5000, 1000); //Speaker on
      j = 0;
    }
    else if (j == 1)
    {
      noTone(buzzer);    //Speaker off
      j = 0;
    }
    state = 'n';
  }
  else if (state == 'v')
  {
    if (j == 0)
    {
      tone(buzzer, 523, 1000); //Speaker on
      j = 0;
    }
    else if (j == 1)
    {
      noTone(buzzer);    //Speaker off
      j = 0;
    }
    state = 'n';
  }
  else if (state == 'X')
  {
    if (z == 0)
    {
      signal1 = 3;
      Indicator(signal1);

      z = 1;
    }
    else if (z == 1)
    {
      Indicator( signal1);
      signal1 = 0;
      z = 0;

    }
    state = 'n';
  }

  /************************Stop*****************************/
  //If state is equal with letter 'S', stop the car
  else if (state == 'S')
  {
    signal1 = 0;
    Indicator(signal1);
    q = 1;
    if (gear >= 1)
    {
      deAcceleration();
    }
    else
    {
      analogWrite(DIR1, 0);
      analogWrite(M1, 0);
      analogWrite(M2, 0);
      analogWrite(DIR2, 0);
    }
  }
  backlight();
  //   **************Battery*****************************/
  //Read battery voltage every 10sec.
  currentMillis = millis();
  if (currentMillis - (previousMillis) > (interval)) {
    previousMillis = currentMillis;
    //Read voltage from analog pin A0 and make calibration:
    voltage = (analogRead(A0) * 5.015 / 1024.0) * 11.132;
    //Calculate percentage...
    perVolt = (voltage * 100) / maxBattery;
    if      (perVolt <= 75)               {
      level = 0;
    }
    else if (perVolt > 75 && perVolt <= 80) {
      level = 1;  //        Battery level
    }
    else if (perVolt > 80 && perVolt <= 85) {
      level = 2;  //Min ------------------------   Max
    }
    else if (perVolt > 85 && perVolt <= 90) {
      level = 3;  //    | 0 | 1 | 2 | 3 | 4 | 5 | >
    }
    else if (perVolt > 90 && perVolt <= 95) {
      level = 4;  //    ------------------------
    }
    else if (perVolt > 95)                {
      level = 5;
    }
    Serial.println(level);
  }

}
