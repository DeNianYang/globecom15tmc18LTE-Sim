#include <stdio.h>
#include <stdlib.h>
#include "System.h"
#include "UE.h"

void clear(UE *user);
void round_robin(UE *user, int *ueIndex);

void clear(UE *user) {
    int i;
    for (i = 0; i < NumOfUE; i++) {
        user[i].start = -1;
        user[i].end = -1;
        user[i].numOfRB = 0;
    }
}

void round_robin(UE *user, int *ueIndex) {
    clear(user);
    int frameIndex = 0;
    int count = 0;
    while (count < NumOfUE) {
        count++;
        if (*ueIndex == NumOfUE) {
            *ueIndex = *ueIndex % NumOfUE;
        }
        user[*ueIndex].numOfRB = rand() % 6 + 2;
        if (frameIndex + user[*ueIndex].numOfRB - 1 > NumOfSubchannel) {
            break;
        } else {
            user[*ueIndex].start = frameIndex;
            user[*ueIndex].end = frameIndex + user[*ueIndex].numOfRB - 1;
            frameIndex = frameIndex + user[*ueIndex].numOfRB;
            (*ueIndex)++;
        }
    }
}
