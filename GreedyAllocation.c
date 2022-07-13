#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"
#include "GreedyAllocation.h"
#include "MCS.h"
#include "System.h"
#include "UE.h"

void GreedyAllocation(Camera *camera, Object *object, int *GreedyCount, FILE *fp){
    int i, j, k, n;
    int x, y;
    int index = 0;
    int numOfLayer = ManhattanStreetSize / 2;
    int layerNumOfMember[numOfLayer];  //Record the number of objects in i layer
    int coverageTemp = 0;
    int cameraTemp = -1;
    int totalRB = 0;
    Object **objectArr = NULL;
    Object swap;
    
    for(i = 0; i < numOfLayer; i++){
        layerNumOfMember[i] = 0;
    }
    
    for(i = 0; i < NumOfCamera; i++){
        camera[i].numOfCoverageTemp = camera[i].numOfCoverage;
    }
    
    for(i = 0; i < NumOfObject; i++){
        layerNumOfMember[object[i].layer - 1]++;
    }
    
    objectArr = (Object**)malloc(sizeof(Object*)*numOfLayer);
    for(i = 0; i < numOfLayer; i++){
        objectArr[i] = (Object*)malloc(sizeof(Object)*layerNumOfMember[i]);
    }
    
    for(i = 0; i < numOfLayer; i++){
        k = 0;
        for(j = 0; j < NumOfObject; j++){
            if((object[j].layer - 1) == i){     //陣列從0開始，第一層要放第0個
                objectArr[i][k] = object[j];
                k++;
            }
        }
    }
    //Sorting each layer
    for(i = 0; i < numOfLayer; i++){
        index = 0;
        x = i + 1;
        y = i + 1;
        for(k = 0; k < ((i + 2 + i) * 4); k++){       //第i+1層最多會有(i + 2 + i) * 4個object的位置，i從0開始
            for(j = 0; j < layerNumOfMember[i]; j++){
                if(objectArr[i][j].X == x && objectArr[i][j].Y == y){
                    objectArr[i][j].index = index;
                    index = index +1;
                }
            }
            //Update x, y according to the coordination
            if(x == (i + 1) && y == (i + 1)){       //In the corner of the first quadrant
                x = x;
                y = y - 1;
            }
            else if(x == (i + 1) && y == (-(i + 1))){ //In the corner of the fourth quadrant
                x = x - 1;
                y = y;
            }
            else if(x == (-(i + 1)) && y == (-(i + 1))){    //In the corner of the third quadrant
                x = x;
                y = y + 1;
            }
            else if(x == (-(i + 1)) && y == (i + 1)){       //In the corner of the second quadrant
                x = x +1;
                y = y;
            }
            else if(x == (i + 1) && y < (i +1) && y > (-(i + 1))){
                x = x;
                y = y - 1;
            }
            else if(x < (i + 1) && x > (-(i + 1)) && y == (-(i + 1))){
                x = x -1;
                y = y;
            }
            else if(x == (-(i + 1)) && y < (i + 1) && y > (-(i + 1))){
                x = x;
                y = y + 1;
            }
            else if(x < (i + 1) && x > (-(i + 1)) && y == (i + 1)){
                x = x + 1;
                y = y;
            }
        }
        //=========================================================
        //before sorting
        /*printf("Before sorting:\n");
        for(j = 0; j < layerNumOfMember[i]; j++){
            printf("(%f, %f, %d)", objectArr[i][j].X,  objectArr[i][j].Y, objectArr[i][j].index);
        }
        printf("\n");*/
        //=========================================================
        //sorting
        for(j = 0; j < (layerNumOfMember[i] - 1); j++){
            for(k = 0; k < (layerNumOfMember[i] - 1 - j); k++){
                if(objectArr[i][k].index > objectArr[i][k+1].index){
                    swap = objectArr[i][k];
                    objectArr[i][k] = objectArr[i][k+1];
                    objectArr[i][k+1] = swap;
                }    
            }
        }
        //=========================================================
    }
    /*for(i = 0; i < numOfLayer; i++){
        for(j = 0; j < layerNumOfMember[i]; j++){
            printf("(%f, %f, %d)", objectArr[i][j].X,  objectArr[i][j].Y, objectArr[i][j].index);
        }
        printf("\n");
    }*/
    //Scheduling
    for(i = 0; i < numOfLayer; i++){
        for(j = 0; j < layerNumOfMember[i]; j++){
            if(objectArr[i][j].isCovered == 0){ //Check whether the object has been covered
                for(k = 0; k < objectArr[i][j].numOfCoveringCamera; k++){
                    if(camera[objectArr[i][j].coveringCamera[k]].isScheduled == 0 && camera[objectArr[i][j].coveringCamera[k]].numOfCoverageTemp > coverageTemp){
                        coverageTemp = camera[objectArr[i][j].coveringCamera[k]].numOfCoverageTemp;
                        cameraTemp = objectArr[i][j].coveringCamera[k];
                    }
                }
                camera[cameraTemp].isScheduled = 1;
                objectArr[i][j].isCovered = 1;
                for(k = 0; k < camera[cameraTemp].numOfCoverage; k++){
                    for(n = 0; n < layerNumOfMember[i]; n++){
                        if(camera[cameraTemp].coverage[k] == objectArr[i][n].id){
                            objectArr[i][n].isCovered = 1;
                        }
                    }
                }
                RefreshCovering(camera, objectArr[i], layerNumOfMember[i]);
            }
            cameraTemp = -1;
            coverageTemp = 0;
        }
    }
    for(i = 0; i < NumOfCamera; i++){
        if(camera[i].isScheduled == 1){
            *GreedyCount = *GreedyCount + (camera[i].requiredDatarate / camera[i].mcs[0]) + 1;  //CQ都一樣所以取第0個
            totalRB = totalRB + (camera[i].requiredDatarate / camera[i].mcs[0]) + 1;
            //printf("MCS: %d\n", camera[i].mcs[0]);
        }
    }
    fprintf(fp, "%d,", totalRB);
    //printf("%d\n", *GreedyCount);
    /*for(i = 0; i < NumOfCamera; i++){
        printf("camera %d: %d", i, camera[i].isScheduled);
        printf("\n");
    }*/
}
void RefreshCovering(Camera *camera, Object *objectArr, int layerNumOfMember){
    int i, j, k;
    int temp = 0;
    //camera coverage
    for(i = 0; i < NumOfCamera; i++){
        temp = 0;
        if(camera[i].isScheduled == 1){
            camera[i].numOfCoverageTemp = -1;
        }
        else{
            for(j = 0; j < camera[i].numOfCoverage; j++){
                for(k = 0; k < layerNumOfMember; k++){
                    if(camera[i].coverage[j] == objectArr[k].id && objectArr[k].isCovered == 1){
                        temp++;
                    }
                }
            }
            camera[i].numOfCoverageTemp = camera[i].numOfCoverageTemp - temp;
        }
    }
    //=========================================
}