#include "BNO055.h"
#include "USARTs.h"
#include "stm32f0xx_hal.h"

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

void BNO055_request_data() {
	// Request 6 bytes of orientation data (starting at 1A register)
	transmit_char(USART1, (char)0xAA);
	transmit_char(USART1, (char)0x00);
	transmit_char(USART1, (char)0x3D);
	transmit_char(USART1, (char)0x01);
	transmit_char(USART1, (char)0x0C);
}

volatile static int euler_index = 0;
volatile static char euler_data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void BNO055_refresh_orientation() {
//	if(USART1_new_data) {
//		USART1_new_data = false;
//		
//		euler_data[euler_index++] = USART1_received_char;
//		
//		euler_index = euler_index == 8 ? 0 : euler_index;

//		switch(euler_data[0]) {
//		case (char)0xBB: // Euler data
//			if (euler_index == 0) {
//					// New values completely received and ready to store.
//				slope_deg = -((float)(euler_data[6] | euler_data[7] << 8) / 16);
//				cant_deg = -((float)(euler_data[4] | euler_data[5] << 8) / 16);
//				azimuth_deg = -((float)(euler_data[2] | euler_data[3] << 8) / 16);

//				orientation_updated = true;
//			}
//		break;
//		case (char)0xEE: // Error
//			// TODO: print error and current buffer
//			euler_index = 0;
//		break;
//		default:
//			// TODO: print buffer
//		break;
//		}
//	}
}
