#ifndef SavePower_h
#define SavePower_h
#include "Arduino.h"

enum Time_Out_Value { WDTO_15MS, WDTO_30MS, WDTO_60MS, WDTO_120MS, WDTO_250MS, WDTO_500MS, WDTO_1S, WDTO_2S, WDTO_4S, WDTO_8S, SLEEP_FOREVER };

class SavePowerClass
{
	public:
		#if defined (__AVR_ATmega32U4__) || defined (__AVR_ATmega16U4__) 
		    void  DivideClockSpeed(int Clock_Division_Factor);
		    void  IdleMode(Time_Out_Value time);		          
			void  ADCNoiseReductionMode(Time_Out_Value time);
			void  PowerDownMode(Time_Out_Value time);
			void  PowerSaveMode(Time_Out_Value time);
			void  StandbyMode(Time_Out_Value time);
			void  ExtendedStandbyMode(Time_Out_Value time);  
			void  DisableAllModules();
			void  DisableSPI();
			void  DisableUSB();
			void  DisableADC();
			void  DisableAC();
			void  DisableUSART();
			void  DisableTWI();
			void  DisableTimer0();
			void  DisableTimer1();
			void  DisableTimer3();
			void  DisableTimer4();
			void  EnableAllModules(); 
			void  EnableSPI();   
			void  EnableUSB();
			void  EnableADC();
			void  EnableAC();
			void  EnableUSART();
			void  EnableTWI();
			void  EnableTimer0();
			void  EnableTimer1();
			void  EnableTimer3();
			void  EnableTimer4();      
			void  LowestConsumption(Time_Out_Value time);    
		#else
		    #error "Make sure that the microcontroller is ATMega32U4 or ATMega16u4. This library supports only these two microcontrollers."
		#endif			
};

extern SavePowerClass SavePower;

#endif
