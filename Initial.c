#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Camera.h"
#include "UE.h"

void initializeCamera(Camera *camera);
void initializeObject(Object *object);
void initializeUE(UE *user);

// Initialize camera id, datarate, #coverage, and #allocation
void initializeCamera(Camera *camera) {
    int i;
    for (i = 0; i < NumOfCamera; i++) {
        camera[i].id = i;
        camera[i].requiredDatarate = (rand() % 10 + 10) * 100;
        //camera[i].requiredDatarate = 1600;      //Used for special case
        camera[i].distance = 100.0;       //Used for special case
        camera[i].numOfCoverage = 0;
        camera[i].numOfCoverageTemp = 0;
        camera[i].numOfAllocation = 0;
        camera[i].isScheduled = 0;
        camera[i].isConsider = 1;
        camera[i].range = 60;
        camera[i].FOV = 160;
        camera[i].no = 0;
        camera[i].isScheduledAllo = 0;
        //printf("camera %d: %d\n", i, camera[i].requiredDatarate);
    }
}

void initializeObject(Object *object){
    int i;
    for(i = 0; i < NumOfObject; i++){
        object[i].numOfCoveringCamera = 0;
        object[i].numOfNeighbor = 0;
        object[i].isCovered = 0;
        object[i].index = -1;
        object[i].isTried = 0;
        object[i].isColored = 0;
    }
}

void initializeUE(UE *user){
    int i;
    for(i = 0; i < NumOfUE; i++){
        user[i].id = i;     
    }
}
