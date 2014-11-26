//
//  clSolo.h
//  cell
//
//  Created by sonir on 11/16/14.
//
//

#ifndef __cell__clSolo__
#define __cell__clSolo__

#include <stdio.h>
#include "ofMain.h"
#include "slCellModel.h"
#include "cellSetup.h"


class clSolo{
    
    public:
        bool init;
        int focused_ag;
    
        clSolo(slCellModel *adr);
        slCellModel *model;
        
        bool isInit();
        void initParam();
        void incOthers();
        bool touchCheck();
        void action();
        bool countCheck();
        void reset();
    
    private:
        int atk_count;
    
};

#endif /* defined(__cell__clSolo__) */
