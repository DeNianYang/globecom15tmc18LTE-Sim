/* 
 * File:   System.h
 * Author: MNet_KenWang
 *
 * Created on 2014年5月6日, 下午 3:25
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#include <math.h>

#define Range 250                   //unit: meter
#define OutsideRange 150
#define NumOfObject 60
#define NumOfSubframe 10
#define NumOfSubchannel 50
#define NumOfCamera 90
#define NumOfUE 20
#define PowerOfCamera 24        //unit: dbm
#define PowerOfUE 24               //unit: dbm
#define PowerOfBS 46               //unit: dbm
#define ThermalNoise (pow(10, -17.4) * 180000)  //unit: mw
#define ManhattanStreetSize 9   //must be odd number

typedef struct {
    int index[NumOfSubframe];      //record the start position of each subframe (time slot)
    int rb[NumOfSubchannel][NumOfSubframe];     //record the allocation result
} Frame;

#endif	/* SYSTEM_H */

