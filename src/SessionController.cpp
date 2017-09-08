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
    cout<<"Startscreens "<<num<<endl;
    for(int i = 0; i < num; i++){
        XMLStartScreens->pushTag("SCREEN", i);
        bool hasImage=XMLStartScreens->getValue("HASIMAGE", 0);
        string myPath = XMLStartScreens->getValue("PATH", "");
        bool preview = XMLStartScreens->getValue("PREVIEW", 0);
        bool previewimage = XMLStartScreens->getValue("PREVIEWIMAGE", 0);
        bool fullscreen = XMLStartScreens->getValue("FULLSCREEN", 0);
        bool hasVideo=XMLStartScreens->getValue("HASVIDEO", 0);
        int videoIndex=XMLStartScreens->getValue("VIDEO", -1);
        bool record=XMLStartScreens->getValue("RECORD", 0);
        bool skip=XMLStartScreens->getValue("SKIP", 1);
        
        
        cout<<"PATH "<<myPath<<endl;

        startScreen s;
        s.bHasImage=hasImage;
        s.path=myPath;
        s.bHasPreview=preview;
        s.bHasPreviewImage=previewimage;
        s.bHasFullImage=fullscreen;
        s.bHasVideo=hasVideo;
        s.initVideoIndex=videoIndex;
        s.record=record;
        s.skip=skip;
        XMLStartScreens->popTag();
        startScreens.push_back(s);
        startScreens.back().img.load(myPath);

    }
    
    
    XMLStartScreens->popTag();

    
    
    /*for(int i=0;i<startScreens.size();i++){
     cout<<"------------------"<<startScreens[i].path<<" "<<startScreens[i].bHasFullImage<<endl;
     }
     */
    
    
    
    
    
    sessions = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
    
    sessions->loadFile("sessions.xml");
    recordingSession=sessions->getNumTags("RECORDINGSESSION");
    cout<<"Session: "<<recordingSession<<endl;
    videorecorder.setup();
    
    
    
    
    
    //  videoplayer.loadStory(story);
    setState(STARTUP);
    
    bTimerReached = true;
    startTime = ofGetElapsedTimeMillis();  // get the start time
    endTime = 5000; // in milliseconds
}

void SessionController::update(){
    
    switch (state) {
        case STARTUP:
            makeNewSession();
            videoplayer.loadStory(story);
            setState(IDLE);
            resetWelcomeScreen();
            setInitToIdle();
            break;
            
        case IDLE:
            //setState(ACTIVE_SESSION_START);
        
            
            
            videoplayer.update();
            break;
            
        case ACTIVE_SESSION_START:
            videoplayer.update();
            videorecorder.update();
            //if(videorecorder.contourFinder.nBlobs < nBlobsThreshold)startBlobTimeOut();
            blobTimer();
            break;
            
        case ACTIVE_SESSION_RECORD:
            videorecorder.update();
            videoplayer.update();
            blobTimer();
          //  if(videorecorder.contourFinder.nBlobs < nBlobsThreshold)startBlobTimeOut();
            break;
            
        case ACTIVE_SESSION_END:
            setState(STARTUP);

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
             drawInit();
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
    
    
    if(debug){
        ofPushStyle();
        // finally, a report:
        ofSetColor(255,0,0);
        stringstream reportStr;
        reportStr
        << "num blobs found " << videorecorder.contourFinder.nBlobs<< ", fps: " << ofGetFrameRate();
        ofDrawBitmapString(reportStr.str(), 20, 600);
        ofPopStyle();
    
    
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
    switch (state) {
        case STARTUP:
            makeNewSession();
            videoplayer.loadStory(story);
            setState(IDLE);
            break;
            
        case IDLE:
            setState(ACTIVE_SESSION_START);
            //resetWelcomeScreen();
            //setInitToIdle();
            handleInitScreens();

            break;
            
        case ACTIVE_SESSION_START:
            handleInitScreens();
            break;
            
        case ACTIVE_SESSION_RECORD:
            handleRecordSession();
            break;
            
        case ACTIVE_SESSION_END:
              reset();
            break;
            
        default:
            break;
    }
    
    
}


void SessionController::buttonPushed(){
    switch (state) {
        case STARTUP:
            makeNewSession();
            videoplayer.loadStory(story);
            setState(IDLE);
            break;
            
        case IDLE:
            setState(ACTIVE_SESSION_START);
            handleInitScreens();
            break;
            
        case ACTIVE_SESSION_START:
            if(!startScreens[screenInd].skip)return;
            handleInitScreens();
            break;
            
        case ACTIVE_SESSION_RECORD:
            handleRecordSession();
            break;
            
        case ACTIVE_SESSION_END:
            reset();
            break;
            
        default:
            break;
    }


}


void SessionController::setInitToIdle(){
    cout<<"Set init to Idle"<<endl;
    //videoplayer.setInitVideo(0);
    videoplayer.showVideo(false);
}



void SessionController::handleInitScreens(){
    cout<<"Handle init Screens"<<endl;
    stopRecording();

    if(screenInd<startScreens.size()-1){
        screenInd++;
        cout<<"handle init screens "<<startScreens.size()-1<<" "<<screenInd<<" has video "<<startScreens[screenInd].bHasVideo<<endl;

        if(startScreens[screenInd].bHasVideo){
            cout<<"I have a video"<<endl;
            videoplayer.setInitVideo(startScreens[screenInd].initVideoIndex);
            videoplayer.showVideo(true);
        }else{
            videoplayer.stop();
            videoplayer.showVideo(false);
        }
        
        if(startScreens[screenInd].bHasPreview){
            videorecorder.setFullscreen(startScreens[screenInd].bHasFullImage);
            videorecorder.setPreview(startScreens[screenInd].bHasPreviewImage);
        }
        
        if(startScreens[screenInd].record){
            startRecording();
        }
    }
    else if(screenInd>=startScreens.size()-1){
        setState(ACTIVE_SESSION_RECORD);
        videoplayer.setVideo(0);
        videorecorder.setFullscreen(false);
        videorecorder.setPreview(true);
    }
    
}


void SessionController::setState(int _state){
    state=_state;
    videorecorder.setFullscreen(false);
    videorecorder.setPreview(false);
}



void SessionController::resetWelcomeScreen(){
    screenInd=0;
    videorecorder.setFullscreen(false);
    videorecorder.setBigPreview(false);
    videorecorder.setPreview(false);
}

void SessionController::clipIsDone(){
    bIsClipDone=true;
    cout<<"Is done "<<bIsClipDone<<endl;
}

void SessionController::setClipIsDone(bool _clipIsDone){
    bIsClipDone=_clipIsDone;
    cout<<"Is done "<<bIsClipDone<<endl;
    switch (state) {
        case STARTUP:
            break;
            
        case IDLE:
            break;
            
        case ACTIVE_SESSION_START:
            next();
            break;
            
        case ACTIVE_SESSION_RECORD:
            break;
            
        case ACTIVE_SESSION_END:
            setState(STARTUP);
            break;
    }
}



void SessionController::handleRecordSession(){
    if(bIsClipDone){

    stopRecording();
    if(videoplayer.getVideoIndex()>videoplayer.getNumberOfVideos()-6){
      videorecorder.setBigPreview(true);
        cout<<"set Big Preview"<<endl;
    }

        videoplayer.forward();
    }

    
}


void SessionController::makeNewSession(){
    int unixT=ofGetUnixTime();
    sessions = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
    sessions->loadFile("sessions.xml");
    recordingSession=unixT;
    
    int recordingSession=sessions->getNumTags("RECORDINGSESSION");
    cout<<"Number of Sessions"<<recordingSession<<endl;
    
    int session=sessions->addTag("RECORDINGSESSION");
    sessions->pushTag("RECORDINGSESSION", session);
    int tagNum = sessions->addTag("SESSIONID");
    sessions->setValue("SESSIONID",  ofGetTimestampString("%Y%m%d%H%M%S%i") ,tagNum);
    sessions->setValue("SESSION_TIME",  ofGetTimestampString() ,tagNum);
    sessions->setValue("SESSION_NUMBER",session,tagNum);
    
    
    sessions->popTag();
    sessions->saveFile("sessions.xml");
    
    
}


void SessionController::startRecording(){
    cout<<"is recording "<<bIsRecording<<endl;
    if(!bIsRecording){
        cout<<"start recording"<<endl;
        videorecorder.startRecording();
        bIsRecording=true;
    }
}
void SessionController::stopRecording(){
    if(bIsRecording){
        videorecorder.stopRecording();
        bIsRecording=false;
    }
}
void SessionController::toggleRecording(){
    videorecorder.toggleRecording();
}

bool SessionController::getIsRecording(){
    return bIsRecording;
}

void SessionController::saveBackground(){
    videorecorder.saveBackground();

}

void SessionController::startBlobTimeOut(){
  /*  if(bTimerReached){
    bTimerReached = false;                     // reset the timer
    startTime = ofGetElapsedTimeMillis();  // get the start time
    endTime = 5000; // in milliseconds
    }*/
}
void SessionController::blobTimer(){
    // update the timer this frame
    
    
    if(videorecorder.contourFinder.nBlobs > nBlobsThreshold){
    startTime = ofGetElapsedTimeMillis();
        bTimerReached=false;
    }
    
    float timer = ofGetElapsedTimeMillis() - startTime;
    if(timer >= endTime && !bTimerReached ) {
        bTimerReached = true;
        ofMessage msg("Timer Reached");
        ofSendMessage(msg);
        reset();
    }
}


void SessionController::reset(){
    cout<<"RESET"<<endl;
    videorecorder.setup();
    setState(STARTUP);

}
