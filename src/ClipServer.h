//
//  ClipServer.h
//  cell
//
//  Created by sonir on 10/28/14.
//
//

#ifndef __cell__ClipServer__
#define __cell__ClipServer__

#define SND_SERVER_IP "49.212.138.54"
//#define SND_SERVER_IP "224.0.0.1"
//#define SND_SERVER_IP "127.0.0.1"


#define SND_SERVER_PORT 57122
#define PRINT_SERVER_IP "224.0.0.1"
#define PRINT_SERVER_PORT 11370

#include <stdio.h>
#include "ofxOsc.h"
#include "Server.h"

typedef struct clip_param_t{
   
    int duration_frame;	// 1-3
    int tick_snd;	//1-1200
    int tick_noise;	//1-1200
    int snd1_type; //0-4
    int snd2_type;  //0-4
    int noise_type; //0-2
    int aeg_frame; //0-2;
    int filter_snd_type; //0-3
    float filter_snd_cutoff; //0.- 1.
     float filter_snd_gain; //0. -n? <<<<<<<<<<<<<
    float filter_snd_q; //0.-1.
    float filter_noise_type; //0-3
    float filter_noise_cutoff; //0.-1.
    float filter_noise_gain; // 0.-n? <<<<<<<<<<<<<
    float filter_noise_q; //0.- 1.
    int ef_deg_type; // 0 - 4
    int ef_pan_type; // 0 - 5
    int ef_am_on; // 0 - 1
    float ef_am_freq; //0. - 1.
     int ef_noiseconv_on; //0,1 <<<<<<<<<<
     float ef_noise_conv_modamp; //<<<<<<<<
    int clip_id; //pod number
    

} clip_param_t;


class ClipServer : Server {
    
	public:
        ClipServer();
//        ofxOscSender soundServer;
        ofxOscSender printServer;
		clip_param_t clip_param;
        void initParam(clip_param_t *pClip);
        void send();
        void send(clip_param_t getParams);
        int createId();
    
};

#endif /* defined(__cell__ClipServer__) */
