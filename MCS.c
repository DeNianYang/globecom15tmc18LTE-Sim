#include "Camera.h"
#include "MCS.h"

void MCS_cal(Camera *camera) {
    int i, j;
    for (i = 0; i < NumOfCamera; i++) {
        for (j = 0; j < NumOfSubchannel; j++) {
            if (camera[i].sinr[j] < -1.5) {
                camera[i].mcs[j] = QPSK_13;
            }
            else if (-1.5 <= camera[i].sinr[j] && camera[i].sinr[j] < 2){
                camera[i].mcs[j] = QPSK_12;
            }           
            else if (2 <= camera[i].sinr[j] && camera[i].sinr[j] < 4.3){
                camera[i].mcs[j] = QPSK_23;
            }
            else if (4.3 <= camera[i].sinr[j] && camera[i].sinr[j] < 5.5){
                camera[i].mcs[j] = QPSK_34;
            }
            else if (5.5 <= camera[i].sinr[j] && camera[i].sinr[j] < 7.9){
                camera[i].mcs[j] = QAM16_12;
            }
            else if (7.9 <= camera[i].sinr[j] && camera[i].sinr[j] < 11.3){
                camera[i].mcs[j] = QAM16_23;
            }
            else{
                camera[i].mcs[j] = QAM16_34;
            }
        }
    }
}

void MCS_cal_special_caseI(Camera *camera){
    int i, j;
    for(i = 0; i < NumOfCamera; i++){
        for(j = 0; j < NumOfSubchannel; j++){
            if(fabs(camera[i].X) >=  fabs(camera[i].Y)){
                if(fabs(camera[i].X) == 1)
                    camera[i].mcs[j] = QAM16_34;
                else if(fabs(camera[i].X) == 2)
                    camera[i].mcs[j] = QAM16_23;
                else if(fabs(camera[i].X) == 3)
                    camera[i].mcs[j] = QPSK_34;
                else if(fabs(camera[i].X) == 4)
                    camera[i].mcs[j] = QPSK_23;
            }
            else if(fabs(camera[i].X) < fabs(camera[i].Y)){
                if(fabs(camera[i].Y) == 1)
                    camera[i].mcs[j] = QAM16_34;
                else if(fabs(camera[i].Y) == 2)
                    camera[i].mcs[j] = QAM16_23;
                else if(fabs(camera[i].Y) == 3)
                    camera[i].mcs[j] = QPSK_34;
                else if(fabs(camera[i].Y) == 4)
                    camera[i].mcs[j] = QPSK_23;            
            }
        }
    }
}

void MCS_cal_special_caseII(Camera *camera){
    int i, j;
    int temp;
     for(i = 0; i < NumOfCamera; i++){
         temp = rand() % 6;
         for(j = 0; j < NumOfSubchannel; j++){
             if(temp == 0)
                 camera[i].mcs[j] = QAM16_34;
             else if(temp == 1)
                 camera[i].mcs[j] = QAM16_23;
             else if(temp == 2)
                 camera[i].mcs[j] = QAM16_12;
             else if(temp == 3)
                 camera[i].mcs[j] = QPSK_34;
             else if(temp == 4)
                 camera[i].mcs[j] = QPSK_23;
             else
                 camera[i].mcs[j] = QPSK_12;
         }
     }
}
