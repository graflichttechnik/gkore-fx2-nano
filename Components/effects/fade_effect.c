/**
 * @file    fade_effect.c
 * @brief   See fade_effect.h.
 */
#include <stdint.h>
#include "fade_effect.h"
#include "effects.h"
#include "pots.h"
#include "pwm_out.h"

#define FADE_MIN_RAMP_MS 50U   /* ramp duration (each of down/up) at S = CW (fastest) */
#define FADE_MAX_RAMP_MS 2000U /* ramp duration (each of down/up) at S = CCW (slowest) */

void FadeEffect(void)
{
    static int16_t lastPwm1 = 0;
    static int16_t lastPwm2 = 0;
    static bool channel1Lit = true;

    if (!Effects_IsTriggerActive())
    {
        PwmOut_Stop(PWM_CHANNEL_1);
        PwmOut_Stop(PWM_CHANNEL_2);
        /* Keep the ramp state matching actual hardware state (off), so the
         * next trigger activation ramps up from 0 instead of jumping straight
         * to whatever value was last held before the trigger went inactive. */
        lastPwm1 = 0;
        lastPwm2 = 0;
        return;
    }

    Pots_t pots = Pots_ReadRaw();
    uint16_t level = Pots_ScaleToPwm(pots.l_raw);
    uint16_t speed = Pots_ScaleToPwm(pots.s_raw);
    uint16_t rampMs = (uint16_t)(FADE_MIN_RAMP_MS +
        ((uint32_t)speed * (FADE_MAX_RAMP_MS - FADE_MIN_RAMP_MS)) / PWM_MAX_VALUE);

    if (channel1Lit)
    {
        PwmOut_StepToward(PWM_CHANNEL_1, &lastPwm1, 0, rampMs);
        PwmOut_StepToward(PWM_CHANNEL_2, &lastPwm2, (int16_t)level, rampMs);
    }
    else
    {
        PwmOut_StepToward(PWM_CHANNEL_2, &lastPwm2, 0, rampMs);
        PwmOut_StepToward(PWM_CHANNEL_1, &lastPwm1, (int16_t)level, rampMs);
    }

    channel1Lit = !channel1Lit;
}
