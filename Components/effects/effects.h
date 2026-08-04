/**
 * @file    effects.h
 * @brief   Selects and runs the active lighting effect based on the DIP switches.
 * @version 1.5
 *
 *   DIP1  DIP2  -> effect            DIP3 -> trigger enable (see below)
 *    0     0       CCT Dimming        DIP4 -> 1 = independent channel control,
 *    0     1       Blink                     overrides DIP1/DIP2 entirely
 *    1     0       Flame
 *    1     1       Fade
 *
 *   DIP3 = 0: trigger input disabled, outputs always follow DIP1/DIP2/DIP4.
 *             DIP3 = 1: outputs stay dark until the solder-pad trigger input is
 *             bridged, then follow DIP1/DIP2/DIP4. DIP1/DIP2/DIP4 are read fresh
 *             on every update in both cases.
 *
 *   Two potentiometers, labeled "S" and "L" on the PCB. What each one does
 *   depends on the active effect - see the comment above each effect
 *   function's declaration in its own <effect_name>.h.
 */
#ifndef EFFECTS_H_
#define EFFECTS_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EFFECTS_MODE_CCT_DIMMING = 0,
    EFFECTS_MODE_BLINK,
    EFFECTS_MODE_FLAME,
    EFFECTS_MODE_FADE,
    EFFECTS_MODE_INDEPENDENT_CHANNELS,
} EffectsMode_t;

/* Reads DIP1/DIP2/DIP4 and returns which effect they select. */
EffectsMode_t Effects_SelectMode(void);

/* Reads DIP3 and, if DIP3 is set, the solder-pad trigger input too. */
bool Effects_IsTriggerActive(void);

/* Runs one update of the given effect: reads the pots, updates the PWM outputs.
 * Meant to be called repeatedly from the main loop. */
void Effects_Execute(EffectsMode_t mode);

#ifdef __cplusplus
}
#endif

#endif /* EFFECTS_H_ */
