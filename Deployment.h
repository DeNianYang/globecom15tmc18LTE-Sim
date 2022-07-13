/* 
 * File:   Deployment.h
 * Author: MNet_KenWang
 *
 * Created on 2014年4月28日, 上午 10:30
 */

#ifndef _DEPLOYMENT_H_
#define _DEPLOYMENT_H_
#include "Camera.h"
#include "UE.h"
#define PI 3.14159

void grid_deploy(Camera *camera);
void deploy_camera_omni(Camera *camera);
void manhattan_street_deploy(Camera *camera, Object *object);
void manhattan_street_deploy_edgecover(Camera *camera, Object *object, int *numOfCamera);
void deploy_camera_direction(Camera *camera);
void deployUE1(UE *user);
void deployUE2(UE *user);
void deployUE3(UE *user);
void deployUE4(UE *user);
void deployUE5(UE *user);
void deployUE6(UE *user);
void deploy_object(Camera *camera);
void deploy_object_camera_omni(Camera *camera, Object *object);
void deploy_object_camera_direction(Camera *camera, Object *object);
void calculate_distance_omni(Camera *camera, Object *object);
void calculate_distance_direction(Camera *camera, Object *object);
void real_map_deploy(Camera *camera, Object *object);

#endif	/* DEPLOYMENT_H */

