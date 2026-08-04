/**
 * @file    effects.c
 * @brief   See effects.h.
 */
#include <stddef.h>
#include "main.h"
#include "effects.h"
#include "cct_dimming.h"
#include "blink_effect.h"
#include "flame_effect.h"
#include "fade_effect.h"
#include "independent_channels.h"

static bool IsDipSet(GPIO_TypeDef *port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
}

EffectsMode_t Effects_SelectMode(void)
{
    if (IsDipSet(DIP4_GPIO_Port, DIP4_Pin))
    {
        return EFFECTS_MODE_INDEPENDENT_CHANNELS;
    }

    bool dip1 = IsDipSet(DIP1_GPIO_Port, DIP1_Pin);
    bool dip2 = IsDipSet(DIP2_GPIO_Port, DIP2_Pin);

    if (dip1 && !dip2) { return EFFECTS_MODE_FLAME; }
    if (!dip1 && dip2) { return EFFECTS_MODE_BLINK; }
    if (dip1 && dip2)  { return EFFECTS_MODE_FADE; }
    return EFFECTS_MODE_CCT_DIMMING;
}

bool Effects_IsTriggerActive(void)
{
    if (!IsDipSet(DIP3_GPIO_Port, DIP3_Pin))
    {
        return true; /* trigger feature disabled -> effect always runs */
    }
    return IsDipSet(SolderPad_GPIO_Port, SolderPad_Pin);
}

typedef void (*EffectFn)(void);

typedef struct {
    EffectsMode_t mode;
    EffectFn run;
} EffectEntry_t;

/* Mode -> effect function. To add a custom effect: write it in its own
 * Components/effects/<name>.c/.h (any file name), include its header above,
 * and add one entry below - nothing else in this file needs to change. */
static const EffectEntry_t kEffectTable[] = {
    { EFFECTS_MODE_CCT_DIMMING, CctDimmingEffect },
    { EFFECTS_MODE_BLINK, BlinkEffect },
    { EFFECTS_MODE_FLAME, FlameEffect },
    { EFFECTS_MODE_FADE, FadeEffect },
    { EFFECTS_MODE_INDEPENDENT_CHANNELS, IndependentChannelsEffect },
};
#define EFFECT_TABLE_SIZE (sizeof(kEffectTable) / sizeof(kEffectTable[0]))

void Effects_Execute(EffectsMode_t mode)
{
    for (size_t i = 0; i < EFFECT_TABLE_SIZE; i++)
    {
        if (kEffectTable[i].mode == mode)
        {
            kEffectTable[i].run();
            return;
        }
    }
}
