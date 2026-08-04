/**
 * @file    pots.c
 * @brief   See pots.h.
 */
#include "pots.h"
#include "pwm_out.h"
#include "adc.h"

/* A potentiometer's mechanical end of travel often doesn't quite reach the
 * true 0V/Vref electrical extreme. Raw readings within this margin of either
 * extreme are treated as that extreme, so both ends of the pot's travel
 * reliably reach the true output extremes. */
#define POTS_RAW_MARGIN 16U

static uint32_t NormalizeRaw(uint16_t raw)
{
    uint16_t upperBound = POTS_ADC_MAX_VALUE - POTS_RAW_MARGIN;
    if (raw < POTS_RAW_MARGIN) { raw = POTS_RAW_MARGIN; }
    if (raw > upperBound) { raw = upperBound; }
    return (uint32_t)raw - POTS_RAW_MARGIN;
}

#define POTS_RAW_SPAN (POTS_ADC_MAX_VALUE - (2U * POTS_RAW_MARGIN))

Pots_t Pots_ReadRaw(void)
{
    Pots_t pots = {0};
    uint16_t raw[2];

    HAL_ADC_Start(&hadc);
    for (int i = 0; i < 2; i++)
    {
        HAL_ADC_PollForConversion(&hadc, 10);
        raw[i] = (uint16_t)HAL_ADC_GetValue(&hadc);
    }
    HAL_ADC_Stop(&hadc);

    pots.s_raw = raw[1]; /* ADC_CHANNEL_1 = PA1 = POT_S, see MX_ADC_Init() in adc.c */
    pots.l_raw = raw[0]; /* ADC_CHANNEL_0 = PA0 = POT_L */
    return pots;
}

uint16_t Pots_ScaleToPwm(uint16_t raw)
{
    return (uint16_t)(PWM_MAX_VALUE - (NormalizeRaw(raw) * PWM_MAX_VALUE / POTS_RAW_SPAN));
}

uint8_t Pots_ScaleToPercent(uint16_t raw)
{
    return (uint8_t)(100U - (NormalizeRaw(raw) * 100U / POTS_RAW_SPAN));
}

/* Everything below this point is used only by IndependentChannelsEffect
 * (DIP4). Deliberately kept fully separate from Pots_ReadRaw()/NormalizeRaw()/
 * Pots_ScaleToPwm() above - IndependentChannelsEffect is not one of the four
 * DIP1/DIP2 effects, and its ADC/scaling behavior must be tunable without
 * ever touching theirs. */
#define POTS_INDEPENDENT_RAW_MARGIN 16U

static uint32_t NormalizeRawIndependent(uint16_t raw)
{
    uint16_t upperBound = POTS_ADC_MAX_VALUE - POTS_INDEPENDENT_RAW_MARGIN;
    if (raw < POTS_INDEPENDENT_RAW_MARGIN) { raw = POTS_INDEPENDENT_RAW_MARGIN; }
    if (raw > upperBound) { raw = upperBound; }
    return (uint32_t)raw - POTS_INDEPENDENT_RAW_MARGIN;
}

#define POTS_INDEPENDENT_RAW_SPAN (POTS_ADC_MAX_VALUE - (2U * POTS_INDEPENDENT_RAW_MARGIN))

Pots_t Pots_ReadRawIndependent(void)
{
    Pots_t pots = {0};
    uint16_t raw[2];

    HAL_ADC_Start(&hadc);
    for (int i = 0; i < 2; i++)
    {
        HAL_ADC_PollForConversion(&hadc, 10);
        raw[i] = (uint16_t)HAL_ADC_GetValue(&hadc);
    }
    HAL_ADC_Stop(&hadc);

    pots.s_raw = raw[1]; /* ADC_CHANNEL_1 = PA1 = POT_S, see MX_ADC_Init() in adc.c */
    pots.l_raw = raw[0]; /* ADC_CHANNEL_0 = PA0 = POT_L */
    return pots;
}

uint16_t Pots_ScaleToPwmIndependent(uint16_t raw)
{
    return (uint16_t)(PWM_MAX_VALUE - (NormalizeRawIndependent(raw) * PWM_MAX_VALUE / POTS_INDEPENDENT_RAW_SPAN));
}
