#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"
#include "EdgeCover.h"
#include "MCS.h"
#include "System.h"
#include "UE.h"

void Grouping(Object *object, Object *copiedObject, int flag);
void ShortestPath(Object *object, Object *copiedObject, int num, int path[num][num], Cover distCal[num][num]);
int Schedule(Camera *scheduleCamera, int num);

void EdgeCover(Camera *camera, Object *object, int *EdgeCoverCount, FILE *fp, int *numOfCamera){
    MCSoverRB(camera);
    int i, j, k, m, n;
    int temp = 100;
    int cameraTemp = -1;
    int edgeCoverCamera;
    int edgeCoverObject;
    int minNumOfRB = 100;   //初始化成一個很大的數
    int totalRB = 0;
    
    for(i = 0; i < NumOfCamera; i++){   //Record the min RBs and use it as the weight of camera
        for(j = 0; j < camera[i].numOfAllocation; j++){
            if(camera[i].allocation[j].numOfRB < minNumOfRB){
                minNumOfRB = camera[i].allocation[j].numOfRB;
            }
        }
        camera[i].weight = minNumOfRB;
        minNumOfRB = 100;
    }
    
    //是否要copy camera?
    k = 0;
    for(i = 0; i < *numOfCamera; i++){
        if(camera[i].numOfCoverage == 1){
            camera[i].isScheduled = 1;
        }
        else {
            k++;
        }
    }
    
    edgeCoverCamera = k;
    Camera copiedCamera[edgeCoverCamera*2];
    j = 0;
    for(i = 0; i < *numOfCamera; i++){
        if(camera[i].numOfCoverage == 1){}
        else{
            copiedCamera[j] = camera[i];
            j++;
        }
    }
    
    for(i = 0; i < edgeCoverCamera; i++){   //Record the min RBs and use it as the weight of camera
        for(j = 0; j < copiedCamera[i].numOfAllocation; j++){
            if(copiedCamera[i].allocation[j].numOfRB < minNumOfRB){
                minNumOfRB = copiedCamera[i].allocation[j].numOfRB;
            }
        }
        copiedCamera[i].weight = minNumOfRB;
        minNumOfRB = 100;
    }
    
    for(i = 0; i < edgeCoverCamera; i++){   //用edge cover來解將圖複製一遍
        copiedCamera[i+edgeCoverCamera] = copiedCamera[i];
    }
    //=======================================================
    
    k = 0;
    for(i = 0; i < NumOfObject; i++){
        if(object[i].numOfNeighbor > 0){
            k++;
        }
    }
    
    edgeCoverObject = k;
    Object copiedObject[edgeCoverObject*2];
    Cover dist[edgeCoverObject*2][edgeCoverObject*2];
    Cover distCal[edgeCoverObject*2][edgeCoverObject*2];
    int path[edgeCoverObject*2][edgeCoverObject*2];
    
    for(i = 0; i < edgeCoverObject*2; i++){
        for(j = 0; j < edgeCoverObject*2; j++){
            path[i][j] = 10000;
            dist[i][j].weight = 10000;
        }
    }    
    
    j = 0;
    for(i = 0; i < NumOfObject; i++){
        if(object[i].numOfNeighbor > 0){
            copiedObject[j] = object[i];
            copiedObject[j].id = j;
            copiedObject[j].isConsidered = 0;
            j++;
        }
    }
    
    for(i = 0; i < edgeCoverObject; i++){
        k = 0;
        for(j = 0; j < edgeCoverObject; j++){
            if(i != j){
                for(m = 0; m < copiedObject[i].numOfCoveringCamera; m++){
                    for(n = 0; n < copiedObject[j].numOfCoveringCamera; n++){
                        if(copiedObject[i].coveringCamera[m] ==  copiedObject[j].coveringCamera[n]){
                            copiedObject[i].neighbor[k] = j;
                            dist[i][j].weight = camera[copiedObject[i].coveringCamera[m]].weight;
                            dist[j][i].weight = dist[i][j].weight;   //i到j和j到i的距離一樣
                            dist[i][j].whichCamera = copiedObject[i].coveringCamera[m];
                            dist[j][i].whichCamera = dist[i][j].whichCamera;
                            k++;
                        }
                    }
                }
            }
        }
    }
    
    
    for(i = 0; i < edgeCoverObject; i++){
        copiedObject[i+edgeCoverObject] = copiedObject[i];
    }
    for(i = edgeCoverObject; i < edgeCoverObject*2; i++){
        copiedObject[i].id = i;
    }

    for(i = 0; i < edgeCoverObject; i++){
        for(j = 0; j < edgeCoverObject; j++){
            dist[i+edgeCoverObject][j+edgeCoverObject] = dist[i][j];
        }
    }
    
    for(i = 0; i < edgeCoverObject; i++){
        for(j = 0; j < copiedObject[i].numOfCoveringCamera; j++){
            if(camera[copiedObject[i].coveringCamera[j]].weight < temp){
                temp = camera[copiedObject[i].coveringCamera[j]].weight;
                cameraTemp = copiedObject[i].coveringCamera[j];
            }
        }
        dist[i][i+edgeCoverObject].weight = 2 * temp;
        dist[i][i+edgeCoverObject].whichCamera = cameraTemp;
        dist[i+edgeCoverObject][i].weight = 2 * temp;
        dist[i+edgeCoverObject][i].whichCamera = cameraTemp;
        temp = 100;
        cameraTemp = -1;
    }
    for(i = 0; i < edgeCoverObject; i++){
        Grouping(copiedObject, &copiedObject[i], 0);
    }
    
    for(i = 0; i < edgeCoverObject; i++){
        copiedObject[i+edgeCoverObject].group = copiedObject[i].group*(-1);
    }
 
    for(i = 0; i < edgeCoverObject*2; i++){
        for(j = 0; j < edgeCoverObject*2; j++){
            if(dist[i][j].weight != 10000 && copiedObject[i].group == -1 && copiedObject[j].group == 1){
                dist[i][j].from = -1;
                dist[i][j].weight = dist[i][j].weight;
                dist[j][i].from = 1;
                dist[j][i].weight = 10000;
            }
            else{
                dist[i][j].from = 1;
                dist[i][j].weight = 10000;
            }
        }
    }
    
    for(i = 0; i < edgeCoverObject*2; i++){
        for(j = 0; j < edgeCoverObject*2; j++){
            distCal[i][j] = dist[i][j];
        }
    }
    
    int min[edgeCoverObject*2];
    int whichObject[edgeCoverObject*2];
    int minDis = 10000; //Record the temporary shortest path
    int end = -1;
    int start = -1;
    int sol[edgeCoverObject*2]; //record the path
    
    for(i = 0; i < edgeCoverObject*2; i++){
        min[i] = 10000;
        whichObject[i] = -1;
        sol[i] = -1;
        copiedObject[i].prev = (int*)malloc(sizeof(int)*(edgeCoverObject*2));
        for(j = 0; j < edgeCoverObject*2; j++){
            copiedObject[i].prev[j] = -1;
        }
    }
    //==========================================================
    int a;
    for(a = 0; a < edgeCoverObject*2; a++){
        for(i = 0; i < edgeCoverObject*2; i++){
            if(copiedObject[i].group == -1 && copiedObject[i].isConsidered == 0){
                ShortestPath(copiedObject, &copiedObject[i], edgeCoverObject*2, path, distCal);
                for(j = 0; j < edgeCoverObject*2; j++){
                    if(path[copiedObject[i].id][j] < min[copiedObject[i].id] && copiedObject[j].isConsidered == 0){
                        min[copiedObject[i].id] = path[copiedObject[i].id][j];
                        whichObject[copiedObject[i].id] = j;
                    }
                }
                for(m = 0; m < edgeCoverObject*2; m++){
                    for(n = 0; n < edgeCoverObject*2; n++){
                        distCal[m][n] = dist[m][n];
                    }
                }
            }
        }
        
        for(i = 0; i < edgeCoverObject*2; i++){
            if(min[i] < minDis){
                minDis = min[i];
                end = whichObject[i];
                start = i;
            }
        }
        if(start != -1 && end != -1){
            copiedObject[start].isConsidered = 1;
            copiedObject[end].isConsidered = 1;
            i = 0;
            sol[i] = copiedObject[end].id;
            //printf("End Object: %d, %d\n", copiedObject[end].id, end);
            //printf("Prev: %d\n", copiedObject[sol[i]].prev);
            i++;

            while(copiedObject[sol[i-1]].prev[start] != -1){ //紀錄路徑
                sol[i] = copiedObject[sol[i-1]].prev[start];
                i++;
            }
            for(j = 0; j < i-1; j++){
                    dist[sol[j]][sol[j+1]].weight = -dist[sol[j+1]][sol[j]].weight;
                    dist[sol[j+1]][sol[j]].weight = 10000;
                    if(sol[j] >= edgeCoverObject && sol[j+1] >= edgeCoverObject){}
                    else{
                        if(camera[dist[sol[j]][sol[j+1]].whichCamera].isScheduled == 0){
                            camera[dist[sol[j]][sol[j+1]].whichCamera].isScheduled = 1;
                        }
                        else{
                            camera[dist[sol[j]][sol[j+1]].whichCamera].isScheduled = 1;
                        }
                    }
            }
            
            minDis = 10000;
            end = -1;
            start = -1;
            for(i = 0; i < edgeCoverObject*2; i++){
                for(j = 0; j < edgeCoverObject*2; j++){
                    distCal[i][j] = dist[i][j];
                    path[i][j] = 10000;
                    copiedObject[i].prev[j] = -1;
                }
                sol[i] = -1;
                min[i] = 10000;
                whichObject[i] = -1;
            }
        }
    }
   for(i = 0; i < *numOfCamera; i++){
        if(camera[i].isScheduled == 1){
            printf("Camera: %d, %d, %d\n", i, camera[i].id, camera[i].weight);
        }
    }
    totalRB = Schedule(camera, *numOfCamera);
    fprintf(fp, "%d,", totalRB);
    *EdgeCoverCount = *EdgeCoverCount + totalRB;
    //============================================================
    
    //print the information
    /*for(i = 0; i < edgeCoverObject; i++){
        printf("object %d: ", copiedObject[i].id);
        for(j = 0; j < copiedObject[i].numOfNeighbor; j++){
            printf("%d, ", copiedObject[i].neighbor[j]);
        }
        printf("\n");
    }*/  
    
    /*for(i = 0; i < edgeCoverObject; i++){
        printf("object %d: %d\n", i, copiedObject[i].group);
    }
    
    for(i = 0; i < edgeCoverObject*2; i++){
        for(j = 0; j < edgeCoverObject*2; j++){
            if(dist[i][j].weight == 10000)
                printf("0  ");
            else
                printf("%d", dist[i][j].weight);
        }
        printf("\n");
    }*/
    //=========================================
}

void Grouping(Object *object, Object *copiedObject, int flag){  //bipartite graph分成兩邊
    int i, j;
    if(copiedObject->isColored == 0){
        if(flag == 0){
            copiedObject->group = 1;
            copiedObject->isColored = 1;
            //copiedObject->prev = -1;
        }
        for(i = 0; i < copiedObject->numOfNeighbor; i++){
            object[copiedObject->neighbor[i]].group = copiedObject->group * (-1);
            object[copiedObject->neighbor[i]].isColored = 1;
        }
        for(i = 0; i < copiedObject->numOfNeighbor; i++){
            Grouping(object, &object[copiedObject->neighbor[i]], 1);
        }
    }
}

void ShortestPath(Object *object, Object *copiedObject, int num, int path[num][num], Cover distCal[num][num]){
    int i, j;
    int min = 10000;
    int count = 1;
    int whichObject;
    int start;

    for(i = 0; i < num; i++){
        object[i].flag = 0;
    }
    copiedObject->prev[copiedObject->id] = -1; //represent the first node
    object[copiedObject->id].prev[copiedObject->id] = -1;
    object[copiedObject->id].flag = 1;
    start = copiedObject->id;

    for(i = 0; i < num; i++){
        if(distCal[copiedObject->id][i].weight < min && object[i].isConsidered == 0){
            object[i].prev[copiedObject->id] = copiedObject->id;
        }
    }
    while(count < num){
        if(count == 1){
            for(i = 0; i < num; i++){
                if(object[i].group == 1 && object[i].isConsidered == 0 && object[i].flag == 0){
                    if(distCal[copiedObject->id][i].weight < min && object[i].flag == 0){
                        min = distCal[copiedObject->id][i].weight;
                        whichObject = i;
                    }
                }
            }
            if(min != 10000){
                object[whichObject].flag = 1;
                object[whichObject].prev[copiedObject->id] = copiedObject->id;
                path[copiedObject->id][whichObject] = min;
            }
            start = whichObject;
            min = 10000;
            count++;
        }
        else{
            for(i = 0; i < num; i++){
                if(object[i].group == 1 && object[i].isConsidered == 0 && object[i].flag == 0){
                    if((distCal[copiedObject->id][start].weight + distCal[start][i].weight) < distCal[copiedObject->id][i].weight && object[i].flag == 0){
                        distCal[copiedObject->id][i].weight = distCal[copiedObject->id][start].weight + distCal[start][i].weight;
                        object[i].prev[copiedObject->id] = start;
                    }
                }
            }
            for(i = 0; i < num; i++){
                if(object[i].group == 1 && object[i].isConsidered == 0 && object[i].flag == 0){
                    if(distCal[copiedObject->id][i].weight < min && object[i].flag == 0){
                        min = distCal[copiedObject->id][i].weight;
                        whichObject = i;
                    }
                }
            }
            if(min != 10000){
                object[whichObject].flag = 1;
                object[whichObject].prev[copiedObject->id] = start;
                path[copiedObject->id][whichObject] = min;
            }
            start = whichObject;
            min = 10000;
            count++;
        }
    }
}

int Schedule(Camera *scheduleCamera, int num){
    int i, j, k;
    int m, n;
    int temp = 100;
    int cameraTemp = -1;
    int allocationTemp = -1;
    int count = 0;
    int total = 0;
    Frame f;
    for(i = 0; i < NumOfSubchannel; i++){
        for(j = 0; j < NumOfSubframe; j++){
            f.rb[i][j] = 0;
        }
    }
    for(i = 0; i < num; i++){
        if(scheduleCamera[i].isScheduled == 1)
            count++;
    }
    
    m = 0, n = 0;
    while(count != 0){
        for(i = 0; i < num; i++){
            if(scheduleCamera[i].isScheduled == 1 && scheduleCamera[i].isScheduledAllo == 0){
                for(k = 0; k < scheduleCamera[i].numOfAllocation; k++){
                    if(scheduleCamera[i].allocation[k].start == m && scheduleCamera[i].allocation[k].numOfRB < temp){
                        temp = scheduleCamera[i].allocation[k].numOfRB;
                        cameraTemp = i;
                        allocationTemp = k;
                    }
                }
            }
        }
        if(cameraTemp == -1){
            m = 0;
            n++;
        }
        else{
            for(i = scheduleCamera[cameraTemp].allocation[allocationTemp].start; i < scheduleCamera[cameraTemp].allocation[allocationTemp].end; i++){
                f.rb[i][n] = 1;
            }
            m = i;
            scheduleCamera[cameraTemp].isScheduledAllo = 1;
            temp = 100;
            cameraTemp = -1;
            allocationTemp = -1;
            count--;
        }
    }
    for(i = 0; i < NumOfSubchannel; i++){
        for(j = 0; j < NumOfSubframe; j++){
            if(f.rb[i][j] == 1){
                total++;
            }
        }
    }
    
    printf("S: %d, E: %d\n", scheduleCamera[2].allocation[1].start,  scheduleCamera[2].allocation[1].end);
    printf("Total: %d\n", total);
    return total;
}


