/**
 * @file    blink_effect.h
 * @brief   Blink: DIP1 = 0, DIP2 = 1.
 */
#ifndef BLINK_EFFECT_H_
#define BLINK_EFFECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* L = brightness, same level on both channels while each is lit. S = blink rate. */
void BlinkEffect(void);

#ifdef __cplusplus
}
#endif

#endif /* BLINK_EFFECT_H_ */
