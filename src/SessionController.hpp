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
    
    
    
    Videoplayer videoplayer;
    void setState(int _state);
    
    
    Videorecorder videorecorder;

// STORYHANDLING
    void next();

    
    
private:
    SessionController();
    static SessionController* instance;
    bool initialized;
    
    
    int story;

    
    
    ofPtr<ofxXmlSettings>	sessions;
    int recordingSession;
    int clipNumber;
    
    int state=STARTUP;


    
    vector<ofImage> startScreens;
    int screenInd=0;
    
    void drawInit();
    void resetWelcomeScreen();
    

};

#endif /* SessionController_hpp */
