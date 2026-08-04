/**
 * @file    independent_channels.h
 * @brief   Independent channel control: DIP4 = 1, overrides DIP1/DIP2.
 */
#ifndef INDEPENDENT_CHANNELS_H_
#define INDEPENDENT_CHANNELS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* S = channel 1 brightness, L = channel 2 brightness, set directly and
 * independently. Each reaches true 0 at the pot's minimum. */
void IndependentChannelsEffect(void);

#ifdef __cplusplus
}
#endif

#endif /* INDEPENDENT_CHANNELS_H_ */
