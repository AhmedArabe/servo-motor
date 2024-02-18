
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/TIMERS/TIMERS_Interface.h"




void Servo_Init(void)
{
	Timer1_Init(TIMER1_FASTPWM_OCRA_TOP_MODE , TIMER1_SCALER_8);
	Timer1_OCRB1Mode(OCRB_NON_INVERTING);
	TIMER1_PreloadOCR1A(19999);
	TIMER1_PreloadOCR1B(999);
}

void Servo_SetAngle(u8 angle)
{
//	u16 OCR_value=(((u32)angle*1000)/180)+999;
//	u16 OCR_value=(((u32)angle*3500)/180)+999;
	u16 OCR_value=(((u32)angle+44)*100)/9;

	TIMER1_PreloadOCR1B(OCR_value);
}

