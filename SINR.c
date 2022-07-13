#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Camera.h"
#include "SINR.h"
#include "UE.h"
#include "System.h"

void SINR_camera(Camera *camera, UE *user1, UE* user2, UE* user3, UE* user4, UE* user5, UE* user6) {
    int i, j, k;
    double shadowing = 0.0;
    double interference[NumOfSubchannel];
    for(i = 0; i < NumOfSubchannel; i++){
        interference[i] = 0.0;
    }
    for (i = 0; i < NumOfCamera; i++) {
        camera[i].pathloss = 15.3 + 37.6 * log10(camera[i].distance);
        shadowing = shadow();
        camera[i].rxPower = PowerOfCamera + 3 - camera[i].pathloss - shadowing; //unit: dbm
    }
    //Calculate the interference from other UEs
    for (i = 0; i < NumOfSubchannel; i++) {
        for (j = 0; j < NumOfUE; j++) {
            if (user1[j].start == i) {
                for(k = 0; k < user1[j].numOfRB; k++){
                    interference[i + k] = interference[i + k] + pow(10, (user1[j].rxPower / 10));
                }
            }
            if (user2[j].start == i) {
                for(k = 0; k < user2[j].numOfRB; k++){
                    interference[i + k] = interference[i + k] + pow(10, (user2[j].rxPower / 10));
                }
            }
            if (user3[j].start == i) {
                for(k = 0; k < user3[j].numOfRB; k++){
                    interference[i + k] = interference[i + k] + pow(10, (user3[j].rxPower / 10));
                }
            }
            if (user4[j].start == i) {
                for(k = 0; k < user4[j].numOfRB; k++){
                    interference[i + k] = interference[i + k] + pow(10, (user4[j].rxPower / 10));
                }
            }
            if (user5[j].start == i) {
                for(k = 0; k < user5[j].numOfRB; k++){
                    interference[i + k] = interference[i + k] + pow(10, (user5[j].rxPower / 10));
                }
            }
            if (user6[j].start == i) {
                for(k = 0; k < user6[j].numOfRB; k++){
                    interference[i + k] = interference[i + k] + pow(10, (user6[j].rxPower / 10));
                }
            }
        }
    }
    for(i = 0; i < NumOfCamera; i++){
        for(j = 0; j < NumOfSubchannel; j++){
            camera[i].sinr[j] = 10 * log10(pow(10, (camera[i].rxPower / 10)) / (ThermalNoise + interference[j]));
        }
    }
    /*printf("Distance: %.2lf\n", camera[5].distance);
    printf("Pathloss: %.10lf\n", camera[5].pathloss);
    printf("rxPower: %.10lf\n", camera[5].rxPower);
    printf("Thermal noise: %.20lf\n", ThermalNoise);
    printf("sinr: %.3lf\n", camera[5].sinr[0]);*/
}

void rx_power(UE *user) {
    int i;
    double shadowing = 0.0;
    for (i = 0; i < NumOfUE; i++) {
        user[i].pathloss = 15.3 + 37.6 * log10(user[i].distance);
        shadowing = shadow();
        user[i].rxPower = PowerOfUE - user[i].pathloss - shadowing;
        //printf("r%d: %.10lf\n", i, user[i].rxPower);
    }
}

double shadow() {
    int SEED;
    SEED = 0 - (rand() % 100 + 1);
    long *idum;
    idum = (long *) malloc(sizeof (long)*1);
    *idum = SEED; //SEED must be a negative integer.

    // the AWGN channel
    double slevel = 0;
    double Runiform[2] = {0.0, 0.0};
    double Noise[2] = {0.0, 0.0};
    double noiseVariance = 8;
    double logvalue;
    double stemp;
    do {
        Runiform[0] = random_variable(idum);
        Runiform[1] = random_variable(idum);
        Runiform[0] = 2.0 * Runiform[0] - 1.0;
        Runiform[1] = 2.0 * Runiform[1] - 1.0;
        slevel = Runiform[0] * Runiform[0] + Runiform[1] * Runiform[1];
    } while (slevel >= 1.0);
    logvalue = log(slevel);
    stemp = (-2) * logvalue / slevel;
    Noise[0] = sqrt(noiseVariance) * Runiform[0] * sqrt(stemp);
    Noise[1] = sqrt(noiseVariance) * Runiform[1] * sqrt(stemp);
    //printf("Noise[0]: %lf\n", Noise[0]);
    //printf("Noise[1]: %lf\n", Noise[1]);
    return Noise[0];
}

double random_variable(long *idum) {
    int j;
    long k;
    static long iy = 0;
    static long iv[NTAB];
    double temp;
    if (*idum <= 0 || !iy) {
        if (-(*idum) < 1) *idum = 1;
        else *idum = -(*idum);
        for (j = NTAB + 7; j >= 0; j--) {
            k = (*idum) / IQ;
            *idum = IA * (*idum - k * IQ) - IR*k;
            if (*idum < 0) *idum += IM;
            if (j < NTAB) iv[j] = *idum;
        }
        iy = iv[0];
    }
    k = (*idum) / IQ;
    *idum = IA * (*idum - k * IQ) - IR*k;
    if (*idum < 0) *idum += IM;
    j = iy / NDIV;
    iy = iv[j];
    iv[j] = *idum;
    if ((temp = AM * iy) > RNMX)
        return RNMX;
    else
        return temp;
}
