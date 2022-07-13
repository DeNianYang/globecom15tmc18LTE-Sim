/* 
 * File:   MAMC.h
 * Author: MNet_KenWang
 *
 * Created on 2014年5月2日, 下午 12:37
 */

#ifndef _MAMC_H_
#define _MAMC_H_
#include "Camera.h"
#include "System.h"

void MAMC(Camera *camera, int *MAMCCount, double *MAMCCoverage, FILE *fp, int *numOfCamera);
void MCSoverRB(Camera *camera);
int most_robust_MCS(Camera *checkedCamera, int start, int end);
void refresh_coverage(Camera *camera, int *checkObject);
void adjustment(Camera *camera, Frame *frame, int *remainObject, int *checkObject);

#endif	/* MAMC_H */

