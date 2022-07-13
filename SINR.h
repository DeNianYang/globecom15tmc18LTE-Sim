/* 
 * File:   SINR.h
 * Author: MNet_KenWang
 *
 * Created on 2014年4月26日, 下午 9:31
 */

#ifndef _SINR_H_
#define _SINR_H_
#include "Camera.h"
#include "UE.h"

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

void SINR_camera(Camera *camera, UE *user1, UE* user2, UE* user3, UE* user4, UE* user5, UE* user6);
void rx_power(UE *user);
double shadow();
double random_variable(long *idum);

#endif

