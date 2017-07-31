//
//  SessionController.cpp
//  facemigration
//
//  Created by Flückiger Michael on 10.07.17.
//
//

#include "SessionController.hpp"

SessionController* SessionController::instance = 0;
SessionController* SessionController::getInstance() {
    if (!instance) {
        instance = new SessionController();
    }
    return instance;
}




SessionController::SessionController(){
    
}


SessionController::~SessionController(){
    
}

void SessionController::initialize() {
    initialized=true;
 

    
}
bool SessionController::isInitialized(){
    return initialized;
}

void SessionController::setup(){
    videoplayer.setup();
    story=0;

    
    // Startscreens
    ofImage img;
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen.jpg");
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen2.png");
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen3.jpg");
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen4.jpg");
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen5.jpg");
    
    
    
    sessions = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
    
    sessions->loadFile("clips.xml");
    recordingSession=sessions->getNumTags("RECORDINGSESSION");
    cout<<"Session: "<<recordingSession<<endl;
    videorecorder.setup(sessions);


    videoplayer.loadStory(story);

    

}

void SessionController::update(){
    
    switch (state) {
            
            
        case STARTUP:
            
            break;
            
        case IDLE:
            videoplayer.update();

            break;
            
        case ACTIVE_SESSION_START:
            videorecorder.update();
            break;
            
        case ACTIVE_SESSION_RECORD:
            videorecorder.update();
            videoplayer.update();
            break;
            
        case ACTIVE_SESSION_END:
            break;
            
        default:
            break;
    }


}

void SessionController::draw(){
    
    switch (state) {
            
        case STARTUP:
            
            break;
            
            
        case IDLE:
            videoplayer.draw();

            break;
            
        case ACTIVE_SESSION_START:
            videorecorder.draw();
            drawInit();
            
            break;
            
        case ACTIVE_SESSION_RECORD:
            videoplayer.draw();
            videorecorder.draw();
            
            break;
            
        case ACTIVE_SESSION_END:
            break;
            
        default:
            break;
    }


}




void SessionController::drawInit(){
    ofPushMatrix();
    ofTranslate(0, ofGetHeight());
    ofRotate(-90);
    
    startScreens[screenInd].draw(0,0);
    ofPopMatrix();
}


void SessionController::next(){
    
    cout<<"this state"<<state<<endl;
    switch (state) {
            
        case STARTUP:
            setState(IDLE);
            videoplayer.forward();

            break;
            
            
        case IDLE:
            setState(ACTIVE_SESSION_START);
            resetWelcomeScreen();


            break;
            
        case ACTIVE_SESSION_START:
            if(screenInd<startScreens.size()-1){
                screenInd++;
            }else{
                resetWelcomeScreen();
                setState(ACTIVE_SESSION_RECORD);
            }
            break;
            
        case ACTIVE_SESSION_RECORD:
            videoplayer.forward();
            break;
            
        case ACTIVE_SESSION_END:
            // load new Story
            videoplayer.loadStory(story);
            resetWelcomeScreen();


            break;
            
        default:
            break;
    }
    
    
}


void SessionController::setState(int _state){
    state=_state;
}



void SessionController::resetWelcomeScreen(){
    screenInd=0;
    
}


void SessionController::makeNewSession(){
    int unixT=ofGetUnixTime();
    sessions = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
    sessions->loadFile("sessions.xml");
    recordingSession=unixT;

}