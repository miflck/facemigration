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
        bool hasImage=XMLStartScreens->getValue("HASIMAGE", 0);
        string myPath = XMLStartScreens->getValue("PATH", "");
        bool preview = XMLStartScreens->getValue("PREVIEW", 0);
        bool previewimage = XMLStartScreens->getValue("PREVIEWIMAGE", 0);
        bool fullscreen = XMLStartScreens->getValue("FULLSCREEN", 0);
        bool hasVideo=XMLStartScreens->getValue("HASVIDEO", 0);
        int videoIndex=XMLStartScreens->getValue("VIDEO", -1);


        startScreen s;
        s.bHasImage=hasImage;
        s.path=myPath;
        s.bHasVideo=preview;
        s.bHasPreviewImage=previewimage;
        s.bHasFullImage=fullscreen;
        s.bHasVideo=hasVideo;
        s.initVideoIndex=videoIndex;
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


  //  videoplayer.loadStory(story);

    

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
    
    cout<<"this state "<<state<<endl;
    switch (state) {
            
        case STARTUP:
            videoplayer.loadStory(story);
            setState(IDLE);
            break;
            
            
        case IDLE:
            setState(ACTIVE_SESSION_START);
            resetWelcomeScreen();
            break;
            
        case ACTIVE_SESSION_START:
            handleInitScreens();
            break;
            
        case ACTIVE_SESSION_RECORD:
            videoplayer.forward();
            break;
            
        case ACTIVE_SESSION_END:
                setState(IDLE);
            break;
            
        default:
            break;
    }
    
    
}


void SessionController::handleInitScreens(){
    
    if(screenInd<startScreens.size()-1){
        screenInd++;
        
       if(startScreens[screenInd].bHasVideo){
            videoplayer.setInitVideo(startScreens[screenInd].initVideoIndex);
        }
        
        videorecorder.setFullscreen(startScreens[screenInd].bHasFullImage);
        videorecorder.setPreview(startScreens[screenInd].bHasPreviewImage);
        
        
        
        
    }else{
        setState(ACTIVE_SESSION_RECORD);
        videoplayer.setVideo(0);
        videorecorder.setFullscreen(false);
        videorecorder.setPreview(true);
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


//--------------------------------------------------------------
void SessionController::gotMessage(ofMessage msg){
    cout<<"********** msg ********* "<<msg.message<<endl;
    
}
