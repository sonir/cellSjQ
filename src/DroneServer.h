//
//  DroneServer.h
//  cell
//
//  Created by sonir on 10/28/14.
//
//

#ifndef __cell__DroneServer__
#define __cell__DroneServer__

//ForIC #define SND_SERVER_IP "49.212.138.54"
#define SND_SERVER_IP "224.0.0.1"
#define SND_SERVER_PORT 57122

#include <stdio.h>
#include "ofxOsc.h"
#include "Server.h"

typedef struct drone_param_t{
   
   int twitter_announce;
   int twitter_log;
   float synth_pitch1_vol;
   float synth_pitch2_vol;
   float synth_pitch3_vol;
   float synth_pitch4_vol;
   float synth_pitch5_vol;
   float synth_pitch6_vol;
   float synth_pitch7_vol;
   float synth_pitch8_vol;
   float synth_pitch9_vol;
   float synth_pitch10_vol;
   float synth_pitch11_vol;
   float synth_pitch12_vol;
   float synth_pitch13_vol;
   int degrade_on;
   float degrade_sample_rate;
   int vocoder_on;
   float vocoder_dry;
   int reverb_on;
   float reverb_dry;
   int autopan_on;
   float autopan_rate;
   float autopan_ammount;
   float autopan_randomness;
   int autofilter_on;
   float auto_filter_ammount;
   float auto_filter_rate;
   float auto_filter_cutoff;
   int sequece_pattern; //0=white noise , 1=pattern1, 2=pattern2
   

} drone_param_t;

class DroneServer : Server {
    
	public:
        DroneServer();
//        ofxOscSender soundServer;
		drone_param_t drone_param;
        void initParam(drone_param_t *pDrone);
        void send();
        void send(drone_param_t getParams);
    
};

#endif /* defined(__cell__DroneServer__) */
