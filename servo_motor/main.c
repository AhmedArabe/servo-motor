#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"

#include "MCAL/DIO/DIO_interface.h"
#include "MCAL/ADC/ADC_interface.h"
#include "MCAL/GIE/GIE_interface.h"

#include "HAL/LCD/LCD_interface.h"
#include "HAL/KPD/KPD_Interface.h"
#include "HAL/servo/Servo_Interface.h"

#include "avr/delay.h"
void APP (void);
u16 ADC_Result;
volatile u8 temp=0;
volatile u32 volt, digital;

void main (void)
{
	u8 Local_u8Key=0;
	u8 Password=0;
	u8 Flag_star=0;
	u8 number_of_tries=0;
	u8 welcome_flag=0;
	u8 Fan_flag=0;
	u8 LED_flag =0;
	u8 Door_flag=0;

	ADC_voidInit();
	GIE_voidEnable();
	HLCD_VoidInit();
	Servo_Init();

	DIO_u8SetPinDirection(DIO_u8_PORTA,DIO_u8_PIN0,DIO_u8_PIN_INPUT);
	DIO_u8SetPinValue(DIO_u8_PORTA,DIO_u8_PIN0,DIO_u8_PIN_LOW);

	DIO_u8SetPinDirection(DIO_u8_PORTA,DIO_u8_PIN1,DIO_u8_PIN_OUTPUT);

	DIO_u8SetPinDirection(DIO_u8_PORTA,DIO_u8_PIN2,DIO_u8_PIN_OUTPUT);

	DIO_u8SetPinDirection(DIO_u8_PORTD,DIO_u8_PIN4,DIO_u8_PIN_OUTPUT);

	HLCD_VoidSendString((u8*)"Welcome Back");
	_delay_ms(1000);
	HLCD_VoidSendCommand(DISPLAYCLEAR);

	HLCD_VoidSendString((u8*)"Enter Password");
	_delay_ms(1000);
	HLCD_VoidSendCommand(DISPLAYCLEAR);

	while(Local_u8Key !='=')
	{
		Password = Password*10+Local_u8Key;
		if(Flag_star!=0)
		{
			HLCD_VoidSendChar('*');
		}
		do
		{
			Local_u8Key=KPD_u8GetPressedKey();
			Flag_star=1;
		}while(Local_u8Key==KPD_NO_PRESSED_KEY );

	}

	while(1)
	{

		if(Password == 123)
		{
			HLCD_VoidSendCommand(DISPLAYCLEAR);
			if(welcome_flag==0)
			{
				HLCD_VoidSendCommand(DISPLAYCLEAR);
				HLCD_VoidSendString((u8*)"Welcome Ahmed ");
				_delay_ms(1000);
				HLCD_VoidSendCommand(DISPLAYCLEAR);
				welcome_flag=1;
			}

			if(Door_flag == 0)
			{
				HLCD_VoidSendString((u8*)"1-Open door ");
			}
			else if(Door_flag ==1)
			{
				HLCD_VoidSendString((u8*)"1-Close door ");
			}
			if(Fan_flag ==0)
			{
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendString((u8*)"2-OpenFan 3-More");
			}
			else if(Fan_flag ==1)
			{
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendString((u8*)"2-CloseFan 3-More");
			}
			Local_u8Key =KPD_NO_PRESSED_KEY;
			do
			{
				Local_u8Key=KPD_u8GetPressedKey();
			}while(Local_u8Key==KPD_NO_PRESSED_KEY );
			if(Local_u8Key ==1 && Door_flag ==0)
			{
				Servo_SetAngle(90);
				Door_flag =1;
			}
			else if(Local_u8Key ==1 && Door_flag ==1)
			{
				Servo_SetAngle(0);
				Door_flag =0;
			}
			else if(Local_u8Key ==2 && Fan_flag ==0)
			{
				DIO_u8SetPinValue(DIO_u8_PORTA,DIO_u8_PIN2,DIO_u8_PIN_HIGH);
				Fan_flag=1;
			}
			else if( Local_u8Key ==2 && Fan_flag ==1)
			{
				DIO_u8SetPinValue(DIO_u8_PORTA,DIO_u8_PIN2,DIO_u8_PIN_LOW);
				Fan_flag=0;
			}
			else if(Local_u8Key == 3)
			{

				HLCD_VoidSendCommand(DISPLAYCLEAR);
				if(LED_flag ==0)
				{
					HLCD_VoidSendString((u8*)"4-Turn On LED");
				}
				else if(LED_flag ==1)
				{
					HLCD_VoidSendString((u8*)"4-Turn Off LED");
				}
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendString((u8*)"5-Temperature ");
				Local_u8Key = KPD_NO_PRESSED_KEY;
				do
				{
					Local_u8Key=KPD_u8GetPressedKey();
				}while(Local_u8Key==KPD_NO_PRESSED_KEY );

				if(Local_u8Key == 4 && LED_flag ==0)
				{
					DIO_u8SetPinValue(DIO_u8_PORTA,DIO_u8_PIN1,DIO_u8_PIN_HIGH);
					LED_flag =1;
				}
				else if(Local_u8Key ==4 && LED_flag ==1)
				{
					DIO_u8SetPinValue(DIO_u8_PORTA,DIO_u8_PIN1,DIO_u8_PIN_LOW);
					LED_flag =0;
				}
				else if(Local_u8Key ==5)
				{
					HLCD_VoidSendCommand(DISPLAYCLEAR);
					ADC_u16StartConversionAsynchronous(0,APP);
					_delay_ms(100);
					HLCD_VoidSendString((u8*)"Temp =  ");
					HLCD_VoidSendNumber(temp);
					_delay_ms(1000);
				}

			}

		}
		else if(Password != 123 && number_of_tries <2)
		{
			HLCD_VoidSendCommand(DISPLAYCLEAR);
			HLCD_VoidSendString((u8*)"Wrong password");
			_delay_ms(500);
			HLCD_VoidSendCommand(DISPLAYCLEAR);
			HLCD_VoidSendString((u8*)"Try again");
			_delay_ms(500);
			HLCD_VoidSendCommand(DISPLAYCLEAR);
			Password=0;
			Local_u8Key =0;
			Flag_star =0;
			while(Local_u8Key !='=')
			{
				Password = Password*10+Local_u8Key;
				if(Flag_star!=0)
				{
					HLCD_VoidSendChar('*');
				}
				do
				{
					Local_u8Key=KPD_u8GetPressedKey();
					Flag_star=1;
				}while(Local_u8Key==KPD_NO_PRESSED_KEY );

			}
			number_of_tries++;
		}
		else
		{
			if(welcome_flag ==0)
			{
				HLCD_VoidSendCommand(DISPLAYCLEAR);
				HLCD_VoidSendString((u8*)"exceed ");
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendString((u8*)"valid tries");
				welcome_flag =1;
			}
		}
	}
}
void APP(void)
{
	HLCD_VoidGotoXY(0,0);
	volt =ADC_GetVolt();
	temp = volt /10;
}
