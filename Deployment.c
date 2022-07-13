#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"
#include "Deployment.h"
#include "System.h"
#include "UE.h"

//Deploy cameras randomly in an BS area

void grid_deploy(Camera *camera) { //grid-based deployment
    int i;
    int j = -1;
    for (i = 0; i < NumOfCamera; i++) {
        if (i % (int) sqrt(NumOfCamera) == 0) {
            j++;
        }
        camera[i].X = -210 + (i % (int) sqrt(NumOfCamera)) * 70;
        camera[i].Y = -210 + j * 70;
        camera[i].distance = sqrt(pow(camera[i].X, 2) + pow(camera[i].Y, 2));
        if (camera[i].distance == 0.0) {
            camera[i].distance += 35;
        }
        //printf("(%.2lf, %.2lf)\n", camera[i].X, camera[i].Y);
    }
}

//All cameras are outward
void manhattan_street_deploy(Camera *camera, Object *object){
    int i, j, k, m;
    int temp, quotient, remainder;
    int size = ManhattanStreetSize * ManhattanStreetSize;
    int location[size];
    
    for(i = 0; i < size; i++){
        location[i] = 0;
        if(i == (size/2))
            location[i] = 1;
    }
    for(i = 0; i < NumOfObject; i++){
        quotient = 0;
        remainder = 0;
        temp = rand() % size;
        while(location[temp] == 1){
            temp = rand() % size;
        }
        location[temp] = 1;
        quotient = temp / ManhattanStreetSize;
        remainder = temp % ManhattanStreetSize;
        //printf("Q %d, R %d\n", quotient, remainder);
        quotient = quotient + (ManhattanStreetSize / 2) - (quotient * 2);
        remainder = remainder - (ManhattanStreetSize / 2);
        object[i].X = remainder;
        object[i].Y = quotient;
        object[i].id = i;
        //printf("object %d: (%f, %f)\n", i, object[i].X, object[i].Y);

        //After deploy object, we need to deploy camera behind it on the grid        
        if(object[i].X == 0){
            camera[i].X = object[i].X - 1;
            camera[i].Y = object[i].Y;
        }
        else if(object[i].Y == 0){
            camera[i].X = object[i].X;
            camera[i].Y = object[i].Y + 1;
        }
        else{
            camera[i].X = object[i].X;
            camera[i].Y = object[i].Y;
        }
        //printf("camera %d: (%f, %f)\n\n", i, camera[i].X, camera[i].Y);
        //==================================================
    }
    //Record the camera direction and its coverage
    for(i = 0; i < NumOfObject; i++){   //先只考慮每個object旁的camera
        //The camera whose direction is right
        if(fabs(camera[i].X) > fabs(camera[i].Y) && camera[i].X > 0){
            camera[i].direction = right;
            for(j = 0; j < NumOfObject; j++){
                if(camera[i].Y > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y - 1))
                        camera[i].numOfCoverage++;
                }
                else if(camera[i].Y < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y + 1))
                        camera[i].numOfCoverage++;
                }
            }
            camera[i].coverage = (int*) malloc(sizeof (int)*camera[i].numOfCoverage);
            k = 0;
            for(j = 0; j < NumOfObject; j++){
                if(camera[i].Y > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y - 1)){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }
                else if(camera[i].Y < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y + 1)){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }                
            }
        }
        //The camera whose direction is left
        else if(fabs(camera[i].X) > fabs(camera[i].Y) && camera[i].X < 0){
            camera[i].direction = left;
            for(j = 0; j < NumOfObject; j++){
                if(camera[i].Y > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y - 1))
                        camera[i].numOfCoverage++;
                }
                else if(camera[i].Y < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y + 1))
                        camera[i].numOfCoverage++;
                }
            }
            camera[i].coverage = (int*) malloc(sizeof (int)*camera[i].numOfCoverage);
            k = 0;
            for(j = 0; j < NumOfObject; j++){
                if(camera[i].Y > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y - 1)){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }
                else if(camera[i].Y < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y + 1)){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }                
            }
        }
        //The camera whose direction is up
        else if(fabs(camera[i].X) < fabs(camera[i].Y) && camera[i].Y > 0){
            camera[i].direction = up;
            for(j = 0; j < NumOfObject; j++){
                if(camera[i].X > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == (camera[i].X - 1) && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                }
                else if(camera[i].X < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == (camera[i].X + 1) && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                }
            }
            camera[i].coverage = (int*) malloc(sizeof (int)*camera[i].numOfCoverage);
            k = 0;
            for(j = 0; j < NumOfObject; j++){
                if(camera[i].X > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == (camera[i].X - 1) && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }
                else if(camera[i].X < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == (camera[i].X + 1) && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }                
            }
        }
        //The camera whose direction is down
        else if(fabs(camera[i].X) < fabs(camera[i].Y) && camera[i].Y < 0){
            camera[i].direction = down;
            for(j = 0; j < NumOfObject; j++){
                if(camera[i].X > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == (camera[i].X - 1) && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                }
                else if(camera[i].X < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == (camera[i].X + 1) && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                }
            }
            camera[i].coverage = (int*) malloc(sizeof (int)*camera[i].numOfCoverage);
            k = 0;
            for(j = 0; j < NumOfObject; j++){
                if(camera[i].X > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == (camera[i].X - 1) && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }
                else if(camera[i].X < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == (camera[i].X + 1) && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }                
            }
        }
        //For the camera in the corner, we let it can cover at most three objects
        else if(fabs(camera[i].X) == fabs(camera[i].Y)){
            for(j = 0; j < NumOfObject; j++){
                //For cameras in the first quadrant
                if(camera[i].X > 0 && camera[i].Y > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == (camera[i].X - 1) && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y - 1))
                        camera[i].numOfCoverage++;
                }
                //For cameras in the second quadrant
                else if(camera[i].X < 0 && camera[i].Y > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == (camera[i].X + 1) && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y - 1))
                        camera[i].numOfCoverage++;        
                }
                //For cameras in the third quadrant
                else if(camera[i].X < 0 && camera[i].Y < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == (camera[i].X + 1) && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y + 1))
                        camera[i].numOfCoverage++;        
                }
                //For cameras in the fourth quadrant
                else if(camera[i].X > 0 && camera[i].Y < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == (camera[i].X - 1) && object[j].Y == camera[i].Y)
                        camera[i].numOfCoverage++;
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y + 1))
                        camera[i].numOfCoverage++;         
                }
            }
            camera[i].coverage = (int*) malloc(sizeof (int)*camera[i].numOfCoverage);
            k = 0;
            for(j = 0; j < NumOfObject; j++){
                //For cameras in the first quadrant
                if(camera[i].X > 0 && camera[i].Y > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == (camera[i].X - 1) && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y - 1)){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }
                //For cameras in the second quadrant
                else if(camera[i].X < 0 && camera[i].Y > 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == (camera[i].X + 1) && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y - 1)){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }
                //For cameras in the third quadrant
                else if(camera[i].X < 0 && camera[i].Y < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == (camera[i].X + 1) && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y + 1)){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }
                //For cameras in the fourth quadrant
                else if(camera[i].X > 0 && camera[i].Y < 0){
                    if(object[j].X == camera[i].X && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == (camera[i].X - 1) && object[j].Y == camera[i].Y){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                    else if(object[j].X == camera[i].X && object[j].Y == (camera[i].Y + 1)){
                        camera[i].coverage[k] = j;
                        k++;
                    }
                }
            }
        }
    }
    for(i = 0; i < NumOfObject; i++){
        if(fabs(object[i].X) >=  fabs(object[i].Y)){
            object[i].layer = fabs(object[i].X);
        }
        else if(fabs(object[i].X) <  fabs(object[i].Y)){
            object[i].layer = fabs(object[i].Y);
        }
        if(fabs(camera[i].X) >=  fabs(camera[i].Y)){
            camera[i].layer = fabs(camera[i].X);
        }
        else if(fabs(camera[i].X) <  fabs(camera[i].Y)){
            camera[i].layer = fabs(camera[i].Y);
        }
    }
    
    //Record which camera covers the object
    for(i = 0; i < NumOfObject; i++){
        for(j = 0; j < NumOfObject; j++){       //for camera
            for(k = 0; k < camera[j].numOfCoverage; k++){
                if(camera[j].coverage[k] == i){
                    object[i].numOfCoveringCamera++;
                }
            }
        }
        object[i].coveringCamera = (int*)malloc(sizeof(int)*object[i].numOfCoveringCamera);
        m = 0;
        for(j = 0; j < NumOfObject; j++){
            for(k = 0; k < camera[j].numOfCoverage; k++){
                if(camera[j].coverage[k] == i){
                    object[i].coveringCamera[m] = j;
                    m++;
                }
            }
        }
    }
    
    /*for(i = 0; i < NumOfObject; i++){
        printf("camera %d: ", i);
        for(j = 0; j < camera[i].numOfCoverage; j++){
                printf("%d, ", camera[i].coverage[j]);
        }
        printf("\n");
    }*/
}

void manhattan_street_deploy_edgecover(Camera *camera, Object *object, int *numOfCamera){
    int i, j, k, m;
    int temp, quotient, remainder;
    int size = ManhattanStreetSize * ManhattanStreetSize;
    int location[size];
    int grid[ManhattanStreetSize][ManhattanStreetSize];
    
    for(i = 0; i < ManhattanStreetSize; i++){
        for(j = 0; j < ManhattanStreetSize; j++){
            grid[i][j] = 0;
        }
    }
    
    for(i = 0; i < size; i++){
        location[i] = 0;
        if(i == (size/2))
            location[i] = 1;
    }
    for(i = 0; i < NumOfObject; i++){
        quotient = 0;
        remainder = 0;
        temp = rand() % size;
        while(location[temp] == 1){
            temp = rand() % size;
        }
        location[temp] = 1;
        quotient = temp / ManhattanStreetSize;
        remainder = temp % ManhattanStreetSize;
        //printf("Q %d, R %d\n", quotient, remainder);
        
        quotient = quotient + (ManhattanStreetSize / 2) - (quotient * 2);
        remainder = remainder - (ManhattanStreetSize / 2);
        object[i].X = remainder;
        object[i].Y = quotient;
        object[i].id = i;
        //printf("object %d: (%f, %f)\n", i, object[i].X, object[i].Y);
    }
    for(i = 0; i < NumOfObject; i++){
        for(j = 0; j < NumOfObject; j++){
            if(object[j].X == object[i].X + 1 && object[j].Y == object[i].Y){
                object[i].numOfNeighbor++;
            }
            else if(object[j].X == object[i].X - 1 && object[j].Y == object[i].Y){
                object[i].numOfNeighbor++;
            }
            else if(object[j].X == object[i].X && object[j].Y == object[i].Y - 1){
                object[i].numOfNeighbor++;
            }
            else if(object[j].X == object[i].X && object[j].Y == object[i].Y + 1){
                object[i].numOfNeighbor++;
            }
        }
    }
    for(i = 0; i < NumOfObject; i++){
        k = 0;
        object[i].neighbor = (int*)malloc(sizeof(int)*object[i].numOfNeighbor);
        for(j = 0; j < NumOfObject; j++){
             if(object[j].X == object[i].X + 1 && object[j].Y == object[i].Y){
                object[i].neighbor[k] = object[j].id;
                k++;
            }
            else if(object[j].X == object[i].X - 1 && object[j].Y == object[i].Y){
                object[i].neighbor[k] = object[j].id;
                k++;
            }
            else if(object[j].X == object[i].X && object[j].Y == object[i].Y - 1){
                object[i].neighbor[k] = object[j].id;
                k++;
            }
            else if(object[j].X == object[i].X && object[j].Y == object[i].Y + 1){
                object[i].neighbor[k] = object[j].id;
                k++;
            }
        }
    }
    
    k = 0;
    for(i = 0; i < NumOfObject; i++){
        if(object[i].numOfNeighbor == 0){
            camera[k].numOfCoverage = 1;
            camera[k].coverage = (int*)malloc(sizeof(int) * 1);
            camera[k].coverage[0] = object[i].id;
            k++;            
        }
        else{
            for(j = 0; j < object[i].numOfNeighbor; j++){
                if(object[object[i].neighbor[j]].isTried == 1){
                    continue;
                }
                else{
                    camera[k].numOfCoverage = 2;
                    camera[k].coverage = (int*)malloc(sizeof(int) * 2);
                    camera[k].coverage[0] = object[i].id;
                    camera[k].coverage[1] = object[i].neighbor[j];
                    k++;
                }
            }
        }
        object[i].isTried = 1;
    }

    *numOfCamera = k;
    for(i = k; i < NumOfCamera; i++){
        camera[i].isScheduled = 1;
        camera[i].isConsider = 0;
        camera[i].no = 1;
    }
    
    for(i = 0; i < NumOfObject; i++){
        for(j = 0; j < NumOfObject; j++){       //for camera
            for(k = 0; k < camera[j].numOfCoverage; k++){
                if(camera[j].coverage[k] == i){
                    object[i].numOfCoveringCamera++;
                }
            }
        }
        object[i].coveringCamera = (int*)malloc(sizeof(int)*object[i].numOfCoveringCamera);
        m = 0;
        for(j = 0; j < NumOfObject; j++){
            for(k = 0; k < camera[j].numOfCoverage; k++){
                if(camera[j].coverage[k] == i){
                    object[i].coveringCamera[m] = j;
                    m++;
                }
            }
        }
    }
    for(i = 0; i < NumOfCamera; i++)
        camera[i].numOfCoverageTemp = camera[i].numOfCoverage;
    
    //Print the info. of cameras and objects
    /*for(i = 0; i < NumOfCamera; i++){
        printf("camera %d: ", i);
        for(j = 0; j < camera[i].numOfCoverage; j++){
            printf("%d, ", camera[i].coverage[j]);
        }
        printf("\n");
    }

    for(i = 0; i < NumOfObject; i++){
        printf("object %d: ", i);
        for(j = 0; j < object[i].numOfNeighbor; j++){
            printf("%d, ", object[i].neighbor[j]);
        }
        printf("\n");
    }*/
    //===================================
}

void deploy_camera_omni(Camera *camera) { //for omnidirectional camera
    int i;
    //In order to gurantee that each object is covered by at least one camera, we deploy object with camera first,
    //then we add redundant camera
    for (i = NumOfObject; i < NumOfCamera; i++) {
        camera[i].R = 125 + (rand() % (Range - 125));
        camera[i].magnitude = rand() % 360;
        camera[i].X = camera[i].R * cos((camera[i].magnitude * PI) / 180);
        camera[i].Y = camera[i].R * sin((camera[i].magnitude * PI) / 180);
        camera[i].distance = sqrt(pow(camera[i].X, 2) + pow(camera[i].Y, 2));
        //printf("%lf, (%.2lf, %.2lf)\n", camera[i].R, camera[i].X, camera[i].Y);
        //printf("Camera %d's distance: %.5lf meters\n", i, camera[i].distance);
    }
    //printf("\n");
}

void deploy_camera_direction(Camera *camera) { //for directional camera
    int i;
    //In order to gurantee that each object is covered by at least one camera, we deploy object with camera first,
    //then we add redundant camera
    for (i = NumOfObject; i < NumOfCamera; i++) {
        camera[i].R = rand() % Range;
        camera[i].magnitude = rand() % 360;
        camera[i].cameraDirection = rand() % 360;
        camera[i].X = camera[i].R * cos((camera[i].magnitude * PI) / 180);
        camera[i].Y = camera[i].R * sin((camera[i].magnitude * PI) / 180);
        camera[i].referenceX = camera[i].range * cos((camera[i].cameraDirection * PI) / 180) + camera[i].X;
        camera[i].referenceY = camera[i].range * cos((camera[i].cameraDirection * PI) / 180) + camera[i].Y;
        camera[i].distance = sqrt(pow(camera[i].X, 2) + pow(camera[i].Y, 2));
    }
}

//Decide the number of objects covered by each camera,
//and which objects does the camera cover

void deploy_object(Camera *camera) { //for grid-based deployment
    double mean = 100;
    double std = 40;
    double u, v;
    int i, j, k;
    int checkObject[NumOfObject]; //Check that all objects are distributed to cameras
    int *coverageTemp;
    int reassignedCamera;
    Object object[NumOfObject];
    for (i = 0; i < NumOfObject; i++) {
        checkObject[i] = -1;
    }
    //uniformly deploy objects
    for (i = 0; i < NumOfObject; i++) {
        object[i].R = rand() % Range;
        object[i].magnitude = rand() % 360;
        object[i].X = object[i].R * cos((object[i].magnitude * PI) / 180);
        object[i].Y = object[i].R * sin((object[i].magnitude * PI) / 180);
    }
    //deploy objects based on gaussian distribuion
    /*for (i = 0; i < NumOfObject; i++) {
        u = rand() / (double) RAND_MAX;
        v = rand() / (double) RAND_MAX;
        object[i].X = sqrt(-2 * log(u)) * cos(2 * M_PI * v) * std + mean;
        u = rand() / (double) RAND_MAX;
        v = rand() / (double) RAND_MAX;
        object[i].Y = sqrt(-2 * log(u)) * cos(2 * M_PI * v) * std + mean;
    }*/
    for (i = 0; i < NumOfObject; i++) {
        for (j = 0; j < NumOfCamera; j++) {
            object[i].distanceWithCamera[j] = sqrt(pow(object[i].X - camera[j].X, 2) + pow(object[i].Y - camera[j].Y, 2));
            if (object[i].distanceWithCamera[j] < camera[j].range) {
                camera[j].numOfCoverage++;
                checkObject[i] = 1;
            }
        }
    }

    for (i = 0; i < NumOfCamera; i++) {
        camera[i].numOfCoverageTemp = camera[i].numOfCoverage;
        if (camera[i].numOfCoverage > 0) {
            camera[i].coverage = (int*) malloc(sizeof (int)*camera[i].numOfCoverage);
        }
        k = 0;
        for (j = 0; j < NumOfObject; j++) {
            if (object[j].distanceWithCamera[i] < camera[i].range) {
                camera[i].coverage[k] = j;
                checkObject[j] = 1;
                k++;
            }
        }
    }

    //Check whether there are objects that were not covered
    //If so, allocate it to a random camera
    double minDisatnce = 2 * Range;
    for (i = 0; i < NumOfObject; i++) {
        minDisatnce = 2 * Range;
        if (checkObject[i] == -1) {
            printf("XX\n");
            for (j = 0; j < NumOfCamera; j++) {
                if (object[i].distanceWithCamera[j] < minDisatnce) {
                    minDisatnce = object[i].distanceWithCamera[j];
                    reassignedCamera = j;
                }
            }
            coverageTemp = camera[reassignedCamera].coverage;
            camera[reassignedCamera].numOfCoverage = camera[reassignedCamera].numOfCoverage + 1;
            camera[reassignedCamera].numOfCoverageTemp = camera[reassignedCamera].numOfCoverage;
            camera[reassignedCamera].coverage = (int*) malloc(sizeof (int)*camera[reassignedCamera].numOfCoverage);
            for (j = 0; j < camera[reassignedCamera].numOfCoverage - 1; j++) {
                camera[reassignedCamera].coverage[j] = coverageTemp[j];
            }
            camera[reassignedCamera].coverage[j] = i;
            checkObject[i] = 1;
        }
    }

    /*for (i = 0; i < NumOfCamera; i++) {
        printf("Camera %d's coverage: {", camera[i].id);
        for (j = 0; j < camera[i].numOfCoverage; j++) {
            if (j == camera[i].numOfCoverage - 1) {
                printf("%d}", camera[i].coverage[j]);
            } else {
                printf("%d, ", camera[i].coverage[j]);
            }
        }
        printf("\n");
    }
    printf("\n");*/
}

//In order to ensure basic coverage, we randomly deploy the objects with at least one camera nearby it
void deploy_object_camera_omni(Camera *camera, Object *object) {
    int i;
    //In order to gurantee that each object is covered by at least one camera
    for (i = 0; i < NumOfObject; i++) {
        object[i].R = 125 + (rand() % (Range - 125));
        object[i].magnitude = rand() % 360;
        object[i].X = object[i].R * cos((object[i].magnitude * PI) / 180);
        object[i].Y = object[i].R * sin((object[i].magnitude * PI) / 180);
        camera[i].R = 5 + rand() % 15;
        camera[i].magnitude = rand() % 360;
        camera[i].X = camera[i].R * cos((camera[i].magnitude * PI) / 180) + object[i].X;
        camera[i].Y = camera[i].R * sin((camera[i].magnitude * PI) / 180) + object[i].Y;
        while (camera[i].X > 250 || camera[i].X < -250 || camera[i].Y > 250 || camera[i].Y < -250) {
            camera[i].R = 5 + rand() % 15;
            camera[i].magnitude = rand() % 360;
            camera[i].X = camera[i].R * cos((camera[i].magnitude * PI) / 180) + object[i].X;
            camera[i].Y = camera[i].R * sin((camera[i].magnitude * PI) / 180) + object[i].Y;
        }
        camera[i].distance = sqrt(pow(camera[i].X, 2) + pow(camera[i].Y, 2));
    }
}

void deploy_object_camera_direction(Camera *camera, Object *object) {
    int i;
    //In order to gurantee that each object is covered by at least one camera
    for (i = 0; i < NumOfObject; i++) {
        object[i].R = rand() % Range;
        object[i].distance = object[i].R;
        object[i].magnitude = rand() % 360;
        object[i].X = object[i].R * cos((object[i].magnitude * PI) / 180);
        object[i].Y = object[i].R * sin((object[i].magnitude * PI) / 180);
        camera[i].R = 5 + rand() % 15;
        camera[i].magnitude = rand() % 360;
        camera[i].cameraDirection = (int) (camera[i].magnitude + 180) % 360;
        //camera[i].FOVStart = camera[i].cameraDirection - (camera[i].FOV / 2);
        //camera[i].FOVEnd = camera[i].cameraDirection + (camera[i].FOV / 2);
        camera[i].X = camera[i].R * cos((camera[i].magnitude * PI) / 180) + object[i].X;
        camera[i].Y = camera[i].R * sin((camera[i].magnitude * PI) / 180) + object[i].Y;
        while (camera[i].X > 250 || camera[i].X < -250 || camera[i].Y > 250 || camera[i].Y < -250) {
            camera[i].R = 5 + rand() % 15;
            camera[i].magnitude = rand() % 360;
            camera[i].cameraDirection = (int) (camera[i].magnitude + 180) % 360;
            //camera[i].FOVStart = camera[i].cameraDirection - (camera[i].FOV / 2);
            //camera[i].FOVEnd = camera[i].cameraDirection + (camera[i].FOV / 2);
            camera[i].X = camera[i].R * cos((camera[i].magnitude * PI) / 180) + object[i].X;
            camera[i].Y = camera[i].R * sin((camera[i].magnitude * PI) / 180) + object[i].Y;
        }
        camera[i].distance = sqrt(pow(camera[i].X, 2) + pow(camera[i].Y, 2));
        camera[i].referenceX = camera[i].range * cos((camera[i].cameraDirection * PI) / 180) + camera[i].X;
        camera[i].referenceY = camera[i].range * sin((camera[i].cameraDirection * PI) / 180) + camera[i].Y;
    }
}

void calculate_distance_omni(Camera *camera, Object *object) {
    int i, j, k;
    for (i = 0; i < NumOfObject; i++) {
        for (j = 0; j < NumOfCamera; j++) {
            object[i].distanceWithCamera[j] = sqrt(pow(object[i].X - camera[j].X, 2) + pow(object[i].Y - camera[j].Y, 2));
            if (object[i].distanceWithCamera[j] < camera[j].range) {
                camera[j].numOfCoverage++;
            }
        }
    }

    for (i = 0; i < NumOfCamera; i++) {
        camera[i].numOfCoverageTemp = camera[i].numOfCoverage;
        if (camera[i].numOfCoverage > 0) {
            camera[i].coverage = (int*) malloc(sizeof (int)*camera[i].numOfCoverage);
        }
        k = 0;
        for (j = 0; j < NumOfObject; j++) {
            if (object[j].distanceWithCamera[i] < camera[i].range) {
                camera[i].coverage[k] = j;
                k++;
            }
        }
    }
}

void calculate_distance_direction(Camera *camera, Object *object) {
    int i, j, k;
    double costheta;
    double arccos;
    for (i = 0; i < NumOfObject; i++) {
        for (j = 0; j < NumOfCamera; j++) {
            object[i].distanceWithCamera[j] = sqrt(pow(object[i].X - camera[j].X, 2) + pow(object[i].Y - camera[j].Y, 2));
            if (object[i].distanceWithCamera[j] < camera[j].range) {
                costheta = ((pow(object[i].X - camera[j].X, 2) + pow(object[i].Y - camera[j].Y, 2)) + pow(camera[j].range, 2)
                        - (pow(object[i].X - camera[j].referenceX, 2) + pow(object[i].Y - camera[j].referenceY, 2)))
                        / (2 * sqrt(pow(object[i].X - camera[j].X, 2) + pow(object[i].Y - camera[j].Y, 2)) * camera[j].range);
                arccos = acos(costheta) * 180 / PI;
                if (arccos < (camera[j].FOV / 2)) {
                    camera[j].numOfCoverage++;
                }
            }
        }
    }

    for (i = 0; i < NumOfCamera; i++) {
        camera[i].numOfCoverageTemp = camera[i].numOfCoverage;
        if (camera[i].numOfCoverage > 0) {
            camera[i].coverage = (int*) malloc(sizeof (int)*camera[i].numOfCoverage);
        }
        k = 0;
        for (j = 0; j < NumOfObject; j++) {
            if (object[j].distanceWithCamera[i] < camera[i].range) {
                costheta = ((pow(object[j].X - camera[i].X, 2) + pow(object[j].Y - camera[i].Y, 2)) + pow(camera[i].range, 2)
                        - (pow(object[j].X - camera[i].referenceX, 2) + pow(object[j].Y - camera[i].referenceY, 2)))
                        / (2 * sqrt(pow(object[j].X - camera[i].X, 2) + pow(object[j].Y - camera[i].Y, 2)) * camera[i].range);
                arccos = acos(costheta) * 180 / PI;
                if (arccos < (camera[i].FOV / 2)) {
                    camera[i].coverage[k] = j;
                    k++;
                }
            }
        }
    }
}

void real_map_deploy(Camera *camera, Object *object) {
    int i;
    //In order to gurantee that each object is covered by at least one camera
    camera[0].cameraDirection = 5;
    camera[0].X = -225;
    camera[0].Y = -95;

    camera[1].cameraDirection = 110;
    camera[1].X = -190;
    camera[1].Y = -40;

    camera[2].cameraDirection = 300;
    camera[2].X = -188;
    camera[2].Y = -95;

    camera[3].cameraDirection = 50;
    camera[3].X = -175;
    camera[3].Y = -100;

    camera[4].cameraDirection = 125;
    camera[4].X = -125;
    camera[4].Y = 30;

    camera[5].cameraDirection = 300;
    camera[5].X = -100;
    camera[5].Y = 100;

    camera[6].cameraDirection = 270;
    camera[6].X = -100;
    camera[6].Y = -140;

    camera[7].cameraDirection = 90;
    camera[7].X = -80;
    camera[7].Y = 75;

    camera[8].cameraDirection = 70;
    camera[8].X = -70;
    camera[8].Y = 150;

    camera[9].cameraDirection = 320;
    camera[9].X = -50;
    camera[9].Y = 70;

    camera[10].cameraDirection = 40;
    camera[10].X = -10;
    camera[10].Y = -55;

    camera[11].cameraDirection = 270;
    camera[11].X = 35;
    camera[11].Y = -50;

    camera[12].cameraDirection = 145;
    camera[12].X = 60;
    camera[12].Y = 40;

    camera[13].cameraDirection = 260;
    camera[13].X = 85;
    camera[13].Y = -130;

    camera[14].cameraDirection = 45;
    camera[14].X = 145;
    camera[14].Y = 160;

    camera[15].cameraDirection = 180;
    camera[15].X = 170;
    camera[15].Y = -40;

    camera[16].cameraDirection = 160;
    camera[16].X = 175;
    camera[16].Y = -100;

    for (i = 0; i < NumOfCamera; i++) {
        camera[i].distance = sqrt(pow(camera[i].X, 2) + pow(camera[i].Y, 2));
        camera[i].referenceX = camera[i].range * cos((camera[i].cameraDirection * PI) / 180) + camera[i].X;
        camera[i].referenceY = camera[i].range * sin((camera[i].cameraDirection * PI) / 180) + camera[i].Y;
    }

    object[0].X = -165;
    object[0].Y = 40;

    object[1].X = -165;
    object[1].Y = -48;

    object[2].X = -162;
    object[2].Y = -120;

    object[3].X = -98;
    object[3].Y = -160;

    object[4].X = -90;
    object[4].Y = 85;

    object[5].X = -78;
    object[5].Y = 170;

    object[6].X = 10;
    object[6].Y = 40;

    object[7].X = 5;
    object[7].Y = -20;

    object[8].X = 50;
    object[8].Y = -60;

    object[9].X = 90;
    object[9].Y = -150;

    object[10].X = 165;
    object[10].Y = 220;

    object[11].X = 140;
    object[11].Y = 0;

    object[12].X = 140;
    object[12].Y = -40;

    object[13].X = 150;
    object[13].Y = -85;

    object[14].X = -130;
    object[14].Y = -200;

    object[15].X = 40;
    object[15].Y = 75;

    object[16].X = -230;
    object[16].Y = 0;

    object[17].X = -90;
    object[17].Y = 25;

    object[18].X = -45;
    object[18].Y = 22;

    object[19].X = 8;
    object[19].Y = -110;

    for (i = 0; i < NumOfObject; i++) {
        object[i].distance = sqrt(pow(object[i].X, 2) + pow(object[i].Y, 2));
    }
}

void deployUE1(UE *user) {
    int i;
    for (i = 0; i < NumOfUE; i++) {
        user[i].R = rand() % Range + OutsideRange;
        user[i].magnitude = rand() % 60;
        user[i].X = user[i].R * cos((user[i].magnitude * PI) / 180);
        user[i].Y = user[i].R * sin((user[i].magnitude * PI) / 180);
        user[i].distance = sqrt(pow(user[i].X, 2) + pow(user[i].Y, 2));
        //printf("UE %d's distance: %.5lf meters\n", i, user[i].distance);
    }
    //printf("\n");
}

void deployUE2(UE *user) {
    int i;
    for (i = 0; i < NumOfUE; i++) {
        user[i].R = rand() % Range + OutsideRange;
        user[i].magnitude = rand() % 60 + 60;
        user[i].X = user[i].R * cos((user[i].magnitude * PI) / 180);
        user[i].Y = user[i].R * sin((user[i].magnitude * PI) / 180);
        user[i].distance = sqrt(pow(user[i].X, 2) + pow(user[i].Y, 2));
        //printf("UE %d's distance: %.5lf meters\n", i, user[i].distance);
    }
    //printf("\n");
}

void deployUE3(UE *user) {
    int i;
    for (i = 0; i < NumOfUE; i++) {
        user[i].R = rand() % Range + OutsideRange;
        user[i].magnitude = rand() % 60 + 120;
        user[i].X = user[i].R * cos((user[i].magnitude * PI) / 180);
        user[i].Y = user[i].R * sin((user[i].magnitude * PI) / 180);
        user[i].distance = sqrt(pow(user[i].X, 2) + pow(user[i].Y, 2));
        //printf("UE %d's distance: %.5lf meters\n", i, user[i].distance);
    }
    //printf("\n");
}

void deployUE4(UE *user) {
    int i;
    for (i = 0; i < NumOfUE; i++) {
        user[i].R = rand() % Range + OutsideRange;
        user[i].magnitude = rand() % 60 + 180;
        user[i].X = user[i].R * cos((user[i].magnitude * PI) / 180);
        user[i].Y = user[i].R * sin((user[i].magnitude * PI) / 180);
        user[i].distance = sqrt(pow(user[i].X, 2) + pow(user[i].Y, 2));
        //printf("UE %d's distance: %.5lf meters\n", i, user[i].distance);
    }
    //printf("\n");
}

void deployUE5(UE *user) {
    int i;
    for (i = 0; i < NumOfUE; i++) {
        user[i].R = rand() % Range + OutsideRange;
        user[i].magnitude = rand() % 60 + 240;
        user[i].X = user[i].R * cos((user[i].magnitude * PI) / 180);
        user[i].Y = user[i].R * sin((user[i].magnitude * PI) / 180);
        user[i].distance = sqrt(pow(user[i].X, 2) + pow(user[i].Y, 2));
        //printf("UE %d's distance: %.5lf meters\n", i, user[i].distance);
    }
    //printf("\n");
}

void deployUE6(UE *user) {
    int i;
    for (i = 0; i < NumOfUE; i++) {
        user[i].R = rand() % Range + OutsideRange;
        user[i].magnitude = rand() % 60 + 300;
        user[i].X = user[i].R * cos((user[i].magnitude * PI) / 180);
        user[i].Y = user[i].R * sin((user[i].magnitude * PI) / 180);
        user[i].distance = sqrt(pow(user[i].X, 2) + pow(user[i].Y, 2));
        //printf("UE %d's distance: %.5lf meters\n", i, user[i].distance);
    }
    //printf("\n");
}