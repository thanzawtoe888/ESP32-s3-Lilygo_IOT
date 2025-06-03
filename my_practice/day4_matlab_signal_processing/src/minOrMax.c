/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: minOrMax.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

/* Include Files */
#include "minOrMax.h"
#include "dtmf_detect_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *x
 * Return Type  : float
 */
float maximum(const emxArray_real32_T *x)
{
  const float *x_data;
  float ex;
  float f;
  int idx;
  int k;
  int last;
  boolean_T exitg1;
  x_data = x->data;
  last = x->size[1];
  if (x->size[1] <= 2) {
    if (x->size[1] == 1) {
      ex = x_data[0];
    } else {
      ex = x_data[x->size[1] - 1];
      if ((!(x_data[0] < ex)) && ((!rtIsNaNF(x_data[0])) || rtIsNaNF(ex))) {
        ex = x_data[0];
      }
    }
  } else {
    if (!rtIsNaNF(x_data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= last)) {
        if (!rtIsNaNF(x_data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (idx == 0) {
      ex = x_data[0];
    } else {
      ex = x_data[idx - 1];
      idx++;
      for (k = idx; k <= last; k++) {
        f = x_data[k - 1];
        if (ex < f) {
          ex = f;
        }
      }
    }
  }
  return ex;
}

/*
 * File trailer for minOrMax.c
 *
 * [EOF]
 */
