//
//  ArduinoServer.cpp
//  cell
//
//  Created by sonir on 10/30/14.
//
//

#include "ArduinoServer.h"

ArduinoServer::ArduinoServer(){
    
    server.setup(ARDUINO_SERVER_IP, ARDUINO_SERVER_PORT);
    this->initPram(&arduino_param);

    cout << "listening for osc messages on port " << ARDUINO_RECEIVING_PORT << "\n";
    receiver.setup(ARDUINO_RECEIVING_PORT);
    
    //Init structure for receiving message
    for(int i=0; i<TOUCH_SENSOR_CH_NUM;i++)touch_sensor_state.ch[i]=0;
    
    
}


void ArduinoServer::send(){

    ofxOscMessage m;
    m.setAddress("/ardControl");
    m.addFloatArg(arduino_param.servo1);
	m.addFloatArg(arduino_param.servo2);
	m.addFloatArg(arduino_param.servo3);
	m.addFloatArg(arduino_param.servo4);
	server.sendMessage(m);

}


void ArduinoServer::send(arduino_param_t getParams){

    //Check the value was updated.
    int diff_chk = 0;
    if(getParams.servo1 != arduino_param.servo1) diff_chk++;
    if(getParams.servo2 != arduino_param.servo2) diff_chk++;
    if(getParams.servo3 != arduino_param.servo3) diff_chk++;
    if(getParams.servo4 != arduino_param.servo4) diff_chk++;
    
    
    if(diff_chk>0){ //If the position was updated
        arduino_param = getParams;
        this->send();
    }
}


void ArduinoServer::initPram(arduino_param_t *pArduino){

	pArduino->servo1 = 0.0f;
	pArduino->servo2 = 0.0f;
	pArduino->servo3 = 0.0f;
	pArduino->servo4 = 0.0f;

}

