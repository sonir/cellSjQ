//
//  ServerManager.h
//  cell
//
//  Created by sonir on 10/28/14.
//
//

#ifndef __cell__ServerManager__
#define __cell__ServerManager__

//Sound Setup
#define JUDGE_DEGRADER 0.7
#define JUDGE_AUTOPAN 0.3

#include <stdio.h>
#include "cellSetup.h"
#include "slCellModel.h"
#include "slEuclid.h"
#include "DroneServer.h"
#include "ClipServer.h"
#include "ArduinoServer.h"
#include "Toolkit.h"

enum update_destination {SOUND, CLIP, ARDUINO};

typedef struct agent_snap_t {
    
    agent ag[4];
    
} agent_snap_t;


class ServerManager {
    
    public:
        ServerManager();
        int degradeCheck();
        int autoPanCheck();
        void mappingToDrone();
        void mappingToClip();
        void mappingToArduino();
        void update(update_destination to, agent_snap_t snap);
    
        DroneServer *droneServer;
        ClipServer *clipServer;
        ArduinoServer *arduinoServer;
    
    private:
        agent_snap_t model;
        drone_param_t drone_params;
        clip_param_t clip_params;
        arduino_param_t arduino_params;
		ToolKit *tool;
    
};

#endif /* defined(__cell__ServerManager__) */
