/**
 * @file    cct_dimming.h
 * @brief   CCT dimming: DIP1 = 0, DIP2 = 0.
 */
#ifndef CCT_DIMMING_H_
#define CCT_DIMMING_H_

#ifdef __cplusplus
extern "C" {
#endif

/* L = total brightness (channel 1 + channel 2 always sum to this). S = mix
 * ratio between the two channels. */
void CctDimmingEffect(void);

#ifdef __cplusplus
}
#endif

#endif /* CCT_DIMMING_H_ */
