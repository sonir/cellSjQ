//
//  DigitalFis.h
//  PhysicalGismo
//
//  Created by sonir on 10/8/14.
//
//

#ifndef __PhysicalGismo__DigitalFis__
#define __PhysicalGismo__DigitalFis__

//Define for OSC
#define HOST "224.0.0.1" //Utilizing MultiCast Address
#define FIS_PORT 54321
#define NUM_MSG_STRINGS 20


#include <stdio.h>
#include "ofxOsc.h"

enum performer_id {ALL, PF, TB, GT, BS, DR};
enum fis_color {BLACK_FLASH, RED_FLASH, YELLOW_FLASH, BLUE_FLASH};

class DigitalFis {

	public:	
		ofxOscSender sender;

		DigitalFis(){

			// open an outgoing connection to HOST:PORT
			sender.setup(HOST, FIS_PORT);

		}

		void  flash (performer_id performer, fis_color color){

			// Basic
			// ofxOscMessage m;
			// m.setAddress("/test");
			// m.addIntArg(1);
			// m.addFloatArg(3.5f);
			// m.addStringArg("hello");
			// m.addFloatArg(ofGetElapsedTimef());
			// sender.sendMessage(m);

			ofxOscMessage m;
			m.setAddress("/fisColor");
			m.addIntArg(performer);
			m.addIntArg(color);			
			sender.sendMessage(m);

		}

};

#endif /* defined(__PhysicalGismo__DigitalFis__) */
