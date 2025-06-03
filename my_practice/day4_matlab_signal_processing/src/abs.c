/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: abs.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

/* Include Files */
#include "abs.h"
#include "dtmf_detect_emxutil.h"
#include "dtmf_detect_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 *                emxArray_real32_T *y
 * Return Type  : void
 */
void b_abs(const emxArray_creal32_T *x, emxArray_real32_T *y)
{
  const creal32_T *x_data;
  float a;
  float b;
  float *y_data;
  int k;
  int nx_tmp;
  x_data = x->data;
  nx_tmp = x->size[1];
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = x->size[1];
  emxEnsureCapacity_real32_T(y, k);
  y_data = y->data;
  for (k = 0; k < nx_tmp; k++) {
    a = (float)fabs(x_data[k].re);
    b = (float)fabs(x_data[k].im);
    if (a < b) {
      a /= b;
      y_data[k] = b * (float)sqrt(a * a + 1.0F);
    } else if (a > b) {
      b /= a;
      y_data[k] = a * (float)sqrt(b * b + 1.0F);
    } else if (rtIsNaNF(b)) {
      y_data[k] = rtNaNF;
    } else {
      y_data[k] = a * 1.41421354F;
    }
  }
}

/*
 * File trailer for abs.c
 *
 * [EOF]
 */
