#ifndef PULSE_H
#define PULSE_H
#include <avr/io.h>
#include <avr/interrupt.h>

#define PULSE_ORIG    0xff
#define PULSE_PORTA   PORTF
#define PULSE_PINA    PINF
#define PULSE_DDRA    DDRF

#define PULSE_PORTB   PORTK
#define PULSE_PINB    PINK
#define PULSE_DDRB    DDRK

#define TRIG_PORT     PORTB
#define TRIG_DDR      DDRB
#define TRIG_PIN      PINB
#define TRIG_PINS     PB5

#if defined(TSOURCE_T0)
#define TOUT_PORT     PORTB
#define TOUT_DDR      DDRB
#define TOUT_PINS     PB7
#define TS_CCRA       TCCR0A
#define TS_CCRB       TCCR0B
#define TS_CNT        TCNT0
#define TS_OCRA       OCR0A
#define TS_CS1        CS01
#define TS_COMA1      COM0A1
#define TS_COMA0      COM0A0
#define TS_WGM1       WGM01
#define LED_PORT      PORTB
#define LED_DDR       DDRB
#define LED_PINS      PB4
#else
#define TOUT_PORT     PORTB
#define TOUT_DDR      DDRB
#define TOUT_PINS     PB4
#define TS_CCRA       TCCR2A
#define TS_CCRB       TCCR2B
#define TS_CNT        TCNT2
#define TS_OCRA       OCR2A
#define TS_CS1        CS21
#define TS_COMA1      COM2A1
#define TS_COMA0      COM2A0
#define TS_WGM1       WGM21
#define LED_PORT      PORTB
#define LED_DDR       DDRB
#define LED_PINS      PB7
#endif

/*T1,T3,T4 or T5 register*/
#if defined(PULSE_T5)
#define TIN_PORT      PORTL
#define TIN_DDR       DDRL
#define TIN_PINS      PL2
#define TP_CCRA       TCCR5A
#define TP_CCRB       TCCR5B
#define TP_CNT        TCNT5
#define TP_CNTL       TCNT5L
#define TP_CNTH       TCNT5H
#define TP_OCRA       OCR5A
#define TP_CS2        CS52
#define TP_CS1        CS51
#define TP_CS0        CS50
#define TP_COMA1      COM5A1
#define TP_COMA0      COM5A0
#define TP_TIFR       TIFR5
#define TP_OVF_vect   TIMER5_OVF_vect
#define TP_TIMSK      TIMSK5
#define TP_TOIE       TOIE5
#elif defined(PULSE_T4)
#define TIN_PORT      PORTH
#define TIN_DDR       DDRH
#define TIN_PINS      PH7
#define TP_CCRA       TCCR4A
#define TP_CCRB       TCCR4B
#define TP_CNT        TCNT4
#define TP_CNTL       TCNT4L
#define TP_CNTH       TCNT4H
#define TP_OCRA       OCR4A
#define TP_CS2        CS42
#define TP_CS1        CS41
#define TP_CS0        CS40
#define TP_COMA1      COM4A1
#define TP_COMA0      COM4A0
#define TP_TIFR       TIFR4
#define TP_OVF_vect   TIMER4_OVF_vect
#define TP_TIMSK      TIMSK4
#define TP_TOIE       TOIE4
#elif defined(PULSE_T3)
#define TIN_PORT      PORTE
#define TIN_DDR       DDRE
#define TIN_PINS      PE6
#define TP_CCRA       TCCR3A
#define TP_CCRB       TCCR3B
#define TP_CNT        TCNT3
#define TP_CNTL       TCNT3L
#define TP_CNTH       TCNT3H
#define TP_OCRA       OCR3A
#define TP_CS2        CS32
#define TP_CS1        CS31
#define TP_CS0        CS30
#define TP_COMA1      COM3A1
#define TP_COMA0      COM3A0
#define TP_TIFR       TIFR3
#define TP_OVF_vect   TIMER3_OVF_vect
#define TP_TIMSK      TIMSK3
#define TP_TOIE       TOIE3
#else
#define TIN_PORT      PORTD
#define TIN_DDR       DDRD
#define TIN_PINS      PD6
#define TP_CCRA       TCCR1A
#define TP_CCRB       TCCR1B
#define TP_CNT        TCNT3
#define TP_CNTL       TCNT1L
#define TP_CNTH       TCNT1H
#define TP_OCRA       OCR1A
#define TP_CS2        CS12
#define TP_CS1        CS11
#define TP_CS0        CS10
#define TP_COMA1      COM1A1
#define TP_COMA0      COM1A0
#define TP_TIFR       TIFR1
#define TP_OVF_vect   TIMER1_OVF_vect
#define TP_TIMSK      TIMSK1
#define TP_TOIE       TOIE1
#endif

typedef struct{
  uint16_t time;
  uint8_t sta;
} pusle_t;

typedef struct{
  uint16_t td;
  uint16_t tw;
}tdw_t;

uint8_t pulse_capture (void);
tdw_t *get_times(void);

#endif
