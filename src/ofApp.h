#pragma once

#include "cellSetup.h"
#include "ofMain.h"
#include "ofxOsc.h"
#include "slCircular.h"
#include "slEuclid.h"
#include "slMetro.h"
#include "DigitalFis.h"
#include "ServerManager.h"
#include "clSolo.h"
#include "slCellModel.h"
#include "clDronePhase.h"


typedef struct system_t {
    
    int fps;
    float step_interval;
    float step_interval_normal_mode;
    float sending_interval;
    float arduino_update_interval;
    
    int reset_flg;
    int stop_flg;
    int clock_flg;
    int step_count;
    bool sent_drone;
    
    //    float light;
    //    float temp;
    
    //For IC
    float beat_fix;
    bool ag_atk[AG_MAX_NUM];
    int ag_dexterity_org[AG_MAX_NUM];
    
    phase_t phase;
    string now_mode;
    
} system_t;


typedef struct preset_t{
    
    int fps;
    float step_interval;
    float step_interval_normal_mode;

    float mov_fix;
    float random_walk_fix;
    
    int clock_flg;
    
    
} preset_t;


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void initPresetmodes();
        preset_mode_t setPresetMode(preset_mode_t target_mode);
        void updateSystemValue();


        system_t system;
        preset_mode_t preset_mode_now;
        preset_t preset[5];
        slCellModel *model;
    
    	//OSC Receive
		ofxOscReceiver receiver;
		int current_msg_string;
		string msg_strings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];
        void sendData();
        agent ag_org[AG_MAX_NUM];

    

    private:

        //OSC
        ofxOscSender server;
    
        //Utilities
        slMetro *timerAgentStep;
        slMetro *timerSendingParameters;
        slMetro *timerArduinoUpdate;
        slMetro *timerClipEvent;
		slMetro *modeInterval;
        slMetro *droneClipTimer;
//        slMetro *droneSlow;    
    
        //Fis
    	DigitalFis digitalFis;
    
        //Graphics
        float dispAgentParam(float top_offset, int ag_id);
        //Drawing Arm
        slEuclid* space;
        float pirad;
        slCircular* arm[ARM_NUM];
        float pirad_circle1;
       	float pirad_circle2;
       	float pirad_circle3;
       	float pirad_circle4;
    
		//System Variables
	    int agent_count;
        ToolKit toolKit;
        //Font
        ofTrueTypeFont h1;
        ofTrueTypeFont h2;
        ofTrueTypeFont body;
    
        //Agent (model level)
        void addAgents();

	    //Agent (drawing level)
        float counter;
		bool checkIntersect(slCircular *target);
		void setupSeeds();
		void displayAgents();
		void listenOsc();

    
        //Bridge Graphic and Model
        void syncPosition(int ag_id);
        void syncPositions();
    
        //Sound
        ServerManager sound;
        agent_snap_t snap;
    
        //touch_event hundler
        phisical_contact_t touched;
        void initTouched();
    
    
        //IC
        void atkCheck();
        void atkCheck(int ag_id);
        void resetAtk();
        void resetAtk(int ag_id);
        void resetAgent();    
        clSolo *solo;
        clDronePhase *dronePhase;

};
