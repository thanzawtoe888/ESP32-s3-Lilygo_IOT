/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findpeaks.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 03-Jun-2025 18:18:56
 */

/* Include Files */
#include "findpeaks.h"
#include "dtmf_detect_emxutil.h"
#include "dtmf_detect_types.h"
#include "eml_setop.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *Yin
 *                float Ypk_data[]
 *                int Ypk_size[2]
 *                double Xpk_data[]
 *                int Xpk_size[2]
 * Return Type  : void
 */
void findpeaks(const emxArray_real32_T *Yin, float Ypk_data[], int Ypk_size[2],
               double Xpk_data[], int Xpk_size[2])
{
  emxArray_int32_T *c;
  emxArray_int32_T *iInfinite;
  emxArray_int32_T *iPk;
  emxArray_int32_T *idx;
  emxArray_int32_T *iwork;
  emxArray_int32_T *s;
  emxArray_int32_T *y;
  const float *Yin_data;
  float f;
  float yk;
  float ykfirst;
  int fPk_data[2];
  int i;
  int j;
  int k;
  int kEnd;
  int kfirst;
  int nInf;
  int nPk;
  int n_tmp;
  int p;
  int pEnd;
  int q;
  int qEnd;
  int *c_data;
  int *iPk_data;
  int *idx_data;
  int *s_data;
  char dir;
  char previousdir;
  boolean_T isinfyk;
  boolean_T isinfykfirst;
  Yin_data = Yin->data;
  emxInit_int32_T(&s, 1);
  i = Yin->size[1];
  kfirst = s->size[0];
  s->size[0] = Yin->size[1];
  emxEnsureCapacity_int32_T(s, kfirst);
  s_data = s->data;
  emxInit_int32_T(&iInfinite, 1);
  kfirst = iInfinite->size[0];
  iInfinite->size[0] = Yin->size[1];
  emxEnsureCapacity_int32_T(iInfinite, kfirst);
  iPk_data = iInfinite->data;
  emxInit_int32_T(&idx, 1);
  nPk = 0;
  nInf = 0;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInfF;
  isinfykfirst = true;
  for (k = 1; k <= i; k++) {
    yk = Yin_data[k - 1];
    if (rtIsNaNF(yk)) {
      yk = rtInfF;
      isinfyk = true;
    } else if (rtIsInfF(yk) && (yk > 0.0F)) {
      isinfyk = true;
      nInf++;
      iPk_data[nInf - 1] = k;
    } else {
      isinfyk = false;
    }
    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
      } else if (yk < ykfirst) {
        dir = 'd';
        if (previousdir == 'i') {
          nPk++;
          s_data[nPk - 1] = kfirst;
        }
      } else {
        dir = 'i';
      }
      ykfirst = yk;
      kfirst = k;
      isinfykfirst = isinfyk;
    }
  }
  if (nPk < 1) {
    i = 0;
  } else {
    i = nPk;
  }
  kfirst = s->size[0];
  s->size[0] = i;
  emxEnsureCapacity_int32_T(s, kfirst);
  s_data = s->data;
  kfirst = iInfinite->size[0];
  if (nInf < 1) {
    iInfinite->size[0] = 0;
  } else {
    iInfinite->size[0] = nInf;
  }
  emxEnsureCapacity_int32_T(iInfinite, kfirst);
  emxInit_int32_T(&iPk, 1);
  kfirst = iPk->size[0];
  iPk->size[0] = i;
  emxEnsureCapacity_int32_T(iPk, kfirst);
  iPk_data = iPk->data;
  nPk = 0;
  for (k = 0; k < i; k++) {
    ykfirst = Yin_data[s_data[k] - 1];
    if (ykfirst > rtMinusInfF) {
      yk = Yin_data[s_data[k]];
      f = Yin_data[s_data[k] - 2];
      if ((f >= yk) || rtIsNaNF(yk)) {
        yk = f;
      }
      if (ykfirst - yk >= 0.0F) {
        nPk++;
        iPk_data[nPk - 1] = s_data[k];
      }
    }
  }
  i = iPk->size[0];
  if (nPk < 1) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = nPk;
  }
  emxEnsureCapacity_int32_T(iPk, i);
  emxInit_int32_T(&c, 1);
  do_vectors(iPk, iInfinite, c, idx, s);
  c_data = c->data;
  emxFree_int32_T(&iPk);
  n_tmp = c->size[0];
  emxInit_int32_T(&y, 2);
  i = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = c->size[0];
  emxEnsureCapacity_int32_T(y, i);
  iPk_data = y->data;
  if (c->size[0] > 0) {
    iPk_data[0] = 1;
    nPk = 1;
    for (k = 2; k <= n_tmp; k++) {
      nPk++;
      iPk_data[k - 1] = nPk;
    }
  }
  i = idx->size[0];
  idx->size[0] = c->size[0];
  emxEnsureCapacity_int32_T(idx, i);
  idx_data = idx->data;
  for (i = 0; i < n_tmp; i++) {
    idx_data[i] = iPk_data[i];
  }
  emxFree_int32_T(&y);
  if (idx->size[0] != 0) {
    i = s->size[0];
    s->size[0] = c->size[0];
    emxEnsureCapacity_int32_T(s, i);
    s_data = s->data;
    for (i = 0; i < n_tmp; i++) {
      s_data[i] = 0;
    }
    nPk = idx->size[0] + 1;
    emxInit_int32_T(&iwork, 1);
    i = iwork->size[0];
    iwork->size[0] = c->size[0];
    emxEnsureCapacity_int32_T(iwork, i);
    iPk_data = iwork->data;
    i = idx->size[0] - 1;
    for (k = 1; k <= i; k += 2) {
      ykfirst = Yin_data[c_data[idx_data[k - 1] - 1] - 1];
      if ((ykfirst >= Yin_data[c_data[idx_data[k] - 1] - 1]) ||
          rtIsNaNF(ykfirst)) {
        s_data[k - 1] = k;
        s_data[k] = k + 1;
      } else {
        s_data[k - 1] = k + 1;
        s_data[k] = k;
      }
    }
    if (((unsigned int)idx->size[0] & 1U) != 0U) {
      s_data[idx->size[0] - 1] = c->size[0];
    }
    kfirst = 2;
    while (kfirst < nPk - 1) {
      nInf = kfirst << 1;
      j = 1;
      for (pEnd = kfirst + 1; pEnd < nPk; pEnd = qEnd + kfirst) {
        p = j - 1;
        q = pEnd;
        qEnd = j + nInf;
        if (qEnd > nPk) {
          qEnd = nPk;
        }
        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          ykfirst = Yin_data[c_data[idx_data[s_data[p] - 1] - 1] - 1];
          i = s_data[q - 1];
          if ((ykfirst >= Yin_data[c_data[idx_data[i - 1] - 1] - 1]) ||
              rtIsNaNF(ykfirst)) {
            iPk_data[k] = s_data[p];
            p++;
            if (p + 1 == pEnd) {
              while (q < qEnd) {
                k++;
                iPk_data[k] = s_data[q - 1];
                q++;
              }
            }
          } else {
            iPk_data[k] = i;
            q++;
            if (q == qEnd) {
              while (p + 1 < pEnd) {
                k++;
                iPk_data[k] = s_data[p];
                p++;
              }
            }
          }
          k++;
        }
        for (k = 0; k < kEnd; k++) {
          s_data[(j + k) - 1] = iPk_data[k];
        }
        j = qEnd;
      }
      kfirst = nInf;
    }
    emxFree_int32_T(&iwork);
    i = iInfinite->size[0];
    iInfinite->size[0] = c->size[0];
    emxEnsureCapacity_int32_T(iInfinite, i);
    iPk_data = iInfinite->data;
    for (i = 0; i < n_tmp; i++) {
      iPk_data[i] = idx_data[s_data[i] - 1];
    }
    i = idx->size[0];
    idx->size[0] = c->size[0];
    emxEnsureCapacity_int32_T(idx, i);
    idx_data = idx->data;
    for (i = 0; i < n_tmp; i++) {
      idx_data[i] = iPk_data[i];
    }
  }
  emxFree_int32_T(&s);
  emxFree_int32_T(&iInfinite);
  if (idx->size[0] > 2) {
    nPk = 2;
    i = idx->size[0];
    idx->size[0] = 2;
    emxEnsureCapacity_int32_T(idx, i);
    idx_data = idx->data;
  } else {
    nPk = c->size[0];
  }
  for (i = 0; i < nPk; i++) {
    fPk_data[i] = c_data[idx_data[i] - 1];
  }
  emxFree_int32_T(&c);
  emxFree_int32_T(&idx);
  Ypk_size[0] = 1;
  Ypk_size[1] = nPk;
  Xpk_size[0] = 1;
  Xpk_size[1] = nPk;
  for (i = 0; i < nPk; i++) {
    kfirst = fPk_data[i];
    Ypk_data[i] = Yin_data[kfirst - 1];
    Xpk_data[i] = (unsigned int)kfirst;
  }
}

/*
 * File trailer for findpeaks.c
 *
 * [EOF]
 */
