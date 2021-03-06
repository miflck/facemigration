//
//  SessionController.hpp
//  facemigration
//
//  Created by Flückiger Michael on 10.07.17.
//
//

#ifndef SessionController_hpp
#define SessionController_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Videoplayer.hpp"
#include "Videorecorder.hpp"



#define STARTUP 99

#define IDLE 100


#define ACTIVE_SESSION_START 110
#define ACTIVE_SESSION_RECORD 120
#define ACTIVE_SESSION_END 130


#define ACTIVE_SESSION_STARTSCREEN_1 201
#define ACTIVE_SESSION_STARTSCREEN_2 202
#define ACTIVE_SESSION_STARTSCREEN_3 203
#define ACTIVE_SESSION_STARTSCREEN_4 204
#define ACTIVE_SESSION_STARTSCREEN_5 205



struct startScreen{
    bool bHasImage;
    string path;
    bool bHasPreviewImage;
    bool bHasPreview;
    bool bHasFullImage;
    bool bHasVideo;
    int initVideoIndex;
    bool record;
    bool skip;
    ofImage img;
};


//for convenience
#define SC SessionController::getInstance()

class SessionController: public ofBaseApp{
public:
    static SessionController* getInstance();
    void initialize();
    bool isInitialized();
    virtual ~SessionController();
    
    void setup();
    void update();
    void draw();
    void makeNewSession();
    
    void clipIsDone();
    void setClipIsDone(bool _clipIsDone);
    void handleRecordSession();
    
    Videoplayer videoplayer;
    void setState(int _state);
    
    
    Videorecorder videorecorder;
    void close();
    void openvideorecorder();
    
    void startRecording();
    void stopRecording();
    void toggleRecording();
    bool getIsRecording();
    

// STORYHANDLING
    void next();

    void buttonPushed();
    void saveBackground();
    void startBlobTimeOut();
    void blobTimer();
    int nBlobsThreshold=0;
    
    
    bool debug=false;
    void reset();
    void resetState();

    
    
private:
    SessionController();
    static SessionController* instance;
    bool initialized;
    
    
    int story;

    
    
    ofPtr<ofxXmlSettings>	sessions;
    int recordingSession;
    int clipNumber;
    
    int state=STARTUP;


    ofPtr<ofxXmlSettings>	XMLStartScreens;

    vector<startScreen> startScreens;

    
    int screenInd=0;
    
    void drawInit();
    void resetWelcomeScreen();
    
    void handleInitScreens();
    void setInitToIdle();
    
    bool bIsClipDone=false;
    
    bool bIsRecording=false;
    
    
    
    float startTime; // store when we start time timer
    float endTime; // when do want to stop the timer
    
    bool  bTimerReached; // used as a trigger when we hit the timer
    
    

};

#endif /* SessionController_hpp */
