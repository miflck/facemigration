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

    
    XMLStartScreens = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
    XMLStartScreens->loadFile("startscreens.xml");
    XMLStartScreens->pushTag("STARTSCREENS");
    
    int num = XMLStartScreens->getNumTags("SCREEN");
    cout<<"Startscreens"<<num<<endl;
    for(int i = 0; i < num; i++){
        XMLStartScreens->pushTag("SCREEN", i);
        string myPath = XMLStartScreens->getValue("PATH", "");
        bool preview = XMLStartScreens->getValue("PREVIEW", 0);
        bool previewimage = XMLStartScreens->getValue("PREVIEWIMAGE", 0);
        bool fullscreen = XMLStartScreens->getValue("FULLSCREEN", 0);
        startScreen s;
        s.path=myPath;
        s.bHasVideo=preview;
        s.bHasPreviewImage=previewimage;
        s.bHasFullImage=fullscreen;
        XMLStartScreens->popTag();
        startScreens.push_back(s);
        startScreens.back().img.load(myPath);
    }
    
    XMLStartScreens->popTag();

    
    
    
    for(int i=0;i<startScreens.size();i++){
        cout<<"------------------"<<startScreens[i].path<<" "<<startScreens[i].bHasFullImage<<endl;
    }
    
    
    
    
    
    
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
            videoplayer.update();
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
            videoplayer.draw();
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
    
    
    startScreens[screenInd].img.draw(0,0);

    
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
                
                
                videorecorder.setFullscreen(startScreens[screenInd].bHasFullImage);
                videorecorder.setPreview(startScreens[screenInd].bHasPreviewImage);
                
            }else{
                resetWelcomeScreen();
                setState(ACTIVE_SESSION_RECORD);
                videorecorder.setFullscreen(false);
                videorecorder.setPreview(true);
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