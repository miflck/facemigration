//
//  Videorecorder.hpp
//  facemigration
//
//  Created by Flückiger Michael on 29.05.17.
//
//

#ifndef Videorecorder_hpp
#define Videorecorder_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxXmlSettings.h"




class Videorecorder: public ofBaseApp{
    
public:
    
    Videorecorder();
    virtual ~Videorecorder();
    
    void setup();
    void update();
    void draw();
    
    
    
    ofVideoGrabber 			vidGrabber;
    ofPtr<ofQTKitGrabber>	vidRecorder;
    
    ofVideoPlayer recordedVideoPlayback;
    
    void videoSaved(ofVideoSavedEventArgs& e);
    
    vector<string> videoDevices;
    vector<string> audioDevices;
    
    bool bLaunchInQuicktime;
    
    
    void startRecording();
    void stopRecording();
    void toggleRecording();
    void pauseRecording(bool p);
    
    void setFullscreen(bool _fullscreen);
    void setPreview(bool _preview);


    
private:
    
    ofPtr<ofxXmlSettings>	sessions;
    int recordingSession;
    int clipNumber;
    
    int lastSessionNumber;
    string myFileName;
    bool bIsPaused;
    
    bool bIsFullscreen=false;
    bool bHasPreview=false;

    ofRectangle videoGrabberRect;
    ofRectangle previewWindow;
    ofRectangle fullwidth;



};

#endif /* Videorecorder_hpp */
