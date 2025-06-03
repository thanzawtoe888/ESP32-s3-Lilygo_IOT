/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: dtmf_detect_initialize.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

/* Include Files */
#include "dtmf_detect_initialize.h"
#include "dtmf_detect_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void dtmf_detect_initialize(void)
{
  rt_InitInfAndNaN();
  isInitialized_dtmf_detect = true;
}

/*
 * File trailer for dtmf_detect_initialize.c
 *
 * [EOF]
 */
