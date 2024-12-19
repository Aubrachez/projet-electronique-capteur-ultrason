#include "main.h"
#include "uc_configuration.h"

#define WAIT_TIME 500;


void ConfigureMicroController() {
    // Configuration de base (ne pas modifier)

	__disable_irq();

    SystemInit();
    EnableClockToPeripherals();

    // Configuration utilisateur (peut être modifiée)
    SetupGPIOs();
    InitializeTimer2();
    InitializeTimer3();
    ADC_config();
    EnableInterrupts();
    ConfigureModulationTimer();
    SetupGPIOsPmw();
    __enable_irq();
}


void SystemInit(void)
{
  /* Paramètres FPU: Donne un accès complet au FPU ------------------------------------*/
  SCB->CPACR |= (3UL << 20U)|(3UL << 22U);  /* Accès complet CP10 et CP11 */
}

void InitializeTimer2() {
	//! Prescaler for a counting period of 1ms (internal clock: 4MHz => Prescaler : /4000)
	//! By default, AHB Prescaler and APBx Prescalers are configured to /1
	TIM2->PSC = 20-1;

	// Specify the ARR value to 1000, for the counter to reset after 1 second
	TIM2->ARR =500*2;
	// Specify the compare value of channel 1 to 500 (half of the period)
	TIM2->CCR1 =WAIT_TIME ;

	// Specify that TIM2 should function as a PWM generator
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
	TIM2->CCMR1 |= 0b0110UL << TIM_CCMR1_OC1M_Pos;

	TIM2->CCMR1 &= ~TIM_CCMR1_OC1PE_Msk;
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

	// Enable Channel 1
	TIM2->CCER |= TIM_CCER_CC1E;

	// Enable counter
	TIM2->CR1 |= TIM_CR1_CEN_Msk;
}

/*!
 * Activation de l'horloge pour les périphériques princpipaux
 */
void EnableClockToPeripherals() {

	//! Activation horloge DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMA2EN;

    //! Activation de l'horloge pour tous les GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN
    			  | RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOFEN
				  | RCC_AHB2ENR_GPIOGEN | RCC_AHB2ENR_GPIOHEN;


    //! Activation de l'horloge pour l'ADC
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

    //! Activation de l'horloge pour TIM2, TIM3, TIM4, TIM5, TIM6, TIM6
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN | RCC_APB1ENR1_TIM4EN
    		       | RCC_APB1ENR1_TIM5EN | RCC_APB1ENR1_TIM6EN | RCC_APB1ENR1_TIM7EN;

    //! Activation de l'horloge pour UART2, all I2C et DAC1
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN | RCC_APB1ENR1_I2C1EN | RCC_APB1ENR1_I2C2EN
    		      | RCC_APB1ENR1_I2C3EN | RCC_APB1ENR1_DAC1EN;

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

}

void EnableInterrupts(){
	 	NVIC_EnableIRQ(TIM2_IRQn);             // Activer l'interruption Timer 2
		NVIC_EnableIRQ(EXTI9_5_IRQn);

	}

void SetupGPIOs()
{
	// Interrupt on rising edge
	EXTI->RTSR1 |= 1 << 7;   // Configurer le front montant pour EXTI7 (PC7)
	// And on falling edge
	EXTI->FTSR1 |= 1 << 7;   // Configurer le front descendant pour EXTI7 (PC7)
	// Enable interrupts for this line
	EXTI->IMR1 |= 1 << 7;    // Activer l'interruption pour EXTI7
	// Select port C as interrupt source for EXTI line 7
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PC;  // Configurer PC7 comme source d'interruption pour EXTI7

	//
	// Interrupt on rising edge
		    EXTI->RTSR1 |= 1 << 13;
		    // And on falling edge
		    EXTI->FTSR1 |= 1 << 13;
		    // Enable interrupts for this line
		    EXTI->IMR1 |= 1 << 13;
		    // Select port C as interrupt source for EXTI line 13
		    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	//Configuration de l'écran LCD
	//--------------------------------------------------
		//Configuration de D2 (PA10) comme sortie
		GPIOA->MODER &= ~GPIO_MODER_MODE10_Msk;//RESET les bits dans MODE10
		GPIOA->MODER |= GPIO_MODER_MODE10_0;//SET le bit 20 sur 1 (output mode '01')
		//Configuration de D3 (PB3) comme sortie
		GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;//RESET les bits dans MODE3
		GPIOB->MODER |= GPIO_MODER_MODE3_0;//SET le bit 6 sur 1 (output mode '01')
		//Configuration de D4 (PB5) comme sortie
		GPIOB->MODER &= ~GPIO_MODER_MODE5_Msk;//RESET les bits dans MODE5
		GPIOB->MODER |= GPIO_MODER_MODE5_0;//SET le bit 10 sur 1 (output mode '01')
		//Configuration de D5 (PB4)comme sortie
		GPIOB->MODER &= ~GPIO_MODER_MODE4_Msk;//RESET les bits dans MODE4
		GPIOB->MODER |= GPIO_MODER_MODE4_0;//SET le bit 8 sur 1 (output mode '01')
		//Configuration de D6 (PB10) comme sortie
		GPIOB->MODER &= ~GPIO_MODER_MODE10_Msk;//RESET les bits dans MODE10
		GPIOB->MODER |= GPIO_MODER_MODE10_0;//SET le bit 20 sur 1 (output mode '01')
		//Configuration de D7 (PA8) comme sortie
		GPIOA->MODER &= ~GPIO_MODER_MODE8_Msk;//RESET les bits dans MODE5
		GPIOA->MODER |= GPIO_MODER_MODE8_0;//SET le bit 16 sur 1 (output mode '01')
	//--------------------------------------------------

	//Configuration de l'entrée analogique
		GPIOA->MODER |= GPIO_MODER_MODE0_Msk;//Selection du mode analogique ('11')
		GPIOA->ASCR  |= GPIO_ASCR_ASC0;//Connection entre l'adc et la pin d'entrée

		GPIOA->MODER |= GPIO_MODER_MODE0_Msk;//Selection du mode analogique ('11')
		GPIOA->ASCR  |= GPIO_ASCR_ASC1;//Connection entre l'adc et la pin d'entrée
	//--------------------------------------------------

	// Set the pin PA4 as an input
		 // 2. Configurer PA4 en mode "entrée" dans le registre MODER
		    GPIOA->MODER &= ~(3U << (4 * 2)); // PA4: mettre les bits [9:8] à 00 (mode d'entrée)

		    GPIOA->PUPDR &= ~(3U << (4 * 2)); // PA4: clear bits [9:8]
		      GPIOA->PUPDR |= (2U << (4 * 2));  // PA4: mettre les bits [9:8] à 10 (pull-down)

		      // Set the pin PB0 as an input
		      		 // 2. Configurer PA4 en mode "entrée" dans le registre MODER
		      		 GPIOB->MODER &= ~(3U << (0 * 2)); // PA4: mettre les bits [9:8] à 00 (mode d'entrée)

		      		 GPIOB->PUPDR &= ~(3U << (0 * 2)); // PA4: clear bits [9:8]
		      		 GPIOB->PUPDR |= (2U << (0 * 2));  // PA4: mettre les bits [9:8] à 10 (pull-down)

		      		//Enable interrupt EXTI15_10 in NVIC
		      			//--------------------------------------------------
}

void InitializeTimer3() {

    //! Set un prescaler pour une période de 1 µs (horloge interne: 4MHz => Prescaler : /4000)
    //! Par défaut, les prescaler AHB et APBx sont configurés sur /1
    //TIM3->PSC = 4;

    //Genere un update event pour mettre a jour le prescaler
    // Specifie la valeur à laquelle le conteur se reinitialise
    //TIM3->ARR = WAIT_TIME;

    // Reset le flag d'interruption pour eviter de déclancher une interruption non voulue
    //while(!(TIM3->SR & TIM_SR_UIF_Msk)){} // Pour s'assurer qu'il est bel et bien nécéssaire de reinitialiser le flag d'interruption
    //TIM3->SR &= ~TIM_SR_UIF_Msk; // Reset le flag d'interruption

    // Active l'interruption de mise à jour
    //TIM3->DIER |= TIM_DIER_UIE_Msk;

    // Active l'interruption de TIM3 dans le registre Cortex M4
   // NVIC->ISER[0] = 1 << TIM3_IRQn;

    // Active TIM3
   // TIM3->CR1 |= TIM_CR1_CEN_Msk;

}




void ADC_config(){
	 RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

	 //! Select Sysclk for ADC
	 RCC->CCIPR |= 0b11 << RCC_CCIPR_ADCSEL_Pos;


	 // Configurer PA1 en mode analogique
	 GPIOA->MODER &= ~(3U << (1 * 2));    // Effacer les bits pour PA1
	 GPIOA->MODER |= (3U << (1 * 2));     // Mettre PA1 en mode analogique ('11')

	 // Désactiver les résistances pull-up et pull-down
	 GPIOA->PUPDR &= ~(3U << (1 * 2));    // Désactiver pull-up/pull-down pour PA1




	 //Configuration of the ADC
	 	//--------------------------------------------------


	 		// Configure ADC settings in ADC_CFGR
	 		ADC1->CFGR &= ~ADC_CFGR_DMAEN; // No DMA for ADC
	 		ADC1->CFGR &= ~ADC_CFGR_RES; // Set resolution to 12 bits
	 		ADC1->CFGR &= ~ADC_CFGR_ALIGN; // Right align data
	 		//ADC1->CFGR |= ADC_CFGR_EXTEN_0; // Set external trigger to rising edge
	 		ADC1->CFGR &= ~ADC_CFGR_EXTEN_Msk; // Pas de déclencheur externe
	 		//ADC1->CFGR &= ~ADC_CFGR_CONT; // Clear le bit CONT pour une conversion unique

	 		// Configure ADC channel and sequence
	 		ADC1->SQR1 &= ~ADC_SQR1_L; // Clear L bits
	 		ADC1->SQR1 |= ADC_SQR1_L_Msk;
	 		ADC1->SQR1 &= ~ADC_SQR1_SQ1; // Clear SQ1 bits
	 }

void SetupGPIOsPmw() {

	// Configure le buzzer sur (PA5) en alternate function (PWM from TIM2 channel 1),  voir le tuto du cours pour comprendre la cofiguration
	// Reset PA5's mode of operation
	GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
	// Set PA5 as alternate function
	GPIOA->MODER |= 0b10UL << GPIO_MODER_MODE5_Pos;
	//! Reset alternate function selection for PA5
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5_Msk;
	//! Specify alternate function 1 for PA5
	GPIOA->AFR[0] |= (1UL << GPIO_AFRL_AFSEL5_Pos);
}
