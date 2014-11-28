//
//  VisualServer.h
//  cell
//
//  Created by sonir on 11/28/14.
//
//

#ifndef __cell__VisualServer__
#define __cell__VisualServer__

#include <stdio.h>
#include "Server.h"

#define VISUAL_SERVER_IP "224.0.0.1"
#define VISUAL_SERVER_PORT 55137

typedef struct visual_param_t{
    
    float level;
//    float gravity_x;
//    float gravity_y;
    
} visual_param_t;

class VisualServer : Server {
    
    public:
        VisualServer();
        ofxOscSender visualMachine;
        visual_param_t visual_param;
        void initParam(visual_param_t *param);
//        void send();
        void send(visual_param_t *param);

    
};

#endif /* defined(__cell__VisualServer__) */
