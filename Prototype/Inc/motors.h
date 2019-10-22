#ifndef __MOTORS_H
#define __MOTORS_H

#include <stdbool.h>

void motors_init(void);
void update_motors(void);
void step_roll_motor(bool clockwise); // true is clockwise, false is counter clockwise
void step_pitch_motor(bool clockwise); // true is clockwise, false is counter clockwise
void set_target_roll_steps(int target_steps);
void set_target_pitch_steps(int target_steps);
int degrees_to_roll_steps(int degrees);
int degrees_to_pitch_steps(int degrees);


#endif /* __MOTORS_H */
