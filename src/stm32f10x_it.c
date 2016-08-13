
#include "stdio.h"
#include "stm32f10x_it.h"

void NMI_Handler(void) {
	printf("NMI_Handler");
}

void HardFault_Handler(void) {
	printf("HardFault_Handler");
	while (1) {	}
}

void MemManage_Handler(void) {
	printf("MemManage_Handler");
	while (1) {	}
}

void BusFault_Handler(void) {
	printf("BusFault_Handler");
	while (1) {	}
}

void UsageFault_Handler(void) {
	printf("UsageFault_Handler");
	while (1) {	}
}

void SVC_Handler(void) {
	printf("SVC_Handler");
}

void DebugMon_Handler(void) {
	printf("DebugMon_Handler");
}

void PendSV_Handler(void) {
	printf("PendSV_Handler");
}

