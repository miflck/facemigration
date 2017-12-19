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
#include "ofxOpenCv.h"




class Videorecorder: public ofBaseApp{
    
public:
    
    Videorecorder();
    virtual ~Videorecorder();
    
    void setup();
    void update();
    void draw();
    
    
    
    ofVideoGrabber 			vidGrabber;
    
   // ofVideoGrabber 			cvGrabber;

    
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
    void setBigPreview(bool _preview);
    
    
    void saveBackground();


    ofxCvColorImage			colorImg;
    
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    ofxCvContourFinder 	contourFinder;
    
    int 				threshold;
    bool				bLearnBakground;
    bool debug=false;
    
private:
    
    
    int grabberWidth=1920;
    int grabberHeight=1080;
    
  //   int grabberWidth=1280;
  //   int grabberHeight=720;
    
    ofPtr<ofxXmlSettings>	sessions;
    int recordingSession;
    
    int clipNumber;
    
    int lastSessionNumber;
    string myFileName;
    bool bIsPaused;
    
    bool bIsFullscreen=false;
    bool bHasPreview=false;
    bool bHasBigPreview=false;

    ofRectangle videoGrabberRect;
    ofRectangle previewWindow;
    ofRectangle fullwidth;
    ofRectangle bigpreview;
    
    ofRectangle recordRect;



};

#endif /* Videorecorder_hpp */
