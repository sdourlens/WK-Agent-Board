

#ifndef PWM_Bridge_h
#define PWM_Bridge_h
    

/*****************************************************************/
/************************* FONCTIONS PWM *************************/
/*****************************************************************/
int start_PWM_Bridge(int pwm_in, int percent);
int stop_PWM_Bridge(int pwm_in);
int change_PWM_Bridge_value(int pwm_in, int percent);
int read_PWM_Bridge_SENSE(int pwm_in);


#endif
