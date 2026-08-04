/**
 * @file    pwm_out.h
 * @brief   Named access to the two PWM outputs driven by TIM2.
 */
#ifndef PWM_OUT_H_
#define PWM_OUT_H_

#include <stdint.h>
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Matches TIM2's ARR (see MX_TIM2_Init() in tim.c): 32 MHz / 1065 = ~30 kHz PWM,
 * above the flicker-perception threshold. Keep both values in sync if ever changed. */
#define PWM_MAX_VALUE 1064U

typedef enum {
    PWM_CHANNEL_1 = TIM_CHANNEL_4, /* PWM_OUT1_Pin, PA3 */
    PWM_CHANNEL_2 = TIM_CHANNEL_3, /* PWM_OUT2_Pin, PA2 */
} PwmChannel_t;

void PwmOut_Start(PwmChannel_t channel);
void PwmOut_Stop(PwmChannel_t channel); /* also zeroes the compare register */

/* Clamped to PWM_MAX_VALUE. Stops the channel outright at value 0 instead of
 * leaving it running at a 0 compare value, and (re)starts it for any nonzero
 * value - avoids a hardware-observed flicker right at the zero boundary. */
void PwmOut_Set(PwmChannel_t channel, uint16_t value);

/* Ramps *state toward target in fixed-size increments spread evenly over durationMs,
 * then snaps exactly to target. Blocking (uses HAL_Delay internally). Works for a
 * jump in either direction. *state must already hold the channel's current value. */
void PwmOut_StepToward(PwmChannel_t channel, int16_t *state, int16_t target, uint16_t durationMs);

#ifdef __cplusplus
}
#endif

#endif /* PWM_OUT_H_ */
