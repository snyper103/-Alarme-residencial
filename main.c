#include "stm32f1xx.h" // Biblioteca STM
char alarmeLigado = 0;  // Usado para verificar se o alarme ja foi ligado, e assim tocar no maximo 3 vezes de 10 segundos

void msDelay(int ms)    // Gera atraso em ms
{
	volatile int x = 809;

	while ( ms-- > 0 && (GPIOA->IDR & GPIO_IDR_IDR8) )  // Enquanto ms for maior q zero e a porta 8 estiver ligada, faz o loop
	{
		while ( x-- > 0 && (GPIOA->IDR & GPIO_IDR_IDR8) )   // Enquanto x for maior q zero e a porta 8 estiver ligada, faz o loop
			__asm("nop"); // faz "nada"

        x = 809;
	}
}

void gpioConfig(void)   // Configura os registradores
{
    /*
     *  Porta 8 enable/disable do switch (ENTRADA) (B)
     *  Porta 9 acesso da saída do switch  (ENTRADA) (M e J)
    */
    
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Habilita clock do barramento APB2 para GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Habilita clock do barramento APB2 para GPIOC
    
    GPIOC->CRH |= GPIO_CRH_MODE13_1;    // Configura pino PC13 como saída de 2 MHz
    GPIOA->CRH |= ~(GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1);    // Configura pino PA9 como entrada
    GPIOA->CRH |= ~(GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1);    // Configura pino PA11 como entrada

    GPIOA->CRH |= GPIO_CRH_CNF8_1; // pull-up e pull-down
    GPIOA->CRH |= GPIO_CRH_CNF9_1; // pull-up e pull-down
}

void ligaAlarme(void) // Liga o alarme por 10 segundos em intervalos de 1 segundo
{
    register short int i = 0;

    // Toca o speaker e liga a LED três vezes por 10 segundo com intervalos de 1 segundo, até q seja desligado o alarme
    for ( i = 0; i < 3 && !alarmeLigado; ++i )
    {
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        msDelay((int)10e3);
        GPIOC->ODR ^= GPIO_ODR_ODR13; 
        msDelay((int)1e3);

        if ( !(GPIOA->IDR & GPIO_IDR_IDR8) )   // Se a porta 8 estiver desligada, sai do loop
            break;
    }

    alarmeLigado = 1;
}

// Função principal
int main(void)
{
    gpioConfig();
    GPIOC->ODR ^= GPIO_ODR_ODR13;
    alarmeLigado = 0;

    while ( 1 ) // Loop infinito
    {
        if ( (GPIOA->IDR & GPIO_IDR_IDR8) && (GPIOA->IDR & GPIO_IDR_IDR9) ) // Se a porta 8 (enable/disable do switch (B)) e a porta 9 (M e J) estvier ligadas, liga o alarme
            ligaAlarme();

        if ( !(GPIOA->IDR & GPIO_IDR_IDR8) )    // Se a porta 8 estiver desligada, reseta o alarmeLigado. Permitindo reativar o alarme
            alarmeLigado = 0;
    }

    return 0;
}