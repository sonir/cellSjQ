//
//  clSolo.cpp
//  cell
//
//  Created by sonir on 11/16/14.
//
//

#include "clSolo.h"

clSolo::clSolo(slCellModel *adr){
    

//  Set main's model
    model = adr;
    init = true;
    focused_ag = 0;
    atk_count = 0;
    
    
}

bool clSolo::isInit(){
    
    if(init){
        
        init = false;
        return true;
        
    }else{
        
        return false;
        
    }
    
}

void clSolo::initParam(){
    
    
    for(int i=0; i<AG_MAX_NUM; i++){
        
        agent tmp = model->getAgent(i);
        tmp.circumference_posi = SOLO_INIT_POSI;
        model->setAgent(i,&tmp);
        
    }
    
    focused_ag = (int)ofRandom(AG_MAX_NUM);
    agent tmp = model->getAgent(focused_ag);
    tmp.circumference_posi = SOLO_FUCUSED_INIT_POSI;
    model->setAgent(focused_ag,&tmp);
    
    
}

void clSolo::incOthers(){
    
    if( atk_count <( (int) SOLO_DURATION*0.5)) return;
    
    for(int i=0; i<AG_MAX_NUM;i++){
        
        if(i==focused_ag)continue; //Ignore focused one
        
        agent tmp = model->getAgent(i);
        tmp.circumference_posi+=SOLO_MOV_INC;
        tmp.circumference_posi=model->toolKit.moveLimitter(tmp.circumference_posi);
        model->setAgent(i,&tmp);
        
    }
    
}

bool clSolo::touchCheck(){
    
    phisical_contact_t contact = model->touched;
    bool my = contact.ag[focused_ag];
    bool other = false;
    
    for(int i=0; i< AG_MAX_NUM; i++){
        
        if( i==focused_ag )continue;
        if( contact.ag[i]==true ) other == true;
        
    }
    
    
    if(my&&other){
        cout << "my:" << my << "other:" << other << endl;
        return true;
    } else {
        return false;
    }
    
}

void clSolo::action(){
    agent tmp = model->getAgent(focused_ag);
    model->toolKit.randomWalk(&tmp);
    model->setAgent(focused_ag, &tmp);
    incOthers();
    atk_count++;
    
}

bool clSolo::countCheck(){

    if(atk_count>SOLO_DURATION)return true;
    else return false;

}

void clSolo::reset(){

    cout << "reset" << endl;
    atk_count =0;
    init = true;
    
}