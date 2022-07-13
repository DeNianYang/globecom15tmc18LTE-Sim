/* 
 * File:   Camera.h
 * Author: MNet_KenWang
 *
 * Created on 2014年5月2日, 下午 3:07
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "System.h"
#define right 1
#define up 2
#define left 3
#define down 4

typedef struct {
    int start;
    int end;
    int subframe;
    int allocationDatarate;
    int numOfRB;
    int flag;
    int *A;
} Allocation;

typedef struct {
    int id;
    int *coverage;
    int numOfCoverage;
    int numOfCoverageTemp; //Record the number of coverage during the process
    double X; //coordinate X
    double Y; //coordinate Y
    double distance; //distance between camera and the basestation
    int requiredDatarate;
    double pathloss;
    double rxPower;     //unit: dbm
    double sinr[NumOfSubchannel];
    int mcs[NumOfSubchannel];
    Allocation *allocation;
    int numOfAllocation;
    int isScheduled;
    int allocationUsed; //Record which allocation is used
    int subframeUsed; //Record which subframe is used
    int isConsider; //Decide whether this camera is considered or not: baseline approach
    double magnitude; //the magnitude between camera and the basestation
    double R; //used to calculate the  coordination of camera
    double range; //camera coverage range
    double cameraDirection;
    double FOV;
    //double FOVStart;
    //double FOVEnd;
    double referenceX; //for directional and FOV camera
    double referenceY; //for directional and FOV camera
    int isDeployed;
    int direction;
    int layer;
    int no;
    int weight; //Used for edge cover
    int isScheduledAllo;
} Camera;

#endif	/* CAMERA_H */

