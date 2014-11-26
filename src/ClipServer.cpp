//
//  ClipServer.cpp
//  cell
//
//  Created by sonir on 10/28/14.
//
//

#include "ClipServer.h"

ClipServer::ClipServer(){
    
    cout << "clips" << endl;
    server.setup(SND_SERVER_IP, SND_SERVER_PORT);
    printServer.setup(PRINT_SERVER_IP, PRINT_SERVER_PORT);
    this->initParam(&clip_param);
}


void ClipServer::send(){
    
    ofxOscMessage m;
    m.setAddress("/clip");
    m.addIntArg(clip_param.duration_frame);
    m.addIntArg(clip_param.tick_snd);
    m.addIntArg(clip_param.tick_noise);
    m.addIntArg(clip_param.snd1_type);
    m.addIntArg(clip_param.snd2_type);
    m.addIntArg(clip_param.noise_type);
    m.addIntArg(clip_param.aeg_frame);
    m.addIntArg(clip_param.filter_snd_type);
    m.addFloatArg(clip_param.filter_snd_cutoff);
    m.addFloatArg(clip_param.filter_snd_gain);
    m.addFloatArg(clip_param.filter_snd_q);
    m.addFloatArg(clip_param.filter_noise_type);
    m.addFloatArg(clip_param.filter_noise_cutoff);
    m.addFloatArg(clip_param.filter_noise_gain);
    m.addFloatArg(clip_param.filter_noise_q);
    m.addIntArg(clip_param.ef_deg_type);
    m.addIntArg(clip_param.ef_pan_type);
    m.addIntArg(clip_param.ef_am_on);
    m.addFloatArg(clip_param.ef_am_freq);
    m.addIntArg(clip_param.ef_noiseconv_on);
    m.addFloatArg(clip_param.ef_noise_conv_modamp);
    m.addIntArg(clip_param.clip_id);
    //TODO: Rest until Start
    server.sendMessage(m);
    
    ofxOscMessage m2;
    m2.setAddress("/printQR");
    m2.addIntArg((int)clip_param.clip_id);
    printServer.sendMessage(m2);
    
}

void ClipServer::send(clip_param_t getParams){
    clip_param = getParams;
    this->send();
}

void ClipServer::initParam(clip_param_t *pClip){

    pClip->duration_frame = (int)ofRandom(3)+1; // 1-3
    pClip->tick_snd = ofRandom(1199)+1;   //1-1200
    pClip->tick_noise = ofRandom(1199)+1; //1-1200
    pClip->snd1_type = ofRandom(4); //0-4
    pClip->snd2_type = ofRandom(4);  //0-4
    pClip->noise_type = ofRandom(2); //0-2
    pClip->aeg_frame = ofRandom(2); //0-2;
    pClip->filter_snd_type = ofRandom(3); //0-3
    pClip->filter_snd_cutoff = ofRandom(1.); //0.- 1.
    pClip->filter_snd_q = ofRandom(1.); //0.-1.
    pClip->filter_noise_type = ofRandom(3.); //0-3
    pClip->filter_noise_cutoff = ofRandom(1.); //0.-1.
    pClip->filter_noise_gain = ofRandom(1.); // 0.-n?
    pClip->filter_noise_q = ofRandom(1.); //0.- 1.
    pClip->ef_deg_type = ofRandom(4); // 0 - 4
    pClip->ef_pan_type = ofRandom(5); // 0 - 5
    pClip->ef_am_on = ofRandom(1); // 0 - 1
    pClip->ef_am_freq = ofRandom(1.); //0. - 1.
    pClip->ef_noiseconv_on = ofRandom(1); //0,1
    pClip->ef_noise_conv_modamp = ofRandom(1.);
    pClip->clip_id = this->createId(); //pod number
    
    
//    pClip->duration_frame; // 1-3
//    pClip->tick_snd;   //1-1200
//    pClip->tick_noise; //1-1200
//    pClip->snd1_type; //0-4
//    pClip->snd2_type;  //0-4
//    pClip->noise_type; //0-2
//    pClip->aeg_frame; //0-2;
//    pClip->filter_snd_type; //0-3
//    pClip->filter_snd_cutoff; //0.- 1.
//    // pClip->filter_snd_gain; //0. -n?
//    pClip->filter_snd_q; //0.-1.
//    pClip->filter_noise_type; //0-3
//    pClip->filter_noise_cutoff; //0.-1.
//    //pClip->filter_noise_gain; // 0.-n?
//    pClip->filter_noise_q; //0.- 1.
//    pClip->ef_deg_type; // 0 - 4
//    pClip->ef_pan_type; // 0 - 5
//    pClip->ef_am_on; // 0 - 1
//    pClip->ef_am_freq; //0. - 1.
//    // int ef_noiseconv_on; //0,1
//    // float ef_noise_conv_modamp;
//    pClip->clip_id; //pod number
//    

}


int ClipServer::createId(){
    
    //Creat odd number
    return (((int)ofRandom(60466175)>>1)<<1)+1;
    
    
}