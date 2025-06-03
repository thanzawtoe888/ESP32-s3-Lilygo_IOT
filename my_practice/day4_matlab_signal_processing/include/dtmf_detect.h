/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: dtmf_detect.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

#ifndef DTMF_DETECT_H
#define DTMF_DETECT_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void dtmf_detect(const int data[1600], int fs, int N, double sp_amp[2],
                        double dtmf[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for dtmf_detect.h
 *
 * [EOF]
 */
