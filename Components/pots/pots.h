/**
 * @file    pots.h
 * @brief   Reads the two front-panel potentiometers and scales the raw ADC values.
 */
#ifndef POTS_H_
#define POTS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define POTS_ADC_MAX_VALUE 1023U /* 10-bit ADC, see MX_ADC_Init() in adc.c */

/* Raw readings of the two potentiometers, labeled "S" and "L" on the PCB. Range
 * 0..POTS_ADC_MAX_VALUE. What each one controls depends on the active effect -
 * see effects.c. */
typedef struct {
    uint16_t s_raw;
    uint16_t l_raw;
} Pots_t;

/* One fresh ADC sample of both potentiometers. Each input already has a
 * hardware RC low-pass filter on the PCB (see schematic), so no additional
 * software filtering is applied here. */
Pots_t Pots_ReadRaw(void);

/* Scales a raw reading to 0..PWM_MAX_VALUE (see pwm_out.h). Inverted: higher raw
 * value means lower output, reaching exactly 0. */
uint16_t Pots_ScaleToPwm(uint16_t raw);

/* Scales a raw reading to a percentage (0..100), same inversion as Pots_ScaleToPwm(). */
uint8_t Pots_ScaleToPercent(uint16_t raw);

/* Used only by IndependentChannelsEffect (DIP4). Kept fully separate from
 * Pots_ReadRaw()/Pots_ScaleToPwm() above, which the other four effects share -
 * DIP4 is not one of those effects, and its own ADC/scaling behavior must be
 * tunable without ever affecting theirs. */
Pots_t Pots_ReadRawIndependent(void);

/* Same scaling as Pots_ScaleToPwm(), reaching exactly 0, but uses its own
 * separate raw-margin tuning (see pots.c). */
uint16_t Pots_ScaleToPwmIndependent(uint16_t raw);

#ifdef __cplusplus
}
#endif

#endif /* POTS_H_ */
