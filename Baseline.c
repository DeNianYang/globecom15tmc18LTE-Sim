#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"
#include "System.h"
#include "MCS.h"

void Baseline(Camera *camera, int *baselineCount, double *baselineCoverage, FILE *fp, int *numOfCamera);

void Baseline(Camera *camera, int *baselineCount, double *baselineCoverage, FILE *fp, int *numOfCamera) {
    Frame frame;
    int i, j, k, m;
    int start = 0, end = 0;
    int maxMCSTemp = 0;
    int cameraTemp;
    int datarateTemp;
    int mostRobustMCS;
    int contiguousDatarate;
    int nextSubframe = 0;
    int count = 0;
    int totalRB = 0;
    int checkObject[NumOfObject];
    int remainObject = NumOfObject;
    int checkRemainObject = 0;
    int reCoverage[NumOfObject];
    int totalCoverage = 0;
    double averageCoverage = 0.0;
    for (i = 0; i < NumOfObject; i++) {
        checkObject[i] = -1;
        reCoverage[i] = 0;
    }
    
    /*for(i = 0; i < NumOfCamera; i++){
        printf("Baselinecamera: %d, number: %d, coverage: ", i, camera[i].numOfCoverage);
        for(j = 0; j < camera[i].numOfCoverage; j++){
                printf("%d, ", camera[i].coverage[j]);
        }
        printf("\n");
    }*/

    //Initialize the frame
    for (i = 0; i < NumOfSubchannel; i++) {
        for (j = 0; j < NumOfSubframe; j++) {
            frame.index[j] = 0;
            frame.rb[i][j] = -1;
        }
    }

    for (i = 0; i < NumOfSubframe; i++) {
        if (checkRemainObject == 1) {
            checkRemainObject = 0;
            break;
        }
        start = 0;
        end = 0;
        for (m = 0; m < NumOfCamera; m++) {
            if (camera[m].isScheduled == 0 && camera[m].isConsider == 0 && camera[m].numOfCoverageTemp > 0) {
                camera[m].isConsider = 1;
            }
        }

        for (j = 0; j < NumOfSubchannel; j++) {
            maxMCSTemp = 0;
            for (k = 0; k < NumOfCamera; k++) {
                if (camera[k].mcs[j] > maxMCSTemp
                        && camera[k].isScheduled == 0
                        && camera[k].isConsider == 1
                        && camera[k].numOfCoverageTemp > 0) {
                    maxMCSTemp = camera[k].mcs[j];
                    cameraTemp = k;
                }
            }

            if (cameraTemp == -1) {
                nextSubframe = 1;
                goto NEXTSUBFRAME;
            }
            datarateTemp = camera[cameraTemp].requiredDatarate;
            while (datarateTemp > 0) {
                if (j == NumOfSubchannel) {
                    camera[cameraTemp].isConsider = 0;
                    nextSubframe = 1;
                    goto NEXTSUBFRAME;
                }
                datarateTemp = datarateTemp - camera[cameraTemp].mcs[j];
                j++;
            }
            end = j - 1;
            mostRobustMCS = most_robust_MCS(&camera[cameraTemp], start, end + 1);
            contiguousDatarate = mostRobustMCS * (end - start + 1);
            while (contiguousDatarate < camera[cameraTemp].requiredDatarate) {
                if (j == NumOfSubchannel) {
                    camera[cameraTemp].isConsider = 0;
                    nextSubframe = 1;
                    goto NEXTSUBFRAME;
                }
                if (camera[cameraTemp].mcs[j] < mostRobustMCS) {
                    mostRobustMCS = camera[cameraTemp].mcs[j];
                    contiguousDatarate = mostRobustMCS * (j - start + 1);
                } else {
                    contiguousDatarate = contiguousDatarate + mostRobustMCS;
                }
                j++;
                end++;
            }
            camera[cameraTemp].isScheduled = 1;
            camera[cameraTemp].isConsider = 0;
            for (m = start; m <= end; m++) { //allocate the resources to the camera
                frame.rb[m][i] = cameraTemp;
            }
            for (m = 0; m < camera[cameraTemp].numOfCoverage; m++) {
                checkObject[camera[cameraTemp].coverage[m]] = 1;
                reCoverage[camera[cameraTemp].coverage[m]]++;
            }
            //printf("1.remain: %d\n", remainObject);
            remainObject = remainObject - camera[cameraTemp].numOfCoverageTemp;
            //printf("2.remain: %d\n", remainObject);
            if (remainObject <= 0) {
                checkRemainObject = 1;
                break;
            }
            refresh_coverage(camera, checkObject);
            for (m = 0; m < NumOfCamera; m++) {
                if (camera[m].numOfCoverageTemp == 0) {
                    camera[m].isScheduled = 1;
                    camera[m].isConsider = 0;
                }
            }

            start = j; //the next start position
            j = j - 1; //need to be decreased because the loop will increase j again
            cameraTemp = -1;

NEXTSUBFRAME:
            if (nextSubframe == 1) {
                //printf("enter, count:%d\n", count);
                nextSubframe = 0;
                j = start - 1;
                cameraTemp = -1;
                for (m = 0; m < NumOfCamera; m++) {
                    if (camera[m].isConsider == 0) {
                        count++;
                    }
                }
                if (count == NumOfCamera) {
                    //printf("count:%d\n", count);
                    count = 0;
                    break;
                } else {
                    //printf("count:%d\n", count);
                    count = 0;
                    continue;
                }
            }
        }
    }

    //Print the allocation result
    for (i = 0; i < NumOfSubchannel; i++) {
        for (j = 0; j < NumOfSubframe; j++) {
            if (frame.rb[i][j] != -1) {
                totalRB++;
            }
            //printf("%5d", frame.rb[i][j]);
        }
        //printf("\n");
    }
    printf("Baseline RBs: %d\n", totalRB);
    fprintf(fp, "%d,", totalRB);
    *baselineCount = *baselineCount + totalRB;
    /*for(i = 0; i < NumOfObject; i++){
        totalCoverage = totalCoverage + reCoverage[i];
    }
    averageCoverage = (double)totalCoverage / NumOfObject;
    *baselineCoverage = *baselineCoverage + averageCoverage;*/
}
