/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: dtmf_detect.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

/* Include Files */
#include "dtmf_detect.h"
#include "abs.h"
#include "dtmf_detect_data.h"
#include "dtmf_detect_emxutil.h"
#include "dtmf_detect_initialize.h"
#include "dtmf_detect_types.h"
#include "fft.h"
#include "findpeaks.h"
#include "mean.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_roundd_snf(double u);

/* Function Definitions */
/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }
  return y;
}

/*
 * Arguments    : const int data[1600]
 *                int fs
 *                int N
 *                double sp_amp[2]
 *                double dtmf[2]
 * Return Type  : void
 */
void dtmf_detect(const int data[1600], int fs, int N, double sp_amp[2],
                 double dtmf[2])
{
  emxArray_creal32_T *b_f;
  emxArray_creal32_T *f;
  emxArray_real32_T *sp;
  creal32_T *b_f_data;
  creal32_T *f_data;
  double locs_data[2];
  float b_data[1600];
  float a__1_data[2];
  int a__1_size[2];
  int locs_size[2];
  int i;
  int loop_ub;
  unsigned int q;
  unsigned int x;
  if (!isInitialized_dtmf_detect) {
    dtmf_detect_initialize();
  }
  for (i = 0; i < 1600; i++) {
    b_data[i] = (float)data[i];
  }
  emxInit_creal32_T(&f, 2);
  fft(b_data, N, f);
  f_data = f->data;
  i = (int)rt_roundd_snf((double)N / 2.0);
  if (i < 1) {
    loop_ub = 0;
  } else {
    loop_ub = i;
  }
  emxInit_creal32_T(&b_f, 2);
  i = b_f->size[0] * b_f->size[1];
  b_f->size[0] = 1;
  b_f->size[1] = loop_ub;
  emxEnsureCapacity_creal32_T(b_f, i);
  b_f_data = b_f->data;
  for (i = 0; i < loop_ub; i++) {
    b_f_data[i] = f_data[i];
  }
  emxFree_creal32_T(&f);
  emxInit_real32_T(&sp);
  b_abs(b_f, sp);
  emxFree_creal32_T(&b_f);
  sp_amp[0] = mean(sp);
  sp_amp[1] = maximum(sp);
  findpeaks(sp, a__1_data, a__1_size, locs_data, locs_size);
  emxFree_real32_T(&sp);
  if (N == 0) {
    if (fs == 0) {
      i = 0;
    } else if (fs < 0) {
      i = MIN_int32_T;
    } else {
      i = MAX_int32_T;
    }
  } else if (N == 1) {
    i = fs;
  } else {
    if (fs >= 0) {
      x = (unsigned int)fs;
    } else if (fs == MIN_int32_T) {
      x = 2147483648U;
    } else {
      x = (unsigned int)-fs;
    }
    if ((unsigned int)N == 0U) {
      q = MAX_uint32_T;
    } else {
      q = x / (unsigned int)N;
    }
    x -= q * (unsigned int)N;
    if ((x > 0U) && (x >= ((unsigned int)N >> 1U) + ((unsigned int)N & 1U))) {
      q++;
    }
    i = (int)q;
    if (fs < 0) {
      i = -(int)q;
    }
  }
  dtmf[0] = locs_data[0] * (double)i;
  dtmf[1] = locs_data[1] * (double)i;
}

/*
 * File trailer for dtmf_detect.c
 *
 * [EOF]
 */
