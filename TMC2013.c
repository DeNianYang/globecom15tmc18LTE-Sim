#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"
#include "System.h"
#include "MCS.h"

void TMC(Camera *camera, int *TMCCount, double *TMCCoverage, FILE *fp, int *numOfCamera);

void TMC(Camera *camera, int *TMCCount, double *TMCCoverage, FILE *fp, int *numOfCamera) {
    MCSoverRB(camera);
    //Iteratively find the most suitable camera for scheduling
    Frame frame;
    int i, j, k;
    int numOfRBTemp = NumOfSubchannel; //Record the minimum number of RBs currently found
    int remainObject = NumOfObject; //Record the number of objects uncovered
    int remainCamera = *numOfCamera;
    int cameraTemp; //Record the camera that is allocating
    int subframeTemp = 0; //Record the subframe that camera is allocated
    int allocationTemp; //Record the allocation that the allocated camera is using 
    int checkObject[NumOfObject];
    int totalRB = 0;
    int reCoverage[NumOfObject];
    int totalCoverage = 0;
    double averageCoverage = 0.0;
    for (i = 0; i < NumOfObject; i++) {
        checkObject[i] = -1;
        reCoverage[i] = 0;
    }
    
    //Initialize the frame
    for (i = 0; i < NumOfSubchannel; i++) {
        for (j = 0; j < NumOfSubframe; j++) {
            frame.index[j] = 0;
            frame.rb[i][j] = -1;
        }
    }
    
    while (remainObject > 0 && remainCamera > 0 && subframeTemp < NumOfSubframe) {
        cameraTemp = -1;
        numOfRBTemp = NumOfSubchannel;
        //for (i = 0; i < NumOfSubframe; i++) {
        for (j = 0; j < NumOfCamera; j++) {
            for (k = 0; k < camera[j].numOfAllocation; k++) {
                if (camera[j].allocation[k].start == frame.index[subframeTemp]
                        && camera[j].allocation[k].end <= NumOfSubchannel
                        && camera[j].allocation[k].numOfRB < numOfRBTemp
                        && camera[j].isScheduled == 0
                        && camera[j].distance <= Range) {
                    numOfRBTemp = camera[j].allocation[k].numOfRB;
                    cameraTemp = j;
                    allocationTemp = k;
                }
            }
        }
        if (cameraTemp == -1) {
            subframeTemp++;
            continue;
        }
        //}

        //allocate resources to the selected camera and record the allocated information
        camera[cameraTemp].isScheduled = 1;
        camera[cameraTemp].allocationUsed = allocationTemp;
        camera[cameraTemp].subframeUsed = subframeTemp;
        for (i = camera[cameraTemp].allocation[allocationTemp].start; i <= camera[cameraTemp].allocation[allocationTemp].end; i++) {
            frame.rb[i][subframeTemp] = cameraTemp;
        }
        frame.index[subframeTemp] = camera[cameraTemp].allocation[allocationTemp].end + 1;

        for (i = 0; i < camera[cameraTemp].numOfCoverage; i++) {
            checkObject[camera[cameraTemp].coverage[i]] = 1;
            reCoverage[camera[cameraTemp].coverage[i]]++;
        }
        remainObject = remainObject - camera[cameraTemp].numOfCoverageTemp;
        remainCamera = remainCamera - 1;
        //printf("after #objects: %d\n", remainObject);
        refresh_coverage(camera, checkObject);
    }

    //print the allocation result
    for (i = 0; i < NumOfSubchannel; i++) {
        for (j = 0; j < NumOfSubframe; j++) {
            if (frame.rb[i][j] != -1) {
                totalRB++;
            }
            //printf("%5d", frame.rb[i][j]);
        }
        //printf("\n");
    }
    printf("TMC RBs: %d\n", totalRB);
    fprintf(fp, "%d,", totalRB);
    *TMCCount = *TMCCount + totalRB;
    /*for(i = 0; i < NumOfObject; i++){
        totalCoverage = totalCoverage + reCoverage[i];
    }
    averageCoverage = (double)totalCoverage / NumOfObject;
    *TMCCoverage = *TMCCoverage + averageCoverage;*/
}
