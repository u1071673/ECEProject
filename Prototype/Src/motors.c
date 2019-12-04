#include "motors.h"
#include "GPIOs.h"
#include "stm32f0xx_hal.h"

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET
#define MAX_ROLL_STEPS 200
#define MAX_PITCH_STEPS 200
#define ON_DELAY 10
#define OFF_DELAY 10
#define MOTOR_TH 10
#define CW 1
#define CCW 0

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
volatile static int current_roll_steps, current_pitch_steps = 0;

void set_roll_dir_pin(bool clockwise);
void set_pitch_dir_pin(bool clockwise);
void set_roll_step_pin(bool state);
void set_pitch_step_pin(bool state);
int steps_to_degrees(int steps, int MAX_STEPS);
void set_step_pin_manually(uint16_t step_pin, bool is_roll, bool set_pin_high);
void set_direction_pins(uint16_t dir_pins, bool is_roll, bool set_pin_high);
void toggle_step_pin(uint16_t step_pin, bool is_roll);
	
void motors_init(void) {
	printf("Initializing Motors...\n");
	// Initialize PB5 and PB6 for the ROLL motor's direction and step Pins.
	// Initialize PB7 and PB8 for the PITCH motor's direction and step Pins.
	GPIO_PINs_init();
	printf("Motor Initialization Complete!\n");

}

void update_motors(void) {
	bool roll_update_needed = true;
	bool pitch_update_needed = true;
	bool roll_dir = false;
	bool pitch_dir = false;

	volatile int roll_cw_dist = mod((target_roll_steps - actual_roll_steps), MAX_ROLL_STEPS);
	volatile int roll_ccw_dist = mod((actual_roll_steps - target_roll_steps), MAX_ROLL_STEPS);
	
	volatile int pitch_cw_dist = mod((target_pitch_steps - actual_pitch_steps), MAX_PITCH_STEPS);
	volatile int pitch_ccw_dist = mod((actual_pitch_steps - target_pitch_steps), MAX_PITCH_STEPS);
	
	if(roll_cw_dist < 20 || roll_ccw_dist < 20)
			roll_update_needed = false;
	else if(roll_cw_dist < roll_ccw_dist) // find shortest path
		roll_dir = CW;
	else if(roll_cw_dist > roll_ccw_dist)
		roll_dir = CCW;
	else if (roll_cw_dist) // If it ever gets here we know the distances are equal. T.F. keep going same direction to get to target.
		roll_dir = prev_roll_dir;
	else// Distance is Zero
		roll_update_needed = false;
	
	if(pitch_cw_dist < 20 || pitch_ccw_dist < 20)
		pitch_update_needed = false;
	else if(pitch_cw_dist < pitch_ccw_dist)
		pitch_dir = CW;
	else if(pitch_cw_dist > pitch_ccw_dist)
		pitch_dir = CCW;
	else if (pitch_cw_dist) // If it ever gets here we know the distances are equal. T.F. keep going same direction to get to target.
		pitch_dir = prev_pitch_dir;
	else // Distance is Zero
		pitch_update_needed = false;

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

int degrees_to_steps(int degrees, int MAX_STEPS) {
	return MAX_STEPS * (degrees/360);
}


int degrees_to_roll_steps(int degrees) {
	return steps_to_degrees(degrees, MAX_ROLL_STEPS);
}

int degrees_to_pitch_steps(int degrees) {
	return steps_to_degrees(degrees, MAX_PITCH_STEPS);
}

int steps_to_degrees(int steps, int MAX_STEPS) {
	return 360 * (steps/MAX_STEPS);
}

void set_roll_dir_pin(bool clockwise) {
	set_step_pin_manually(ROLL_DIR_PIN, true, clockwise); // Set Roll Direction
}

void set_pitch_dir_pin(bool clockwise) {
	set_step_pin_manually(PITCH_DIR_PIN, false, clockwise); // Set Pitch Direction
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
			current_roll_steps = roll_clockwise ? ((current_roll_steps + 1) % MAX_ROLL_STEPS) : ((current_roll_steps - 1) % MAX_ROLL_STEPS);
		} else {
			roll_toggle = true;
		}
	} else { // is_pitch
		pin_state = pitch_toggle ? HIGH : LOW;
		gpiox_base = PITCH_MOTORS_GPIO_BASE;
		if(pitch_toggle) {
			pitch_toggle = false;
			current_pitch_steps = pitch_clockwise ? ((current_pitch_steps + 1) % MAX_PITCH_STEPS) : ((current_pitch_steps - 1) % MAX_PITCH_STEPS);
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
		current_roll_steps = roll_clockwise ? ((current_roll_steps + 1) % MAX_ROLL_STEPS) : ((current_roll_steps - 1) % MAX_ROLL_STEPS);
	} else {
		gpiox_base = PITCH_MOTORS_GPIO_BASE;
		current_pitch_steps = pitch_clockwise ? ((current_pitch_steps + 1) % MAX_PITCH_STEPS) : ((current_pitch_steps - 1) % MAX_PITCH_STEPS);
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
