//
//  Server.h
//  cell
//
//  Created by sonir on 10/30/14.
//
//

#ifndef __cell__Server__
#define __cell__Server__

#define NUM_MSG_STRINGS 20

#include <stdio.h>
#include "ofxOsc.h"
#include <iostream.h>
#include <stdlib.h>
#include <time.h>



typedef struct sending_message_t {

	int ival;
	float fval;

} sending_message_t;

class Server{
    
    public:
        Server(); //CONSTRUCTOR
        ofxOscSender server;
        ofxOscReceiver receiver;
        sending_message_t sending_param;
        virtual void initParam(sending_message_t *pMessage);
        virtual void send();
        virtual void send(sending_message_t getParams);

};

#endif /* defined(__cell__Server__) */
