#include "USARTs.h"
#include <stdlib.h>

#define BAUD_RATE 115200

volatile static bool USART1_new_data = false;
volatile static bool USART3_new_data = false;
volatile static char USART1_received_char = 0;
volatile static char USART3_received_char = 0;
volatile static int USART1_rx_index = 0;
volatile static char USART1_rx[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile static bool orientation_updated = false;
volatile static euler_data orientation;
volatile static bool write_success = false;
volatile static bool read_success = false;
void add_to_USART1_buffer(char c);
char USART1_resp_hdr();
char USART1_resp_status();

// Initializes PB6 and PB7 for USART1_TX (SCL/Rx) and USART1_RX (SDA/Tx).
void USART1_init(void) {
	printf("Initializing USART1...\n");
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// Configuring PB6 and PB7 to alternate function mode
	GPIOB->MODER &= ~(0xF<<12); // Clear bits 15-12
	GPIOB->MODER |= ((1<<15) | (1<<13));
	// Configuring the RX and TX lines to alternate function mode
	// AF0 for PB6 (USART1_TX) and PB7 (USART1_RX) // Note: actual installation of the IMU is backword (PB6 is SCL/Rx and PB7 is SDA/Tx)
	GPIOB->AFR[0] &= ~(0xFF<<24); // Clear all bits for AF0
	// Set the baud rate for communication to be 115200 bits/seconds
	USART1->BRR = HAL_RCC_GetHCLKFreq()/BAUD_RATE;
	// Enabling the transmitter and reciever hardware.
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;
	// Enabling the receive register non empty interupt
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 1);
	printf("USART1 Initialization Complete!\n");
}

// Initializes PC4 and PC5 for USART3_TX (RXD) and USART3_RX (TXD).
void USART3_init(void) {
	printf("Initializing USART3...\n");
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	// Configuring PC4 and PC5 to alternate function mode
	GPIOB->MODER &= ~(0xF<<8); // Clear bits 11-8
	GPIOC->MODER |= ((1<<11) | (1<<9));
	// Configuring the RX and TX lines to alternate function mode
	// AF1 for PC4 (USART3_TX) and PC5 (USART3_RX) // Note: actual installation of debug cable is backword (PC4 is RXD and PC5 is TXD)
	GPIOC->AFR[0] |= (1<<20 | 1<<16);
	GPIOC->AFR[0] &= ~(0xE<<20 | 0xE<<16);
	// Set the baud rate for communication to be 115200 bits/seconds
	USART3->BRR = HAL_RCC_GetHCLKFreq()/BAUD_RATE;
	// Enabling the transmitter and reciever hardware.
	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;
	// Enabling the receive register non empty interupt
	NVIC_EnableIRQ(USART3_4_IRQn);
	NVIC_SetPriority(USART3_4_IRQn, 1);
	printf("USART3 Initialization Complete!\n");
}

// This is called every time a char is received on USART1
void USART1_IRQHandler(void) {
	USART1_received_char = receive_char(USART1);
	USART1_new_data = true;
	add_to_USART1_buffer(USART1_received_char);
	return;
}

int length_to_receive = 2;
void add_to_USART1_buffer(char c) {
		// Set buffer with received char
		USART1_rx[USART1_rx_index++] = c;
		// Check if we are done getting data and reset the counter if we are
		USART1_rx_index = USART1_rx_index == length_to_receive ? 0 : USART1_rx_index; // Reset to zero once we got 8 bytes

		switch(USART1_rx[0]) { // Check response header
		case (char)0xBB: // Euler data
			// If we are receiving the second value then set the length accordingly
			if(USART1_rx_index == 2) {
				length_to_receive = c + '0';
			}
	
			read_success = false;
			write_success = false;
			if (USART1_rx_index == 0) { // If we are done reading values store the value
				length_to_receive = 2; // Reset length to receive
				
				orientation.slope_deg = -((float)(USART1_rx[6] | USART1_rx[7] << 8) / 16);
				orientation.cant_deg = -((float)(USART1_rx[4] | USART1_rx[5] << 8) / 16);
				orientation.azimuth_deg = -((float)(USART1_rx[2] | USART1_rx[3] << 8) / 16);
				orientation_updated = true;
				read_success = false;
			}
		break;
		case (char)0xEE:
			read_success = false;
			// Checking if write successfull
			if(USART1_rx_index == 2) {
					write_success = USART1_resp_status() == 1;
					USART1_rx_index = 0;
			}
		break;
		default:
			// TODO: print buffer
		break;
		}
}

char USART1_resp_hdr(void) {
	return USART1_rx[0];
}

char USART1_resp_len_or_status(void) {
	return USART1_rx[1];
}

bool get_USART1_read_success(void) {
	bool rs = read_success;
	read_success = false;
	return rs;
}

bool get_USART1_write_success(void) {
	bool ws = write_success;
	write_success = false;
	return ws;
}

bool has_new_orientation(void) {
	return orientation_updated;
}

euler_data get_orientation_data(void) {
	orientation_updated = false;
	return orientation;
}

// Used to check if USART1 has data
bool USART1_has_data(void) {
	return USART1_new_data;
}
// Used to retrieve and clear USART1's received char
char get_USART1_data(void) {
	char c = USART1_received_char;
	USART1_received_char = 0;
	USART1_new_data = false; // No longer new data.
	return c;
}
// This is called every time a char is received on USART3_4
void USART3_4_IRQHandler(void) {
	USART3_received_char = receive_char(USART3);
	USART3_new_data = true;
	return;
}

// Used to check if USART3 has data
bool USART3_has_data(void) {
	return USART3_new_data;
}

// Used to retrieve and clear USART3's received char
char get_USART3_data(void) {
	char c = USART3_received_char;
	USART3_received_char = 0;
	USART3_new_data = false; // No longer new data.
	return c;
}

// Wait for USART1 new data to arrive
char wait_for_USART1_data(void) {
	USART1_new_data = false;
	USART1_received_char = 0;
	while(!USART1_new_data);
	char c = USART1_received_char;
	USART1_received_char = 0; // Clear, to discourcage use of it else where. (Adds un-needed complexity if used elsewhere)
	USART1_new_data = false;
	return c;
}

// Wait for USART3 new data to arrive
char wait_for_USART3_data(void) {
	USART3_new_data = false;
	USART3_received_char = 0;
	while(!USART3_new_data);
	char c = USART3_received_char;
	USART3_received_char = 0; // Clear, to discourcage use of it else where. (Adds un-needed complexity if used elsewhere)
	USART3_new_data = false;
	return c;
}

// Wait till USART1 sends a character and returns true if character matches the parameter.
bool wait_for_USART1_char(char c) {
	char data = wait_for_USART1_data();
	return data == c;
}

// Wait till USART3 sends a character and returns true if character matches the parameter.
bool wait_for_USART3_char(char c) {
	char data = wait_for_USART3_data();
	return data == c;
}

// Receives the char from the USARTX buffer.
char receive_char(USART_TypeDef *USARTx) {
	// Wait for RXNE=0 inside ISR
	while(!(USARTx->ISR & USART_ISR_RXNE));
	char c = USARTx->RDR;
	if(USARTx == USART1) {
		printf("USART1 Received: 0x%X\n", c);
	} else if(USARTx == USART3) {
		//printf("USART3 Received: %c\n", c);
	}
	return c;
}

// Transmits a char over USARTx.
void transmit_char(USART_TypeDef *USARTx, char c) {
	// Waiting on the USART status flag to indicate the transmit register is empty.
	while(!(USARTx->ISR & USART_ISR_TXE));
	if(USARTx == USART1) {
		printf("USART1 Transmitting: 0x%X\n", c);
	} else if(USARTx == USART3) {
		//printf("USART3 Transmitting: %c\n", c);
	}
	USARTx->TDR = c;

}

// Transmits every char in the string over USART3.
void transmit_string(USART_TypeDef *USARTx, char *string) {
	int i = 0;
	while(string[i]) {
		transmit_char(USARTx, string[i]);
		i++;
	}
}

