#include "pulse.h"
#define TP_OVF_FLAG     0x01U
#define TP_COMA_FLAG    0x02U
#define TP_COMB_FLAG    0x04U
#define TP_COM_FLAG     (TP_COMA_FLAG|TP_COMB_FLAG)
pusle_t startA[9];
pusle_t startB[9];
pusle_t stopA[9];
pusle_t stopB[9];
tdw_t times[16];
volatile uint8_t tp_flag;

ISR(TP_OVF_vect)
{
  tp_flag |= TP_OVF_FLAG ;
}

void pulse_init(void)  __attribute__ ((naked)) \
__attribute__ ((section (".init1")));

/**
*/
void pulse_init(void)
{
  PULSE_DDRA = 0x00;
  PULSE_DDRB = 0x00;
  PULSE_PORTA = 0xff;
  PULSE_PORTB = 0xff;

  TRIG_DDR |= _BV(TRIG_PINS);
  TRIG_PORT &= ~_BV(TRIG_PINS);

  TOUT_DDR |= _BV(TOUT_PINS);
  TIN_DDR &= ~_BV(TIN_PINS);
  TIN_PORT |= _BV(TIN_PINS);

  TS_CCRA = _BV(TS_COMA0)|_BV(TS_WGM1);
  TS_CCRB = 0x00;
  TS_OCRA = 0x64;
  TS_CNT = 0x00;

  TP_CCRA = _BV(TP_COMA0);
  TP_CCRB = 0x00;
  TP_CNT = 0x0000;

  LED_DDR |= _BV(LED_PINS);
  LED_PORT &= ~_BV(LED_PINS);

  tp_flag = 0x00;
}

/**
*/
uint8_t pulse_capture (void)
{
  uint8_t sta_a,sta_b,pre_a,pre_b, ia = 0,ib = 0;
  uint16_t tm;
  __builtin_avr_cli();
  for(ia=0;ia<9;ia++)
  {
    startA[ia].sta = 0;
    startA[ia].time = 0;
    startB[ia].sta = 0;
    startB[ia].time = 0;
    stopA[ia].sta = 0;
    stopA[ia].time = 0;
    stopB[ia].sta = 0;
    stopB[ia].time = 0;
  }
  for(ia = 0;ia<16;ia++)
  {
    times[ia].td = 0;
    times[ia].tw = 0;
  }
  ia = 0;
  TOUT_PORT |= _BV(TOUT_PINS);
  LED_PORT |= _BV(LED_PINS);
  TRIG_PORT |= _BV(TRIG_PINS);
  TS_CCRB |= _BV(TS_CS1);
  TP_CCRB |= _BV(TP_CS0)|_BV(TP_CS1)|_BV(TP_CS2);
  TP_TIMSK |= _BV(TP_TOIE);
  tp_flag = 0x00;
  pre_a = PULSE_ORIG;
  pre_b = PULSE_ORIG;
  __builtin_avr_sei();
  while(((tp_flag&TP_OVF_FLAG) != TP_OVF_FLAG)&&((tp_flag & TP_COM_FLAG)!=TP_COM_FLAG))
  {
    sta_a = PULSE_PINA;
    sta_b = PULSE_PINB;
    if((sta_a != pre_a)||(sta_b != pre_b))
    {
      tm = TP_CNT;
      if((uint8_t)(sta_a & pre_a) != pre_a)
      {
        startA[ia].time = tm;
        startA[ia].sta = sta_a ^ pre_a;
        pre_a = sta_a;
        if( ia < 8)
        {
          ia++;
        }
      }
      else if((uint8_t)(sta_a | pre_a) != pre_a)
      {
        startA[ib].time = tm;
        startA[ib].sta = sta_a ^ pre_a;
        pre_a = sta_a;
        if( ia < 8)
        {
          ia++;
        }
        if(pre_a == PULSE_ORIG)
        {
          tp_flag |= TP_COMA_FLAG;
        }
      }
      else
      {
        ;/**/
      }
      if((uint8_t)(sta_b & pre_b) != pre_b)
      {
        startB[ib].time = tm;
        startB[ib].sta = sta_b ^ pre_b;
        pre_b = sta_b;
        if( ib < 8)
        {
          ib++;
        }
      }
      else if((uint8_t)(sta_b | pre_b) != pre_b)
      {
        startB[ib].time = tm;
        startB[ib].sta = sta_b ^ pre_b;
        pre_b = sta_b;
        if( ib < 8)
        {
          ib++;
        }
        if(pre_b == PULSE_ORIG)
        {
          tp_flag |= TP_COMB_FLAG;
        }
      }
      else
      {
        ;/**/
      }
    }
  }
  TS_CCRB = 0x00;
  TS_CNT = 0x00;
  TP_CCRB = 0x00;
  TP_CNT = 0x0000;
  TP_TIMSK &= ~_BV(TP_TOIE);
  TRIG_PORT &= ~_BV(TRIG_PINS);
  LED_PORT  &= ~_BV(LED_PINS);
  if(tp_flag & TP_COM_FLAG)
  {
    if(tp_flag & TP_COMA_FLAG)
    {
      ia = 0;
      pre_a = startA[ia].sta;
      while((pre_a != 0)&&(ia < 8))
      {
        sta_b = 0x01U;
        for(ib = 0;ib < 8U;ib++)
        {
            if(((sta_b & pre_a)!=0)&&(times[ib].td == 0))
            {
              times[ib].td = startA[ib].time;
            }
            sta_b <<= 1;
        }
        ia++;
        pre_a = startA[ia].sta;
      }
      ia = 0;
      pre_a = stopA[ia].sta;
      while((pre_a != 0)&&(ia < 8))
      {
        sta_b =0x01U;
        for(ib = 0;ib < 8U;ib++)
        {
            if(((sta_b & pre_a)!=0)&&(times[ib].tw == 0))
            {
              times[ib].tw = stopA[ib].time;
            }
            sta_b <<= 1;
        }
        ia++;
        pre_a = stopA[ia].sta;
      }
    }
    if(tp_flag & TP_COMB_FLAG)
    {
      ia = 0;
      pre_a = startB[ia].sta;
      while((pre_a != 0)&&(ia < 8))
      {
        sta_b =0x01U;
        for(ib = 0;ib < 8U;ib++)
        {
            if(((sta_b & pre_a)!=0)&&(times[ib+8].td == 0))
            {
              times[ib+8].td = startB[ib].time;
            }
            sta_b <<= 1;
        }
        ia++;
        pre_a = startB[ia].sta;
      }
      ia = 0;
      pre_a = stopB[ia].sta;
      while((pre_a != 0)&&(ia < 8))
      {
        sta_b =0x01U;
        for(ib = 0;ib < 8U;ib++)
        {
            if(((sta_b & pre_a)!=0)&&(times[ib+8].tw == 0))
            {
              times[ib+8].tw = stopB[ib].time;
            }
            sta_b <<= 1;
        }
        ia++;
        pre_a = stopB[ia].sta;
      }
    }
  }
  return tp_flag;
}

tdw_t *get_times(void)
{
  return times;
}
