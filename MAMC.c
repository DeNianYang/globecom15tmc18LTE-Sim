#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"
#include "MAMC.h"
#include "MCS.h"
#include "System.h"

int numOfRBScheduled = 0;

void MAMC(Camera *camera, int *MAMCCount, double *MAMCCoverage, FILE *fp, int *num) {
    MCSoverRB(camera);
    //Iteratively find the most suitable camera for scheduling
    Frame frame;
    int i, j, k;
    int numOfRBTemp = NumOfSubchannel; //Record the minimum number of RBs currently found
    int remainObject = NumOfObject; //Record the number of objects uncovered
    int remainCamera = *num;
    int cameraTemp; //Record the camera that is allocating
    int subframeTemp; //Record the subframe that camera is allocated
    int allocationTemp; //Record the allocation that the allocated camera is using 
    float averageCost;
    int checkObject[NumOfObject];
    int count = 0;
    int adjust = 0; //Check whether enter adjustment step
    int checkAllocation = 0; //Record the number of allocation that cannot be used
    int totalRB = 0;
    int reCoverage[NumOfObject];
    int totalCoverage = 0;
    double averageCoverage = 0.0;
    for (i = 0; i < NumOfObject; i++) {
        checkObject[i] = -1;
        reCoverage[i] = 0;
    }
    
    /*for(i = 0; i < NumOfCamera; i++){
        printf("MAMCcamera: %d, number: %d, coverage: ", i, camera[i].numOfCoverage);
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
    printf("dis: %d\n", (int)camera[2].distance);
    printf("%d\n", remainCamera);
    while (remainObject > 0 && remainCamera > 0) {
        //printf("before #objects: %d\n", remainObject);
        averageCost = NumOfSubchannel;
        for (i = 0; i < NumOfSubframe; i++) {
            for (j = 0; j < NumOfCamera; j++) {
                for (k = 0; k < camera[j].numOfAllocation; k++) {
                    if (camera[j].allocation[k].start == frame.index[i]
                            && camera[j].allocation[k].end <= NumOfSubchannel
                            && camera[j].numOfCoverageTemp > 0
                            && camera[j].isScheduled == 0
                            && (float) (camera[j].allocation[k].numOfRB / camera[j].numOfCoverageTemp) < averageCost
                            && camera[j].distance <= 200.0) {                        
                        averageCost = camera[j].allocation[k].numOfRB / camera[j].numOfCoverageTemp;
                        numOfRBTemp = camera[j].allocation[k].numOfRB;
                        subframeTemp = i;
                        cameraTemp = j;
                        allocationTemp = k;
                    }
                }
            }
        }
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

        /*Check whether there's still allocation can be used in greedy scheduling.
         If not, enter the adjustment step*/
        for (i = 0; i < camera[cameraTemp].numOfAllocation; i++) {
            camera[cameraTemp].allocation[i].flag = 1; //record whether the allocation is used
        }
        count = 0;
        for (i = 0; i < NumOfCamera; i++) {
            for (j = 0; j < camera[i].numOfAllocation; j++) {
                for (k = 0; k < NumOfSubframe; k++) {
                    if (frame.index[k] > camera[i].allocation[j].start) {
                        count++;
                    } else {
                        continue;
                    }
                    if (count == NumOfSubframe) {
                        camera[i].allocation[j].flag = 1;
                    }
                }
                count = 0;
            }
            if (camera[i].numOfCoverageTemp == 0) {
                for (j = 0; j < camera[i].numOfAllocation; j++) {
                    camera[i].allocation[j].flag = 1;
                }
            }
        }
        count = 0;
        checkAllocation = 0;
        for (i = 0; i < NumOfCamera; i++) {
            checkAllocation = checkAllocation + camera[i].numOfAllocation;
            for (j = 0; j < camera[i].numOfAllocation; j++) {
                if (camera[i].allocation[j].flag == 1) {
                    count++;
                }
            }
        }
        //printf("Check:%d, count:%d\n", checkAllocation, count);
        if (adjust == 0) {
            if (count == checkAllocation && remainObject > 0) {
                adjustment(camera, &frame, &remainObject, checkObject);
                adjust = 1;
            } else if (remainObject == 0) {
                //printf("Feasible solution\n");
            } else {
                //printf("Infeasible solution\n");
            }
        } else {
            break;
        }
        //printf("=======================\n");
    }
    for (i = 0; i < NumOfSubchannel; i++) {
        for (j = 0; j < NumOfSubframe; j++) {
            if (frame.rb[i][j] != -1) {
                totalRB++;
            }
            //printf("%5d", frame.rb[i][j]);
        }
        //printf("\n");
    }
    printf("MAMC RBs: %d\n", totalRB);
    fprintf(fp, "%d,", totalRB);
    *MAMCCount = *MAMCCount + totalRB;
    /*for(i = 0; i < NumOfObject; i++){
        totalCoverage = totalCoverage + reCoverage[i];
    }
    averageCoverage = (double)totalCoverage / NumOfObject;
    *MAMCCoverage = *MAMCCoverage + averageCoverage;*/
}

//Calculate the MCS for each camera on each RB, and find all minimum feasible allocation
//for each camera on different started position

void MCSoverRB(Camera *camera) {
    int i, j, k, m;
    int mostRobustMCS;
    int datarateTemp = 0;
    int contiguousDatarate = 0;
    int numOfAllocationTemp = 0;
    int allocationRecord[NumOfSubchannel][4];
    for (i = 0; i < NumOfCamera; i++) {
        for (j = 0; j < NumOfSubchannel; j++) { //subchannel index
            for (k = j; k < NumOfSubchannel; k++) {
                datarateTemp = datarateTemp + camera[i].mcs[k];
                if (datarateTemp < camera[i].requiredDatarate)
                    continue;
                else {
                    mostRobustMCS = most_robust_MCS(&camera[i], j, k + 1);
                    contiguousDatarate = mostRobustMCS * (k - j + 1);
                    if (contiguousDatarate < camera[i].requiredDatarate) {
                        continue;
                    } else {
                        //Record the allocation of camera i
                        allocationRecord[numOfAllocationTemp][0] = j;
                        allocationRecord[numOfAllocationTemp][1] = k;
                        allocationRecord[numOfAllocationTemp][2] = contiguousDatarate;
                        allocationRecord[numOfAllocationTemp][3] = k - j + 1;
                        numOfAllocationTemp++;
                        //printf("Camera%d: start:%d, end:%d, rate:%d, #RBs:%d \n", i, j, k, contiguousDatarate, k - j + 1);
                        break;
                    }
                }
            }
            datarateTemp = 0;
            contiguousDatarate = 0;
        }
        camera[i].numOfAllocation = numOfAllocationTemp;
        //printf("Camera%d #allocation: %d\n", i, numOfAllocationTemp);
        //printf("------------------------------------------------------------\n");
        camera[i].allocation = (Allocation*) malloc(sizeof (Allocation) * numOfAllocationTemp);
        for (m = 0; m < numOfAllocationTemp; m++) { //The information of allocations for each camera
            camera[i].allocation[m].start = allocationRecord[m][0];
            camera[i].allocation[m].end = allocationRecord[m][1];
            camera[i].allocation[m].allocationDatarate = allocationRecord[m][2];
            camera[i].allocation[m].numOfRB = allocationRecord[m][3];
            camera[i].allocation[m].flag = 0;
        }
        numOfAllocationTemp = 0;
    }
}

//Calculate the most robust mcs among allocated RBs for the camera

int most_robust_MCS(Camera *checkCamera, int start, int end) {
    int i;
    int temp = QAM16_34;
    for (i = start; i < end; i++) {
        if (checkCamera -> mcs[i] < temp)
            temp = checkCamera -> mcs[i];
    }
    return temp;
}

//Refresh the number of uncovered objects for each camera, i.e. |S_k∩Z|

void refresh_coverage(Camera *camera, int *checkObject) {
    int i, j, k;
    for (i = 0; i < NumOfCamera; i++) {
        camera[i].numOfCoverageTemp = camera[i].numOfCoverage;
        for (j = 0; j < NumOfObject; j++) {
            for (k = 0; k < camera[i].numOfCoverage; k++) {
                if (camera[i].coverage[k] == j && checkObject[j] == 1) {
                    camera[i].numOfCoverageTemp = camera[i].numOfCoverageTemp - 1;
                }
            }
        }
    }
}

void adjustment(Camera *camera, Frame *frame, int *remainObject, int *checkObject) {
    int i = 0, j = 0, k = 0, m = 0, n = 0;
    int SLINDEX = 0;
    int USLINDEX = 0;
    int count = 0;
    int RBTemp = 0;
    int RBTempMin = 2 * NumOfSubchannel;
    int scheduleCameraTemp;
    int unscheduleCameraTemp;
    int subframeTemp;
    int unscheduleAllocationTemp;
    int scheduleAllocationTemp;
    int interchange = 0;
    for (i = 0; i < NumOfCamera; i++) { // Calculate the number of scheduled cameras
        if (camera[i].isScheduled == 1 && camera[i].no == 0) {
            count++;
        }
    }

    Camera * schedulingList[count];
    Camera * unschedulingList[NumOfCamera - count];
    Camera *temp;
    count = 0;
    for (i = 0; i < NumOfCamera; i++) { // Build the scheduling list and unscheduling list
        if (camera[i].isScheduled == 1 && camera[i].no == 0) {
            schedulingList[SLINDEX] = &camera[i];
            SLINDEX++;
        } else if(camera[i].isScheduled == 0 && camera[i].no == 0){
            unschedulingList[USLINDEX] = &camera[i];
            USLINDEX++;
        }
    }

    /*Sort list based on the number of coverage temp for the purpose to
     * first consider the camera that have the maximum coverage*/
    for (i = 0; i < USLINDEX; i++) {
        for (j = i; j < USLINDEX; j++) {
            if (unschedulingList[j]->numOfCoverageTemp > unschedulingList[i]->numOfCoverageTemp) {
                temp = unschedulingList[j];
                unschedulingList[j] = unschedulingList[i];
                unschedulingList[i] = temp;
            }
        }
    }
    /*printf("=======================\n");
    for (i = 0; i < USLINDEX; i++) {
        printf("%d, ", unschedulingList[i]->numOfCoverageTemp);
    }
    printf("\n");*/


    for (i = 0; i < USLINDEX; i++) { //i: the length of unscheduling list
        for (j = 0; j < SLINDEX; j++) { //j: the length of scheduling list
            for (k = 0; k < unschedulingList[i]->numOfAllocation; k++) { //unschedulingList's allocation
                if (unschedulingList[i]->allocation[k].start >= schedulingList[j]->allocation[schedulingList[j]->allocationUsed].start
                        && unschedulingList[i]->allocation[k].end <= schedulingList[j]->allocation[schedulingList[j]->allocationUsed].end) {
                    for (m = 0; m < NumOfSubframe; m++) {
                        for (n = 0; n < schedulingList[j]->numOfAllocation; n++) {
                            if (schedulingList[j]->allocation[n].start >= frame->index[m]) {
                                RBTemp = (unschedulingList[i]->allocation[k].end - unschedulingList[i]->allocation[k].start) + 1
                                        + (schedulingList[j]->allocation[n].end - schedulingList[j]->allocation[n].start) + 1;
                                interchange = 1;
                                if (RBTemp < RBTempMin) {
                                    RBTempMin = RBTemp;
                                    unscheduleCameraTemp = i;
                                    scheduleCameraTemp = j;
                                    unscheduleAllocationTemp = k;
                                    subframeTemp = m;
                                    scheduleAllocationTemp = n;
                                }
                                //printf("Enter interchange stage!\n");
                            }
                        }
                    }
                }
            }
        }
        if (interchange == 1) {
            interchange = 0;
            /*Set -1 to the posistion that the unscheduled camera will be scheduled to*/
            for (j = schedulingList[scheduleCameraTemp]->allocation[schedulingList[scheduleCameraTemp]->allocationUsed].start;
                    j <= schedulingList[scheduleCameraTemp]->allocation[schedulingList[scheduleCameraTemp]->allocationUsed].end; j++) {
                frame->rb[j][schedulingList[scheduleCameraTemp]->subframeUsed] = -1;
            }
            for (j = unschedulingList[unscheduleCameraTemp]->allocation[unscheduleAllocationTemp].start;
                    j <= unschedulingList[unscheduleCameraTemp]->allocation[unscheduleAllocationTemp].end; j++) {
                frame->rb[j][schedulingList[scheduleCameraTemp]->subframeUsed] = unschedulingList[unscheduleCameraTemp]->id;
            }
            for (j = schedulingList[scheduleCameraTemp]->allocation[scheduleAllocationTemp].start;
                    j <= schedulingList[scheduleCameraTemp]->allocation[scheduleAllocationTemp].end; j++) {
                frame->rb[j][subframeTemp] = schedulingList[scheduleCameraTemp]->id;
            }
            frame->index[subframeTemp] = schedulingList[scheduleCameraTemp]->allocation[scheduleAllocationTemp].end + 1;
            unschedulingList[unscheduleCameraTemp]->allocationUsed = unscheduleAllocationTemp;
            unschedulingList[unscheduleCameraTemp]->subframeUsed = schedulingList[scheduleCameraTemp]->subframeUsed;
            unschedulingList[unscheduleCameraTemp]->isScheduled = 1;
            schedulingList[scheduleCameraTemp]->allocationUsed = scheduleAllocationTemp;
            schedulingList[scheduleCameraTemp]->subframeUsed = subframeTemp;
            *remainObject = *remainObject - unschedulingList[unscheduleCameraTemp]->numOfCoverageTemp;
            /*for (j = 0; j < NumOfSubchannel; j++) {
                for (k = 0; k < NumOfSubframe; k++) {
                    printf("%5d", frame->rb[j][k]);
                }
                printf("\n");
            }*/
        }
    }
    //printf("Adjustment step\n");
}
