#include "Arduino.h"
#include "global.h"
#include "PINSMAPPING.h"
#include "PWM_Bridge.h"



/*****************************************************************/
/************************* FONCTIONS PWM *************************/
/*****************************************************************/

//---------------  Initialize PWM OUTPUTS, INPUTS AND ENABLE -------------------
void init_PWM_Bridge()
{
  pinMode(PIN_PWM_ENABLE1, OUTPUT);
  pinMode(PIN_PWM_ENABLE2, OUTPUT);
  pinMode(PIN_PWM_SENSE1, INPUT);    
  pinMode(PIN_PWM_SENSE2, INPUT);
  
  pinMode(PIN_PWM_IN[0], OUTPUT);
  pinMode(PIN_PWM_IN[1], OUTPUT);
  pinMode(PIN_PWM_IN[2], OUTPUT);
  pinMode(PIN_PWM_IN[3], OUTPUT);  
}

//--------------- START PWM GENERATION -------------------
int start_PWM_Bridge(int pwm_in, int percent)
{
  
  if(pwm_in<0 || pwm_in>3)   //bad parameters
  {
    return BAD_ARG;
  }
  
  if(percent<0 || percent>100)   //bad parameters
  {
    return BAD_ARG;
  }
  
  if(pwm_in==0 || pwm_in==1)            //Select the good enable according to the pwm output
  {
    digitalWrite(PIN_PWM_ENABLE1, HIGH);
  }
  else
  {
    digitalWrite(PIN_PWM_ENABLE2, HIGH);
  }
  
  analogWrite(PIN_PWM_IN[pwm_in], percent*255/100);  //Write value 0 to 255 on PWM output
  
  return(API_OK);

}

//--------------- STOP THE PWM GENERATION  -------------------
int stop_PWM_Bridge(int pwm_in)
{
  if(pwm_in<0 || pwm_in>3)   //bad parameters
  {
    return BAD_ARG;
  }

  if(pwm_in==0 || pwm_in==1)  //Select the good enable according to the pwm output
  {
    digitalWrite(PIN_PWM_ENABLE1, LOW);
  }
  else
  {
    digitalWrite(PIN_PWM_ENABLE2, LOW);
  }
  
  digitalWrite(pwm_in, LOW);  //stop PWM generation
  
  return(API_OK);
}

//--------------- MODIFY THE PWM COMMAND VALUE-------------------
int change_PWM_Bridge_value(int pwm_in, int percent)
{
   if(pwm_in<0 || pwm_in>3)   //bad parameters
  {
    return BAD_ARG;
  }
  
  if(percent<0 || percent>100)   //bad parameters
  {
    return BAD_ARG;
  }
  
  analogWrite(PIN_PWM_IN[pwm_in], percent*255/100);  //write value 0 to 255 on PWM output
  
  return (API_OK);
}

//---------------  -------------------
int read_PWM_Bridge_SENSE(int pwm_in)    //unused==> create a read analog function to replace the sense
{

}



