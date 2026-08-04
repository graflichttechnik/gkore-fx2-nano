/**
 * @file    blink_effect.c
 * @brief   See blink_effect.h.
 */
#include <stdint.h>
#include "main.h"
#include "blink_effect.h"
#include "effects.h"
#include "pots.h"
#include "pwm_out.h"

#define BLINK_MIN_HALF_PERIOD_MS 50U   /* half-period at S = 0 (fastest) */
#define BLINK_MAX_HALF_PERIOD_MS 2000U /* half-period at S = max (slowest) */

void BlinkEffect(void)
{
    static uint32_t lastToggleMs = 0;
    static bool channel1Lit = true;

    if (!Effects_IsTriggerActive())
    {
        PwmOut_Stop(PWM_CHANNEL_1);
        PwmOut_Stop(PWM_CHANNEL_2);
        return;
    }

    Pots_t pots = Pots_ReadRaw();
    uint16_t level = Pots_ScaleToPwm(pots.l_raw);
    uint16_t speed = Pots_ScaleToPwm(pots.s_raw);
    uint16_t halfPeriodMs = (uint16_t)(BLINK_MIN_HALF_PERIOD_MS +
        ((uint32_t)speed * (BLINK_MAX_HALF_PERIOD_MS - BLINK_MIN_HALF_PERIOD_MS)) / PWM_MAX_VALUE);

    if ((HAL_GetTick() - lastToggleMs) >= halfPeriodMs)
    {
        lastToggleMs = HAL_GetTick();
        channel1Lit = !channel1Lit;
    }

    PwmOut_Set(PWM_CHANNEL_1, channel1Lit ? level : 0);
    PwmOut_Set(PWM_CHANNEL_2, channel1Lit ? 0 : level);
}
