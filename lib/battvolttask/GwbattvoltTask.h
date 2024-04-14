/*
battvoltTask.h is for the battvoltTask.ccp and 
for defining the ports etc for the ttgo-t7-v14-mini32 board.
*/


#pragma once
#include "GwApi.h"


void battvoltTask(GwApi *param);
//void battvoltInit(GwApi *param);
//DECLARE_USERTASK(battvoltTask);
DECLARE_USERTASK_PARAM(battvoltTask,3000);
//DECLARE_INITFUNCTION(battvoltInit);

//we only compile for some boards
#ifdef ttgo-t7-v14-mini32
    #define inputPin 35

void battvoltInit(GwApi *param);

//let the core call an init function before the 
//N2K Stuff and the communication is set up
//especially this init function will register the real task at the core
//this gives you some flexibility to decide based on config or defines whether you
//really want to start the task or not
//this function must return when done - otherwise the core will not start up
DECLARE_INITFUNCTION(battvoltInit);

/**
 * an interface for the example task
*/
class battvoltTaskIf : public GwApi::TaskInterfaces::Base{
    public:
    long count=0;
    String someValue;
};
DECLARE_TASKIF(battvoltTaskIf);


#endif
