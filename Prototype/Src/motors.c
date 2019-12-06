#include "motors.h"
#include "GPIOs.h"
#include "stm32f0xx_hal.h"

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET
#define MAX_ROLL_STEPS 200
#define MAX_PITCH_STEPS 200
#define ON_DELAY 10
#define OFF_DELAY 10
#define MOTOR_TH 5
#define CW 1
#define CCW 0
#define CONFIDENCE_N_LEVEL 4000
#define GYRO_TOLERANCE 2

int mod(int a, int b) { 
	if(a < 0) {
		return b + (a%b);
	} else {
		return a % b;
	}
}

volatile static bool roll_toggle, pitch_toggle = false; // Used to toggle step pin from on to off
volatile static bool prev_roll_dir, prev_pitch_dir = false; // Used to track previous direction of motor to find fastest route
volatile static bool roll_clockwise, pitch_clockwise = false; // Used 
volatile static int16_t roll_delay, pitch_delay = 10;
volatile static uint8_t xlow, xhigh, ylow, yhigh  = 0;
volatile static int target_roll_steps, target_pitch_steps = 0;
volatile static int actual_roll_steps, actual_pitch_steps = 0;
volatile static int prev_actual_roll_steps, prev_actual_pitch_steps = 0;
volatile static int current_roll_steps, current_pitch_steps = 0;
volatile static int gyro_roll_confidence, gyro_pitch_confidence = 0;


void set_roll_dir_pin(bool clockwise);
void set_pitch_dir_pin(bool clockwise);
void set_roll_step_pin(bool state);
void set_pitch_step_pin(bool state);
void set_step_pin_manually(uint16_t step_pin, bool is_roll, bool set_pin_high);
void set_direction_pins(uint16_t dir_pins, bool is_roll, bool set_pin_high);
void toggle_step_pin(uint16_t step_pin, bool is_roll);
	
void motors_init(void) {
	// Initialize PB5 and PB6 for the ROLL motor's direction and step Pins.
	// Initialize PB7 and PB8 for the PITCH motor's direction and step Pins.
	GPIO_PINs_init();
}

//int est_roll_steps = 0;
//int est_pitch_steps = 0;
bool roll_is_close_enough = false;
bool pitch_is_close_enough = false;

void update_motors(void) {
	bool roll_update_needed = true;
	bool pitch_update_needed = true;
	bool roll_dir = false;
	bool pitch_dir = false;
	
	int roll_cw_dist = mod((target_roll_steps - current_roll_steps), MAX_ROLL_STEPS);
	int roll_ccw_dist = mod((current_roll_steps - target_roll_steps), MAX_ROLL_STEPS);
	
	int pitch_cw_dist = mod((target_pitch_steps - current_pitch_steps), MAX_PITCH_STEPS);
	int pitch_ccw_dist = mod((current_pitch_steps - target_pitch_steps), MAX_PITCH_STEPS);
	
	pitch_is_close_enough = false;
	// FIND SHORTEST PATH FOR ROLL MOTOR
	if(roll_cw_dist < roll_ccw_dist)
		roll_dir = CW;
	else if(roll_cw_dist > roll_ccw_dist)
		roll_dir = CCW;
	else if (roll_cw_dist) // If it ever gets here we know the distances are equal. T.F. keep going same direction to get to target.
		roll_dir = prev_roll_dir;
	else { // Distance is Zero we may have reached our target.
		roll_update_needed = false;
	}
//		// Since gyro pitch access is limited, we can only rely on it from 0-50 and 150-200
//		if(((current_pitch_steps < 10) || (190 < current_pitch_steps)) 
//			&& (current_roll_steps >= 175 || current_roll_steps <= 25)) {
//			// Find out if we are close enough to Gyro value
//			// Make sure we get the same gyro value at least N times before we rely on it's integrity.
//			if(roll_is_close_enough || gyro_roll_confidence >= CONFIDENCE_N_LEVEL) {
//				pitch_is_close_enough = gyro_pitch_confidence > (0.9 * CONFIDENCE_N_LEVEL); // Check if pitch is close enought to adjust also otherwise when the roll moves it will mess up pitch.
//				gyro_roll_confidence = 0; // Reset confidence because we are about to move the motor
//				
//				// Find out if we are close enough to Gyro value
//				int roll_cw_gryo_dist = mod((prev_actual_roll_steps - current_roll_steps), MAX_ROLL_STEPS);
//				int roll_ccw_gyro_dist = mod((current_roll_steps - prev_actual_roll_steps), MAX_ROLL_STEPS);
//				
//				if(roll_cw_gryo_dist < GYRO_TOLERANCE || roll_ccw_gyro_dist < GYRO_TOLERANCE) {// Close enough to gyro value.	 no more updates needed.
//					roll_update_needed = false;
//					//est_roll_steps = 0;
//				}
//				else  // Not close enough to gyro value
//					current_roll_steps = prev_actual_roll_steps;
//			} 
//			else if(actual_roll_steps == prev_actual_roll_steps) {// Got the same gyro value as previously, become more confident.
//				gyro_roll_confidence++; 
//				roll_update_needed = false;
//	//			if(est_roll_steps == 0) {
//	//				est_roll_steps = actual_roll_steps;
//	//			} else {
//	//				est_roll_steps = 0.875 * est_roll_steps + 0.125 * actual_roll_steps;
//	//			}
//			}
//			else {// Didn't get the same value as last sample. Reset confidence
//				//est_roll_steps = 0;
//				gyro_roll_confidence = 0;
//				roll_update_needed = false;
//			}
//		}	else { // We can't rely on the gyro at this point. We are in unknown territory.
//				pitch_update_needed = false;
//		}
//	}
	
	roll_is_close_enough = false;
	// FIND SHORTEST PATH FOR PITCH MOTOR
	if(pitch_cw_dist < pitch_ccw_dist)
		pitch_dir = CW;
	else if(pitch_cw_dist > pitch_ccw_dist)
		pitch_dir = CCW;
	else if (pitch_cw_dist) // If it ever gets here we know the distances are equal. T.F. keep going same direction to get to target.
		pitch_dir = prev_pitch_dir;
	else { // Distance is Zero we may have reached our target.
		pitch_update_needed = false;
//		// Since gyro pitch access is limited, we can only rely on it from 0-50 and 150-200
//		if(((current_pitch_steps < 10) || (190 < current_pitch_steps)) 
//			&& (current_roll_steps >= 175 || current_roll_steps <= 25)) {
//			// Make sure we get the same gyro value at least N times before we rely on it's integrity.
//			if(pitch_is_close_enough || gyro_pitch_confidence >= CONFIDENCE_N_LEVEL) {
//				roll_is_close_enough = gyro_roll_confidence > (0.9 * CONFIDENCE_N_LEVEL); // Check if roll is close enough to adjust also otherwise when the pitch moves it will mess up roll.
//				gyro_pitch_confidence = 0; // Reset confidence because we are about to move the motor
//				
//				// Find out if we are close enough to Gyro value
//				int pitch_cw_gryo_dist = mod((prev_actual_pitch_steps - current_pitch_steps), MAX_PITCH_STEPS);
//				int pitch_ccw_gyro_dist = mod((current_pitch_steps - prev_actual_pitch_steps), MAX_PITCH_STEPS);
//				
//				if(pitch_cw_gryo_dist < GYRO_TOLERANCE || pitch_ccw_gyro_dist < GYRO_TOLERANCE) { // Close enough to gyro value. no more updates needed.
//					pitch_update_needed = false;
//					// est_pitch_steps = 0;
//				} else { // Not close enough to gyro value
//					current_pitch_steps = prev_actual_pitch_steps;
//				}
//			}
//			else if(actual_pitch_steps == prev_actual_pitch_steps) { // Got the same gyro value as previously, become more confident.
//				gyro_pitch_confidence++; 
//				pitch_update_needed = false;
////				if(est_pitch_steps == 0) {
////					est_pitch_steps = actual_pitch_steps;
////				} else {
////					est_pitch_steps = 0.875 * est_pitch_steps + 0.125 * actual_pitch_steps;
////				}
//			}
//			else {// Didn't get the same value as last sample. Reset confidence
//				gyro_pitch_confidence = 0;
//				// est_pitch_steps = 0;
//				pitch_update_needed = false;
//			}
//		} else { // We can't rely on the gyro at this point. We are in unknown territory.
//			pitch_update_needed = false;
//		}
	}
	// Set direction pins accordingly
	set_roll_dir_pin(roll_dir);
	set_pitch_dir_pin(pitch_dir);
	
	// Step toward target if not already there
	if(roll_update_needed) {
		set_roll_step_pin(true); // Set high to enable a step
	}
	if(pitch_update_needed) {
		set_pitch_step_pin(true); // Set high to enable a step
	}
	HAL_Delay(ON_DELAY);
	
	if(roll_update_needed) {
		set_roll_step_pin(false); // Set low to finish a step
	}
	if(pitch_update_needed) {
		set_pitch_step_pin(false); // Set low to finish a step
	}
	HAL_Delay(OFF_DELAY);	
	
	// Store actual steps to be used as previous steps on next loop. This is to help see if gyro is giving good values.
	prev_actual_roll_steps = actual_roll_steps;
	prev_actual_pitch_steps = actual_pitch_steps;

}

void step_roll_motor(bool clockwise) {	
	set_roll_dir_pin(clockwise); // Set the direction
	
	set_roll_step_pin(true); // Set high to enable a step
	HAL_Delay(ON_DELAY);
	
	set_roll_step_pin(false); // Set low to finish a step
	HAL_Delay(OFF_DELAY);	
}

void step_pitch_motor(bool clockwise) {
	set_pitch_dir_pin(clockwise); // Set the direction

	set_pitch_step_pin(true); // Set high to enable a step
	HAL_Delay(ON_DELAY);
	
	set_pitch_step_pin(false); // Set low to finish a step
	HAL_Delay(OFF_DELAY);		
}

void set_current_roll_steps(int current_steps) {
	current_roll_steps = current_steps;
}

void set_current_pitch_steps(int current_steps) {
	current_pitch_steps = current_steps;
}

void set_target_roll_steps(int target_steps) {
  target_roll_steps = target_steps;
}

void set_target_pitch_steps(int target_steps) {
	target_pitch_steps = target_steps;
}

void set_actual_roll_steps(float roll) {
	if(roll < 0) { // negative
		actual_roll_steps = (int)(100.0f + ((180.0f + roll) * (0.56f)));
	} else {
		actual_roll_steps = (int)(roll * (0.56f));
	}
	actual_roll_steps = actual_roll_steps % 200;
}

void set_actual_pitch_steps(float pitch) {
	if (pitch < 0) { // negative
		actual_pitch_steps = (int)((-pitch) * (100.0f/180.0f));
	} else {
		actual_pitch_steps = (int)(200.0f - (pitch * (100.0f/180.0f)));
	}
	actual_pitch_steps = actual_pitch_steps % 200;
}

int get_actual_roll_steps(void) {
	return actual_roll_steps;
}

int get_actual_pitch_steps(void) {
	return actual_pitch_steps;
}


int get_current_roll_steps(void) {
	return current_roll_steps;
}

int get_current_pitch_steps(void) {
	return current_pitch_steps;
}


int get_target_roll_steps(void) {
	return target_roll_steps;
}

int get_target_pitch_steps(void) {
	return target_pitch_steps;
}

int degrees_to_steps(int degrees, int MAX_STEPS) {
	return (int)((float)mod(degrees - 90, 360) * 0.56f);
}

int degrees_to_roll_steps(int degrees) {
	return degrees_to_steps(degrees, MAX_ROLL_STEPS);
}

int degrees_to_pitch_steps(int degrees) {
	return degrees_to_steps(degrees, MAX_PITCH_STEPS);
}

void set_roll_dir_pin(bool clockwise) {
	roll_clockwise = clockwise;
	bool pin_state = !clockwise;  // Roll motor is flipped
	set_direction_pins(ROLL_DIR_PIN, true, pin_state);
}

void set_pitch_dir_pin(bool clockwise) {
	pitch_clockwise = clockwise; // Pitch motor is flipped
	set_direction_pins(PITCH_DIR_PIN, false, clockwise);
}

void set_roll_step_pin(bool state) { // High or low
	set_step_pin_manually(ROLL_STEP_PIN, true, state);
}

void set_pitch_step_pin(bool state) { // High or low
	set_step_pin_manually(PITCH_STEP_PIN, false, state);
}

// Sets the direction pin according to according to the target direction. 
void toggle_step_pin(uint16_t step_pin, bool is_roll) {
	GPIO_TypeDef *gpiox_base;
	GPIO_PinState pin_state;
	if(is_roll) {
		pin_state = roll_toggle ? HIGH : LOW;
		gpiox_base = ROLL_MOTORS_GPIO_BASE;
		if(roll_toggle) {
			roll_toggle = false;
			current_roll_steps = roll_clockwise ? mod(current_roll_steps + 1, MAX_ROLL_STEPS) : mod(current_roll_steps - 1, MAX_ROLL_STEPS);
		} else {
			roll_toggle = true;
		}
	} else { // is_pitch
		pin_state = pitch_toggle ? HIGH : LOW;
		gpiox_base = PITCH_MOTORS_GPIO_BASE;
		if(pitch_toggle) {
			pitch_toggle = false;
			current_pitch_steps = pitch_clockwise ? mod(current_pitch_steps + 1, MAX_PITCH_STEPS) : mod(current_pitch_steps - 1, MAX_PITCH_STEPS);
		} else {
			pitch_toggle = true;
		}
	}
	HAL_GPIO_WritePin(gpiox_base, step_pin, pin_state);
}

void set_step_pin_manually(uint16_t step_pin, bool is_roll, bool set_pin_high) {
	GPIO_TypeDef *gpiox_base;
	GPIO_PinState pin_state = set_pin_high ? HIGH : LOW;
	// Track steps taken.
	if(is_roll) {
		gpiox_base = ROLL_MOTORS_GPIO_BASE;
		if(step_pin == ROLL_STEP_PIN && set_pin_high)
			current_roll_steps = roll_clockwise ? mod(current_roll_steps + 1, MAX_ROLL_STEPS) : mod(current_roll_steps - 1, MAX_ROLL_STEPS);
		
	} else {
		gpiox_base = PITCH_MOTORS_GPIO_BASE;
		if(step_pin == PITCH_STEP_PIN && set_pin_high)
			current_pitch_steps = pitch_clockwise ? mod(current_pitch_steps + 1, MAX_PITCH_STEPS) : mod(current_pitch_steps - 1, MAX_PITCH_STEPS);
	}
	HAL_GPIO_WritePin(gpiox_base, step_pin, pin_state);
}

// Sets the direction pins according to according to the target direction. 
void set_direction_pins(uint16_t dir_pins, bool is_roll, bool set_pin_high) {
	GPIO_TypeDef *gpiox_base = is_roll ? ROLL_MOTORS_GPIO_BASE : PITCH_MOTORS_GPIO_BASE;
	if(set_pin_high)
		HAL_GPIO_WritePin(gpiox_base, dir_pins, HIGH);
	else
		HAL_GPIO_WritePin(gpiox_base, dir_pins, LOW);
}
