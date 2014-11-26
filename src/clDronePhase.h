//
//  clDronePhase.h
//  cell
//
//  Created by sonir on 11/17/14.
//
//

#ifndef __cell__clDronePhase__
#define __cell__clDronePhase__

#include <stdio.h>
#include "ofMain.h"
#include "slCellModel.h"
#include "cellSetup.h"
#include "slCellModel.h"


class clDronePhase{
    
public:
    bool init;
    int focused_ag;
    
    clDronePhase(slCellModel *adr);
    slCellModel *model;
    
    bool isInit();
    void initParam();
    void incOthers();
    bool touchCheck();
    void action();
    void reset();
    
private:
    agent ag_org[AG_MAX_NUM];
    
};


#endif /* defined(__cell__clDronePhase__) */
