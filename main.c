#include "main.h"
#include "uc_configuration.h"
float voltage;
float temperature;
int distance_max = 300;
int distance_min=25;
int pourcentage_progression=0;
int valeur_division = 0;
int valeur_curseur=0;
int valeur_curseur_secondaire=0;
/*!
 * Entry point of your source code
 */
uint8_t test=0;//Permet de ne faire qu'une seule fois la routine d'initialisation du LCD


int main()
{

    //! Configuration du microcontrolleur (GPIOs/Peripheriques/Horloge)


	ConfigureMicroController();
    while(1)
    {
    	if(test==0){
    		LCD_Configuration(0b00000011);
    						for(int i = 0; i<1e4;i++);
    						LCD_Configuration(0b00000011);
    						for(int i = 0; i<1e4;i++);
    						LCD_Configuration(0b00000011);
    						for(int i = 0; i<1e4;i++);
    						LCD_Configuration(0b00000010);

    						LCD_Configuration(0b00000010);
    						LCD_Configuration(0b00001000);

    						LCD_Configuration(0b00000000);
    						LCD_Configuration(0b00001000);

    						LCD_Configuration(0b00000000);
    						LCD_Configuration(0b00000001);

    						LCD_Configuration(0b00000000);
    						LCD_Configuration(0b00000111);
    						for(unsigned i =0; i<1e5;i++);//delay
    						LCD_Configuration(0b00000000);//Return Home
    						LCD_Configuration(0b00000010);
    						for(unsigned i =0; i<1e5;i++);//delay
    						LCD_Configuration(0b00000000);//Display ON/OFF Control
    						LCD_Configuration(0b00001100);
    		test++;


//
    	}
    	void EXTI9_5_IRQHandler(void) {
    	    if (EXTI->PR1 & EXTI_PR1_PIF7) { // Vérifier si EXTI7 a généré l'interruption
    	        EXTI->PR1 |= EXTI_PR1_PIF7;  // Effacer le drapeau d'interruption
    	        menu_demarrage();            // Appeler la fonction
    	    }
    	}

    		menu_demarrage();

    }
}



void recuperation_temperature(){
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

	// Activer l'ADC
	ADC1->CR &= ~ADC_CR_DEEPPWD;       // Désactiver le mode Deep Power Down
	ADC1->CR |= ADC_CR_ADVREGEN;       // Activer le régulateur ADC
	for (volatile int i = 0; i < 10000; i++); // Attendre la stabilisation

	// Activer l'ADC
	ADC1->ISR |= ADC_ISR_ADRDY;        // Effacer le drapeau ADRDY
	ADC1->CR |= ADC_CR_ADEN;           // Activer l'ADC
	while (!(ADC1->ISR & ADC_ISR_ADRDY)); // Attendre l'activation complète
	// Démarrer la conversion
	ADC1->CR |= ADC_CR_ADSTART;        // Lancer la conversion

	while (!(ADC1->ISR & ADC_ISR_EOC)); // Attendre la fin de la conversion

	// Lire et retourner la valeur convertie
	volatile uint32_t temp =(ADC1->DR);// Récupérer la valeur numérique
	float voltage = (ADC1->DR * 4095) / 5000;
	char valeur[3];
	ADC1->SQR1;
	sprintf(valeur, "%lu",ADC1->DR);  // Convertit le compteur en chaîne de caractères
	LCD_Adress(8);
	for(unsigned i =0; i<1e1;i++);
	LCD_Adress(0);
	for(unsigned i =0; i<1e1;i++);
	affichage_mot(valeur);
	for(unsigned i =0; i<1e4;i++);

}





void LCD_Configuration(uint8_t mot)
{
	for (int i = 0; i<= 7; i++)
	{
		uint8_t bit = (mot >> i) & 0x01;

		if(i==0)//DB4 on PB5
		{
			if (bit == 1)
			{
				GPIOB->BSRR = GPIO_BSRR_BS5;
			}
			else
			{
				GPIOB->BSRR = GPIO_BSRR_BR5;
			}
		}
		else if(i==1)//DB5 on PB4
		{
			if (bit == 1)
			{
				GPIOB->BSRR = GPIO_BSRR_BS4;
			}
			else
			{
				GPIOB->BSRR = GPIO_BSRR_BR4;
			}
		}
		else if(i==2)//DB6 on PB10
		{
			if (bit == 1)
			{
				GPIOB->BSRR = GPIO_BSRR_BS10;
			}
			else
			{
				GPIOB->BSRR = GPIO_BSRR_BR10;
			}
		 }
		else if(i==3)//DB7 on PA8
		{
			if (bit == 1)
			{
				GPIOA->BSRR = GPIO_BSRR_BS8;
			}
			else
			{
				GPIOA->BSRR = GPIO_BSRR_BR8;
			}
		}
		else if(i==5)//RS on PA10
		{
			GPIOA->BSRR = GPIO_BSRR_BR10;//0 during the configuration
		}
	}

		//Creation of signal Enable
		GPIOB->BSRR = GPIO_BSRR_BS3;
		GPIOB->BSRR = GPIO_BSRR_BR3;
}

void LCD_Communication(uint8_t mot)
{
	for (int i = 0; i<= 7; i++)
		{
			uint8_t bit = (mot >> i) & 0x01;

			if(i==0)//DB4 on PB5
			{
				if (bit == 1)
				{
					GPIOB->BSRR = GPIO_BSRR_BS5;
				}
				else
				{
					GPIOB->BSRR = GPIO_BSRR_BR5;
				}
			}
			else if(i==1)//DB5 on PB4
			{
				if (bit == 1)
				{
					GPIOB->BSRR = GPIO_BSRR_BS4;
				}
				else
				{
					GPIOB->BSRR = GPIO_BSRR_BR4;
				}
			}
			else if(i==2)//DB6 PB10
			{
				if (bit == 1)
				{
					GPIOB->BSRR = GPIO_BSRR_BS10;
				}
				else
				{
					GPIOB->BSRR = GPIO_BSRR_BR10;
				}
			 }
			else if(i==3)//DB7 on PA8
			{
				if (bit == 1)
				{
					GPIOA->BSRR = GPIO_BSRR_BS8;
				}
				else
				{
					GPIOA->BSRR = GPIO_BSRR_BR8;
				}
			}
			else if(i==5)//RS on PA10
			{
				GPIOA->BSRR = GPIO_BSRR_BS10;//1 during the configuration
			}
		}

			//Creation of signal Enable
			GPIOB->BSRR = GPIO_BSRR_BS3;
			GPIOB->BSRR = GPIO_BSRR_BR3;
}

void LCD_Adress(uint8_t adress)
{
		for (int i = 0; i<= 7; i++)
		{
			uint8_t bit = (adress >> i) & 0x01;

			if(i==0)//DB4 on PB5
			{
				if (bit == 1)
				{
					GPIOB->BSRR = GPIO_BSRR_BS5;
				}
				else
				{
					GPIOB->BSRR = GPIO_BSRR_BR5;
				}
			}
			else if(i==1)//DB5 on PB4
			{
				if (bit == 1)
				{
					GPIOB->BSRR = GPIO_BSRR_BS4;
				}
				else
				{
					GPIOB->BSRR = GPIO_BSRR_BR4;
				}
			}
			else if(i==2)//DB6 PB10
			{
				if (bit == 1)
				{
					GPIOB->BSRR = GPIO_BSRR_BS10;
				}
				else
				{
					GPIOB->BSRR = GPIO_BSRR_BR10;
				}
			 }
			else if(i==3)//DB7 on PA8
			{
				if (bit == 1)
				{
					GPIOA->BSRR = GPIO_BSRR_BS8;
				}
				else
				{
					GPIOA->BSRR = GPIO_BSRR_BR8;
				}

			}
			else if(i==5)//RS on PA10
			{
				GPIOA->BSRR = GPIO_BSRR_BR10;//0 during Adressing
			}
		}
		//Creation of signal Enable
		GPIOB->BSRR = GPIO_BSRR_BS3;
		GPIOB->BSRR = GPIO_BSRR_BR3;
}

void affichage_mot(const char *mot ){

	    for (int i = 0; mot[i] != '\0'; i++) {  // Parcours jusqu'au caractère nul '\0'
	    	if (mot[i]=='a')
	    	{
	    		LCD_Communication(4);
	    		LCD_Communication(1);
	    	}
	    else if (mot[i]=='b'){
	    	LCD_Communication(4);
	        LCD_Communication(2);
	    }
	    else if (mot[i]=='c'){
	    	    	LCD_Communication(4);
	    	        LCD_Communication(3);
	    	    }
	    else if (mot[i]=='d'){
	    	    	LCD_Communication(4);
	    	        LCD_Communication(4);
	    	    }
	    else if (mot[i]=='e'){
	    	    	LCD_Communication(4);
	    	        LCD_Communication(5);
	    	    }
	    else if (mot[i]=='f'){
	    	    	LCD_Communication(4);
	    	        LCD_Communication(6);
	    	    }
	    else if (mot[i]=='g'){
	    	    	LCD_Communication(4);
	    	        LCD_Communication(7);
	    	    }
	    else if (mot[i]=='h'){
	    	    	    	LCD_Communication(4);
	    	    	        LCD_Communication(8);
	    	    	    }
	    else if (mot[i]=='i'){
	    	    	    	LCD_Communication(4);
	    	    	        LCD_Communication(9);
	    	    	    }
	    else if (mot[i]=='j'){
	    	    	    	LCD_Communication(4);
	    	    	        LCD_Communication(10);
	    	    	    }
	    else if (mot[i]=='k'){
	    	    	    	LCD_Communication(4);
	    	    	        LCD_Communication(11);
	    	    	    }
	    else if (mot[i]=='l'){
	    	    	    	LCD_Communication(4);
	    	    	        LCD_Communication(12);
	    	    	    }
	    else if (mot[i]=='m'){
	    	    	    	LCD_Communication(4);
	    	    	        LCD_Communication(13);
	    	    	    }
	    else if (mot[i]=='n'){
	    	    	    	LCD_Communication(4);
	    	    	        LCD_Communication(14);
	    	    	    }
	    else if (mot[i]=='o'){
	    	    	    	LCD_Communication(4);
	    	    	        LCD_Communication(15);
	    	    	    }
	    else if (mot[i]=='p'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(0);
	    	    	    }
	    else if (mot[i]=='q'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(1);
	    	    	    }
	    else if (mot[i]=='r'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(2);
	    	    	    }
	    else if (mot[i]=='s'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(3);
	    	    	    }
	    else if (mot[i]=='t'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(4);
	    	    	    }
	    else if (mot[i]=='u'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(5);
	    	    	    }
	    else if (mot[i]=='v'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(6);
	    	    	    }
	    else if (mot[i]=='w'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(7);
	    	    	    }
	    else if (mot[i]=='x'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(8);
	    	    	    }
	    else if (mot[i]=='y'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(9);
	    	    	    }
	    else if (mot[i]=='z'){
	    	    	    	LCD_Communication(5);
	    	    	        LCD_Communication(10);
	    	    	    }
	    else if (mot[i]=='0'){
	    	    	    	    	LCD_Communication(3);
	    	    	    	        LCD_Communication(0);
	    	    	    	    }
	    else if (mot[i]=='1'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(1);
	    	    	    	    	    }
	    else if (mot[i]=='2'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(2);
	    	    	    	    	    }
	    else if (mot[i]=='3'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(3);
	    	    	    	    	    }
	    else if (mot[i]=='4'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(4);
	    	    	    	    	    }
	    else if (mot[i]=='5'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(5);
	    	    	    	    	    }
	    else if (mot[i]=='6'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(6);
	    	    	    	    	    }
	    else if (mot[i]=='7'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(7);
	    	    	    	    	    }
	    else if (mot[i]=='8'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(8);
	    	    	    	    	    }
	    else if (mot[i]=='9'){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(9);
	    	    	    	    	    	    }
	    else if (mot[i]=='0'){
	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	        LCD_Communication(0);
	    	    	    	    	    }
	    	    else if (mot[i]==1){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(1);
	    	    	    	    	    	    }
	    	    else if (mot[i]==2){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(2);
	    	    	    	    	    	    }
	    	    else if (mot[i]==3){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(3);
	    	    	    	    	    	    }
	    	    else if (mot[i]==4){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(4);
	    	    	    	    	    	    }
	    	    else if (mot[i]==5){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(5);
	    	    	    	    	    	    }
	    	    else if (mot[i]==6){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(6);
	    	    	    	    	    	    }
	    	    else if (mot[i]==7){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(7);
	    	    	    	    	    	    }
	    	    else if (mot[i]==8){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(8);
	    	    	    	    	    	    }
	    	    else if (mot[i]==9){
	    	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	    	        LCD_Communication(9);
	    	    	    	    	    	    	    }
	    else if (mot[i]==':'){
	    	    	    	    	    	    	LCD_Communication(3);
	    	    	    	    	    	        LCD_Communication(10);
	    	    	    	    	    	    }
	    else if (mot[i]==')'){
	    	    	    	    	    	    	    	LCD_Communication(2);
	    	    	    	    	    	    	        LCD_Communication(3);
	    	    	    	    	    	    	    }
	    else if (mot[i]=='$'){
	   	    	    	    	    	    	    	    	LCD_Communication(2);
	   	    	    	    	    	    	    	        LCD_Communication(0);
	   	    	    	    	    	    	    	    }


	    }}

void fonction_chargement(){
	//croissance

	while(1) {
		 for (unsigned i = 0; i < 100000; i++);
		 calcul_dist();
			}
}

void aff(const char* str){
	for(unsigned i =0; i<1;i++);
	LCD_Adress(8);
	for(unsigned i =0; i<1e1;i++);
	LCD_Adress(0);
	affichage_mot(str);
}




void calcul_dist(){


volatile uint32_t pulseCounter = 0;  // Compteur de pulses
char vraiment_pulse[4] = "non";      // Variable pour vérifier si un pulse est détecté

// Fonction de délai en microsecondes
void delay_us(uint32_t us) {
    TIM2->CNT = 0;
    while (TIM2->CNT < us);
}

// Configuration de PA9 en sortie
void configure_GPIO_PA9_output(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  // Activer l'horloge GPIOA
    GPIOA->MODER &= ~GPIO_MODER_MODE9_Msk;  // Effacer les bits de configuration
    GPIOA->MODER |= GPIO_MODER_MODE9_0;     // PA9 en sortie
}

// Configuration de PA9 en entrée
void configure_GPIO_PA9_input(void) {
    GPIOA->MODER &= ~GPIO_MODER_MODE9_Msk;  // PA9 en entrée (reset value = 00)
}

// Configuration de TIM2 pour délai précis en µs
void configure_TIM2(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;  // Activer l'horloge de TIM2
    TIM2->PSC = 3;                    // Prescaler
    TIM2->ARR = 0xFFFF;                    // Période maximale
    TIM2->CR1 |= TIM_CR1_CEN;              // Démarre le timer
}

// Fonction d'affichage (à adapter selon ton écran)


// Fonction principale de récupération de distance

    configure_TIM2();                // Configuration du timer pour les délais

    // Étape 1 : Configurer PA9 en sortie et générer un signal HIGH pendant 3 µs
    configure_GPIO_PA9_output();
    GPIOA->BSRR = GPIO_BSRR_BS9;     // PA9 HIGH
    delay_us(3);                     // Attente de 3 µs
    GPIOA->BSRR = GPIO_BSRR_BR9;     // PA9 LOW

    // Étape 2 : Reconfigurer PA9 en entrée et attendre 750 µs
    configure_GPIO_PA9_input();
    delay_us(750);

    // Étape 3 : Détection de pulses à 40 kHz
    pulseCounter = 0;
    uint32_t timeout = 1000;  // Timeout en µs pour éviter une boucle infinie
    while (timeout--) {
        if (GPIOA->IDR & GPIO_IDR_ID9_Msk) {  // Si PA9 est à l'état haut (détection pulse)
            pulseCounter = 0;
            while (GPIOA->IDR & GPIO_IDR_ID9_Msk) {  // Tant que le pulse est présent
                delay_us(1);  // Délai de 1 µs
                pulseCounter++;  // Incrémente le compteur chaque µs
            }
            int distance_cm = (pulseCounter*0.2323);
            for(unsigned i =0; i<1;i++);
            LCD_Adress(8);
            for(unsigned i =0; i<1e1;i++);
            LCD_Adress(0);
            affichage_mot("$$$$$$$$$$$$$$");
            char str[20];

            	sprintf(str, "%d", distance_cm);
				aff(str);  // Affiche le nombre de µs du pulse détecté
				for(unsigned i =0; i<1;i++);
				LCD_Adress(8);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(3);
				affichage_mot("cm");

				if(distance_cm <=distance_min){
						for(unsigned i =0; i<1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(4);
						affichage_mot("bip$bip$bip");
					}
				if(distance_cm <=2*distance_min){
					for(unsigned i =0; i<1;i++);
					LCD_Adress(9);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(4);
					affichage_mot("bip$bip");
				}
				if(distance_cm <=3*distance_min){
					for(unsigned i =0; i<1;i++);
					LCD_Adress(9);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(4);
					affichage_mot("bip");
				}
				progression(distance_cm);


            //quand on appui sur le bouton bleu on lance la fonction calcul distance
            	if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 || (GPIOB->IDR & GPIO_IDR_ID0) != 0) {
            		lcd_clear();
            		while((GPIOA->IDR & GPIO_IDR_ID4) != 0 || (GPIOB->IDR & GPIO_IDR_ID0) != 0){}//fonction anti rebond
            		            menu_demarrage(); // Appeler la fonction chargement
            		        }


            return;
        }
        delay_us(1);  // Attente entre deux vérifications
    }





}

void progression(int distance){
	pourcentage_progression = (distance_max-distance_min) / 20;
	valeur_division = (distance-distance_min) / pourcentage_progression;


	if(valeur_division >0){
			for(unsigned i =0; i<1;i++);
			LCD_Adress(9);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(4);
			affichage_mot("$$$$$$$");
		}
	// on clear l'affichage de la mesure
	for (int i=0; i<=15; i++){
						for(unsigned i =0; i<1;i++);
						LCD_Adress(12);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(i);
						affichage_mot("$");
				}
				for (int i=0; i<4; i++){
							for(unsigned i =0; i<1;i++);
							LCD_Adress(13);
							for(unsigned i =0; i<1e1;i++);
							LCD_Adress(i);
							affichage_mot("$");
					}

	if(valeur_division <=4 ){
		for (int i=0; i<=15; i++){
					for(unsigned i =0; i<1;i++);
					LCD_Adress(12);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(i);
					affichage_mot(")");
			}
			for (int i=0; i<(4-valeur_division); i++){
						for(unsigned i =0; i<1;i++);
						LCD_Adress(13);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(i);
						affichage_mot(")");
				}
	}else{
		for (int i=0; i<=(19-valeur_division); i++){
		for(unsigned i =0; i<1;i++);
		LCD_Adress(12);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(i);
		affichage_mot(")");
}
}
}
void toto(){
	lcd_clear();
	for(unsigned i =0; i<1;i++);
			LCD_Adress(8);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(0);
			affichage_mot("toto");
}
void fonction_paramètre(){
	valeur_curseur=2;
	while(1){


		//gestion curseur
		switch (valeur_curseur) {
		        case 2:
		        	eff_curseur(12, 0);
		        	eff_curseur(13, 4);
		            curseur(8, 0);
		            break;
		        case 3:
		        	eff_curseur(8, 0);
					eff_curseur(13, 4);
		            curseur(12, 0);
		            break;
		        case 4:
		        	eff_curseur(8, 0);
					eff_curseur(12,0);
		            curseur(13, 4);
		            break;
		        default:
		            // Gestion par défaut si nécessaire
		            break;
		    }

		if ((GPIOB->IDR & GPIO_IDR_ID0) != 0){

					while((GPIOB->IDR & GPIO_IDR_ID0) != 0){}//fonction anti rebond
					switch (valeur_curseur) {
							        case 2:
							        	valeur_curseur=3;
							            break;
							        case 3:
							        	valeur_curseur=4;
							            break;
							        case 4:
										valeur_curseur=2;
										break;

							        default:
							            // Gestion par défaut si nécessaire
							            break;
							    }
			}

		if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur==2 ) {
					lcd_clear();
					while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
					fct_gestion_valeur_max(); // Appeler la fonction paramètre
				}
		if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur==3 ) {
					lcd_clear();
					while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
					fct_gestion_valeur_min(); // Appeler la fonction paramètre
				}
		if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur==4 ) {
					lcd_clear();
					while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
					menu_demarrage(); // Appeler la fonction paramètre
				}

		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(8);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(1);
		affichage_mot("distance$max");

		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(12);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(1);
		affichage_mot("distance$min");

		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(13);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(5);
		affichage_mot("retour");
//		if ((GPIOB->IDR & GPIO_IDR_ID0) != 0) {
//				lcd_clear();
//				while((GPIOB->IDR & GPIO_IDR_ID0) != 0){}//fonction anti rebond
//				menu_demarrage(); // Appeler la fonction chargement
//					        }

	}
}

void menu_demarrage(){


	lcd_clear();

	valeur_curseur=0;
	while(1){
		void EXTI9_5_IRQHandler(void) {
						    if (EXTI->PR1 & EXTI_PR1_PIF7) {   // Vérifier si l'interruption provient de PC7
						        EXTI->PR1 |= EXTI_PR1_PIF7;    // Effacer le drapeau d'interruption
						        toto();                        // Appeler la fonction
						    }
						}
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(12);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(1);
		affichage_mot("lancer$le$compteur");

		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(13);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(5);
		affichage_mot("parametres");

		//gestion du curseur
		if (valeur_curseur==0){
			curseur(12,0);
		}

		if (valeur_curseur==1){
				curseur(13,4);
			}
		//quand on appui sur le bouton bleu on lance une fonction
		if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur==0 ) {
			lcd_clear();
			while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
			fonction_chargement(); // Appeler la fonction chargement
		}

		if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur==1 ) {
			lcd_clear();
			while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
			fonction_paramètre(); // Appeler la fonction paramètre
		}

		if ((GPIOB->IDR & GPIO_IDR_ID0) != 0){
			lcd_clear();
			while((GPIOB->IDR & GPIO_IDR_ID0) != 0){}//fonction anti rebond
			if(valeur_curseur==0)
			{
				valeur_curseur++;
			}
			else
			{
				valeur_curseur--;
			}
			}



	}
}
void fct_gestion_valeur_max(){
	lcd_clear();
	valeur_curseur_secondaire=0;
	curseur(12,0);
	while(1){
		if ((GPIOB->IDR & GPIO_IDR_ID0) != 0){
					lcd_clear();
					while((GPIOB->IDR & GPIO_IDR_ID0) != 0){}//fonction anti rebond
					switch(valeur_curseur_secondaire)
					{
					case 0:
						curseur(9,4);
						valeur_curseur_secondaire=1;


						break;
					case 1:
						valeur_curseur_secondaire=2;
						curseur(13,4);
						break;
					case 2:
						valeur_curseur_secondaire=0;
						curseur(12,0);
						break;
					}

			}


			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(8);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(1);
			affichage_mot("distance$max");

			char str[20];
			sprintf(str, "%d", distance_max);

			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(9);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(0);
			affichage_mot(str);

			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(12);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(1);
			affichage_mot("plus");


			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(9);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(5);
			affichage_mot("moins");


			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(13);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(5);
			affichage_mot("retour");

// augmente la distance max
			if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur_secondaire==0 ) {

						while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
						distance_max=distance_max+10;
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
						affichage_mot("$$$");
						char str[20];
						sprintf(str, "%d", distance_max);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
						affichage_mot(str);

					}
// diminue la distance max
			if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur_secondaire==1 ) {

						while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
						distance_max=distance_max-10;
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
						affichage_mot("$$$");
						char str[20];
						sprintf(str, "%d", distance_max);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
						affichage_mot(str);
					}
// ramene au menu parametre
						if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur_secondaire==2 ) {
									lcd_clear();
									while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
									fonction_paramètre(); // Appeler la fonction paramètre
								}
}
}

void fct_gestion_valeur_min(){

	lcd_clear();
	valeur_curseur_secondaire=0;
	curseur(12,0);
	while(1){
		if ((GPIOB->IDR & GPIO_IDR_ID0) != 0){
					lcd_clear();
					while((GPIOB->IDR & GPIO_IDR_ID0) != 0){}//fonction anti rebond
					switch(valeur_curseur_secondaire)
					{
					case 0:
						curseur(9,4);
						valeur_curseur_secondaire=1;


						break;
					case 1:
						valeur_curseur_secondaire=2;
						curseur(13,4);
						break;
					case 2:
						valeur_curseur_secondaire=0;
						curseur(12,0);
						break;
					}

			}


			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(8);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(1);
			affichage_mot("distance$min");

			char str[20];
			sprintf(str, "%d", distance_min);

			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(9);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(0);
			affichage_mot(str);

			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(12);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(1);
			affichage_mot("plus");


			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(9);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(5);
			affichage_mot("moins");


			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(13);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(5);
			affichage_mot("retour");

// augmente la distance max
			if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur_secondaire==0 ) {

						while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
						distance_min=distance_min+10;
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
						affichage_mot("$$$");
						char str[20];
						sprintf(str, "%d", distance_min);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
						affichage_mot(str);

					}
// diminue la distance max
			if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur_secondaire==1 ) {

						while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
						distance_min=distance_min-10;
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
						affichage_mot("$$$");
						char str[20];
						sprintf(str, "%d", distance_max);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(9);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
						affichage_mot(str);
					}
// ramene au menu parametre
						if ((GPIOA->IDR & GPIO_IDR_ID4) != 0 && valeur_curseur_secondaire==2 ) {
									lcd_clear();
									while((GPIOA->IDR & GPIO_IDR_ID4) != 0){}//fonction anti rebond
									fonction_paramètre(); // Appeler la fonction paramètre
								}
}

}

void init_lcd(){
				LCD_Configuration(0b00000011);
				for(int i = 0; i<1e4;i++);
				LCD_Configuration(0b00000011);
				for(int i = 0; i<1e4;i++);
				LCD_Configuration(0b00000011);
				for(int i = 0; i<1e4;i++);
				LCD_Configuration(0b00000010);

				LCD_Configuration(0b00000010);
				LCD_Configuration(0b00001000);

				LCD_Configuration(0b00000000);
				LCD_Configuration(0b00001000);

				LCD_Configuration(0b00000000);
				LCD_Configuration(0b00000001);

				LCD_Configuration(0b00000000);
				LCD_Configuration(0b00000111);
				for(unsigned i =0; i<1e5;i++);//delay
				LCD_Configuration(0b00000000);//Return Home
				LCD_Configuration(0b00000010);
				for(unsigned i =0; i<1e5;i++);//delay
				LCD_Configuration(0b00000000);//Display ON/OFF Control
				LCD_Configuration(0b00001100);
}

void lcd_clear(){
	for(unsigned b =8; b<16;b++){
		for(unsigned a =0; a<16;a++){
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(b);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(a);
			affichage_mot("$");
		}
	}

}

void curseur(int x, int y){
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(x);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(y);
		LCD_Communication(2);
		LCD_Communication(10);
}

void eff_curseur(int x, int y){
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(x);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(y);
		LCD_Communication(2);
		LCD_Communication(0);
}

void EXTI15_10_IRQHandler()//Interrupt for Rotary Encoder
{
	if(GPIOC->IDR & GPIO_IDR_ID12_Msk)//If B signal is High level
	{
		//Clock wise Rotation

		if(1==1)
		{
		toto();		}
		else
		{

			toto();
		}
	}
	else
	{
		//Unclock wise Rotation

		if(1==1)
		{
			toto();
		}
		else
		{

			toto();
		}
	}
	EXTI->PR1 = EXTI_PR1_PIF10;//Clear flag
}

