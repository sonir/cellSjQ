//
//  clDronePhase.cpp
//  cell
//
//  Created by sonir on 11/17/14.
//
//

#include "clDronePhase.h"

//
//  clDronePhase.cpp
//  cell
//
//  Created by sonir on 11/16/14.
//
//


clDronePhase::clDronePhase(slCellModel *adr){
    
    
    //  Set main's model
    model = adr;
    init = true;
    focused_ag = 0;
    
}

bool clDronePhase::isInit(){
    
    if(init){
        
        init = false;
        return true;
        
    }else{
        
        return false;
        
    }
    
}

void clDronePhase::initParam(){
    
    
    for(int i=0; i<AG_MAX_NUM; i++){
        
        agent tmp = model->getAgent(i);
        //Copy the ORG PARAM
        ag_org[i] = tmp;
        //tmp.circumference_posi = SOLO_INIT_POSI;
//        tmp.dexterity = 30;
        tmp.circumference_posi=0.0;
        model->setAgent(i,&tmp);
        
        cout << ag_org[i].dexterity << endl;
    }
    
    
    
}


void clDronePhase::action(){
    
//    agent tmp = model->getAgent(focused_ag);
//    model->toolKit.randomWalk(&tmp);
//    model->setAgent(focused_ag, &tmp);
//    incOthers();
    
}


void clDronePhase::reset(){
    
    cout << "reset" << endl;
    
    // Recover the original param
//    for(int i=0; i<AG_MAX_NUM; i++){
//        agent tmp = model->getAgent(i);
//        tmp = ag_org[i];
//        model->setAgent(i,&tmp);
//    }
    
    init = true;
    
}