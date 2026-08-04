/**
 * @file    independent_channels.c
 * @brief   See independent_channels.h.
 */
#include <stdint.h>
#include "independent_channels.h"
#include "effects.h"
#include "pots.h"
#include "pwm_out.h"

void IndependentChannelsEffect(void)
{
    if (!Effects_IsTriggerActive())
    {
        PwmOut_Stop(PWM_CHANNEL_1);
        PwmOut_Stop(PWM_CHANNEL_2);
        return;
    }

    Pots_t pots = Pots_ReadRawIndependent();
    uint16_t ch1 = Pots_ScaleToPwmIndependent(pots.s_raw);
    uint16_t ch2 = Pots_ScaleToPwmIndependent(pots.l_raw);

    PwmOut_Set(PWM_CHANNEL_1, ch1);
    PwmOut_Set(PWM_CHANNEL_2, ch2);
}
