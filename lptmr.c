#include "MKL46Z4.h"
#include "lptmr.h"
#include "lcd.h"
//#include <stdio.h>

void irclk_ini()
{
  MCG->C1 = MCG_C1_IRCLKEN(1) | MCG_C1_IREFSTEN(1);
  MCG->C2 = MCG_C2_IRCS(0); //0 32KHZ internal reference clock; 1= 4MHz irc
}

void lptmr_clear_registers() {
  LPTMR0->CSR=0x00;
  LPTMR0->PSR=0x00;
  LPTMR0->CMR=0x00;
}

void lptmr_config() {
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
  SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;
}

void lptmr_example() {
  //4000ms
  int compare_value = 4000;
  //int seg = 0;

  //Para facer reset
  lptmr_clear_registers();

  LPTMR0->PSR = (LPTMR_PSR_PRESCALE(0) | LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK);  //Uso bypass prescale

  LPTMR0->CSR = (LPTMR_CSR_TCF_MASK | LPTMR_CSR_TIE_MASK);

  LPTMR0->CMR = LPTMR_CMR_COMPARE(compare_value);  //Poño o valor a comparar

  //NVIC_ClearPendingIRQ(LPTMR_IRQ_NBR); Falla LPTMR_IRQ_NBR
  //NVIC_EnableIRQ(LPTMR_IRQ_NBR);

  //printf("Test reloxo para que conte 4 segundos:\n");

  LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; //Inicio LPTMR

  while((LPTMR0->CSR & LPTMR_CSR_TCF_MASK)==0)
  {
    lcd_display_dec((int)LPTMR0->CNR);
    /*if (((int)LPTMR0->CNR % 1000) == 0)
	seg++;
    printf("O valor de CNR é %d\n",(int)LPTMR0->CNR);*/
  }
  

  //printf("Esperáronse %d ms\n",compare_value);
}

/********************************************************************/
int main (void)
{
  irclk_ini();
  lcd_ini();
  lptmr_config();
  lptmr_example();
}


