/**
 * @file    cct_dimming.c
 * @brief   See cct_dimming.h.
 */
#include <stdint.h>
#include "cct_dimming.h"
#include "effects.h"
#include "pots.h"
#include "pwm_out.h"

void CctDimmingEffect(void)
{
    if (!Effects_IsTriggerActive())
    {
        PwmOut_Stop(PWM_CHANNEL_1);
        PwmOut_Stop(PWM_CHANNEL_2);
        return;
    }

    Pots_t pots = Pots_ReadRaw();
    uint16_t total = Pots_ScaleToPwm(pots.l_raw);
    uint16_t mix = Pots_ScaleToPwm(pots.s_raw);

    uint16_t ch1 = (uint16_t)(((uint32_t)total * mix) / PWM_MAX_VALUE);
    uint16_t ch2 = (uint16_t)(total - ch1);

    PwmOut_Set(PWM_CHANNEL_1, ch1);
    PwmOut_Set(PWM_CHANNEL_2, ch2);
}
