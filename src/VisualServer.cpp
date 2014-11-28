//
//  VisualServer.cpp
//  cell
//
//  Created by sonir on 11/28/14.
//
//

#include "VisualServer.h"

VisualServer::VisualServer(){
    
    server.setup(VISUAL_SERVER_IP, VISUAL_SERVER_PORT);

    
}

void VisualServer::initParam(visual_param_t *param){
    
    visual_param.level = 0.5;
    
    
    
}


void VisualServer::send(visual_param_t *param){
    
    
    
}

