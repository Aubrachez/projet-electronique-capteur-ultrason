#include "main.h"
#include "uc_configuration.h"
float voltage;
float temperature;

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
    		recuperation_temperature();

//
    	}
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
	float voltage = (temp * 5) / 4095.0;
	char valeur[100];
	sprintf(valeur, "%lu", ADC1->DR);  // Convertit le compteur en chaîne de caractères
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
		// Vérifie si PC13 est à l'état haut (non pressé, car le bouton tire vers 0 lorsqu'appuyé)
			 if(GPIOC->IDR & GPIO_IDR_ID13){}
			 else// PC13 à l'état bas
			        {
			            // Appel de la fonction lorsque le bouton n'est pas pressé (PC13 haut)
			            menu_demarrage();

			            // Délai pour éviter des appels multiples inutiles
			            for (unsigned i = 0; i < 1000; i++); // Attendre ~quelques ms
			        }
	for (int place = 4; place <= 22; place++) {
		if(place<=15){
					LCD_Adress(13);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(place);
					for(unsigned i =0; i<1e1;i++);
					affichage_mot(")");
					for(unsigned i =0; i<1e4;i++);
		}
		if(place>=15){
						LCD_Adress(14);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(place-15);
						for(unsigned i =0; i<1e1;i++);
						affichage_mot(")");
						for(unsigned i =0; i<1e4;i++);
					}

				}
	calcul_dist();
	//décroissance
	for (int place = 23; place >= 4; place--) {
				if(place<=15){
							LCD_Adress(13);
							for(unsigned i =0; i<1e1;i++);
							LCD_Adress(place);
							for(unsigned i =0; i<1e1;i++);
							affichage_mot("$");
							for(unsigned i =0; i<1e4;i++);
				}
				if(place>=15){
								LCD_Adress(14);
								for(unsigned i =0; i<1e1;i++);
								LCD_Adress(place-16);
								for(unsigned i =0; i<1e1;i++);
								affichage_mot("$");
								for(unsigned i =0; i<1e4;i++);
							}

						}
						//for(unsigned i =0; i<1;i++);

						//LCD_Adress(8);
						//for(unsigned i =0; i<1e1;i++);
						//LCD_Adress(0);
						//TIM2->CR1 |= TIM_CR1_CEN_Msk;

						//char str[20];
						//uint32_t timer_value = TIM2->CNT;
						//sprintf(str, "%d", timer_value);
						//affichage_mot(str);
						//TIM2->CNT = 0;
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


void tiktok(){


	// la fréquence de la puce est de 4MHz
	// donc en 1µs elle fait 4 actions

	//Le capteur PING)) détecte les objets en émettant une courte salve d'ultrasons et en « écoutant » l'écho.
	//Sous le contrôle d'un microcontrôleur hôte (impulsion de déclenchement), le capteur émet une brève salve d'ultrasons de 40 kHz.
	//Cette salve se propage dans l'air, frappe un objet et rebondit vers le capteur.   Le capteur PING)))
	//fournit une impulsion de sortie à l'hôte qui se termine lorsque l'écho est détecté.
	//La largeur de cette impulsion correspond donc à la distance de la cible.

	// le pin de controle est D8 = PA9
	// objectif : mettre le pin en mode sortie emetre un signal haut pendant 5 µs seconde
	// le pulse va de 15µs à 18,5 ms
	//il y a un delay de
	//ensuite passer le pin en mode in et lancé le compteur.
	//
	// il faut attendre 200µs entre deux mesure dans notre cas on fera une mesure toute les 100 000µs

	//PA9 en mode sortie
	GPIOA->MODER &= ~GPIO_MODER_MODE9_Msk;
	GPIOA->MODER |= GPIO_MODER_MODE9_0;

	// Mettre PA9 à l'état haut
	GPIOA->BSRR = GPIO_BSRR_BS9;

	// attendre 2µs
	for(unsigned i =0; i<10;i++);
	// Mettre le pin à 0
	GPIOA->BSRR = GPIO_BSRR_BR9;
	for(unsigned i =0; i<2;i++);

	// mettre le pin en mode in
	GPIOA->MODER &= ~GPIO_MODER_MODE9_Msk;

	// attendre 750 µs
	for(unsigned i =0; i<3000;i++);
	//lancé le timer 3

	//TIM3->CR1 |= TIM_CR1_CEN_Msk;
	//commencé à écouté si un signal reviens

	// fin signal detecté = timer fin
	// récupérer valeur timer
	// convertir valeur en distance
	//ajuster distance avec température ambiante
	//affiché distance calculé
	// reinitialiser tim3

	//attendre 0,1s
	//14073 de tim3 = +- 5,73 sec




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
    TIM2->PSC = 3;                    // Prescaler : 80 MHz / 80 = 1 MHz (1 µs)
    TIM2->ARR = 0xFFFF;                    // Période maximale
    TIM2->CR1 |= TIM_CR1_CEN;              // Démarre le timer
}

// Fonction d'affichage (à adapter selon ton écran)
void affichage(uint32_t counter) {
    char str[20];
    sprintf(str, "%lu", counter);  // Convertit le compteur en chaîne de caractères
    aff(str);
    // Code spécifique pour afficher `str` sur ton écran (ex. LCD)
}

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
            if (distance_cm >= 20 ){
            	sprintf(str, "%d", distance_cm);
				aff(str);  // Affiche le nombre de µs du pulse détecté
				for(unsigned i =0; i<1;i++);
				LCD_Adress(8);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(3);
				affichage_mot("cm");
				maj_progression();
				progression(distance_cm);
            }
            else{
            	aff("out$of$range");
            }

            return;
        }
        delay_us(1);  // Attente entre deux vérifications
    }





}

void progression(int distance){
	if (distance > 10) {
		for(unsigned i =0; i<1;i++);
		LCD_Adress(12);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(0);
		affichage_mot(")");
	}
	if (distance > 20){
		for(unsigned i =0; i<1;i++);
		LCD_Adress(12);
		for(unsigned i =0; i<1e1;i++);
		LCD_Adress(1);
	    affichage_mot(")");
	}
	if (distance > 30){
			for(unsigned i =0; i<1;i++);
			LCD_Adress(12);
			for(unsigned i =0; i<1e1;i++);
			LCD_Adress(2);
		    affichage_mot(")");
		}
	if (distance > 40){
				for(unsigned i =0; i<1;i++);
				LCD_Adress(12);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(3);
			    affichage_mot(")");
			}
	if (distance > 50){
				for(unsigned i =0; i<1;i++);
				LCD_Adress(12);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(4);
			    affichage_mot(")");
			}
	if (distance > 60){
				for(unsigned i =0; i<1;i++);
				LCD_Adress(12);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(5);
			    affichage_mot(")");
			}
	if (distance > 70){
				for(unsigned i =0; i<1;i++);
				LCD_Adress(12);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(6);
			    affichage_mot(")");
			}
	if (distance > 80){
				for(unsigned i =0; i<1;i++);
				LCD_Adress(12);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(7);
			    affichage_mot(")");
			}
	if (distance > 90){
				for(unsigned i =0; i<1;i++);
				LCD_Adress(12);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(8);
			    affichage_mot(")");
			}
	if (distance > 100){
				for(unsigned i =0; i<1;i++);
				LCD_Adress(12);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(9);
			    affichage_mot(")");
			}
	if (distance > 110){
				for(unsigned i =0; i<1;i++);
				LCD_Adress(12);
				for(unsigned i =0; i<1e1;i++);
				LCD_Adress(10);
			    affichage_mot(")");
			}
	if (distance > 120){
					for(unsigned i =0; i<1;i++);
					LCD_Adress(12);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(11);
				    affichage_mot(")");
				}
	if (distance > 130){
					for(unsigned i =0; i<1;i++);
					LCD_Adress(12);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(12);
				    affichage_mot(")");
				}
	if (distance > 140){
					for(unsigned i =0; i<1;i++);
					LCD_Adress(12);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(13);
				    affichage_mot(")");
				}
	if (distance > 150){
					for(unsigned i =0; i<1;i++);
					LCD_Adress(12);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(14);
				    affichage_mot(")");
				}
	if (distance > 160){
					for(unsigned i =0; i<1;i++);
					LCD_Adress(12);
					for(unsigned i =0; i<1e1;i++);
					LCD_Adress(15);
				    affichage_mot(")");
				}
	if (distance > 170){
						for(unsigned i =0; i<1;i++);
						LCD_Adress(13);
						for(unsigned i =0; i<1e1;i++);
						LCD_Adress(0);
					    affichage_mot(")");
					}
	if (distance > 180){
							for(unsigned i =0; i<1;i++);
							LCD_Adress(13);
							for(unsigned i =0; i<1e1;i++);
							LCD_Adress(1);
						    affichage_mot(")");
						}
	if (distance > 190){
							for(unsigned i =0; i<1;i++);
							LCD_Adress(13);
							for(unsigned i =0; i<1e1;i++);
							LCD_Adress(2);
						    affichage_mot(")");
						}
	if (distance > 200){
							for(unsigned i =0; i<1;i++);
							LCD_Adress(13);
							for(unsigned i =0; i<1e1;i++);
							LCD_Adress(3);
						    affichage_mot(")");
						}
}

void maj_progression(){
	for(unsigned place =0; place<16;place++){
									for(unsigned i =0; i<1;i++);
									LCD_Adress(12);
									for(unsigned i =0; i<1e1;i++);
									LCD_Adress(place);
								    affichage_mot("$");
	}
	for(unsigned place =0; place<4;place++){
										for(unsigned i =0; i<1;i++);
										LCD_Adress(13);
										for(unsigned i =0; i<1e1;i++);
										LCD_Adress(place);
									    affichage_mot("$");
		}

}

void menu_demarrage(){
	while(1){



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

}
}



void toto(){
	LCD_Adress(8);
	for(unsigned i =0; i<1e1;i++);
	LCD_Adress(0);
	for(unsigned i =0; i<1e1;i++);
	affichage_mot("hello$world");
	for(unsigned i =0; i<1e4;i++);
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

