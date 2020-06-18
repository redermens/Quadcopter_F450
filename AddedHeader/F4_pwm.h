#ifndef F4_PWM_H
#define F4_PWM_H

#include "cfg_func_f407.h"

void pwm_init(void);
void accelerator_motor_set(uint16_t motor_0,uint16_t motor_1,uint16_t motor_2,uint16_t motor_3);
void motor_pwm2tim(void);


#endif
