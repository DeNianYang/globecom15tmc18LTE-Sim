/* 
 * File:   EdgeCover.h
 * Author: KenWang
 *
 * Created on 2015年7月11日, 下午 8:17
 */

#ifndef _EDGECOVER_H_
#define _EDGECOVER_H_
#include "UE.h"

void EdgeCover(Camera *camera, Object *object, int *EdgeCoverCount, FILE *fp, int *numOfCamera);

typedef struct{ //記錄object i到j的cost以及是哪個camera負責cover
    int whichCamera;
    int weight;
    int from; //表示weight是從group 1到-1還是從group -1到1
} Cover;

#endif	/* EDGECOVER_H */

