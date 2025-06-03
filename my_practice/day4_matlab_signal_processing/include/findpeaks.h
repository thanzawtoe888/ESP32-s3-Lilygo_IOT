/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findpeaks.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

#ifndef FINDPEAKS_H
#define FINDPEAKS_H

/* Include Files */
#include "dtmf_detect_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void findpeaks(const emxArray_real32_T *Yin, float Ypk_data[], int Ypk_size[2],
               double Xpk_data[], int Xpk_size[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for findpeaks.h
 *
 * [EOF]
 */
