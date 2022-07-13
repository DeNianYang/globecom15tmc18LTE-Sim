#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"
#include "GreedyAllocation.h"
#include "MCS.h"
#include "System.h"
#include "UE.h"

void DP_based(Camera *camera, Object *object, int *DPCount, FILE *fp){
    int i, j, k, m, n;
    int a, b;
    int c;
    int x, y;
    int index = 0;      //給object做編號排序用
    int numOfLayer = ManhattanStreetSize / 2;
    int layerNumOfMember[numOfLayer];  //Record the number of objects in i layer
    //int coverageTemp = 0;
    int cameraTemp = -1;
    //針對每一個object來看
    int numOfRB[NumOfObject][NumOfObject];
    int *cameraSet[NumOfObject][NumOfObject];
    int cameraCount[NumOfObject][NumOfObject];
    //===================================
    int numOfRBTemp = 1000;
    int length = 0;     //要考慮多少個連續的object
    int division;       //記錄DP從哪邊切
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
    
    for(i = 0; i < NumOfObject; i++){
        for(j = 0; j < NumOfObject; j++){
            numOfRB[i][j] = 1000;
        }
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
        /*printf("DP Before sorting:\n");
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
    //DP Scheduling
    n = 0;
    for(i = 0; i < numOfLayer; i++){
        for(j = 0; j < layerNumOfMember[i]; j++){
            for(k = 0; k < objectArr[i][j].numOfCoveringCamera; k++){
                if((camera[objectArr[i][j].coveringCamera[k]].requiredDatarate/camera[objectArr[i][j].coveringCamera[k]].mcs[0]) + 1 < numOfRBTemp){
                    cameraTemp = objectArr[i][j].coveringCamera[k];
                    numOfRBTemp = (camera[cameraTemp].requiredDatarate/camera[cameraTemp].mcs[0]) + 1;
                }
            }
            numOfRB[n][n] = numOfRBTemp;
            cameraCount[n][n] = 1;
            cameraSet[n][n] = (int*)malloc(sizeof(int)*cameraCount[n][n]);
            cameraSet[n][n][0] = cameraTemp;
            //printf("%d: %d\n", n, cameraSet[n][n][0]);
            cameraTemp = -1;
            numOfRBTemp = 1000;
            n++;
        }
    }
    
    n = 0;
    for(i = 0; i < numOfLayer; i++){
        if(layerNumOfMember[i] >= 2){
            for(j = 0; j < layerNumOfMember[i] - 1; j++){
                for(k = 0; k < objectArr[i][j].numOfCoveringCamera; k++){
                    for(m = 0; m < objectArr[i][j+1].numOfCoveringCamera; m++){
                        if(objectArr[i][j].coveringCamera[k] == objectArr[i][j+1].coveringCamera[m]){
                            cameraTemp = objectArr[i][j].coveringCamera[k];
                            numOfRBTemp = (camera[cameraTemp].requiredDatarate/camera[cameraTemp].mcs[0]) + 1;
                        }
                    }
                }
                if(cameraTemp != -1 && (numOfRB[n][n] + numOfRB[n+1][n+1]) > numOfRBTemp){
                    numOfRB[n][n+1] = numOfRBTemp;
                    cameraCount[n][n+1] = 1;
                    cameraSet[n][n+1] = (int*)malloc(sizeof(int)*cameraCount[n][n+1]);
                    cameraSet[n][n+1][0] = cameraTemp;
                    cameraTemp = -1;
                    numOfRBTemp = 1000;
                    n++;
                }
                else{
                    numOfRB[n][n+1] = numOfRB[n][n] + numOfRB[n+1][n+1];
                    cameraCount[n][n+1] = 2;
                    cameraSet[n][n+1] = (int*)malloc(sizeof(int)*cameraCount[n][n+1]);
                    cameraSet[n][n+1][0] = cameraSet[n][n][0];
                    cameraSet[n][n+1][1] = cameraSet[n+1][n+1][0];
                    cameraTemp = -1;
                    numOfRBTemp = 1000;
                    n++;                    
                }
            }
            n++;    
        //因為在到下一層的時候這一層的object不會和下一層一起計算，
        //也就是這一層的最後一個object，和下一層的第一個object沒關係
        }
    }
    
    n = 0;
    for(i = 0; i < numOfLayer; i++){
        if(layerNumOfMember[i] >= 3){
            for(length = 3; length <= layerNumOfMember[i]; length++){
                for(j = 0; j < layerNumOfMember[i] - length + 1; j++){
                    b = j + length - 1;
                    for(a = j + n; a < b + n; a++){
                        numOfRBTemp = numOfRB[j+n][a] + numOfRB[a+1][b+n];
                        if(numOfRBTemp < numOfRB[j+n][b+n]){
                            numOfRB[j+n][b+n] = numOfRBTemp;
                            division = a;
                        }
                    }
                    cameraCount[j+n][b+n] = cameraCount[j+n][division] + cameraCount[division+1][b+n];
                    cameraSet[j+n][b+n] = (int*)malloc(sizeof(int)*cameraCount[j+n][b+n]);
                    for(c = 0; c < cameraCount[j+n][b+n]; c++){
                        if(c < cameraCount[j+n][division]){
                            cameraSet[j+n][b+n][c] = cameraSet[j+n][division][c];
                        }
                        else{
                            cameraSet[j+n][b+n][c] = cameraSet[division+1][b+n][c-cameraCount[j+n][division]];
                        }
                    }
                    numOfRBTemp = 1000;
                }
            }
        }
        n = n + layerNumOfMember[i];
    }
    j = 0;
    for(i = 0; i < numOfLayer; i++){
        *DPCount = *DPCount + numOfRB[j][j+layerNumOfMember[i]-1];
        totalRB = totalRB + numOfRB[j][j+layerNumOfMember[i]-1];
        //printf("DP: %d\n", numOfRB[j][j+layerNumOfMember[i]-1]);
        j = j + layerNumOfMember[i];
    }
    fprintf(fp, "%d,", totalRB);
}
