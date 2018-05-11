#include "pulse.h"
#include "uart.h"
#define HALFSENCON (F_CPU/2)
int main(void)
{
  uint8_t tmp;
  for(tmp = 0;tmp<3;tmp++)
  {
    __builtin_avr_delay_cycles(HALFSENCON);
    LED_PORT ^= _BV(LED_PINS);
    __builtin_avr_delay_cycles(HALFSENCON);
    LED_PORT ^= _BV(LED_PINS);
  }
  __builtin_avr_delay_cycles(HALFSENCON);
  (void)pulse_capture();
  return 0;
}
