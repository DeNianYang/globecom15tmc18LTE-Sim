/* 
 * File:   GreedyAllocation.h
 * Author: MNet_KenWang
 *
 * Created on 2015年6月10日, 下午 2:11
 */

#ifndef _GREEDYALLOCATION_H_
#define _GREEDYALLOCATION_H_
#include"UE.h"

void GreedyAllocation(Camera *camera, Object *object, int *GreedyCount, FILE *fp);
void RefreshCovering(Camera *camera, Object *objectArr, int layerNumOfMember);

#endif	/* GREEDYALLOCATION_H */

