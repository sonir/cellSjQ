//
//  DroneServer.cpp
//  cell
//
//  Created by sonir on 10/28/14.
//
//

#include "DroneServer.h"

DroneServer::DroneServer(){
    
    cout << "drones" << endl;
    server.setup(SND_SERVER_IP, SND_SERVER_PORT);
    this->initParam(&drone_param);
}


void DroneServer::send(){
//    initParam(&drone_param);
    ofxOscMessage m;
    m.setAddress("/drone");
    m.addIntArg(drone_param.twitter_announce);
    m.addIntArg(drone_param.twitter_log);
    m.addFloatArg(drone_param.synth_pitch1_vol);
    m.addFloatArg(drone_param.synth_pitch2_vol);
    m.addFloatArg(drone_param.synth_pitch3_vol);
    m.addFloatArg(drone_param.synth_pitch4_vol);
    m.addFloatArg(drone_param.synth_pitch5_vol);
    m.addFloatArg(drone_param.synth_pitch6_vol);
    m.addFloatArg(drone_param.synth_pitch7_vol);
    m.addFloatArg(drone_param.synth_pitch8_vol);
    m.addFloatArg(drone_param.synth_pitch9_vol);
    m.addFloatArg(drone_param.synth_pitch10_vol);
    m.addFloatArg(drone_param.synth_pitch11_vol);
    m.addFloatArg(drone_param.synth_pitch12_vol);
    m.addFloatArg(drone_param.synth_pitch13_vol);
    m.addIntArg(drone_param.degrade_on);
    m.addFloatArg(drone_param.degrade_sample_rate);
    m.addIntArg(drone_param.vocoder_on);
    m.addFloatArg(drone_param.vocoder_dry);
    m.addIntArg(drone_param.reverb_on);
    m.addFloatArg(drone_param.reverb_dry);
    m.addIntArg(drone_param.autopan_on);
    m.addFloatArg(drone_param.autopan_rate);
    m.addFloatArg(drone_param.autopan_ammount);
    m.addFloatArg(drone_param.autopan_randomness);
	m.addIntArg(drone_param.autofilter_on);
    m.addFloatArg(drone_param.auto_filter_ammount);
    m.addFloatArg(drone_param.auto_filter_rate);
    m.addFloatArg(drone_param.auto_filter_cutoff);
    m.addIntArg(drone_param.sequece_pattern);
    server.sendMessage(m);
    
    
}

void DroneServer::send(drone_param_t getParams){
    drone_param = getParams;
    this->send();
}

void DroneServer::initParam(drone_param_t *pDrone){

    pDrone->twitter_announce = 0;
    pDrone->twitter_log = 0;
    pDrone->synth_pitch1_vol = 0.4f;
    pDrone->synth_pitch2_vol = 0.4f;
    pDrone->synth_pitch3_vol = 0.4f;
    pDrone->synth_pitch4_vol = 0.4f;
    pDrone->synth_pitch5_vol = 0.4f;
    pDrone->synth_pitch6_vol = 0.4f;
    pDrone->synth_pitch7_vol = 0.4f;
    pDrone->synth_pitch8_vol = 0.4f;
    pDrone->synth_pitch9_vol = 0.4f;
    pDrone->synth_pitch10_vol = 0.4f;
    pDrone->synth_pitch11_vol = 0.4f;
    pDrone->synth_pitch12_vol = 0.4f;
    pDrone->synth_pitch13_vol = 0.4f;
    pDrone->degrade_on= 0;
    pDrone->degrade_sample_rate = 1.0f;
    pDrone->vocoder_on = 0;
    pDrone->vocoder_dry = 0.0f;
    pDrone->reverb_on = 0;
    pDrone->reverb_dry = 0.0f;
    pDrone->autopan_on = 0;
    pDrone->autopan_rate = 0.0f;
    pDrone->autopan_ammount = 0.7f;
    pDrone->autopan_randomness = 1.0f;
    pDrone->autofilter_on = 0;
    pDrone->auto_filter_ammount = 0.7f;
    pDrone->auto_filter_rate = 0.7f;
    pDrone->auto_filter_cutoff = 0.7f;
    pDrone->sequece_pattern = 1;

}