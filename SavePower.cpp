/****************************************************************************************
* ATMega32U4/16U4 SavePower Library 
* Version: 1.0.0
* Date: 25-10-2020
* Author: Hamza Taous
* Github Username : taous96
* Github Link Library : https://github.com/taous96/ATMega32U4-16U4_SavePower_Library
* Chips Supported: ATMega32u4, ATmega16u4 (AVR architecture)
* Tested and well verified with Arduino IDE 1.8.8 
*****************************************************************************************/

/***********************************************************************************************************************************************
ATMega32u4/16u4 enable the application to shut down unused modules in the MCU in order to reduce power consumption and extend the lifespan 
of energy sources. In general, the AVR provides various sleep modes allowing the user to tailor the power consumption to the application’s
requirements. For the systems based on ATMega32u4/16u4, we can make the MCU enter six sleep modes using Sleep Mode Control Register SMCR. 

Sleep Mode  	                   Description
==========  	                   ===========
* Idle Mode                        This sleep mode basically halts clkCPU and clkFLASH, while allowing the other clocks to run. 
* ADC Noise Reduction Mode         This sleep mode basically halts clkI/O, clkCPU, and clkFLASH, while allowing the other clocks to run.
* Power Down Mode                  This sleep mode stopps the external oscillator, while the external interrupts, the Two Wire Interface, 
                                   and the Watchdog continue operating.
* Power Save Mode                  For compatibility reasons with AT90USB64/128, this mode is still present. In the case of ATMega32u4/16u4
                                   microcontrollers, this sleep mode is identical to Power Down mode.
* Standby Mode                     This sleep mode is identical to Power Down mode with the exception that the Oscillator is kept running.  
* Extended Standby Mode            For compatibility reasons with AT90USB64/128, this mode is still present. In the case of ATMega32u4/16u4
                                   microcontrollers, this sleep mode is identical to Standby mode.                                   
                                   
The Sleep Mode Control Register SMCR contains control bits for power management (SE, SM0, SM1 and SM2). To select between the six available 
sleep modes, the control bit SE must always be written to logic one and the others must be written as shown below :
      
 --------------------------------------------------------------------------------------------
| Sleep Mode                              SM0               SM1               SM2            | 
 --------------------------------------------------------------------------------------------
| Idle Mode                                0                 0                 0             |                
| ADC Noise Reduction Mode                 1                 0                 0             |
| Power Down Mode                          0                 1                 0             |
| Power Save Mode                          1                 1                 0             |
| Standby Mode                             0                 1                 1             |
| Extended Standby Mode                    1                 1                 1             |
|                                                         RESERVED                           |    
 --------------------------------------------------------------------------------------------

To reduce power consumption, we can also use the Power Reduction Registers PRR0 and PRR1. These two registers provide a method to stop the 
clock to individual peripherals. The module shutdown can be used in Idle mode and Active mode to significantly reduce the overall power
consumption. In all other sleep modes, the clock is already stopped.

         ---------------------------------------------------------------------
PRR0    |  PRTWI  |  -  |  PRTIM0  |  -  |  PRTIM1  |  PRSPI  |  -  |  PRADC  |                              
         ---------------------------------------------------------------------             
         -------------------------------------------------------------------
PRR1    |  PRUSB  |  -  |  -  |  PRTIM4  | PRTIM3  |  -  |  -  |  PRUSART1  |                              
         ------------------------------------------------------------------- 

To reduce energy consumption of systems based on ATMega32u4/16u4, we can also divide the clock speed using the Clock Prescaler Register 
CLKPR. To select between the nine available clock speeds, the control bit CLKPSE must always be written to logic one and the others must be 
written as shown below :
        
 ----------------------------------------------------------------------------------------------------------------------------
|          CLKPS3          CLKPS2          CLKPS1          CLKPS0          Clock Division Factor          Frequency          |
 ----------------------------------------------------------------------------------------------------------------------------
|            0               0               0               0                       1                     8 MHz             |
|            0               0               0               1                       2                     4 MHz             |
|            0               0               1               0                       4                     2 MHz             |
|            0               0               1               1                       8                     1 MHz             |
|            0               1               0               0                       16                    500 KHz           |
|            0               1               0               1                       32                    250 KHz           |
|            0               1               1               0                       64                    125 KHz           |
|            0               1               1               1                       128                   62.5 KHz          |
|            1               0               0               0                       256                   31.25 KHz         |
|                                                         RESERVED                                                           |    
 ----------------------------------------------------------------------------------------------------------------------------        

For waking up the MCU from any of the sleep modes just by the software itself, we can count on the Watchdog Timer (WDT). This last one is 
a timer counting cycles of a separate on-chip 128kHz oscillator. It can give an interrupt to wake the MCU from sleep modes when the counter 
reaches a given time-out value. The Watchdog Timer Control Register WDTCSR allows us to select the operating mode and the time-out value we 
want the timer to reach before waking up the MCU. To wake up the MCU from sleep mode by an interrupt, we have to select the interrupt mode 
first, and on the other side, the time-out value is selected by four control bits (WDP0, WDP1, WDP2 and WDP3), just as shown below :

Interrupt Mode ===> WDTON bit must be written to logic zero, WDE bit must written to logic zero and WDIE bit must written to logic one.  
         
           ----------------------------------------------------------------------
WDTCSR    |  WDIF  |  WDIE  |  WDP3  |  WDCE  |  WDE  |  WDP2  |  WDP1  |  WDP0  |                              
           ----------------------------------------------------------------------  
 -----------------------------------------------------------------------------------------------------
|           WDP3            WDP2            WDP1            WDP0            Time_out value            |
 -----------------------------------------------------------------------------------------------------
|            0               0               0               0                  16 ms                 |
|            0               0               0               1                  32 ms                 |
|            0               0               1               0                  64 ms                 |
|            0               0               1               1                  0.125 s               |
|            0               1               0               0                  0.25 s                |
|            0               1               0               1                  0.5 s                 |
|            0               1               1               0                  1 s                   |
|            0               1               1               1                  2 s                   |
|            1               0               0               0                  4 s                   |
|            1               0               0               1                  8 s                   |
|                                         RESERVED                                                    |    
 -----------------------------------------------------------------------------------------------------

* Please Note:
  ===> Standby modes are only recommended for use with external crystals or resonators.
  ===> If the Analog Digital Converter (ADC) is enabled before entering to any of sleep modes. It will be enabled in all sleep modes. It 
       should be disabled before entering the MCU into the sleep mode to save power.
  ===> If the Analog Comparator (AC) is set up to use the Internal Voltage Reference as input, it should be disabled in all sleep modes.
       Otherwise, the Analog Comparator (AC) should be disabled in Idle and ADC Noise Reduction modes, and it is automatically disabled 
       in other sleep modes.
  ===> Take note that Timer2 and USART0 are not available in ATMega32u4/16u4 architectures.
  ===> Using avr/power.h library makes things easy when we want to enable or disable some peripherals. However, this library will make 
       you discover how things work clearly.
  ===> Disabling the ADC off while a conversion, will terminate this conversion.
  ===> When Timer0 ( or Timer1 or Timer3 or Timer4 ) is enabled, the operation will continue like before the shutdown.
  ===> When I2C ( or SPI or USART1 or USB Interface ) is disabled, it should be reinitialized to ensure proper operation when waking it up.                    
  ===> For systems based on ATMega32u4/16u4 chips, we can't disable the Brown-Out Detector through software.
  ===> Dividing clock speed can be used with all clock source options, and will affect the clock frequency of the CPU and all synchronous 
       peripherals. Therfore, the dividing clock speed is recommended only when the requirement for processing power is low.         
***********************************************************************************************************************************************/

#include "SavePower.h"

#if defined (__AVR__)
  #include <avr/sleep.h>
  #include <avr/wdt.h>
  #include <avr/interrupt.h>
#else
  #error "These libraries support only AVR family of microcontrollers."
#endif

#if defined (__AVR_ATmega32U4__) || defined (__AVR_ATmega16U4__) 

 // Dividing Clock System Frequency 
 // Clock Division Factor = 2
 #ifndef DivideClockSpeedByTwo          
  #define DivideClockSpeedByTwo() {   \
  	 CLKPR = 0x81;                \
  } 									
 #endif
 
 // Clock Division Factor = 4
 #ifndef DivideClockSpeedByFour          
  #define DivideClockSpeedByFour() {   \
  	 CLKPR = 0x82;                 \
  } 									
 #endif
 
 // Clock Division Factor = 8
 #ifndef DivideClockSpeedByEight          
  #define DivideClockSpeedByEight() {   \
  	 CLKPR = 0x83;                  \
  } 									
 #endif
 
 // Clock Division Factor = 16
 #ifndef DivideClockSpeedBySixteen          
  #define DivideClockSpeedBySixteen() {   \
  	 CLKPR = 0x84;                    \
  } 									
 #endif

 // Clock Division Factor = 32
 #ifndef DivideClockSpeedByThirtyTwo          
  #define DivideClockSpeedByThirtyTwo() {   \
  	 CLKPR = 0x85;                      \
  } 									
 #endif
 
 // Clock Division Factor = 64
 #ifndef DivideClockSpeedBySixtyFour          
  #define DivideClockSpeedBySixtyFour() {   \
  	 CLKPR = 0x86;                      \
  } 									
 #endif 
 
 // Clock Division Factor = 128
 #ifndef DivideClockSpeedByOneHundredTwentyEight          
  #define DivideClockSpeedByOneHundredTwentyEight() {   \
  	 CLKPR = 0x87;                                  \
  } 									
 #endif
 
 // Clock Division Factor = 256 
 #ifndef DivideClockSpeedByTwoHundredFiftySix          
  #define DivideClockSpeedByTwoHundredFiftySix() {   \
  	 CLKPR = 0x88;                               \
  } 									
 #endif
 
 // Default State (8 MHz)
 #ifndef ClockSpeedDefaultState          
  #define ClockSpeedDefaultState() {   \
  	 CLKPR = 0x00;                 \
  } 									
 #endif
 
 // Analog Comparator ACSR bit
 #ifndef ACD
  #define ACD 7
 #endif
 
 // Analog Comparator power reduction macros
 #ifndef disable_ac          
  #define disable_ac() {       \
  	 ACSR |= (1 << ACD);   \
  } 									
 #endif

 #ifndef enable_ac          
  #define enable_ac() {         \
  	 ACSR &= ~(1 << ACD);   \
  } 									
 #endif
 
 // Timer4 PRR bit 
 #ifndef PRTIM4
  #define PRTIM4 4
 #endif
  
 #ifndef EnterSleepMode            
  #define EnterSleepMode(mode) {   \
     do {   	                   \
          set_sleep_mode(mode);    \
          cli();  	           \
          sleep_enable();          \
          sei();		   \
          sleep_cpu();	           \
          sleep_disable();         \
          sei();		   \
        } while (0);	           \
  }	
 #endif		  					

// Dividing Clock Speed Method
void SavePowerClass::DivideClockSpeed(int Clock_Division_Factor)
{
  if (Clock_Division_Factor == 1) { ClockSpeedDefaultState(); }
  else if (Clock_Division_Factor == 2) { DivideClockSpeedByTwo(); }
  else if (Clock_Division_Factor == 4) { DivideClockSpeedByFour(); }
  else if (Clock_Division_Factor == 8) { DivideClockSpeedByEight(); }
  else if (Clock_Division_Factor == 16) { DivideClockSpeedBySixteen(); }
  else if (Clock_Division_Factor == 32) { DivideClockSpeedByThirtyTwo(); }
  else if (Clock_Division_Factor == 64) { DivideClockSpeedBySixtyFour(); }
  else if (Clock_Division_Factor == 128) { DivideClockSpeedByOneHundredTwentyEight(); }
  else if (Clock_Division_Factor == 256) { DivideClockSpeedByTwoHundredFiftySix(); }
}

// Entering MCU into Idle Sleep Mode
void  SavePowerClass::IdleMode(Time_Out_Value time)
{
  if (time != SLEEP_FOREVER)
  {
	wdt_enable(time);
	WDTCSR |= (1 << WDIE);	
  }
  EnterSleepMode(SLEEP_MODE_IDLE);
}

// Entering MCU into ADC Noise Reduction Sleep Mode
void  SavePowerClass::ADCNoiseReductionMode(Time_Out_Value time)
{	
  if (time != SLEEP_FOREVER)
  {
	wdt_enable(time);
	WDTCSR |= (1 << WDIE);	
  }
  EnterSleepMode(SLEEP_MODE_ADC);
}

// Entering MCU into Power Down Sleep Mode
void  SavePowerClass::PowerDownMode(Time_Out_Value time)
{	
  if (time != SLEEP_FOREVER)
  {
	wdt_enable(time);
	WDTCSR |= (1 << WDIE);	
  }
  EnterSleepMode(SLEEP_MODE_PWR_DOWN);
}

// Entering MCU into Power Save Sleep Mode
void  SavePowerClass::PowerSaveMode(Time_Out_Value time)
{	
  if (time != SLEEP_FOREVER)
  {
    wdt_enable(time);
    WDTCSR |= (1 << WDIE);	
  }
  EnterSleepMode(SLEEP_MODE_PWR_SAVE);
}

// Entering MCU into Standby Sleep Mode
void  SavePowerClass::StandbyMode(Time_Out_Value time)
{
  if (time != SLEEP_FOREVER)
  {
    wdt_enable(time);
    WDTCSR |= (1 << WDIE);	
  }
  EnterSleepMode(SLEEP_MODE_STANDBY);
}

// Entering MCU into Extended Standby Sleep Mode
void  SavePowerClass::ExtendedStandbyMode(Time_Out_Value time)
{	
  if (time != SLEEP_FOREVER)
  {
    wdt_enable(time);
	WDTCSR |= (1 << WDIE);	
  }
  EnterSleepMode(SLEEP_MODE_EXT_STANDBY);
}

// Disable all microcontroller peripherals
void SavePowerClass::DisableAllModules()
{	
  PRR0 = 0xAD;
  PRR1 = 0x99;
}

// Disable Serial Peripheral Interface (SPI)
void SavePowerClass::DisableSPI()
{
  PRR0 |= (1 << PRSPI);   
}

// Disable USB Interface 
void SavePowerClass::DisableUSB()
{
  PRR1 |= (1 << PRUSB);    
}

// Disable Analog Digital Converter (ADC)
void SavePowerClass::DisableADC()
{
  ADCSRA &= ~(1 << ADEN);
  PRR0 |= (1 << PRADC);   
}

// Disable Analog Comparator (AC) 
void SavePowerClass::DisableAC()
{
  disable_ac();   
}

// Disable Universal Synchronous and Asynchronous Receiver Transmitter (USART)
void SavePowerClass::DisableUSART()
{
  PRR1 |= (1 << PRUSART1);     
}

// Disable Two Wire Interface (TWI or I2C) 
void SavePowerClass::DisableTWI()
{
  PRR0 |= (1 << PRTWI);   
}

// Disable Timer0 
void SavePowerClass::DisableTimer0()
{
  PRR0 |= (1 << PRTIM0);     
}

// Disable Timer1
void SavePowerClass::DisableTimer1()
{
  PRR0 |= (1 << PRTIM1);    
}

// Disable Timer3
void SavePowerClass::DisableTimer3()
{
  PRR1 |= (1 << PRTIM3);     
}

// Disable Timer4
void SavePowerClass::DisableTimer4()
{
  PRR1 |= (1 << PRTIM4);    
}

// Enable all microcontroller peripherals
void SavePowerClass::EnableAllModules()
{
  PRR0 = 0x00;
  PRR1 = 0x00; 
}

// Enable Serial Peripheral Interface (SPI)
void SavePowerClass::EnableSPI()
{
  PRR0 &= ~(1 << PRSPI);     
}

// Enable USB Interface 
void SavePowerClass::EnableUSB()
{
  PRR1 &= ~(1 << PRUSB);    
}

// Enable Analog Digital Converter (ADC)
void SavePowerClass::EnableADC()
{
  PRR0 &= ~(1 << PRADC);  
  ADCSRA |= (1 << ADEN); 
}

// Enable Analog Comparator (AC) 
void SavePowerClass::EnableAC()
{
  enable_ac();    
}

// Enable Universal Synchronous and Asynchronous Receiver Transmitter (USART)
void SavePowerClass::EnableUSART()
{
  PRR1 &= ~(1 << PRUSART1);    
}

// Enable Two Wire Interface (TWI or I2C)
void SavePowerClass::EnableTWI()
{
  PRR0 &= ~(1 << PRTWI);     
}

// Enable Timer0
void SavePowerClass::EnableTimer0()
{
  PRR0 &= ~(1 << PRTIM0);    
}

// Enable Timer1
void SavePowerClass::EnableTimer1()
{
  PRR0 &= ~(1 << PRTIM1);     
}

// Enable Timer3
void SavePowerClass::EnableTimer3()
{
  PRR1 &= ~(1 << PRTIM3);     
}

// Enable Timer4
void SavePowerClass::EnableTimer4()
{
  PRR1 &= ~(1 << PRTIM4);     
}

// Lowest Consumption Method
void SavePowerClass::LowestConsumption(Time_Out_Value time)
{ 
  ACSR |= (1 <<ACD);     
  ADCSRA &= ~(1 << ADEN);
  PRR0 |= (1 << PRADC);  
  if (time != SLEEP_FOREVER)
  {
	wdt_enable(time);
	WDTCSR |= (1 << WDIE);	
  }
  EnterSleepMode(SLEEP_MODE_PWR_DOWN);
  PRR0 &= ~(1 << PRADC);
  ADCSRA |= (1 << ADEN); 
  ACSR &= ~(1 <<ACD);
}

ISR (WDT_vect) {}

#else
  #error "Make sure that the microcontroller is ATMega32U4 or ATMega16u4. This library supports just these two microcontrollers."
#endif

SavePowerClass SavePower;
