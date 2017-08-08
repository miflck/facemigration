//
//  Videorecorder.cpp
//  facemigration
//
//  Created by Flückiger Michael on 29.05.17.
//
//

#include "Videorecorder.hpp"

#ifndef OF_VIDEO_CAPTURE_QTKIT
#error This example requires OF_VIDEO_PLAYER_QTKIT to be defined.
#error For OS X versions > 10.6 and < 10.12 #define OF_VIDEO_PLAYER_QTKIT before including ofMain.h in main.cpp
#endif

Videorecorder::Videorecorder(){
}
//--------------------------------------------------------------

Videorecorder::~Videorecorder(){
}
//--------------------------------------------------------------


void Videorecorder::setup(){
    
    

    
    
    
    // 1. Create a new recorder object.  ofPtr will manage this
    // pointer for us, so no need to delete later.
    vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    
    // 2. Set our video grabber to use this source.
    vidGrabber.setGrabber(vidRecorder);
    
    // 3. Make lists of our audio and video devices.
    videoDevices = vidRecorder->listVideoDevices();
    audioDevices = vidRecorder->listAudioDevices();
    
    // 3a. Optionally add audio to the recording stream.
    // vidRecorder->setAudioDeviceID(2);
     vidRecorder->setUseAudio(true);
    
    // 4. Register for events so we'll know when videos finish saving.
    ofAddListener(vidRecorder->videoSavedEvent, this, &Videorecorder::videoSaved);
    
    // 4a.  If you would like to list available video codecs on your system,
    // uncomment the following code.
     vector<string> videoCodecs = vidRecorder->listVideoCodecs();
     for(size_t i = 0; i < videoCodecs.size(); i++){
         ofLogVerbose("Available Video Codecs") << videoCodecs[i];
     }
    
    // 4b. You can set a custom / non-default codec in the following ways if desired.
    //vidRecorder->setVideoCodec("QTCompressionOptionsJPEGVideo");
     vidRecorder->setVideoCodec(videoCodecs[0]);
    
    // 5. Initialize the grabber.
//    vidGrabber.setup(1920, 1080);
    vidGrabber.setup(1920, 1080);

    
    // If desired, you can disable the preview video.  This can
    // help help speed up recording and remove recording glitches.
    // vidRecorder->setupWithoutPreview();
    
    // 6. Initialize recording on the grabber.  Call initRecording()
    // once after you've initialized the grabber.
    vidRecorder->initRecording();
    
    // 7. If you'd like to launch the newly created video in Quicktime
    // you can enable it here.
    bLaunchInQuicktime = false;
    
    videoGrabberRect.set(0,0,1920/3,1080/3);
    previewWindow.set(0, 0, 1920/3,1080/3);
    fullwidth.set(0,0,1920,1080);
    
    
    
}
//--------------------------------------------------------------


void Videorecorder::update(){
    //if(!bIsPaused)vidGrabber.update();
    vidGrabber.update();
    
    if(recordedVideoPlayback.isLoaded()){
        recordedVideoPlayback.update();
    }
    
    
}
//--------------------------------------------------------------

void Videorecorder::draw(){
    
    
    // draw the preview if available
    if(vidRecorder->hasPreview()){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        ofPushMatrix();
        if(bIsFullscreen){
            videoGrabberRect.scaleTo(fullwidth);
            vidGrabber.draw(videoGrabberRect);
        }
         if(bHasPreview){
            videoGrabberRect.scaleTo(previewWindow);
            vidGrabber.draw(videoGrabberRect);
        }
        
        ofPopMatrix();
        ofPopStyle();
    }
    
  //  if(bHasPreview){
    ofPushStyle();
    ofFill();
    if(vidRecorder->isRecording()){
        //make a nice flashy red record color
        int flashRed = powf(1 - (sin(ofGetElapsedTimef()*10)*.5+.5),2)*255;
        ofSetColor(255, 255-flashRed, 255-flashRed);
        ofDrawCircle(previewWindow.getWidth()/2, previewWindow.getHeight()/2, 20);

    }
    ofPopStyle();
 
    
}

//--------------------------------------------------------------
void Videorecorder::videoSaved(ofVideoSavedEventArgs& e){
    sessions = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
    sessions->loadFile("sessions.xml");
    
    recordingSession=sessions->getNumTags("RECORDINGSESSION");
    cout<<"-------------- recordingSession "<<recordingSession<<endl;
    if( sessions->pushTag("RECORDINGSESSION", recordingSession-1) ){
        int tagNum = sessions->addTag("CLIP");
        sessions->setValue("CLIP:Number",  tagNum,tagNum);
        sessions->setValue("CLIP:Filename",  myFileName,tagNum);
        sessions->popTag();
        sessions->saveFile("sessions.xml");
        
    }
    
    
    if(e.error.empty()){
        cout<<"-------------- SAVED ----------------"<<endl;
    }
    else {
        ofLogError("videoSavedEvent") << "Video save error: " << e.error;
    }
}

//--------------------------------------------------------------

void Videorecorder::startRecording(){
    
    
    sessions = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
    sessions->loadFile("sessions.xml");
    recordingSession=sessions->getNumTags("RECORDINGSESSION")-1;
    cout<<"Recordingsession "<<recordingSession<<" "<<clipNumber<<endl;
    
    
    sessions->pushTag("RECORDINGSESSION", recordingSession);
    clipNumber=sessions->getNumTags("CLIP");
    sessions->popTag();
    sessions->saveFile("sessions.xml");
    
    cout<<"Recordingsession "<<recordingSession<<" "<<clipNumber<<endl;
    
    myFileName="Recodings/MyMovieFile_"+ofToString(recordingSession)+"_"+ofToString(clipNumber)+".mov";
    vidRecorder->startRecording(myFileName);
    bIsPaused=true;
    
 
}
//--------------------------------------------------------------
void Videorecorder::stopRecording(){
    if(vidRecorder->isRecording()){
        vidRecorder->stopRecording();
    }

}
//--------------------------------------------------------------
void Videorecorder::toggleRecording(){
    //if it is recording, stop
    if(vidRecorder->isRecording()){
        vidRecorder->stopRecording();
    }
    else {
        // otherwise start a new recording.
        // before starting, make sure that the video file
        // is already in use by us (i.e. being played), or else
        // we won't be able to record over it.
        if(recordedVideoPlayback.isLoaded()){
            recordedVideoPlayback.close();
        }
        
        sessions = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
        sessions->loadFile("sessions.xml");
        recordingSession=sessions->getNumTags("RECORDINGSESSION")-1;
        cout<<"Recordingsession "<<recordingSession<<" "<<clipNumber<<endl;


        sessions->pushTag("RECORDINGSESSION", recordingSession);
        clipNumber=sessions->getNumTags("CLIP");
        sessions->popTag();
        sessions->saveFile("sessions.xml");
        
        cout<<"Recordingsession "<<recordingSession<<" "<<clipNumber<<endl;
      
        myFileName="Recodings/MyMovieFile_"+ofToString(recordingSession)+"_"+ofToString(clipNumber)+".mov";
        vidRecorder->startRecording(myFileName);
        bIsPaused=true;
    }
    
    
}
//--------------------------------------------------------------



void Videorecorder::pauseRecording(bool p){
    bIsPaused=p;
}


void Videorecorder::setFullscreen(bool _fullscreen){
    bIsFullscreen=_fullscreen;
}

void Videorecorder::setPreview(bool _preview){
    bHasPreview=_preview;
}



