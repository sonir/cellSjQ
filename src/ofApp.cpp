#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    

    //Instanciate Model
    model = new slCellModel(SCREEN_WIDTH, SCREEN_HEIGHT);
    //APP Setup
    initPresetmodes(); //Make presets
    setPresetMode(PS_DEFAULT); //Set values
    system.sending_interval = SENDING_INTERVAL;
    
    //Init system flags
    system.reset_flg = 0; //Init reset flag
    system.stop_flg = DEFAULT_STOP_FLG; // Init stop flag
    system.step_count = 0;
    system.sent_drone = 0;
//    system.temp = 25.0f;
//    system.light = 0.5f;
    system.beat_fix = 1.0;
    system.phase = RHYTHM;
    for(int i =0;i<AG_MAX_NUM;i++)system.ag_atk[i]=false;
    
    //Init Timers
    timerAgentStep = new slMetro(system.step_interval);
    timerSendingParameters = new slMetro(system.sending_interval);
    timerArduinoUpdate = new slMetro(system.arduino_update_interval);
    timerClipEvent = new slMetro(CLIP_EVENT_INTERVAL);
	modeInterval = new slMetro(MODE_INTERVAL);
    
    //Font Setup
    ofTrueTypeFont::setGlobalDpi(72);
    h1.loadFont("Dekar.otf", 30, true, true);
    h2.loadFont("Dekar.otf", 18, true, true);
    body.loadFont("frabk.ttf", 12, false, true);
    
    
    this->initTouched();
    

    int ofSetFrameRate(system.fps); //Setup Frame Rate
    if(false)toolKit.dice(6); //No meaning code to avoid "Unused" warning.
    
	//OSC Setup
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
	current_msg_string = 0;
    // open an outgoing connection to HOST:PORT
    server.setup(SENDER_HOST, SENDER_PORT);


    //Set Corner
	arm[0] = new slCircular(-0.7,0.7,1.1, -0.75);
	arm[1] = new slCircular(0.7,0.7,1.1,0.75);
	arm[2] = new slCircular(0.7,-0.7, 1.1, .25);
	arm[3] = new slCircular(-0.7,-0.7,1.1, -.25);
	space = new slEuclid(SCREEN_WIDTH , SCREEN_HEIGHT);                   

	pirad = 0.;

    //Add Agents
    this->addAgents();
    solo = new clSolo(model);
    dronePhase = new clDronePhase(model);
    

    //INIT modename
    system.now_mode = "mode> PS_DEFAULT";
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
    //Reset check for Mutex
    if(system.reset_flg && !system.stop_flg ){ //do reset when the top of next go
        
        model->reset();
        this->addAgents();
        system.reset_flg = 0;
    }        
    //Do Agent Cycle
    if(!system.stop_flg){
        
        model->syncTouchEvent(touched);
        initTouched(); //Reset the flg after sending
 
        if (system.phase==RHYTHM || system.phase==CLIMAX){
            
//            cout << "phase is rhythm" << endl;
            
            if(timerAgentStep->alart()){
                
                if(system.clock_flg){
                    //Clock Mode
                        atkCheck(system.step_count);
                        model->stroke(system.step_count);
                        model->initTouchEvent(system.step_count);
                        resetAtk(system.step_count);
                        system.step_count++;
                        if(system.step_count>=ARM_NUM){
                            system.step_count=0; //If finished one loop, Reset count
                        }
                    
                }else{ //Normal Mode
                    
                        //check ATK_MOV
                        atkCheck();
                        model->cycle();
                        model->initTouchEvent();
                        resetAtk();
                }
            }
            
        }
        
        if(system.phase==SOLO){
            
            // It was first turn.
            if( solo->isInit() ){
                solo->initParam(); //Init the params
            }else if( solo->countCheck() || solo->touchCheck()){
                
                solo->reset();
                
            }
            
            if(system.ag_atk[solo->focused_ag]){ //If the instrument was played
                
                atkCheck(solo->focused_ag);
                solo->action();
                resetAtk(solo->focused_ag);
                
            }
            
        }

        if(system.phase==DRONE){
            
            // It was first turn.
            if( dronePhase->isInit() ){
                dronePhase->initParam(); //Init the params
            }
            
            //Do interactions with Normal (not clock) mode
            atkCheck();
            model->cycle();
            model->initTouchEvent();
            resetAtk();
            
            
//            if(system.ag_atk[solo->focused_ag]){ //If the instrument was played
//                
//                atkCheck(solo->focused_ag);
//                solo->action();
//                resetAtk(solo->focused_ag);
//                
//            }
            
        }
        
        if(system.phase==REST){
            
            for(int i=0; i<AG_MAX_NUM; i++){
                agent tmp;
                model->setAgent(i, &tmp);
                tmp.circumference_posi=0.0;
                model->setAgent(i,&tmp);
                
            }
            
        }
        
        

        if(timerSendingParameters->alart()){
            //Send Now Agents States
            snap.ag[0] = model->getAgent(0);
            snap.ag[1] = model->getAgent(1);
            snap.ag[2] = model->getAgent(2);
            snap.ag[3] = model->getAgent(3);
            sound.update(SOUND, snap);
            //TODO: make timer for clip
    //        sound.update(CLIP, snap);
            system.sent_drone = true;
        }

        if(timerArduinoUpdate->alart()){
            //Send Now Agents States
            snap.ag[0] = model->getAgent(0);
            snap.ag[1] = model->getAgent(1);
            snap.ag[2] = model->getAgent(2);
            snap.ag[3] = model->getAgent(3);
            sound.update(ARDUINO, snap);
        }

        if(timerClipEvent->alart()){

            //Send Now Agents States
            snap.ag[0] = model->getAgent(0);
            snap.ag[1] = model->getAgent(1);
            snap.ag[2] = model->getAgent(2);
            snap.ag[3] = model->getAgent(3);        
            sound.update(CLIP, snap);
            
        }
        
//        if(modeInterval->alart()){
//            
//            //Send Now Agents States
////            setPresetMode( (preset_mode_t)toolKit.dice(3)+1);
//
//            
//            modeInterval->set(MODE_INTERVAL);
//    		switch (toolKit.dice(10)-1){
//    				
//    			case 0:
//                    setPresetMode(PS_MICRO);
//                    updateSystemValue();
//                    system.now_mode = "mode> PS_MICRO";
//    				break;
//    				
//    			case 1:
//    				setPresetMode(PS_MICRO);
//    				updateSystemValue();				
//                    system.now_mode = "mode> PS_MICRO";
//    				break;
//    				
//    			case 2:
//                    setPresetMode(PS_MICRO);
//                    updateSystemValue();
//                    system.now_mode = "mode> PS_MICRO";
//    				break;
//                    
//                    
//                case 3:
//                    setPresetMode(PS_MICRO);
//                    updateSystemValue();
//                    system.now_mode = "mode> PS_MICRO";
//                    break;
//
//                case 4:
//                    setPresetMode(PS_MICRO);
//                    updateSystemValue();
//                    system.now_mode = "mode> PS_MICRO";
//                    break;
//
//                case 5:
//                    setPresetMode(PS_DEFAULT);
//                    updateSystemValue();
//                    system.now_mode = "mode> PS_DEFAULT";
//                    break;
//
//                case 6:
//                    setPresetMode(PS_DEFAULT);
//                    updateSystemValue();
//                    //    				cout << "mode PS_DEFAULT" << endl;
//                    system.now_mode = "mode> PS_DEFAULT";
//                    break;
//
//                case 7:
//                    setPresetMode(PS_DEFAULT);
//                    updateSystemValue();
//                    //    				cout << "mode PS_DEFAULT" << endl;
//                    system.now_mode = "mode> PS_DEFAULT";
//                    break;
//                    
//                case 8:
//                    setPresetMode(PS_DEFAULT);
//                    updateSystemValue();
//                    system.now_mode = "mode> PS_DEFAULT";
//                    break;
//                    
//                case 9:
//                    setPresetMode(PS_CATHARSIS);
//                    updateSystemValue();
//                    system.now_mode = "mode> PS_CATHARSIS";
//                    modeInterval->set(MODE_INTERVAL_SHORT);
//                    break;
//
//                    
//    			default:
//    				setPresetMode(PS_DEFAULT);
//    				updateSystemValue();				
////    				cout << "mode: by SWITCH_DEFAULT PS_DEFAULT" << endl;
//                    system.now_mode = "mode> PS_DEFAULT";
//    				break;
//                    
//    		}
//         
//        }

        
//        sendData();
    } //End of stop flag limitation

	listenOsc();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    

	sl_screen_position center;
	sl_screen_position node;
    ofBackground(0, 0, 0);

    
    //Draw ARM1
    syncPosition(0);
    center = space->getScreenPosition(arm[0]->center.x,arm[0]->center.y);
    node = space->getScreenPosition(arm[0]->arc_position.x,arm[0]->arc_position.y);
    ofSetHexColor(0xFF0000);
    if(checkIntersect(arm[0])){ //If the lines are crossing
        digitalFis.flash(PF,(fis_color)toolKit.dice(3));
         // digitalFis.flash(PF,(fis_color)getAgent.condition);
    }
    ofLine(center.x,center.y,node.x,node.y); //Change Line Color

    //Draw ARM2
    syncPosition(1);
     center = space->getScreenPosition(arm[1]->center.x,arm[1]->center.y);
     node = space->getScreenPosition(arm[1]->arc_position.x,arm[1]->arc_position.y);
     ofSetHexColor(0x00FF77);
     if(checkIntersect(arm[1])){ //If the lines are crossing
         digitalFis.flash(TB,(fis_color)toolKit.dice(3));
     }
     ofLine(center.x,center.y,node.x,node.y);                

    //Draw ARM3
    syncPosition(2);
     center = space->getScreenPosition(arm[2]->center.x,arm[2]->center.y);
     node = space->getScreenPosition(arm[2]->arc_position.x,arm[2]->arc_position.y);
     ofSetHexColor(0x0000FF);
     if(checkIntersect(arm[2])){ //If the lines are crossing
         digitalFis.flash(GT,(fis_color)toolKit.dice(3));
     }
     ofLine(center.x,center.y,node.x,node.y);                

    //Draw ARM4
    syncPosition(3);
     center = space->getScreenPosition(arm[3]->center.x,arm[3]->center.y);
     node = space->getScreenPosition(arm[3]->arc_position.x,arm[3]->arc_position.y);
     ofSetHexColor(0xFFFF00);
     if(checkIntersect(arm[3])){ //If the lines are crossing
         digitalFis.flash(BS,(fis_color)toolKit.dice(3));
     }
     ofLine(center.x,center.y,node.x,node.y);

    //Draw Fonts
    float top_offset = TOP_OFFSET;
    ofSetColor(225);
    h1.drawString(DISP_TITLE, LEFT_OFFSET, TOP_OFFSET);
    h2.drawString(MESSAGE1, LEFT_OFFSET, top_offset+=LINE_HEIGHT);
    
    if(system.stop_flg){
       
        h1.drawString("STOPPED", 320, 400);
        
    }
    
    //TODO:Is this correct?
    char tmpStr[126];
//    sprintf(tmpStr, "light:%.3f temp:%.3f", system.light, system.temp);
    if(system.phase==SOLO){
        sprintf(tmpStr, "%s (%s)", "phase: SOLO", "key: s-solo n-rhythm");
    } else if (system.phase==CLIMAX) {
        
        sprintf(tmpStr, "%s (%s)", "phase: CLIMAX", "key: s-solo n-rhythm f-climax");
        
    } else if (system.phase==DRONE) {
        
        sprintf(tmpStr, "%s (%s)", "phase: DRONE", "key: s-solo n-rhythm f-climax");
        
        
    }else{
        sprintf(tmpStr, "%s (%s)", "phase: RHYTHM" , "key: s-solo n-rhythm");
    }
    h2.drawString(tmpStr, LEFT_OFFSET, top_offset+=LINE_HEIGHT),
    top_offset = this->dispAgentParam(top_offset,0);
    top_offset = this->dispAgentParam(top_offset,1);
    top_offset = this->dispAgentParam(top_offset,2);
    top_offset = this->dispAgentParam(top_offset,3);
    ofSetColor(255);
    //OSC Status
    ofSetColor(80);
    body.drawString(SENDIG_MES, LEFT_OFFSET+15., SCREEN_HEIGHT-LINE_HEIGHT_BODY);
    ofSetColor(255);
    if(system.sent_drone){
        system.sent_drone = false;
        //ofRect(LEFT_OFFSET, SCREEN_HEIGHT-30.f, 10.f, 10.f);
        body.drawString(SENDIG_MES, LEFT_OFFSET+15., SCREEN_HEIGHT-LINE_HEIGHT_BODY);

    }
    if(system.clock_flg) ofSetColor(0, 150, 0);
    else ofSetColor(0, 80, 0);
    body.drawString(LB_MOVE_MODE1, LEFT_OFFSET+15., SCREEN_HEIGHT-(LINE_HEIGHT_BODY*2));

    //Display mode name
    h2.drawString(system.now_mode, LEFT_OFFSET, LINE_HEIGHT*4);

}

float ofApp::dispAgentParam(float top_offset, int ag_id){

    float ag_top;
    char str_val[40]; //Create Label Str
    agent ag = model->getAgent(ag_id); //Get Agent Param
    
    //Agent
    ofSetColor(0,205,0);
    sprintf(str_val, LB_AGENT_ID, ag.ag_id);
    body.drawString(str_val, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT);
    ag_top = top_offset;
    ofSetColor(0,180,0);
    body.drawString(LB_VIEW, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT_BODY);
    body.drawString(LB_STRENGTH, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT_BODY);
    body.drawString(LB_DEXTERYTY, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT_BODY);
    body.drawString(LB_HP, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT_BODY);
    ofSetColor(0,100,0);
    body.drawString(LB_INTERTIA, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT_BODY);
    body.drawString(LB_C_POSI, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT_BODY);
    body.drawString(LB_CONTACT, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT_BODY);
    body.drawString(LB_ACTION, LEFT_OFFSET_FOR_PRAM,top_offset+=LINE_HEIGHT_BODY);
    //Param
    ofSetColor(0, 205, 0);
    body.drawString(LB_SPACER, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset=ag_top);
    sprintf(str_val, "%.3f", ag.view);
    body.drawString(str_val, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    sprintf(str_val, "%d", ag.strength);
    body.drawString(str_val, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    sprintf(str_val, "%d", ag.dexterity);
    body.drawString(str_val, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    sprintf(str_val, "%.3f", ag.hp);
    body.drawString(str_val, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    ofSetColor(0,150,0);
    sprintf(str_val, "%d", ag.inertia);
    body.drawString(str_val, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    sprintf(str_val, "%.3f", ag.circumference_posi);
    body.drawString(str_val, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    sprintf(str_val, "%d", ag.contact_flg);
    body.drawString(str_val, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    string action;
    switch(ag.action_flg){
        case RANDOM_WALK:
            action = "RAND_W";
            break;
        case CHASE:
            action = "CHASE";
            break;
        case RUN:
            action = "RUN";
            break;
        case ATTACK:
            action = "ATTACK";
            break;
        case DMG:
            action = "DMG";
            break;
        case DEATH:
            action = "DEATH";
            break;

    }
    //sprintf(str_val, "%s", action);
//    body.drawString(str_val, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    if(ag.action_flg==DMG||ag.action_flg==DEATH) ofSetColor(205, 0, 0);
    body.drawString(action, (LEFT_OFFSET_FOR_PRAM+PARAMETER_SPC),top_offset+=LINE_HEIGHT_BODY);
    if(ag.action_flg==DMG||ag.action_flg==DEATH) ofSetColor(0, 205, 0);
    
    return top_offset;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key == OF_KEY_RETURN){
    	system.stop_flg = ( (system.stop_flg-1)*(-1) ); //Invert the value
    } else if (key == 0x20){
    	system.stop_flg = ( (system.stop_flg-1)*(-1) ); //Invert the value
    } else if (key == 'c'){
        system.clock_flg = ( (system.clock_flg-1)*(-1) ); //Invert the value
        //Set the interval
        if(system.clock_flg) timerAgentStep->set(system.step_interval);
        else timerAgentStep->set(system.step_interval_normal_mode);
    } else if (key == '0'){
        setPresetMode(PS_DEFAULT);
        updateSystemValue();
        system.now_mode = "mode> PS_DEFAULT";
    } else if (key == '1'){
        setPresetMode(PS_MICRO);
        updateSystemValue();
        system.now_mode = "mode> PS_MICRO";
    } else if (key == '2'){
        setPresetMode(PS_CATHARSIS);
        updateSystemValue();
        system.now_mode = "mode> PS_CATHARSIS";
    } else if (key == '3'){
        setPresetMode(PS_VIBE);
        updateSystemValue();
        system.now_mode = "mode> PS_VIBE";
    } else if (key == '4'){
        setPresetMode(PS_SYNC);
        updateSystemValue();
        system.now_mode = "mode> PS_SYNC";
    } else if (key == 'r'){
        system.reset_flg = 1;
        system.stop_flg = DEFAULT_STOP_FLG; // Init stop flag
    } else if (key == 'q'){
        
        //Test Code is here
        
        //Test Clip and Print
        //Send Now Agents States
        snap.ag[0] = model->getAgent(0);
        snap.ag[1] = model->getAgent(1);
        snap.ag[2] = model->getAgent(2);
        snap.ag[3] = model->getAgent(3);
        sound.update(CLIP, snap);
        
    } else if (key == 's'){

        solo->reset();
        dronePhase->reset();
        resetAgent();
        
//        model->setMovFix(1.0);
        
        system.phase = SOLO;
        setPresetMode(PS_CATHARSIS);
        system.now_mode = "mode> PS_CATHARSIS";
        updateSystemValue();
    } else if (key == 'd'){
        
        solo->reset();
        dronePhase->reset();
        resetAgent();
        
//        model->setMovFix(1.0);
        
        system.phase = DRONE;
        setPresetMode(PS_VIBE);
        system.now_mode = "mode> PS_VIBE";
        updateSystemValue();

        
    } else if ( key == 'f') {
        
        solo->reset();
        dronePhase->reset();
        resetAgent();
        model->setMovFix(1.0);
        
        dronePhase->initParam();
        
        system.phase = CLIMAX;
        setPresetMode(PS_CATHARSIS);
        system.now_mode = "mode> PS_CATHARSIS";
        system.step_interval = system.step_interval*0.75;
        system.step_interval_normal_mode = system.step_interval_normal_mode*0.75;
        model->setMovFix(14.3); //7.3
        updateSystemValue();

        
    } else if (key == 'n'){
        
        solo->reset();
        dronePhase->reset();
        resetAgent();
//        model->setMovFix(1.0);
        
        system.phase = RHYTHM;
        setPresetMode(PS_DEFAULT);
        system.now_mode = "mode> PS_DEFAULT";
        updateSystemValue();

    } else if (key == '9'){
        
        solo->reset();
        dronePhase->reset();
        resetAgent();
        //        model->setMovFix(1.0);
        
        system.phase = REST;
        setPresetMode(PS_DEFAULT);
        system.now_mode = "mode> PS_DEFAULT";
        updateSystemValue();
        
    } else if (key == 't'){
        
        //Send Now Agents States
        snap.ag[0] = model->getAgent(0);
        snap.ag[1] = model->getAgent(1);
        snap.ag[2] = model->getAgent(2);
        snap.ag[3] = model->getAgent(3);
        sound.update(CLIP, snap);

        
    }

    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::initPresetmodes(){
    
    //TODO: here
    
    //PS_DEFAULT
    preset[PS_DEFAULT].fps = DEFAULT_SYSTWM_FPS;
    preset[PS_DEFAULT].step_interval = DEFAULT_STEP_INTERVAL;
    preset[PS_DEFAULT].step_interval_normal_mode = DEFAULT_STEP_INTERVAL_NORMAL_MODE;
    preset[PS_DEFAULT].mov_fix = DEFAULT_MOV_FIX;
    preset[PS_DEFAULT].random_walk_fix = DEFAULT_RANDOM_WALK_FIX;
    preset[PS_DEFAULT].clock_flg = DEFAULT_CLOCK_MODE;

    //PS_MICRO
    preset[PS_MICRO].fps = PS_MICRO_SYSTWM_FPS;
    preset[PS_MICRO].step_interval = PS_MICRO_STEP_INTERVAL;
    preset[PS_MICRO].step_interval_normal_mode = PS_MICRO_STEP_INTERVAL_NORMAL_MODE;
    preset[PS_MICRO].mov_fix = PS_MICRO_MOV_FIX;
    preset[PS_MICRO].random_walk_fix = PS_MICRO_RANDOM_WALK_FIX;
    preset[PS_MICRO].clock_flg = PS_MICRO_CLOCK_MODE;

    //PS_CATHARSIS
    preset[PS_CATHARSIS].fps = PS_CATHARSIS_SYSTWM_FPS;
    preset[PS_CATHARSIS].step_interval = PS_CATHARSIS_STEP_INTERVAL;
    preset[PS_CATHARSIS].step_interval_normal_mode = PS_CATHARSIS_STEP_INTERVAL_NORMAL_MODE;
    preset[PS_CATHARSIS].mov_fix = PS_CATHARSIS_MOV_FIX;
    preset[PS_CATHARSIS].random_walk_fix = PS_CATHARSIS_RANDOM_WALK_FIX;
    preset[PS_CATHARSIS].clock_flg = PS_CATHARSIS_CLOCK_MODE;

    //PS_VIBE
    preset[PS_VIBE].fps = PS_VIBE_SYSTWM_FPS;
    preset[PS_VIBE].step_interval = PS_VIBE_STEP_INTERVAL;
    preset[PS_VIBE].step_interval_normal_mode = PS_VIBE_STEP_INTERVAL_NORMAL_MODE;
    preset[PS_VIBE].mov_fix = PS_VIBE_MOV_FIX;
    preset[PS_VIBE].random_walk_fix = PS_VIBE_RANDOM_WALK_FIX;
    preset[PS_VIBE].clock_flg = PS_VIBE_CLOCK_MODE;

    //PS_SYNC
    preset[PS_SYNC].fps = PS_SYNC_SYSTWM_FPS;
    preset[PS_SYNC].step_interval = PS_SYNC_STEP_INTERVAL;
    preset[PS_SYNC].step_interval_normal_mode = PS_SYNC_STEP_INTERVAL_NORMAL_MODE;
    preset[PS_SYNC].mov_fix = PS_SYNC_MOV_FIX;
    preset[PS_SYNC].random_walk_fix = PS_SYNC_RANDOM_WALK_FIX;
    preset[PS_SYNC].clock_flg = PS_SYNC_CLOCK_MODE;
   
}

//--------------------------------------------------------------
preset_mode_t ofApp::setPresetMode(preset_mode_t target_mode){
    
    //Change preset mode
    system.fps = preset[target_mode].fps;
    system.step_interval = preset[target_mode].step_interval;
    system.step_interval_normal_mode = preset[target_mode].step_interval_normal_mode;
    system.clock_flg = preset[target_mode].clock_flg;
    model->setMovFix(preset[target_mode].mov_fix);
    model->setRandomWalkFix(preset[target_mode].random_walk_fix);
    
    //Change Now Preset mode
    preset_mode_now = target_mode;
	
    
}

void ofApp::updateSystemValue(){
    
    //Update
    ofSetFrameRate(system.fps);
    
    if(system.clock_flg){
        timerAgentStep->set(system.step_interval*system.beat_fix);
    }else{
        timerAgentStep->set(system.step_interval_normal_mode*system.beat_fix);
    }
    
    
}

//--------------------------------------------------------------
bool ofApp::checkIntersect(slCircular *target){

	// if(target == arm[1])ofSetHexColor(0xFFFFFF);

	slCircular* another;

	for(int i = 0; i<4; i++){

		switch (i){

			case 0:
				another = arm[0];
				break;

			case 1:
				another = arm[1];
				break;

			case 2:
				another = arm[2];			
				break;

			case 3:
				another = arm[3];			
				break;

			default:
				cout << "unknown agent" << endl;
				break;

		}

		if(target == another) continue;

	    sl_scaled_position *a_st = (struct sl_scaled_position *)&target->center;
	    sl_scaled_position *a_ed = (struct sl_scaled_position *)&target->arc_position;
	    sl_scaled_position *b_st = (struct sl_scaled_position *)&another->center;
	    sl_scaled_position *b_ed = (struct sl_scaled_position *)&another->arc_position;

		if( space->intersect(*a_st, *a_ed, *b_st, *b_ed) ){

			ofSetHexColor(0xFFFFFF);
			return true;			
		}

	}

	return false;

}



//--------------------------------------------------------------
void ofApp::addAgents(){
    
    //SetUp Agents
    agent ag1;
    ag1.view = 0.3;
    ag1.strength = 50;
    ag1.dexterity = 50;
    ag1.hp = 0.5;
    ag1.circumference_posi = -1.0f;
    ag1.posi.x = 0.138;
    ag1.posi.y = 0.139;
    model->addAgent(&ag1);
    
    agent ag2;
    ag2.view = 0.3;
    ag2.strength = 50;
    ag2.dexterity = 50;
    ag2.hp = 0.5;
    ag2.circumference_posi = -1.0f;
    ag2.posi.x = 0.138;
    ag2.posi.y = 0.139;
    model->addAgent(&ag2);
    
    agent ag3;
    ag3.view = 0.3;
    ag3.strength = 50;
    ag3.dexterity = 50;
    ag3.hp = 0.5;
    ag3.circumference_posi = -1.0f;
    ag3.posi.x = 0.;
    ag3.posi.y = 0.;
    model->addAgent(&ag3);
    
    agent ag4;
    ag4.view = 0.3;
    ag4.strength = 50;
    ag4.dexterity = 50;
    ag4.hp = 0.5;
    ag4.circumference_posi = -1.0f;
    ag4.posi.x = 1.;
    ag4.posi.y = 1.;
    model->addAgent(&ag4);
    
    for(int i=0; i< AG_MAX_NUM; i++){
        agent tmp = model->getAgent(i);
        system.ag_dexterity_org[i] = tmp.dexterity;
        //TODO: Add AGENT COPY
        ag_org[i]=tmp;
    }
 
    //Sync ag position and arm position
    this->syncPositions();
    
}



//--------------------------------------------------------------

void ofApp::sendData(){

    ofxOscMessage m;
    m.setAddress("/message");
    m.addFloatArg(0.9f);
    server.sendMessage(m);

}

//--------------------------------------------------------------

void ofApp::listenOsc(){

	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        

		//Message for arm[0]
		if(m.getAddress() == "/circleControl"){
            
            slCircular* circle;
            agent ag;
            
			string target = m.getArgAsString(0); //Get the 2nd address
            int target_id;
            if(target=="/circle1") target_id = 0;
			else if (target =="/circle2")target_id = 1;
            else if (target =="/circle3")target_id = 2;
            else if (target =="/circle4")target_id = 3;

            //Set target arm and agent
            circle = arm[target_id];
            ag = model->getAgent(target_id);

            //Set agent position
            ag.circumference_posi = (float)scale(m.getArgAsFloat(1), 1., 0., 1., -1.);
            model->setAgent(target_id, &ag);
            cout << ag.circumference_posi << endl;
			if( m.getArgAsFloat(2) != 0.) circle->radius = m.getArgAsFloat(2);

		}if(m.getAddress() == "/touch"){
            
//            cout << "touch" << m.getArgAsInt32(0) << endl;
            touched.ag[m.getArgAsInt32(0)] = true;
            //system.stop_flg=1;
            
        }if( m.getAddress() == "/pause" ){
            system.stop_flg = ( (system.stop_flg-1)*(-1) ); //Invert the value

            ofxOscMessage m;
            m.setAddress("/stopCallback");
            m.addIntArg(system.stop_flg);
           
        }if( m.getAddress() == "/pauseReset" ){
            
            system.reset_flg = 1;
            system.stop_flg = DEFAULT_STOP_FLG; // Init stop flag
        
            
        }if ( m.getAddress() == "/beat_fix" ){
            
            system.beat_fix = m.getArgAsFloat(0);
//            setPresetMode(PS_DEFAULT);
            updateSystemValue();

        }else if ( m.getAddress() == "/mov_fix" ){
        
            model->setMovFix( m.getArgAsFloat(0) );
            

        }else if ( m.getAddress() == "/mov_random_fix" ){
            model->setRandomWalkFix( m.getArgAsFloat(0) );
            
        }else if ( m.getAddress() == "/atk" ){
            
            //            cout << "atk" << m.getArgAsInt32(0) << endl;
            system.ag_atk[m.getArgAsInt32(0)]=true;

            
        }else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}
	}
}



void ofApp::syncPosition(int ag_id){
    
    agent ag = model->getAgent(ag_id);
    
    //Scale Agent position into pirad
    double pirad = scale(ag.circumference_posi, 1., -1., 1., 0.);
    //Set the arm position with piRad, then, set the eucrid arm position to agent
    ag.posi = arm[ag_id]->piradToPosition( (float)pirad );
    model->setAgent(ag_id, &ag);
    
}

void ofApp::syncPositions(){
    
    //Sync the agent position into arm position
    for(int i; i<ARM_NUM; i++) this->syncPosition(i);

    
}

//initTouchedEvent();
void ofApp::initTouched(){
    
    //Init Touched Event Memory
    for(int i =0; i<AG_MAX_NUM; i++)
    {
        touched.ag[i] = false;
    }
    
    
}


void ofApp::atkCheck(){
    
    for(int i=0; i<AG_MAX_NUM; i++){
        if(system.ag_atk[i]==true){
            
            atkCheck(i);
            
        }
        
    }
    
}

void ofApp::atkCheck(int ag_id){
    
    //Ignore unknown ag_id
    if (ag_id >= AG_MAX_NUM){
     
        cout << "ATKCHK: Unknown AG" << endl;
        return;
        
    }
    
    
    if(system.ag_atk[ag_id]==true){
        
        agent tmp = model->getAgent(ag_id);
        tmp.dexterity += ATK_DEX_FIX;
        model->setAgent(ag_id, &tmp);
        
    }

    
}

void ofApp::resetAtk(){
    
    for(int i=0; i<AG_MAX_NUM;i++){
        
        if(system.ag_atk[i]){
            
            resetAtk(i);
            
        }
        
    }
    
}

void ofApp::resetAtk(int ag_id){
    
    if(system.ag_atk[ag_id]){
        
        agent tmp = model->getAgent(ag_id);
        tmp.dexterity = system.ag_dexterity_org[ag_id];
        model->setAgent(ag_id, &tmp);
        system.ag_atk[ag_id]=false;

    }
    
}

void ofApp::resetAgent(){
    
    for(int i=0;i<AG_MAX_NUM;i++){
        
        agent tmp = ag_org[i];
        model->setAgent(i, &tmp);
        
    }
    
}


