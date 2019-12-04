#ifndef __MOTORS_H
#define __MOTORS_H

#include <stdbool.h>

void motors_init(void);
void update_motors(void);
void step_roll_motor(bool clockwise); // true is clockwise, false is counter clockwise
void step_pitch_motor(bool clockwise); // true is clockwise, false is counter clockwise
void set_target_roll_steps(int target_steps);
void set_target_pitch_steps(int target_steps);
void set_actual_roll_steps(float roll);
void set_actual_pitch_steps(float pitch);
int get_actual_roll_steps(void);
int get_actual_pitch_steps(void);
int degrees_to_roll_steps(int degrees);
int degrees_to_pitch_steps(int degrees);
int mod(int a, int b);

#endif /* __MOTORS_H */
