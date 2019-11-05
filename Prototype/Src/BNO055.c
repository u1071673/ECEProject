#include "BNO055.h"
#include "USARTs.h"
#include "stm32f0xx_hal.h"
#include "putty.h"

volatile static float slope_deg = 0.0;
volatile static float cant_deg = 0.0;
volatile static float azimuth_deg = 0.0;
volatile static bool orientation_updated = false;

void BNO055_init(void) {
	printf("Initializing BNO055...\n");

	// Initialize PB6 and PB7 for USART1_TX (RXD) and USART1_RX (TXD) for BNO055.
	USART1_init();

    // Select "page 0"
	transmit_char(USART1, (char)0xAA);
	transmit_char(USART1, (char)0x00);
	transmit_char(USART1, (char)0x07);
	transmit_char(USART1, (char)0x01);
	transmit_char(USART1, (char)0x00);

	// Set power to "normal" mode
	transmit_char(USART1, (char)0xAA);
	transmit_char(USART1, (char)0x00);
	transmit_char(USART1, (char)0x3E);
	transmit_char(USART1, (char)0x01);
	transmit_char(USART1, (char)0x00);

    // Set operation mode to "NDOF"
	transmit_char(USART1, (char)0xAA);
	transmit_char(USART1, (char)0x00);
	transmit_char(USART1, (char)0x3D);
	transmit_char(USART1, (char)0x01);
	transmit_char(USART1, (char)0x0C);
	
	printf("BNO055 Initialization Complete!\n");

}

void BNO055_request_data(void) {
	// Request 6 bytes of orientation data (starting at 1A register)
	transmit_char(USART1, (char)0xAA);
	transmit_char(USART1, (char)0x01);
	transmit_char(USART1, (char)0x1A);
	transmit_char(USART1, (char)0x06);
}

void BNO055_refresh_orientation(void) {
	if(has_new_orientation()) {
		char string_to_transmit[40];
		euler_data data = get_orientation_data();
		sprintf(string_to_transmit, "BNO055 GOT: %f,%f,%f\r\n", data.slope_deg, data.cant_deg, data.azimuth_deg);
		putty_print(string_to_transmit);
	}

}
