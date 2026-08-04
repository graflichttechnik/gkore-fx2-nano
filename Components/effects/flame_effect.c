/**
 * @file    flame_effect.c
 * @brief   See flame_effect.h.
 */
#include <stdint.h>
#include <stdlib.h>
#include "flame_effect.h"
#include "effects.h"
#include "pots.h"
#include "pwm_out.h"

#define FLAME_RAMP_MIN_MS 50U  /* ramp duration at S = CCW (fast flame) */
#define FLAME_RAMP_MAX_MS 600U /* ramp duration at S = CW (calm candle) */

#define FLAME_MIN_SWING_PCT 25U /* flicker swing at S = CCW, as % of ceiling */
#define FLAME_MAX_SWING_PCT 70U /* flicker swing at S = CW, as % of ceiling */

static uint16_t GetRandomInRange(uint16_t lower, uint16_t upper)
{
    return (uint16_t)((rand() % (upper - lower + 1)) + lower);
}

/* Maps a 0..PWM_MAX_VALUE speed reading onto the bounded ramp duration.
 * Inverted: S fully CCW (pwmSpeed = PWM_MAX_VALUE) gives the fastest ramp. */
static uint16_t SpeedPwmToRampMs(uint16_t pwmSpeed)
{
    return (uint16_t)(FLAME_RAMP_MAX_MS -
        ((uint32_t)pwmSpeed * (FLAME_RAMP_MAX_MS - FLAME_RAMP_MIN_MS)) / PWM_MAX_VALUE);
}

/* Maps the same speed reading onto the flicker swing, as a percentage of the
 * ceiling. Same direction as SpeedPwmToRampMs: narrower at the fast end,
 * wider at the slow end. */
static uint16_t SpeedPwmToSwingPercent(uint16_t pwmSpeed)
{
    return (uint16_t)(FLAME_MAX_SWING_PCT -
        ((uint32_t)pwmSpeed * (FLAME_MAX_SWING_PCT - FLAME_MIN_SWING_PCT)) / PWM_MAX_VALUE);
}

void FlameEffect(void)
{
    static int16_t lastPwm1 = 0;
    static int16_t lastPwm2 = 0;

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
    uint16_t ceiling = Pots_ScaleToPwm(pots.l_raw);
    uint16_t speedPwm = Pots_ScaleToPwm(pots.s_raw);
    uint16_t rampMs = SpeedPwmToRampMs(speedPwm);
    uint16_t swingFloor = (uint16_t)(ceiling -
        (((uint32_t)ceiling * SpeedPwmToSwingPercent(speedPwm)) / 100U));

    int16_t target1 = (int16_t)GetRandomInRange(swingFloor, ceiling);
    PwmOut_StepToward(PWM_CHANNEL_1, &lastPwm1, target1, rampMs);

    int16_t target2 = (int16_t)GetRandomInRange(swingFloor, ceiling);
    PwmOut_StepToward(PWM_CHANNEL_2, &lastPwm2, target2, rampMs);
}
