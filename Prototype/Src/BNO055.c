#include "BNO055.h"
#include "stm32f0xx_hal.h"
#include "putty.h"

#define WAIT_FOR_RESP_TIME 10
#define WAIT_FOR_SETUP_RESP_TIME 100
#define NUMBER_OF_RETRIES 3

#define GYR_CAL_STATUS(d) (((uint8_t)d >> 4) & 0x3)

volatile static float slope_deg = 0.0;
volatile static float cant_deg = 0.0;
volatile static float azimuth_deg = 0.0;
volatile static bool orientation_updated = false;
bool BNO055_gryo_is_calib(void);

void BNO055_init(void) {
	putty_print("Initializing BNO055...\r\n");

	// Initialize PB6 and PB7 for USART1_TX (RXD) and USART1_RX (TXD) for BNO055.
	USART1_init();
	
	do {
		// check startup success
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x36); // Reg: CALIB_STAT
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response
	} while(!USART1_read_successfully() && response_data() != (char)0x0F); // wait till we have calibrated gyro.
	
	bool is_normal_mode = false;
	
	do {
		// check if in normal power mode
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x3E); // Reg: Power Mode
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response_data
		if(USART1_read_successfully()) {
			putty_print("Power mode already set to NORMAL.\r\n");
			is_normal_mode = response_data() == 0x00;
			break;
		}
	} while(true);
	
	bool is_NDOF_op_mode = false;
	
	do {
		// check if in NDOF op mode
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x3D); // Reg: Operation Mode
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response_data
		if(USART1_read_successfully()) {
			putty_print("Operation mode already set to NDOF.\r\n");
			is_NDOF_op_mode = response_data() == 0x0C;
			break;
		}
	} while(true);
	
	bool is_degrees = false;
	
	do {
		// check if in Euler degrees output mode
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x3B); // Reg: Unit_Sel
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response_data
		if(USART1_read_successfully()) {
			putty_print("Output units already set to DEGREES.\r\n");
			is_degrees = (response_data() & 0x04) == 0x00;
			break;
		}
	} while(true);
	
	bool is_default_mapped = false;
	
	do {
		// Check if mapped to a default axis
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x41); // Reg: AXIS_MAP_CONFIG
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response_data
		if(USART1_read_successfully()) {
			putty_print("Default axis already default.\r\n");
			is_default_mapped = (response_data() & 0x3F) == 0x24;
			break;
		}
	} while(true);
	
	int retry_count = NUMBER_OF_RETRIES;
	do {
	// Select "page 0"
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x00); // Write
		transmit_char(USART1, (char)0x07); // Page ID
		transmit_char(USART1, (char)0x01); // Write one byte
		transmit_char(USART1, (char)0x00); // Write 0x00
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response
		retry_count --;
	} while (retry_count && !USART1_wrote_successfully());
 	
	if(!is_normal_mode) { // If we aren't in normal mode set it to normal mode.
		putty_print("Setting to NORMAL power mode.\r\n");
		retry_count = NUMBER_OF_RETRIES;
		do {
			// Set power to "normal" mode
			transmit_char(USART1, (char)0xAA); // Start
			transmit_char(USART1, (char)0x00); // Write
			transmit_char(USART1, (char)0x3E); // Select Register
			transmit_char(USART1, (char)0x01); // Write one byte
			transmit_char(USART1, (char)0x00); // Write 0x00
			HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response
			retry_count--;
		} while (retry_count && !USART1_wrote_successfully());
	}
	
	if(!is_NDOF_op_mode) {
		putty_print("Setting to NDOF operation mode.\r\n");
		retry_count = NUMBER_OF_RETRIES;
		do {
			// Set operation mode to "NDOF"
			transmit_char(USART1, (char)0xAA); // Start
			transmit_char(USART1, (char)0x00); // Write
			transmit_char(USART1, (char)0x3D); // Reg: OPR_MODE
			transmit_char(USART1, (char)0x01); // Bytes to Write: 1
			transmit_char(USART1, (char)0x0C); // Sending data: 0x0C
			HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response
			retry_count--;
		} while (retry_count && !USART1_wrote_successfully());
	}
	
	if(!is_degrees) {
		putty_print("Setting to DEGREES output units.\r\n");
		retry_count = NUMBER_OF_RETRIES;
		do {
			// Set operation mode to "NDOF"
			transmit_char(USART1, (char)0xAA); // Start
			transmit_char(USART1, (char)0x00); // Write
			transmit_char(USART1, (char)0x3B); // Reg: Unit_Sel
			transmit_char(USART1, (char)0x01); // Bytes to Write: 1
			transmit_char(USART1, (char)0x00); // Sending data: 0x0C
			HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response
			retry_count--;
		} while (retry_count && !USART1_wrote_successfully());
	}
 	
	
	// Check if gyro is calibrated
	putty_print("Calibrate the Gyroscope, by keeping the chair in a stable position for a few seconds.\r\n");
	putty_print("Gyroscope is calibrating...\r\n");
	do {
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x35); // Reg: CALIB_STAT
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response_data
		if(USART1_read_successfully() && ((0x30 & response_data()) == 0x30)) { // Check if bits 5 and 4 are both high.
			putty_print("Gyroscope Calibration complete!\r\n");
			break;
		}
	} while (true);
	
	// Check if accelerometer is calibrated
	putty_print("Calibrate the Accelerometer, by orienting the chair in 6 stable position for a period of a few seconds while making sure there is slow movement between each position.\r\n");
	putty_print("Accelerometer is calibrating...\r\n");
	do {
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x35); // Reg: CALIB_STAT
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response_data
		if(USART1_read_successfully() && ((0x0C & response_data()) == 0x0C)) { 	// Check if bits 3 and 2 are both high.
			putty_print("Accelerometer Calibration complete!\r\n");
			break;
		}
	} while (true);
	
	// Check if magnetometer is calibrated
	putty_print("Calibrate the Magnetometer, by moving the chair in a figure 8 pattern (rock forward while tilting right and vise versa)\r\n");
	putty_print("Magnetometer is calibrating...\r\n");
	do {
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x35); // Reg: CALIB_STAT
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response_data
		if(USART1_read_successfully() && ((0x03 & response_data()) == 0x03)) { // Check if bits 1 and 0 are both high.
			putty_print("Magnetometer Calibration complete!\r\n");
			break;
		}
	} while (true);
	
	// Check if entire system is calibrated
	putty_print("Verifying system calibration status...\r\n");
	do {
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x35); // Reg: CALIB_STAT
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response_data
		if(USART1_read_successfully() && ((0xC0 & response_data()) == 0xC0)) { // Check if bits 7 and 6 are both high.
			putty_print("System Calibration complete!\r\n");
			break;
		}
	} while (true);
	
	retry_count = NUMBER_OF_RETRIES;
	do {
		// Remap Pitch (z) = Heading (x), & Roll (y) = Roll (y), Heading (x) = Pitch (z)
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x00); // Write
		transmit_char(USART1, (char)0x41); // Reg: AXIS_MAP_CONFIG
		transmit_char(USART1, (char)0x01); // Write one byte
		transmit_char(USART1, (char)0x21); // Write value
		HAL_Delay(WAIT_FOR_SETUP_RESP_TIME); // Give time for response
		retry_count --;
	} while (retry_count && !USART1_wrote_successfully());
	
	putty_print("BNO055 Initialization and Calibration Complete!\n");
}

void BNO055_request_data(void) {
	// Request 6 bytes of orientation data (starting at 1A register)
	do{
	transmit_char(USART1, (char)0xAA); // Start
	transmit_char(USART1, (char)0x01); // Read
	transmit_char(USART1, (char)0x1A); // Reg: EUL_DATA_X_LSB
	transmit_char(USART1, (char)0x06); // Bytes to Read: 6
	HAL_Delay(WAIT_FOR_RESP_TIME); // Give time for response_data
	} while(!USART1_read_successfully());
}

bool BNO055_orientation_updated(void) {
	return has_new_orientation();
}

euler_data BNO055_get_orientation(void) {
		return get_orientation_data();
}
