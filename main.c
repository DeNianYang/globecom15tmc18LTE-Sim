/* 
 * File:   main.c
 * Author: MNet_KenWang
 *
 * Created on 2014年4月25日, 下午 12:38
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Camera.h"
#include "Deployment.h"
#include "MAMC.h"
#include "MCS.h"
#include "Random.h"
#include "SINR.h"
#include "System.h"
#include "UE.h"

int main(int argc, char** argv) {
    int ueIndex1 = 0, ueIndex2 = 0, ueIndex3 = 0;
    int ueIndex4 = 0, ueIndex5 = 0, ueIndex6 = 0;
    int i, j, k;
    int deploy;
    int simulations;
    int run;
    //Used to count the average #RBs==========
    int baselineCount = 0;
    int TMCCount = 0;
    int MAMCCount = 0;
    int GreedyCount = 0;
    int DPCount = 0;
    int EdgeCoverCount = 0;
    //==================================
    double baselineCoverage = 0.0;
    double TMCCoverage = 0.0;
    double MAMCCoverage = 0.0;
    Camera camera[NumOfCamera];
    Camera baselineCamera[NumOfCamera];
    Camera TMCCamera[NumOfCamera];
    Camera MAMCCamera[NumOfCamera];
    Camera GreedyCamera[NumOfCamera];
    Camera DPCamera[NumOfCamera];
    Camera EdgeCoverCamera[NumOfCamera];
    Object object[NumOfObject];
    Object GreedyObject[NumOfObject];
    Object DPObject[NumOfObject];
    Object EdgeCoverObject[NumOfObject];
    UE userG1[NumOfUE];
    UE userG2[NumOfUE];
    UE userG3[NumOfUE];
    UE userG4[NumOfUE];
    UE userG5[NumOfUE];
    UE userG6[NumOfUE];
    
    FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6;
    fp1 = fopen("./data/Baseline_result.csv", "a");
    fp2 = fopen("./data/TMC_result.csv", "a");
    fp3 = fopen("./data/MAMC_result.csv", "a");
    //fp4 = fopen("./data/Greedy_result.csv", "a");
    //fp5 = fopen("./data/DP_result.csv", "a");
    fp6 = fopen("./data/EdgeCover_result.csv", "a");
    fprintf(fp1, "\n#RBs,");
    fprintf(fp2, "\n#RBs,");
    fprintf(fp3, "\n#RBs,");
    //fprintf(fp4, "\n#RBs,");
    //fprintf(fp5, "\n#RBs,");
    fprintf(fp6, "\n#RBs,");
    
    
    int numOfCamera = NumOfCamera;
    LARGE_INTEGER t1, t2, ts;
    
    randomize();
    for (deploy = 0; deploy < 1; deploy++) {
        for (simulations = 0; simulations < 500; simulations++) {
            initializeUE(userG1);
            initializeUE(userG2);
            initializeUE(userG3);
            initializeUE(userG4);
            initializeUE(userG5);
            initializeUE(userG6);

            deployUE1(userG1);
            deployUE2(userG2);
            deployUE3(userG3);
            deployUE4(userG4);
            deployUE5(userG5);
            deployUE6(userG6);

            rx_power(userG1);
            rx_power(userG2);
            rx_power(userG3);
            rx_power(userG4);
            rx_power(userG5);
            rx_power(userG6);

            initializeCamera(camera);
            initializeObject(object);
            //real_map_deploy(camera, object);
            //deploy_object_camera_direction(camera, object);
            //deploy_camera_direction(camera);
            //calculate_distance_direction(camera, object);
            //manhattan_street_deploy(camera, object);
            manhattan_street_deploy_edgecover(camera, object, &numOfCamera);
            //deploy_object_camera_omni(camera, object);
            //deploy_camera_omni(camera);
            //calculate_distance_omni(camera, object);
            //grid_deploy(camera);
            //deploy_object(camera);
                /*round_robin(userG1, &ueIndex1);
                round_robin(userG2, &ueIndex2);
                round_robin(userG3, &ueIndex3);
                round_robin(userG4, &ueIndex4);
                round_robin(userG5, &ueIndex5);
                round_robin(userG6, &ueIndex6);

                SINR_camera(camera, userG1, userG2, userG3, userG4, userG5, userG6);
                MCS_cal(camera);
                MCSoverRB(camera);
                for(i = 0; i < NumOfCamera; i++){
                    for(j = 0; j < camera[i].numOfAllocation; j++){
                        camera[i].allocation[j].A = (int*) malloc(sizeof (int) * camera[i].allocation[j].numOfRB);
                    }
                }
                
                for(i = 0; i < NumOfCamera; i++){
                    for(j = 0; j < camera[i].numOfAllocation; j++){
                        for(k = 0; k < camera[i].allocation[j].numOfRB; k++){
                            camera[i].allocation[j].A[k] = camera[i].allocation[j].start + k;
                            printf("%d ", camera[i].allocation[j].A[k]);
                        }
                    }
                    printf("\n");
                }*/
            for (run = 0; run < 2; run++) {
                round_robin(userG1, &ueIndex1);
                round_robin(userG2, &ueIndex2);
                round_robin(userG3, &ueIndex3);
                round_robin(userG4, &ueIndex4);
                round_robin(userG5, &ueIndex5);
                round_robin(userG6, &ueIndex6);

                SINR_camera(camera, userG1, userG2, userG3, userG4, userG5, userG6);
                MCS_cal(camera);
                //MCS_cal_special_caseI(camera);
                //MCS_cal_special_caseII(camera);

                for (i = 0; i < NumOfCamera; i++) {
                    baselineCamera[i] = camera[i];
                    TMCCamera[i] = camera[i];
                    MAMCCamera[i] = camera[i];
                    GreedyCamera[i] = camera[i];
                    DPCamera[i] = camera[i];
                    EdgeCoverCamera[i] = camera[i];
                }
                
                for(i = 0; i < NumOfObject; i++){
                    GreedyObject[i] = object[i];
                    DPObject[i] = object[i];
                    EdgeCoverObject[i] = object[i];
                }
                //QueryPerformanceFrequency(&ts);
                //QueryPerformanceCounter(&t1);
                Baseline(baselineCamera, &baselineCount, &baselineCoverage, fp1, &numOfCamera);
                //QueryPerformanceCounter(&t2);
                //printf("Base Time: %lf\n",(t2.QuadPart-t1.QuadPart)/(double)(ts.QuadPart));
                
                //QueryPerformanceCounter(&t1);                
                TMC(TMCCamera, &TMCCount, &TMCCoverage, fp2, &numOfCamera);
                //QueryPerformanceCounter(&t2);
                //printf("TMC Time: %lf\n",(t2.QuadPart-t1.QuadPart)/(double)(ts.QuadPart));
                
                //QueryPerformanceCounter(&t1);
                printf("%d\n", numOfCamera);
                MAMC(MAMCCamera, &MAMCCount, &MAMCCoverage, fp3, &numOfCamera);
                //QueryPerformanceCounter(&t2);
                //printf("MAMC Time: %lf\n",(t2.QuadPart-t1.QuadPart)/(double)(ts.QuadPart));
                
                //GreedyAllocation(GreedyCamera, GreedyObject, &GreedyCount, fp4);
                //DP_based(DPCamera, DPObject, &DPCount, fp5);
                EdgeCover(EdgeCoverCamera, EdgeCoverObject, &EdgeCoverCount, fp6, &numOfCamera);
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    //fclose(fp4);
    //fclose(fp5);
    fclose(fp6);
    printf("Baseline: %d\n", baselineCount / (deploy * simulations * run));
    printf("TMC: %d\n", TMCCount / (deploy * simulations * run));
    printf("MAMC: %d\n", MAMCCount / (deploy * simulations * run));
    printf("Greedy: %d\n", GreedyCount / (deploy * simulations * run));
    printf("DP: %d\n", DPCount / (deploy * simulations * run));
    printf("EdgeCover: %d\n", EdgeCoverCount / (deploy * simulations * run));
    //printf("Baseline average cover: %.2lf\n", baselineCoverage / (deploy * simulations * run));
    //printf("TMC average cover: %.2lf\n", TMCCoverage / (deploy * simulations * run));
    //printf("MAMC average cover: %.2lf\n", MAMCCoverage / (deploy * simulations * run));
    return (EXIT_SUCCESS);
}
