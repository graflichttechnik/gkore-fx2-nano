/**
 * @file    flame_effect.h
 * @brief   Flame: DIP1 = 1, DIP2 = 0.
 */
#ifndef FLAME_EFFECT_H_
#define FLAME_EFFECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* L = flicker ceiling on both channels. S = flicker character: CCW is a
 * fast, narrow flame flicker; CW is a slow, wide candle-like breathing. */
void FlameEffect(void);

#ifdef __cplusplus
}
#endif

#endif /* FLAME_EFFECT_H_ */
