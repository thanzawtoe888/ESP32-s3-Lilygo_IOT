/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FFTImplementationCallback.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

/* Include Files */
#include "FFTImplementationCallback.h"
#include "dtmf_detect_emxutil.h"
#include "dtmf_detect_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void c_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                            int unsigned_nRows,
                                            const emxArray_real32_T *costab,
                                            const emxArray_real32_T *sintab,
                                            emxArray_creal32_T *y);

static void d_FFTImplementationCallback_doH(
    const float x[1600], emxArray_creal32_T *y, int nRows, int nfft,
    const emxArray_creal32_T *wwc, const emxArray_real32_T *costab,
    const emxArray_real32_T *sintab, const emxArray_real32_T *costabinv,
    const emxArray_real32_T *sintabinv);

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
static void c_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                            int unsigned_nRows,
                                            const emxArray_real32_T *costab,
                                            const emxArray_real32_T *sintab,
                                            emxArray_creal32_T *y)
{
  const creal32_T *x_data;
  creal32_T *y_data;
  const float *costab_data;
  const float *sintab_data;
  float im;
  float temp_im;
  float temp_re;
  float temp_re_tmp;
  float twid_im;
  float twid_re;
  int b_temp_re_tmp;
  int i;
  int iDelta2;
  int iheight;
  int ihi;
  int iy;
  int j;
  int ju;
  int k;
  int nRowsD2;
  boolean_T tst;
  sintab_data = sintab->data;
  costab_data = costab->data;
  x_data = x->data;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal32_T(y, iy);
  y_data = y->data;
  if (unsigned_nRows > x->size[0]) {
    iy = y->size[0];
    y->size[0] = unsigned_nRows;
    emxEnsureCapacity_creal32_T(y, iy);
    y_data = y->data;
    for (iy = 0; iy < unsigned_nRows; iy++) {
      y_data[iy].re = 0.0F;
      y_data[iy].im = 0.0F;
    }
  }
  iDelta2 = x->size[0];
  if (iDelta2 > unsigned_nRows) {
    iDelta2 = unsigned_nRows;
  }
  iheight = unsigned_nRows - 2;
  nRowsD2 = (int)((unsigned int)unsigned_nRows >> 1);
  k = nRowsD2 >> 1;
  iy = 0;
  ju = 0;
  for (i = 0; i <= iDelta2 - 2; i++) {
    y_data[iy] = x_data[i];
    iy = unsigned_nRows;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  if (iDelta2 - 2 < 0) {
    iDelta2 = 0;
  } else {
    iDelta2--;
  }
  y_data[iy] = x_data[iDelta2];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= iheight; i += 2) {
      temp_re_tmp = y_data[i + 1].re;
      temp_im = y_data[i + 1].im;
      temp_re = y_data[i].re;
      im = y_data[i].im;
      y_data[i + 1].re = temp_re - temp_re_tmp;
      y_data[i + 1].im = y_data[i].im - y_data[i + 1].im;
      im += temp_im;
      y_data[i].re = temp_re + temp_re_tmp;
      y_data[i].im = im;
    }
  }
  iy = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      b_temp_re_tmp = i + iy;
      temp_re = y_data[b_temp_re_tmp].re;
      temp_im = y_data[b_temp_re_tmp].im;
      y_data[b_temp_re_tmp].re = y_data[i].re - temp_re;
      y_data[b_temp_re_tmp].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    ju = 1;
    for (j = k; j < nRowsD2; j += k) {
      twid_re = costab_data[j];
      twid_im = sintab_data[j];
      i = ju;
      ihi = ju + iheight;
      while (i < ihi) {
        b_temp_re_tmp = i + iy;
        temp_re_tmp = y_data[b_temp_re_tmp].im;
        im = y_data[b_temp_re_tmp].re;
        temp_re = twid_re * im - twid_im * temp_re_tmp;
        temp_im = twid_re * temp_re_tmp + twid_im * im;
        y_data[b_temp_re_tmp].re = y_data[i].re - temp_re;
        y_data[b_temp_re_tmp].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      ju++;
    }
    k >>= 1;
    iy = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iy;
  }
}

/*
 * Arguments    : const float x[1600]
 *                emxArray_creal32_T *y
 *                int nRows
 *                int nfft
 *                const emxArray_creal32_T *wwc
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *costabinv
 *                const emxArray_real32_T *sintabinv
 * Return Type  : void
 */
static void d_FFTImplementationCallback_doH(
    const float x[1600], emxArray_creal32_T *y, int nRows, int nfft,
    const emxArray_creal32_T *wwc, const emxArray_real32_T *costab,
    const emxArray_real32_T *sintab, const emxArray_real32_T *costabinv,
    const emxArray_real32_T *sintabinv)
{
  emxArray_creal32_T *fv;
  emxArray_creal32_T *fy;
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_creal32_T *ytmp;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *b_costab;
  emxArray_real32_T *b_sintab;
  emxArray_real32_T *costab1q;
  emxArray_real32_T *hcostabinv;
  emxArray_real32_T *hsintab;
  emxArray_real32_T *hsintabinv;
  const creal32_T *wwc_data;
  creal32_T *fv_data;
  creal32_T *fy_data;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  creal32_T *ytmp_data;
  const float *costab_data;
  const float *costabinv_data;
  const float *sintab_data;
  const float *sintabinv_data;
  float b_temp_re_tmp;
  float b_ytmp_re_tmp;
  float e;
  float temp_im;
  float temp_re;
  float twid_im;
  float twid_re;
  float ytmp_re_tmp;
  float *b_costab_data;
  float *b_sintab_data;
  float *costab1q_data;
  float *hcostabinv_data;
  float *hsintab_data;
  float *hsintabinv_data;
  int hnRows;
  int i;
  int iheight;
  int ihi;
  int j;
  int ju;
  int k;
  int minHnrowsNxBy2;
  int nRowsD2;
  int nd2;
  int nfft_tmp;
  int temp_re_tmp;
  int *wrapIndex_data;
  boolean_T tst;
  sintabinv_data = sintabinv->data;
  costabinv_data = costabinv->data;
  sintab_data = sintab->data;
  costab_data = costab->data;
  wwc_data = wwc->data;
  y_data = y->data;
  hnRows = (int)((unsigned int)nRows >> 1);
  emxInit_creal32_T(&ytmp, 1);
  iheight = ytmp->size[0];
  ytmp->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(ytmp, iheight);
  ytmp_data = ytmp->data;
  if (hnRows > 1600) {
    iheight = ytmp->size[0];
    ytmp->size[0] = hnRows;
    emxEnsureCapacity_creal32_T(ytmp, iheight);
    ytmp_data = ytmp->data;
    for (iheight = 0; iheight < hnRows; iheight++) {
      ytmp_data[iheight].re = 0.0F;
      ytmp_data[iheight].im = 0.0F;
    }
  }
  nd2 = nRows << 1;
  e = 6.28318548F / (float)nd2;
  j = (int)((unsigned int)nd2 >> 1) >> 1;
  emxInit_real32_T(&costab1q);
  iheight = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = j + 1;
  emxEnsureCapacity_real32_T(costab1q, iheight);
  costab1q_data = costab1q->data;
  costab1q_data[0] = 1.0F;
  nd2 = (j >> 1) - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q_data[k + 1] = (float)cos(e * (float)(k + 1));
  }
  iheight = nd2 + 2;
  for (k = iheight; k < j; k++) {
    costab1q_data[k] = (float)sin(e * (float)(j - k));
  }
  costab1q_data[j] = 0.0F;
  j = costab1q->size[1] - 1;
  nd2 = (costab1q->size[1] - 1) << 1;
  emxInit_real32_T(&b_costab);
  iheight = b_costab->size[0] * b_costab->size[1];
  b_costab->size[0] = 1;
  b_costab->size[1] = nd2 + 1;
  emxEnsureCapacity_real32_T(b_costab, iheight);
  b_costab_data = b_costab->data;
  emxInit_real32_T(&b_sintab);
  iheight = b_sintab->size[0] * b_sintab->size[1];
  b_sintab->size[0] = 1;
  b_sintab->size[1] = nd2 + 1;
  emxEnsureCapacity_real32_T(b_sintab, iheight);
  b_sintab_data = b_sintab->data;
  b_costab_data[0] = 1.0F;
  b_sintab_data[0] = 0.0F;
  for (k = 0; k < j; k++) {
    b_costab_data[k + 1] = costab1q_data[k + 1];
    b_sintab_data[k + 1] = -costab1q_data[(j - k) - 1];
  }
  iheight = costab1q->size[1];
  for (k = iheight; k <= nd2; k++) {
    b_costab_data[k] = -costab1q_data[nd2 - k];
    b_sintab_data[k] = -costab1q_data[k - j];
  }
  nd2 = (int)((unsigned int)costab->size[1] >> 1);
  iheight = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = nd2;
  emxEnsureCapacity_real32_T(costab1q, iheight);
  costab1q_data = costab1q->data;
  emxInit_real32_T(&hsintab);
  iheight = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = nd2;
  emxEnsureCapacity_real32_T(hsintab, iheight);
  hsintab_data = hsintab->data;
  emxInit_real32_T(&hcostabinv);
  iheight = hcostabinv->size[0] * hcostabinv->size[1];
  hcostabinv->size[0] = 1;
  hcostabinv->size[1] = nd2;
  emxEnsureCapacity_real32_T(hcostabinv, iheight);
  hcostabinv_data = hcostabinv->data;
  emxInit_real32_T(&hsintabinv);
  iheight = hsintabinv->size[0] * hsintabinv->size[1];
  hsintabinv->size[0] = 1;
  hsintabinv->size[1] = nd2;
  emxEnsureCapacity_real32_T(hsintabinv, iheight);
  hsintabinv_data = hsintabinv->data;
  for (i = 0; i < nd2; i++) {
    iheight = ((i + 1) << 1) - 2;
    costab1q_data[i] = costab_data[iheight];
    hsintab_data[i] = sintab_data[iheight];
    hcostabinv_data[i] = costabinv_data[iheight];
    hsintabinv_data[i] = sintabinv_data[iheight];
  }
  emxInit_creal32_T(&reconVar1, 1);
  iheight = reconVar1->size[0];
  reconVar1->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar1, iheight);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2, 1);
  iheight = reconVar2->size[0];
  reconVar2->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar2, iheight);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  iheight = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = hnRows;
  emxEnsureCapacity_int32_T(wrapIndex, iheight);
  wrapIndex_data = wrapIndex->data;
  for (i = 0; i < hnRows; i++) {
    iheight = i << 1;
    e = b_sintab_data[iheight];
    temp_re = b_costab_data[iheight];
    reconVar1_data[i].re = e + 1.0F;
    reconVar1_data[i].im = -temp_re;
    reconVar2_data[i].re = 1.0F - e;
    reconVar2_data[i].im = temp_re;
    if (i + 1 != 1) {
      wrapIndex_data[i] = (hnRows - i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  emxFree_real32_T(&b_sintab);
  emxFree_real32_T(&b_costab);
  if (nRows >= 1600) {
    j = 1600;
  } else {
    j = nRows;
  }
  minHnrowsNxBy2 = (int)((unsigned int)j >> 1);
  iheight = (unsigned short)minHnrowsNxBy2;
  for (ju = 0; ju < iheight; ju++) {
    temp_re_tmp = (hnRows + ju) - 1;
    temp_re = wwc_data[temp_re_tmp].re;
    temp_im = wwc_data[temp_re_tmp].im;
    nd2 = ju << 1;
    twid_re = x[nd2];
    twid_im = x[nd2 + 1];
    ytmp_data[ju].re = temp_re * twid_re + temp_im * twid_im;
    ytmp_data[ju].im = temp_re * twid_im - temp_im * twid_re;
  }
  if (minHnrowsNxBy2 + 1 <= hnRows) {
    iheight = minHnrowsNxBy2 + 1;
    for (i = iheight; i <= hnRows; i++) {
      ytmp_data[i - 1].re = 0.0F;
      ytmp_data[i - 1].im = 0.0F;
    }
  }
  nfft_tmp = (int)((unsigned int)nfft >> 1);
  emxInit_creal32_T(&fy, 1);
  iheight = fy->size[0];
  fy->size[0] = nfft_tmp;
  emxEnsureCapacity_creal32_T(fy, iheight);
  fy_data = fy->data;
  if (nfft_tmp > ytmp->size[0]) {
    iheight = fy->size[0];
    fy->size[0] = nfft_tmp;
    emxEnsureCapacity_creal32_T(fy, iheight);
    fy_data = fy->data;
    for (iheight = 0; iheight < nfft_tmp; iheight++) {
      fy_data[iheight].re = 0.0F;
      fy_data[iheight].im = 0.0F;
    }
  }
  minHnrowsNxBy2 = ytmp->size[0];
  if (minHnrowsNxBy2 > nfft_tmp) {
    minHnrowsNxBy2 = nfft_tmp;
  }
  iheight = nfft_tmp - 2;
  nRowsD2 = nfft_tmp >> 1;
  k = nRowsD2 >> 1;
  nd2 = 0;
  ju = 0;
  for (i = 0; i <= minHnrowsNxBy2 - 2; i++) {
    fy_data[nd2] = ytmp_data[i];
    j = nfft_tmp;
    tst = true;
    while (tst) {
      j >>= 1;
      ju ^= j;
      tst = (((unsigned int)ju & (unsigned int)j) == 0U);
    }
    nd2 = ju;
  }
  if (minHnrowsNxBy2 - 2 < 0) {
    j = 0;
  } else {
    j = minHnrowsNxBy2 - 1;
  }
  fy_data[nd2] = ytmp_data[j];
  if (nfft_tmp > 1) {
    for (i = 0; i <= iheight; i += 2) {
      b_temp_re_tmp = fy_data[i + 1].re;
      temp_im = fy_data[i + 1].im;
      twid_im = fy_data[i].re;
      e = fy_data[i].im;
      fy_data[i + 1].re = twid_im - b_temp_re_tmp;
      fy_data[i + 1].im = fy_data[i].im - fy_data[i + 1].im;
      e += temp_im;
      fy_data[i].re = twid_im + b_temp_re_tmp;
      fy_data[i].im = e;
    }
  }
  nd2 = 2;
  minHnrowsNxBy2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += minHnrowsNxBy2) {
      temp_re_tmp = i + nd2;
      temp_re = fy_data[temp_re_tmp].re;
      temp_im = fy_data[temp_re_tmp].im;
      fy_data[temp_re_tmp].re = fy_data[i].re - temp_re;
      fy_data[temp_re_tmp].im = fy_data[i].im - temp_im;
      fy_data[i].re += temp_re;
      fy_data[i].im += temp_im;
    }
    ju = 1;
    for (j = k; j < nRowsD2; j += k) {
      twid_re = costab1q_data[j];
      twid_im = hsintab_data[j];
      i = ju;
      ihi = ju + iheight;
      while (i < ihi) {
        temp_re_tmp = i + nd2;
        b_temp_re_tmp = fy_data[temp_re_tmp].im;
        e = fy_data[temp_re_tmp].re;
        temp_re = twid_re * e - twid_im * b_temp_re_tmp;
        temp_im = twid_re * b_temp_re_tmp + twid_im * e;
        fy_data[temp_re_tmp].re = fy_data[i].re - temp_re;
        fy_data[temp_re_tmp].im = fy_data[i].im - temp_im;
        fy_data[i].re += temp_re;
        fy_data[i].im += temp_im;
        i += minHnrowsNxBy2;
      }
      ju++;
    }
    k >>= 1;
    nd2 = minHnrowsNxBy2;
    minHnrowsNxBy2 += minHnrowsNxBy2;
    iheight -= nd2;
  }
  emxInit_creal32_T(&fv, 1);
  c_FFTImplementationCallback_r2b(wwc, nfft_tmp, costab1q, hsintab, fv);
  fv_data = fv->data;
  emxFree_real32_T(&costab1q);
  emxFree_real32_T(&hsintab);
  for (iheight = 0; iheight < nfft_tmp; iheight++) {
    twid_im = fy_data[iheight].re;
    e = fv_data[iheight].im;
    temp_re = fy_data[iheight].im;
    twid_re = fv_data[iheight].re;
    fy_data[iheight].re = twid_im * twid_re - temp_re * e;
    fy_data[iheight].im = twid_im * e + temp_re * twid_re;
  }
  c_FFTImplementationCallback_r2b(fy, nfft_tmp, hcostabinv, hsintabinv, fv);
  fv_data = fv->data;
  emxFree_creal32_T(&fy);
  emxFree_real32_T(&hsintabinv);
  emxFree_real32_T(&hcostabinv);
  if (fv->size[0] > 1) {
    e = 1.0F / (float)fv->size[0];
    nd2 = fv->size[0];
    for (iheight = 0; iheight < nd2; iheight++) {
      fv_data[iheight].re *= e;
      fv_data[iheight].im *= e;
    }
  }
  iheight = (int)(float)hnRows;
  nd2 = wwc->size[0];
  for (k = iheight; k <= nd2; k++) {
    e = wwc_data[k - 1].re;
    temp_re = fv_data[k - 1].im;
    twid_re = wwc_data[k - 1].im;
    twid_im = fv_data[k - 1].re;
    minHnrowsNxBy2 = k - (int)(float)hnRows;
    ytmp_data[minHnrowsNxBy2].re = e * twid_im + twid_re * temp_re;
    ytmp_data[minHnrowsNxBy2].im = e * temp_re - twid_re * twid_im;
  }
  emxFree_creal32_T(&fv);
  for (i = 0; i < hnRows; i++) {
    iheight = wrapIndex_data[i];
    e = ytmp_data[i].re;
    temp_re = reconVar1_data[i].im;
    twid_re = ytmp_data[i].im;
    twid_im = reconVar1_data[i].re;
    temp_im = ytmp_data[iheight - 1].re;
    b_temp_re_tmp = -ytmp_data[iheight - 1].im;
    ytmp_re_tmp = reconVar2_data[i].im;
    b_ytmp_re_tmp = reconVar2_data[i].re;
    y_data[i].re =
        0.5F * ((e * twid_im - twid_re * temp_re) +
                (temp_im * b_ytmp_re_tmp - b_temp_re_tmp * ytmp_re_tmp));
    y_data[i].im =
        0.5F * ((e * temp_re + twid_re * twid_im) +
                (temp_im * ytmp_re_tmp + b_temp_re_tmp * b_ytmp_re_tmp));
    iheight = hnRows + i;
    y_data[iheight].re = 0.5F * ((e * b_ytmp_re_tmp - twid_re * ytmp_re_tmp) +
                                 (temp_im * twid_im - b_temp_re_tmp * temp_re));
    y_data[iheight].im = 0.5F * ((e * ytmp_re_tmp + twid_re * b_ytmp_re_tmp) +
                                 (temp_im * temp_re + b_temp_re_tmp * twid_im));
  }
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
  emxFree_int32_T(&wrapIndex);
  emxFree_creal32_T(&ytmp);
}

/*
 * Arguments    : const float x[1600]
 *                emxArray_creal32_T *y
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 * Return Type  : void
 */
void c_FFTImplementationCallback_doH(const float x[1600], emxArray_creal32_T *y,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab)
{
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_int32_T *bitrevIndex;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *hcostab;
  emxArray_real32_T *hsintab;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  const float *costab_data;
  const float *sintab_data;
  float b_y_re_tmp;
  float im;
  float re;
  float temp2_im;
  float temp2_re;
  float temp_im;
  float temp_im_tmp;
  float temp_re;
  float temp_re_tmp;
  float y_re_tmp;
  float *hcostab_data;
  float *hsintab_data;
  int b_temp_re_tmp;
  int hszCostab;
  int i;
  int iDelta2;
  int ihi;
  int istart;
  int j;
  int ju;
  int k;
  int nRowsD2_tmp;
  int nRows_tmp;
  int *bitrevIndex_data;
  int *wrapIndex_data;
  boolean_T tst;
  sintab_data = sintab->data;
  costab_data = costab->data;
  y_data = y->data;
  nRows_tmp = (int)((unsigned int)unsigned_nRows >> 1);
  istart = y->size[0];
  if (istart > nRows_tmp) {
    istart = nRows_tmp;
  }
  j = istart - 2;
  ihi = nRows_tmp - 2;
  nRowsD2_tmp = nRows_tmp >> 1;
  k = nRowsD2_tmp >> 1;
  hszCostab = (int)((unsigned int)costab->size[1] >> 1);
  emxInit_real32_T(&hcostab);
  iDelta2 = hcostab->size[0] * hcostab->size[1];
  hcostab->size[0] = 1;
  hcostab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hcostab, iDelta2);
  hcostab_data = hcostab->data;
  emxInit_real32_T(&hsintab);
  iDelta2 = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hsintab, iDelta2);
  hsintab_data = hsintab->data;
  for (i = 0; i < hszCostab; i++) {
    iDelta2 = ((i + 1) << 1) - 2;
    hcostab_data[i] = costab_data[iDelta2];
    hsintab_data[i] = sintab_data[iDelta2];
  }
  emxInit_creal32_T(&reconVar1, 1);
  iDelta2 = reconVar1->size[0];
  reconVar1->size[0] = nRows_tmp;
  emxEnsureCapacity_creal32_T(reconVar1, iDelta2);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2, 1);
  iDelta2 = reconVar2->size[0];
  reconVar2->size[0] = nRows_tmp;
  emxEnsureCapacity_creal32_T(reconVar2, iDelta2);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  iDelta2 = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = nRows_tmp;
  emxEnsureCapacity_int32_T(wrapIndex, iDelta2);
  wrapIndex_data = wrapIndex->data;
  ju = 0;
  hszCostab = 1;
  emxInit_int32_T(&bitrevIndex, 1);
  iDelta2 = bitrevIndex->size[0];
  bitrevIndex->size[0] = nRows_tmp;
  emxEnsureCapacity_int32_T(bitrevIndex, iDelta2);
  bitrevIndex_data = bitrevIndex->data;
  for (i = 0; i < nRows_tmp; i++) {
    temp2_re = sintab_data[i];
    temp2_im = costab_data[i];
    reconVar1_data[i].re = temp2_re + 1.0F;
    reconVar1_data[i].im = -temp2_im;
    reconVar2_data[i].re = 1.0F - temp2_re;
    reconVar2_data[i].im = temp2_im;
    if (i + 1 != 1) {
      wrapIndex_data[i] = (nRows_tmp - i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
    bitrevIndex_data[i] = 0;
  }
  for (iDelta2 = 0; iDelta2 <= j; iDelta2++) {
    bitrevIndex_data[iDelta2] = hszCostab;
    hszCostab = nRows_tmp;
    tst = true;
    while (tst) {
      hszCostab >>= 1;
      ju ^= hszCostab;
      tst = (((unsigned int)ju & (unsigned int)hszCostab) == 0U);
    }
    hszCostab = ju + 1;
  }
  bitrevIndex_data[istart - 1] = hszCostab;
  if (unsigned_nRows >= 1600) {
    istart = 1600;
  } else {
    istart = unsigned_nRows;
  }
  iDelta2 = (unsigned short)((unsigned int)istart >> 1);
  for (i = 0; i < iDelta2; i++) {
    hszCostab = i << 1;
    y_data[bitrevIndex_data[i] - 1].re = x[hszCostab];
    y_data[bitrevIndex_data[i] - 1].im = x[hszCostab + 1];
  }
  emxFree_int32_T(&bitrevIndex);
  if (nRows_tmp > 1) {
    for (i = 0; i <= ihi; i += 2) {
      temp2_re = y_data[i + 1].re;
      temp2_im = y_data[i + 1].im;
      temp_re = temp2_re;
      temp_im = temp2_im;
      re = y_data[i].re;
      im = y_data[i].im;
      temp2_re = re - temp2_re;
      temp2_im = im - temp2_im;
      y_data[i + 1].re = temp2_re;
      y_data[i + 1].im = temp2_im;
      re += temp_re;
      im += temp_im;
      y_data[i].re = re;
      y_data[i].im = im;
    }
  }
  hszCostab = 2;
  iDelta2 = 4;
  ju = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < ju; i += iDelta2) {
      b_temp_re_tmp = i + hszCostab;
      temp_re = y_data[b_temp_re_tmp].re;
      temp_im = y_data[b_temp_re_tmp].im;
      y_data[b_temp_re_tmp].re = y_data[i].re - temp_re;
      y_data[b_temp_re_tmp].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    istart = 1;
    for (j = k; j < nRowsD2_tmp; j += k) {
      temp2_re = hcostab_data[j];
      temp2_im = hsintab_data[j];
      i = istart;
      ihi = istart + ju;
      while (i < ihi) {
        b_temp_re_tmp = i + hszCostab;
        temp_re_tmp = y_data[b_temp_re_tmp].im;
        temp_im = y_data[b_temp_re_tmp].re;
        temp_re = temp2_re * temp_im - temp2_im * temp_re_tmp;
        temp_im = temp2_re * temp_re_tmp + temp2_im * temp_im;
        y_data[b_temp_re_tmp].re = y_data[i].re - temp_re;
        y_data[b_temp_re_tmp].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      istart++;
    }
    k >>= 1;
    hszCostab = iDelta2;
    iDelta2 += iDelta2;
    ju -= hszCostab;
  }
  emxFree_real32_T(&hsintab);
  emxFree_real32_T(&hcostab);
  temp_re_tmp = y_data[0].re;
  temp_im = y_data[0].im;
  y_re_tmp = y_data[0].re * reconVar1_data[0].re;
  temp2_im = y_data[0].re * reconVar1_data[0].im;
  temp_re = -y_data[0].im;
  b_y_re_tmp = temp_re_tmp * reconVar2_data[0].re;
  temp2_re = temp_re_tmp * reconVar2_data[0].im;
  y_data[0].re = 0.5F * ((y_re_tmp - y_data[0].im * reconVar1_data[0].im) +
                         (b_y_re_tmp - temp_re * reconVar2_data[0].im));
  y_data[0].im = 0.5F * ((temp2_im + y_data[0].im * reconVar1_data[0].re) +
                         (temp2_re + temp_re * reconVar2_data[0].re));
  y_data[nRows_tmp].re = 0.5F * ((b_y_re_tmp - temp_im * reconVar2_data[0].im) +
                                 (y_re_tmp - temp_re * reconVar1_data[0].im));
  y_data[nRows_tmp].im = 0.5F * ((temp2_re + temp_im * reconVar2_data[0].re) +
                                 (temp2_im + temp_re * reconVar1_data[0].re));
  for (i = 2; i <= nRowsD2_tmp; i++) {
    temp_re_tmp = y_data[i - 1].re;
    temp_im_tmp = y_data[i - 1].im;
    iDelta2 = wrapIndex_data[i - 1];
    temp2_im = y_data[iDelta2 - 1].re;
    temp_re = y_data[iDelta2 - 1].im;
    y_re_tmp = reconVar1_data[i - 1].im;
    b_y_re_tmp = reconVar1_data[i - 1].re;
    temp2_re = reconVar2_data[i - 1].im;
    temp_im = reconVar2_data[i - 1].re;
    y_data[i - 1].re =
        0.5F * ((temp_re_tmp * b_y_re_tmp - temp_im_tmp * y_re_tmp) +
                (temp2_im * temp_im - -temp_re * temp2_re));
    y_data[i - 1].im =
        0.5F * ((temp_re_tmp * y_re_tmp + temp_im_tmp * b_y_re_tmp) +
                (temp2_im * temp2_re + -temp_re * temp_im));
    hszCostab = (nRows_tmp + i) - 1;
    y_data[hszCostab].re =
        0.5F * ((temp_re_tmp * temp_im - temp_im_tmp * temp2_re) +
                (temp2_im * b_y_re_tmp - -temp_re * y_re_tmp));
    y_data[hszCostab].im =
        0.5F * ((temp_re_tmp * temp2_re + temp_im_tmp * temp_im) +
                (temp2_im * y_re_tmp + -temp_re * b_y_re_tmp));
    re = reconVar1_data[iDelta2 - 1].im;
    im = reconVar1_data[iDelta2 - 1].re;
    temp_im = reconVar2_data[iDelta2 - 1].im;
    temp2_re = reconVar2_data[iDelta2 - 1].re;
    y_data[iDelta2 - 1].re =
        0.5F * ((temp2_im * im - temp_re * re) +
                (temp_re_tmp * temp2_re - -temp_im_tmp * temp_im));
    y_data[iDelta2 - 1].im =
        0.5F * ((temp2_im * re + temp_re * im) +
                (temp_re_tmp * temp_im + -temp_im_tmp * temp2_re));
    iDelta2 = (iDelta2 + nRows_tmp) - 1;
    y_data[iDelta2].re = 0.5F * ((temp2_im * temp2_re - temp_re * temp_im) +
                                 (temp_re_tmp * im - -temp_im_tmp * re));
    y_data[iDelta2].im = 0.5F * ((temp2_im * temp_im + temp_re * temp2_re) +
                                 (temp_re_tmp * re + -temp_im_tmp * im));
  }
  emxFree_int32_T(&wrapIndex);
  if (nRowsD2_tmp != 0) {
    temp_re_tmp = y_data[nRowsD2_tmp].re;
    temp_im_tmp = y_data[nRowsD2_tmp].im;
    y_re_tmp = reconVar1_data[nRowsD2_tmp].im;
    b_y_re_tmp = reconVar1_data[nRowsD2_tmp].re;
    temp2_re = temp_re_tmp * b_y_re_tmp;
    temp2_im = temp_re_tmp * y_re_tmp;
    temp_im = reconVar2_data[nRowsD2_tmp].im;
    re = reconVar2_data[nRowsD2_tmp].re;
    im = temp_re_tmp * re;
    temp_re = temp_re_tmp * temp_im;
    y_data[nRowsD2_tmp].re = 0.5F * ((temp2_re - temp_im_tmp * y_re_tmp) +
                                     (im - -temp_im_tmp * temp_im));
    y_data[nRowsD2_tmp].im = 0.5F * ((temp2_im + temp_im_tmp * b_y_re_tmp) +
                                     (temp_re + -temp_im_tmp * re));
    iDelta2 = nRows_tmp + nRowsD2_tmp;
    y_data[iDelta2].re = 0.5F * ((im - temp_im_tmp * temp_im) +
                                 (temp2_re - -temp_im_tmp * y_re_tmp));
    y_data[iDelta2].im = 0.5F * ((temp_re + temp_im_tmp * re) +
                                 (temp2_im + -temp_im_tmp * b_y_re_tmp));
  }
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
}

/*
 * Arguments    : const float x[1600]
 *                int n2blue
 *                int nfft
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *sintabinv
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_dob(const float x[1600], int n2blue, int nfft,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y)
{
  emxArray_creal32_T *fv;
  emxArray_creal32_T *fy;
  emxArray_creal32_T *wwc;
  creal32_T *fv_data;
  creal32_T *fy_data;
  creal32_T *wwc_data;
  creal32_T *y_data;
  const float *costab_data;
  const float *sintab_data;
  float b_nt_re_tmp;
  float im;
  float nt_im;
  float nt_re;
  float twid_im;
  float twid_re;
  int i;
  int ihi;
  int j;
  int k;
  int minNrowsNx;
  int nInt2;
  int nInt2m1;
  int nRowsD2;
  int nt_re_tmp;
  int rt;
  boolean_T tst;
  sintab_data = sintab->data;
  costab_data = costab->data;
  emxInit_creal32_T(&wwc, 1);
  if ((nfft != 1) && (((unsigned int)nfft & 1U) == 0U)) {
    j = (int)((unsigned int)nfft >> 1);
    nInt2m1 = (j + j) - 1;
    ihi = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal32_T(wwc, ihi);
    wwc_data = wwc->data;
    rt = 0;
    wwc_data[j - 1].re = 1.0F;
    wwc_data[j - 1].im = 0.0F;
    nInt2 = j << 1;
    for (k = 0; k <= j - 2; k++) {
      minNrowsNx = ((k + 1) << 1) - 1;
      if (nInt2 - rt <= minNrowsNx) {
        rt += minNrowsNx - nInt2;
      } else {
        rt += minNrowsNx;
      }
      nt_im = -3.14159274F * (float)rt / (float)j;
      ihi = (j - k) - 2;
      wwc_data[ihi].re = (float)cos(nt_im);
      wwc_data[ihi].im = -(float)sin(nt_im);
    }
    ihi = nInt2m1 - 1;
    for (k = ihi; k >= j; k--) {
      wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
    }
  } else {
    nInt2m1 = (nfft + nfft) - 1;
    ihi = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal32_T(wwc, ihi);
    wwc_data = wwc->data;
    rt = 0;
    wwc_data[nfft - 1].re = 1.0F;
    wwc_data[nfft - 1].im = 0.0F;
    nInt2 = nfft << 1;
    for (k = 0; k <= nfft - 2; k++) {
      minNrowsNx = ((k + 1) << 1) - 1;
      if (nInt2 - rt <= minNrowsNx) {
        rt += minNrowsNx - nInt2;
      } else {
        rt += minNrowsNx;
      }
      nt_im = -3.14159274F * (float)rt / (float)nfft;
      ihi = (nfft - k) - 2;
      wwc_data[ihi].re = (float)cos(nt_im);
      wwc_data[ihi].im = -(float)sin(nt_im);
    }
    ihi = nInt2m1 - 1;
    for (k = ihi; k >= nfft; k--) {
      wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
    }
  }
  ihi = y->size[0];
  y->size[0] = nfft;
  emxEnsureCapacity_creal32_T(y, ihi);
  y_data = y->data;
  if (nfft > 1600) {
    ihi = y->size[0];
    y->size[0] = nfft;
    emxEnsureCapacity_creal32_T(y, ihi);
    y_data = y->data;
    for (ihi = 0; ihi < nfft; ihi++) {
      y_data[ihi].re = 0.0F;
      y_data[ihi].im = 0.0F;
    }
  }
  emxInit_creal32_T(&fy, 1);
  emxInit_creal32_T(&fv, 1);
  if ((n2blue != 1) && (((unsigned int)nfft & 1U) == 0U)) {
    d_FFTImplementationCallback_doH(x, y, nfft, n2blue, wwc, costab, sintab,
                                    costab, sintabinv);
  } else {
    if (nfft <= 1600) {
      minNrowsNx = nfft;
    } else {
      minNrowsNx = 1600;
    }
    ihi = (unsigned short)minNrowsNx;
    for (k = 0; k < ihi; k++) {
      nt_re_tmp = (nfft + k) - 1;
      im = x[k];
      y_data[k].re = wwc_data[nt_re_tmp].re * im;
      y_data[k].im = wwc_data[nt_re_tmp].im * -im;
    }
    ihi = minNrowsNx + 1;
    for (k = ihi; k <= nfft; k++) {
      y_data[k - 1].re = 0.0F;
      y_data[k - 1].im = 0.0F;
    }
    ihi = fy->size[0];
    fy->size[0] = n2blue;
    emxEnsureCapacity_creal32_T(fy, ihi);
    fy_data = fy->data;
    if (n2blue > y->size[0]) {
      ihi = fy->size[0];
      fy->size[0] = n2blue;
      emxEnsureCapacity_creal32_T(fy, ihi);
      fy_data = fy->data;
      for (ihi = 0; ihi < n2blue; ihi++) {
        fy_data[ihi].re = 0.0F;
        fy_data[ihi].im = 0.0F;
      }
    }
    nInt2m1 = y->size[0];
    if (nInt2m1 > n2blue) {
      nInt2m1 = n2blue;
    }
    rt = n2blue - 2;
    nRowsD2 = (int)((unsigned int)n2blue >> 1);
    k = nRowsD2 >> 1;
    minNrowsNx = 0;
    nInt2 = 0;
    for (i = 0; i <= nInt2m1 - 2; i++) {
      fy_data[minNrowsNx] = y_data[i];
      minNrowsNx = n2blue;
      tst = true;
      while (tst) {
        minNrowsNx >>= 1;
        nInt2 ^= minNrowsNx;
        tst = ((nInt2 & minNrowsNx) == 0);
      }
      minNrowsNx = nInt2;
    }
    if (nInt2m1 - 2 < 0) {
      j = 0;
    } else {
      j = nInt2m1 - 1;
    }
    fy_data[minNrowsNx] = y_data[j];
    if (n2blue > 1) {
      for (i = 0; i <= rt; i += 2) {
        b_nt_re_tmp = fy_data[i + 1].re;
        nt_im = fy_data[i + 1].im;
        twid_im = fy_data[i].re;
        im = fy_data[i].im;
        fy_data[i + 1].re = twid_im - b_nt_re_tmp;
        fy_data[i + 1].im = fy_data[i].im - fy_data[i + 1].im;
        im += nt_im;
        fy_data[i].re = twid_im + b_nt_re_tmp;
        fy_data[i].im = im;
      }
    }
    minNrowsNx = 2;
    nInt2m1 = 4;
    rt = ((k - 1) << 2) + 1;
    while (k > 0) {
      for (i = 0; i < rt; i += nInt2m1) {
        nt_re_tmp = i + minNrowsNx;
        nt_re = fy_data[nt_re_tmp].re;
        nt_im = fy_data[nt_re_tmp].im;
        fy_data[nt_re_tmp].re = fy_data[i].re - nt_re;
        fy_data[nt_re_tmp].im = fy_data[i].im - nt_im;
        fy_data[i].re += nt_re;
        fy_data[i].im += nt_im;
      }
      nInt2 = 1;
      for (j = k; j < nRowsD2; j += k) {
        twid_re = costab_data[j];
        twid_im = sintab_data[j];
        i = nInt2;
        ihi = nInt2 + rt;
        while (i < ihi) {
          nt_re_tmp = i + minNrowsNx;
          b_nt_re_tmp = fy_data[nt_re_tmp].im;
          im = fy_data[nt_re_tmp].re;
          nt_re = twid_re * im - twid_im * b_nt_re_tmp;
          nt_im = twid_re * b_nt_re_tmp + twid_im * im;
          fy_data[nt_re_tmp].re = fy_data[i].re - nt_re;
          fy_data[nt_re_tmp].im = fy_data[i].im - nt_im;
          fy_data[i].re += nt_re;
          fy_data[i].im += nt_im;
          i += nInt2m1;
        }
        nInt2++;
      }
      k >>= 1;
      minNrowsNx = nInt2m1;
      nInt2m1 += nInt2m1;
      rt -= minNrowsNx;
    }
    c_FFTImplementationCallback_r2b(wwc, n2blue, costab, sintab, fv);
    fv_data = fv->data;
    minNrowsNx = fy->size[0];
    for (ihi = 0; ihi < minNrowsNx; ihi++) {
      twid_im = fy_data[ihi].re;
      im = fv_data[ihi].im;
      nt_re = fy_data[ihi].im;
      twid_re = fv_data[ihi].re;
      fy_data[ihi].re = twid_im * twid_re - nt_re * im;
      fy_data[ihi].im = twid_im * im + nt_re * twid_re;
    }
    c_FFTImplementationCallback_r2b(fy, n2blue, costab, sintabinv, fv);
    fv_data = fv->data;
    if (fv->size[0] > 1) {
      im = 1.0F / (float)fv->size[0];
      minNrowsNx = fv->size[0];
      for (ihi = 0; ihi < minNrowsNx; ihi++) {
        fv_data[ihi].re *= im;
        fv_data[ihi].im *= im;
      }
    }
    ihi = (int)(float)nfft;
    minNrowsNx = wwc->size[0];
    for (k = ihi; k <= minNrowsNx; k++) {
      im = wwc_data[k - 1].re;
      nt_re = fv_data[k - 1].im;
      twid_re = wwc_data[k - 1].im;
      twid_im = fv_data[k - 1].re;
      nInt2m1 = k - (int)(float)nfft;
      y_data[nInt2m1].re = im * twid_im + twid_re * nt_re;
      y_data[nInt2m1].im = im * nt_re - twid_re * twid_im;
    }
  }
  emxFree_creal32_T(&fv);
  emxFree_creal32_T(&fy);
  emxFree_creal32_T(&wwc);
}

/*
 * File trailer for FFTImplementationCallback.c
 *
 * [EOF]
 */
