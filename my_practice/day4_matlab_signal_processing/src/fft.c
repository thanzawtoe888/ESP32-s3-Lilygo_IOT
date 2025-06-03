/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

/* Include Files */
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "dtmf_detect_emxutil.h"
#include "dtmf_detect_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const float x[1600]
 *                int varargin_1
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void fft(const float x[1600], int varargin_1, emxArray_creal32_T *y)
{
  emxArray_creal32_T *yCol;
  emxArray_real32_T *costab;
  emxArray_real32_T *costab1q;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  creal32_T *yCol_data;
  creal32_T *y_data;
  float e;
  float *costab1q_data;
  float *costab_data;
  float *sintab_data;
  float *sintabinv_data;
  int k;
  int n;
  int pmax;
  int pmin;
  int pow2p;
  boolean_T exitg1;
  boolean_T useRadix2;
  if (varargin_1 == 0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else {
    useRadix2 = ((varargin_1 > 0) && ((varargin_1 & (varargin_1 - 1)) == 0));
    pmin = 1;
    if (useRadix2) {
      pmax = varargin_1;
    } else {
      if (varargin_1 > 0) {
        n = (varargin_1 + varargin_1) - 1;
        pmax = 31;
        if (n <= 1) {
          pmax = 0;
        } else {
          pmin = 0;
          exitg1 = false;
          while ((!exitg1) && (pmax - pmin > 1)) {
            k = (pmin + pmax) >> 1;
            pow2p = 1 << k;
            if (pow2p == n) {
              pmax = k;
              exitg1 = true;
            } else if (pow2p > n) {
              pmax = k;
            } else {
              pmin = k;
            }
          }
        }
        pmin = 1 << pmax;
      }
      pmax = pmin;
    }
    e = 6.28318548F / (float)pmax;
    n = (int)((unsigned int)pmax >> 1) >> 1;
    emxInit_real32_T(&costab1q);
    pow2p = costab1q->size[0] * costab1q->size[1];
    costab1q->size[0] = 1;
    costab1q->size[1] = n + 1;
    emxEnsureCapacity_real32_T(costab1q, pow2p);
    costab1q_data = costab1q->data;
    costab1q_data[0] = 1.0F;
    pmax = (n >> 1) - 1;
    for (k = 0; k <= pmax; k++) {
      costab1q_data[k + 1] = (float)cos(e * (float)(k + 1));
    }
    pow2p = pmax + 2;
    for (k = pow2p; k < n; k++) {
      costab1q_data[k] = (float)sin(e * (float)(n - k));
    }
    costab1q_data[n] = 0.0F;
    emxInit_real32_T(&costab);
    emxInit_real32_T(&sintab);
    emxInit_real32_T(&sintabinv);
    if (!useRadix2) {
      n = costab1q->size[1] - 1;
      pmax = (costab1q->size[1] - 1) << 1;
      pow2p = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = pmax + 1;
      emxEnsureCapacity_real32_T(costab, pow2p);
      costab_data = costab->data;
      pow2p = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = pmax + 1;
      emxEnsureCapacity_real32_T(sintab, pow2p);
      sintab_data = sintab->data;
      costab_data[0] = 1.0F;
      sintab_data[0] = 0.0F;
      pow2p = sintabinv->size[0] * sintabinv->size[1];
      sintabinv->size[0] = 1;
      sintabinv->size[1] = pmax + 1;
      emxEnsureCapacity_real32_T(sintabinv, pow2p);
      sintabinv_data = sintabinv->data;
      for (k = 0; k < n; k++) {
        sintabinv_data[k + 1] = costab1q_data[(n - k) - 1];
      }
      pow2p = costab1q->size[1];
      for (k = pow2p; k <= pmax; k++) {
        sintabinv_data[k] = costab1q_data[k - n];
      }
      for (k = 0; k < n; k++) {
        costab_data[k + 1] = costab1q_data[k + 1];
        sintab_data[k + 1] = -costab1q_data[(n - k) - 1];
      }
      for (k = pow2p; k <= pmax; k++) {
        costab_data[k] = -costab1q_data[pmax - k];
        sintab_data[k] = -costab1q_data[k - n];
      }
    } else {
      n = costab1q->size[1] - 1;
      pmax = (costab1q->size[1] - 1) << 1;
      pow2p = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = pmax + 1;
      emxEnsureCapacity_real32_T(costab, pow2p);
      costab_data = costab->data;
      pow2p = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = pmax + 1;
      emxEnsureCapacity_real32_T(sintab, pow2p);
      sintab_data = sintab->data;
      costab_data[0] = 1.0F;
      sintab_data[0] = 0.0F;
      for (k = 0; k < n; k++) {
        costab_data[k + 1] = costab1q_data[k + 1];
        sintab_data[k + 1] = -costab1q_data[(n - k) - 1];
      }
      pow2p = costab1q->size[1];
      for (k = pow2p; k <= pmax; k++) {
        costab_data[k] = -costab1q_data[pmax - k];
        sintab_data[k] = -costab1q_data[k - n];
      }
      sintabinv->size[0] = 1;
      sintabinv->size[1] = 0;
    }
    emxFree_real32_T(&costab1q);
    emxInit_creal32_T(&yCol, 1);
    if (useRadix2) {
      pow2p = yCol->size[0];
      yCol->size[0] = varargin_1;
      emxEnsureCapacity_creal32_T(yCol, pow2p);
      yCol_data = yCol->data;
      if (varargin_1 > 1600) {
        pow2p = yCol->size[0];
        yCol->size[0] = varargin_1;
        emxEnsureCapacity_creal32_T(yCol, pow2p);
        yCol_data = yCol->data;
        for (pow2p = 0; pow2p < varargin_1; pow2p++) {
          yCol_data[pow2p].re = 0.0F;
          yCol_data[pow2p].im = 0.0F;
        }
      }
      if (varargin_1 != 1) {
        c_FFTImplementationCallback_doH(x, yCol, varargin_1, costab, sintab);
        yCol_data = yCol->data;
      } else {
        yCol_data[0].re = x[0];
        yCol_data[0].im = 0.0F;
      }
    } else {
      c_FFTImplementationCallback_dob(x, pmin, varargin_1, costab, sintab,
                                      sintabinv, yCol);
      yCol_data = yCol->data;
    }
    emxFree_real32_T(&sintabinv);
    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
    pow2p = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = varargin_1;
    emxEnsureCapacity_creal32_T(y, pow2p);
    y_data = y->data;
    for (pow2p = 0; pow2p < varargin_1; pow2p++) {
      y_data[pow2p] = yCol_data[pow2p];
    }
    emxFree_creal32_T(&yCol);
  }
}

/*
 * File trailer for fft.c
 *
 * [EOF]
 */
