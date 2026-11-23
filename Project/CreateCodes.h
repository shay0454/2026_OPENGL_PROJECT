#pragma once
#include"Model.h"
#include"global.h"
#include"Texture.h"
#include"Random.h"
Robot* createRobot();
Mesh* createRoom(float, float);
Truck* createTruck();

Conveyor* loadConveyor();

Conveyor* loadCornerConveyor();
BoxModel* createBox();

Mesh* createSphere();