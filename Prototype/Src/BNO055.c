#include "BNO055.h"
#include "USARTs.h"
#include "stm32f0xx_hal.h"
#include "putty.h"

#define WAIT_FOR_RESP_TIME 5
#define NUMBER_OF_RETRIES 3

#define GYR_CAL_STATUS(d) (((uint8_t)d >> 4) & 0x3)

volatile static float slope_deg = 0.0;
volatile static float cant_deg = 0.0;
volatile static float azimuth_deg = 0.0;
volatile static bool orientation_updated = false;
bool BNO055_gryo_is_calib(void);

void BNO055_init(void) {
	printf("Initializing BNO055...\n");

	// Initialize PB6 and PB7 for USART1_TX (RXD) and USART1_RX (TXD) for BNO055.
	USART1_init();
	
	;
	while(!get_USART1_read_success() && get_USART1_data() != 0xF) {
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x36); // Reg: CALIB_STAT
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		//HAL_Delay(WAIT_FOR_RESP_TIME); // Give time for response
	}
	
	int retry_count = NUMBER_OF_RETRIES;
	while(retry_count && !get_USART1_write_success()) {
	// Select "page 0"
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x00); // Write
		transmit_char(USART1, (char)0x07); // Page ID
		transmit_char(USART1, (char)0x01); 
		transmit_char(USART1, (char)0x00);
		HAL_Delay(WAIT_FOR_RESP_TIME); // Give time for response
		retry_count --;
	}
 	
	retry_count = NUMBER_OF_RETRIES;
	while(retry_count && !get_USART1_write_success()) {
		// Set power to "normal" mode
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x00); // Write
		transmit_char(USART1, (char)0x3E);
		transmit_char(USART1, (char)0x01);
		transmit_char(USART1, (char)0x00);
		HAL_Delay(WAIT_FOR_RESP_TIME); // Give time for response
		retry_count--;
	}
	
	retry_count = NUMBER_OF_RETRIES;
	while(retry_count && !get_USART1_write_success()) {
		// Set operation mode to "NDOF"
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x00); // Write
		transmit_char(USART1, (char)0x3D); // Reg: OPR_MODE
		transmit_char(USART1, (char)0x01); // Bytes to Write: 1
		transmit_char(USART1, (char)0x0C); // Sending data: 0x0C
		HAL_Delay(WAIT_FOR_RESP_TIME); // Give time for response
		retry_count--;
	}
	
	// Check if gyro is calibrated
	bool calibrated = BNO055_gryo_is_calib();
	
	if(calibrated) {
		putty_print("Gyro is calibrated.\r\n");
	} else {
		putty_print("Gyro was not able to calibrate.\r\n");
	}
	
	putty_print("BNO055 Initialization Complete!\n");

}

bool BNO055_gryo_is_calib(void) {
	int retry_count = NUMBER_OF_RETRIES;
	while(retry_count && !get_USART1_read_success()) {
		transmit_char(USART1, (char)0xAA); // Start
		transmit_char(USART1, (char)0x01); // Read
		transmit_char(USART1, (char)0x35); // Reg: CALIB_STAT
		transmit_char(USART1, (char)0x01); // Bytes to Read: 1
		retry_count--;
	}
	return true;
	
//	char Response = get_USART1_data();
//	char 
//	wait_for_USART1_char((char)0xBB); // Response byte
//	uint8_t d = GYR_CAL_STATUS(wait_for_USART1_data());
//	int len = wait_for_USART1_data() - '0';
//	int i;
//	for(i = 0; i < len; i++) {
//		d = GYR_CAL_STATUS(wait_for_USART1_data());
//	}
//	wait_for_USART1_char((char)0x01); // Wait for 1 byte response
//	
//	return d == 0x3;
}

void BNO055_request_data(void) {
	// Request 6 bytes of orientation data (starting at 1A register)
	transmit_char(USART1, (char)0xAA); // Start
	transmit_char(USART1, (char)0x01); // Read
	transmit_char(USART1, (char)0x1A); // Reg: EUL_DATA_X_LSB
	transmit_char(USART1, (char)0x06); // Bytes to Read: 6
}

void BNO055_refresh_orientation(void) {
	if(has_new_orientation()) {
		char string_to_transmit[40];
		euler_data data = get_orientation_data();
		sprintf(string_to_transmit, "BNO055 GOT: %f,%f,%f\r\n", data.slope_deg, data.cant_deg, data.azimuth_deg);
		putty_print(string_to_transmit);
	}

}
