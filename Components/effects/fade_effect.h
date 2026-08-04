/**
 * @file    fade_effect.h
 * @brief   Fade: DIP1 = 1, DIP2 = 1.
 */
#ifndef FADE_EFFECT_H_
#define FADE_EFFECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* L = brightness on both channels. S = crossfade speed (CCW slower, CW
 * faster) - like BlinkEffect, but each switch ramps the currently-lit channel
 * down to off before ramping the other one up, so the two channels never
 * overlap. */
void FadeEffect(void);

#ifdef __cplusplus
}
#endif

#endif /* FADE_EFFECT_H_ */
