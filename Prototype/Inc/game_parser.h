/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLIGHT_GEAR_H
#define __FLIGHT_GEAR_H

typedef struct FG_data {
	int roll_deg;
	int pitch_deg;
	int accel_mmph; // TODO
} FG_data;

// Initialize PC4 and PC5 for USART3_TX (RXD) and USART3_RX (TXD) for Flight Gear.
void FG_init(void);

// Wait for Flight Gear to send data for roll and pitch in the format "\r\n<roll>, <pitch>\r" or until cancel command is given
FG_data wait_for_FG_data(char cancel_command);

#endif /* __FLIGHT_GEAR_H */
