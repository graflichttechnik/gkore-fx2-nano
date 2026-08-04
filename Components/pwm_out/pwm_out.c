/**
 * @file    pwm_out.c
 * @brief   See pwm_out.h.
 */
#include <stdbool.h>
#include "pwm_out.h"

#define PWM_STEP 2 /* fixed increment used by PwmOut_StepToward()'s ramp */

/* Once a channel is stopped, its value must reach at least this before
 * PwmOut_Set() restarts it. Prevents rapid on/off toggling when the computed
 * value hovers right around zero (e.g. residual pot/ADC noise). Only gates
 * the restart - once running, any nonzero value (down to 1) is accepted, and
 * the channel stops again only at exactly 0. */
#define PWM_RESTART_THRESHOLD 5U

static bool *RunningFlag(PwmChannel_t channel)
{
    static bool channel1Running = false;
    static bool channel2Running = false;
    return (channel == PWM_CHANNEL_1) ? &channel1Running : &channel2Running;
}

void PwmOut_Start(PwmChannel_t channel)
{
    HAL_TIM_PWM_Start(&htim2, (uint32_t)channel);
}

void PwmOut_Stop(PwmChannel_t channel)
{
    *RunningFlag(channel) = false;
    __HAL_TIM_SET_COMPARE(&htim2, (uint32_t)channel, 0);
    HAL_TIM_PWM_Stop(&htim2, (uint32_t)channel);
}

void PwmOut_Set(PwmChannel_t channel, uint16_t value)
{
    if (value > PWM_MAX_VALUE)
    {
        value = PWM_MAX_VALUE;
    }

    bool *running = RunningFlag(channel);

    if ((value == 0) || (!*running && (value < PWM_RESTART_THRESHOLD)))
    {
        *running = false;
        __HAL_TIM_SET_COMPARE(&htim2, (uint32_t)channel, 0);
        HAL_TIM_PWM_Stop(&htim2, (uint32_t)channel);
    }
    else
    {
        *running = true;
        __HAL_TIM_SET_COMPARE(&htim2, (uint32_t)channel, value);
        HAL_TIM_PWM_Start(&htim2, (uint32_t)channel);
    }
}

void PwmOut_StepToward(PwmChannel_t channel, int16_t *state, int16_t target, uint16_t durationMs)
{
    int16_t diff = (int16_t)(target - *state);

    if (diff == 0)
    {
        /* Already at target - still pace by durationMs so callers that ramp
         * to the same value repeatedly (e.g. a zero-amplitude jump) take as
         * long as any other jump, instead of finishing instantly. */
        HAL_Delay(durationMs);
        return;
    }

    /* A deliberate ramp moves monotonically and quickly through any
     * intermediate value, unlike a freshly-computed value that might hover
     * near zero from pot/ADC noise - so intermediate steps bypass
     * PwmOut_Set()'s restart threshold and drive the compare register
     * directly. Only the final snap below goes through PwmOut_Set(), so a
     * ramp landing exactly on 0 still stops the channel correctly. */
    PwmOut_Start(channel);
    *RunningFlag(channel) = true;

    if ((diff > -PWM_STEP) && (diff < PWM_STEP))
    {
        /* Jump smaller than one step - nothing to ramp, just pace it like the
         * stepped case below would. */
        HAL_Delay(durationMs);
    }
    else
    {
        int16_t step = (diff > 0) ? PWM_STEP : (int16_t)-PWM_STEP;
        uint16_t steps = (uint16_t)((diff > 0 ? diff : -diff) / PWM_STEP);
        uint16_t stepDelayMs = durationMs / steps;

        for (uint16_t i = 0; i < steps; i++)
        {
            __HAL_TIM_SET_COMPARE(&htim2, (uint32_t)channel, (uint32_t)*state);
            *state = (int16_t)(*state + step);
            HAL_Delay(stepDelayMs);
        }
    }

    /* Snap exactly to target - absorbs any remainder from the integer division above. */
    *state = target;
    PwmOut_Set(channel, (uint16_t)target);
}
