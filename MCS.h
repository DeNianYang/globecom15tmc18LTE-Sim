/* 
 * File:   MCS.h
 * Author: MNet_KenWang
 *
 * Created on 2014年4月26日, 下午 8:58
 */

#ifndef _MCS_H_
#define _MCS_H_
#include "Camera.h"
//define datarate for 1RB (unit: kbps)
#define QPSK_13 112
#define QPSK_12 168
#define QPSK_23 224
#define QPSK_34 252
#define QAM16_12 336
#define QAM16_23 448
#define QAM16_34 504

void MCS_cal(Camera *camera);
void MCS_cal_special_caseI(Camera *camera);
void MCS_cal_special_caseII(Camera *camera);

#endif

