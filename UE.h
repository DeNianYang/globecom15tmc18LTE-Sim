/* 
 * File:   UE.h
 * Author: MNet_KenWang
 *
 * Created on 2014年6月16日, 下午 8:31
 */

#ifndef _UE_H_
#define _UE_H_
#include "System.h"

typedef struct{
    int id;
    double X;
    double Y;
    double distance;
    double pathloss;
    double rxPower;
    int start;
    int end;
    int numOfRB;
    double magnitude;
    double R;
} UE;

typedef struct{
    int id;
    double X;
    double Y;
    double distance;
    double distanceWithCamera[NumOfCamera];
    double magnitude;
    double R;
    int layer;  //Record which layer the object stay
    int numOfCoveringCamera;
    int *coveringCamera;
    int index; //Used for sorting
    int isCovered;
    int numOfNeighbor;
    int *neighbor;
    int isTried;   //Used for edge cover deployment scenario
    int isConsidered; //Used for edge cover algorithm, the copy part and Dijkstra's algorithm
    int group;
    int isColored;
    int *prev;
    int next;
    int flag;
} Object;

#endif

