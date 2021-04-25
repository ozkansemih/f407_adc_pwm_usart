#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f4xx_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f4xx_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f4xx_adc.h"            

uint32_t msTick;
uint16_t Read_ADC(void)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1, ADC_SampleTime_56Cycles); 
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET); 
	return ADC_GetConversionValue(ADC1);	
}
void ADC_inits(){
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1 , ENABLE );
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB , ENABLE );
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOA , ENABLE );
	GPIO_InitTypeDef gp;
	
	gp.GPIO_Mode = GPIO_Mode_AN;
	gp.GPIO_Pin = GPIO_Pin_0;
	gp.GPIO_OType = GPIO_OType_PP;
	gp.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init ( GPIOA , & gp );
	
	ADC_InitTypeDef ad;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; // 4/8/16.. vs olabilir kristalin 4 de 1 i seçtik
	ADC_CommonInit(& ADC_CommonInitStructure);
	
	ad.ADC_Resolution = ADC_Resolution_12b; //8bitlik okuma islemi yapilacak
	ADC_Init(ADC1, & ad);
	ADC_Cmd(ADC1, ENABLE);
	
}
void USART_Puts(USART_TypeDef* USARTx, volatile char	 *data){
	while(*data){
		while(!(USARTx->SR & 0x00000040));
			USART_SendData(USARTx, *data);
			data++;
	}
}
void usart_init(){
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOA , ENABLE );
	GPIO_InitTypeDef gp;
	gp.GPIO_Pin = GPIO_Pin_2;
	gp.GPIO_Mode = GPIO_Mode_AF;
	gp.GPIO_OType = GPIO_OType_OD;
	gp.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init ( GPIOA , & gp );

	GPIO_PinAFConfig ( GPIOA , GPIO_PinSource2 ,GPIO_AF_USART2 );

	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_USART2 , ENABLE );
	USART_InitTypeDef usa;
	usa.USART_BaudRate = 115200;
	usa.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usa.USART_Mode = USART_Mode_Tx;
	usa.USART_Parity = USART_Parity_No;
	usa.USART_StopBits = USART_StopBits_1;
	usa.USART_WordLength = USART_WordLength_8b ;
	USART_Init ( USART2 , & usa );
	USART_Cmd ( USART2 , ENABLE );

}

void pwm_test1(){

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE );
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM4 , ENABLE );

	GPIO_InitTypeDef gp;
	TIM_TimeBaseInitTypeDef timb;
	TIM_OCInitTypeDef timoc;


	gp.GPIO_Mode = GPIO_Mode_AF;
	gp.GPIO_OType = GPIO_OType_PP;
	gp.GPIO_PuPd = GPIO_PuPd_UP;
	gp.GPIO_Speed = GPIO_Speed_100MHz;
	gp.GPIO_Pin = GPIO_Pin_12  | GPIO_Pin_13  | GPIO_Pin_14  | GPIO_Pin_15 ;
	GPIO_Init ( GPIOD , & gp );

	GPIO_PinAFConfig ( GPIOD , GPIO_PinSource12 , GPIO_AF_TIM4 );
	GPIO_PinAFConfig ( GPIOD , GPIO_PinSource13 , GPIO_AF_TIM4 );
	GPIO_PinAFConfig ( GPIOD , GPIO_PinSource14 , GPIO_AF_TIM4 );
	GPIO_PinAFConfig ( GPIOD , GPIO_PinSource15 , GPIO_AF_TIM4 );

	timb.TIM_Prescaler = 254 ;
	timb.TIM_Period = 249;
	timb.TIM_CounterMode = TIM_CounterMode_Up;
	timb.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM4 , &timb );

	timoc.TIM_OCMode = TIM_OCMode_PWM1 ;
	timoc.TIM_OutputState = TIM_OutputNState_Enable ;
	timoc.TIM_Pulse = 200;
	timoc.TIM_OCPolarity = TIM_OCPolarity_High ;

	TIM_OC1Init( TIM4 , &timoc );
	TIM_OC1PreloadConfig(TIM4 , TIM_OCPreload_Enable );	

	TIM_OC2Init( TIM4 , &timoc );
	TIM_OC2PreloadConfig(TIM4 , TIM_OCPreload_Enable );	

	TIM_OC3Init( TIM4 , &timoc );
	TIM_OC3PreloadConfig(TIM4 , TIM_OCPreload_Enable );	

	TIM_OC4Init( TIM4 , &timoc );
	TIM_OC4PreloadConfig(TIM4 , TIM_OCPreload_Enable );	
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	TIM_Cmd(TIM4, ENABLE);
}
void pwm_test2(){

	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB , ENABLE );
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOD , ENABLE );
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_TIM10 , ENABLE );

	GPIO_InitTypeDef gp;
	gp.GPIO_Mode = GPIO_Mode_AF ;
	gp.GPIO_OType = GPIO_OType_PP ;
	gp.GPIO_PuPd = GPIO_PuPd_UP ;
	gp.GPIO_Pin = GPIO_Pin_8 ;
	gp.GPIO_Speed = GPIO_Speed_100MHz ;
	GPIO_Init ( GPIOB , &gp );
	GPIO_PinAFConfig ( GPIOB , GPIO_PinSource8 , GPIO_AF_TIM10 );


	TIM_TimeBaseInitTypeDef timb;
	TIM_OCInitTypeDef timoc;

	timb.TIM_ClockDivision = 0;
	timb.TIM_CounterMode = TIM_CounterMode_Up ;
	timb.TIM_Period = 249;
	timb.TIM_Prescaler = 200 ;
	TIM_TimeBaseInit ( TIM10 , &timb );

	timoc.TIM_OCMode = TIM_OCMode_PWM1 ;
	timoc.TIM_OCNPolarity = TIM_OCPolarity_High ;
	timoc.TIM_OutputState = ENABLE ;
	timoc.TIM_Pulse = 29;
	TIM_OC1Init ( TIM10 , & timoc );

	TIM_OC1PreloadConfig ( TIM10, TIM_OCPreload_Enable );
	TIM_ARRPreloadConfig ( TIM10 , ENABLE );

	TIM_Cmd ( TIM10 , ENABLE );

}
void delay(int ms );
int flag = 1;

uint16_t adc_data; // 8 bitlik adc verisi saklayacak degisken
char adc_str[10]= {0};
int adc_val[10] = {0};
int main () {

	SysTick_Config( SystemCoreClock / 1000 );
	SystemCoreClockUpdate();

	pwm_test2();
	ADC_inits();
	usart_init();

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE );

	GPIO_InitTypeDef gp;

	gp.GPIO_Mode = GPIO_Mode_OUT;
	gp.GPIO_OType = GPIO_OType_PP;
	gp.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gp.GPIO_Speed = GPIO_Speed_100MHz;
	gp.GPIO_Pin = GPIO_Pin_12  | GPIO_Pin_13  | GPIO_Pin_14  | GPIO_Pin_15 ;
	GPIO_Init ( GPIOD , & gp );
	int i = 0;
	
	while ( 1 ) {


		adc_data = Read_ADC();
		adc_val [ i%10 ] = adc_data;
		delay( 10);
		if (  i++ %10 == 0 ) {

			int tot = 0;
			for ( int k = 0;  k<10;k++  ){
				tot += adc_val [k ] ;
			}
			
			tot /= 10;
			adc_str[0] =  '*';
			adc_str[1] = (tot /1000 ) %10 +'0';
			adc_str[2] = (tot /100 ) %10+'0';
			adc_str[3] = (tot /10 ) %10+'0';
			adc_str[4] = (tot /1 ) %10+'0';
			adc_str[5] = 13 ;
			adc_str[6] = 10 ;
			adc_str[7] = 0 ;
			USART_Puts ( USART2 ,  adc_str);
		}
		
		if ( adc_data < 820 ){
			GPIO_SetBits ( GPIOD , GPIO_Pin_12 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_13 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_14 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_15 );
		}
		else if ( adc_data < 1640 ){
			GPIO_SetBits ( GPIOD , GPIO_Pin_13);
			GPIO_ResetBits ( GPIOD , GPIO_Pin_12 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_14 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_15 );
		}		
		else if ( adc_data < 2400 ){
			GPIO_SetBits ( GPIOD , GPIO_Pin_14);
			GPIO_ResetBits ( GPIOD , GPIO_Pin_12 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_13 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_15 );
		}
		else if ( adc_data < 3200 ){
			GPIO_SetBits ( GPIOD , GPIO_Pin_15);
			GPIO_ResetBits ( GPIOD , GPIO_Pin_12 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_14 );
			GPIO_ResetBits ( GPIOD , GPIO_Pin_13 );
		}
		else if ( adc_data < 4096 ){
			GPIO_SetBits ( GPIOD , GPIO_Pin_12);
			GPIO_SetBits ( GPIOD , GPIO_Pin_13);
			GPIO_SetBits ( GPIOD , GPIO_Pin_14);
			GPIO_SetBits ( GPIOD , GPIO_Pin_15);

		}
		
		if ( flag == 1 ) {
			TIM10->ARR -=1 ;
		} 
		else if ( flag == 0 ) {
			TIM10->ARR+= 1;
		}
		if ( TIM10->ARR == 20 | TIM10->ARR == 250) {
			flag ^= 0x01;
		}
	}
}

void delay(int ms ){
	
	uint32_t crr= msTick;
	
	while( msTick - crr < ms ){
	__nop();
	}

}
void SysTick_Handler(){

	msTick++;
}
