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


void Videorecorder::setup(ofPtr<ofxXmlSettings> _XMLclips){
    
    XMLclips=_XMLclips;
    
    
    //  XMLclips->addTag("RECORDINGSESSION");
    // XMLclips->saveFile("clips.xml");
    
    recordingSession=XMLclips->getNumTags("RECORDINGSESSION");
    cout<<" XMLclipsSession: "<<recordingSession<<endl;
    //  clipNumber=XMLclips->getNumTags("RECORDINGSESSION:CLIP");
    
    
    
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
    // vidRecorder->setUseAudio(true);
    
    // 4. Register for events so we'll know when videos finish saving.
    ofAddListener(vidRecorder->videoSavedEvent, this, &Videorecorder::videoSaved);
    
    // 4a.  If you would like to list available video codecs on your system,
    // uncomment the following code.
    // vector<string> videoCodecs = vidRecorder->listVideoCodecs();
    // for(size_t i = 0; i < videoCodecs.size(); i++){
    //     ofLogVerbose("Available Video Codecs") << videoCodecs[i];
    // }
    
    // 4b. You can set a custom / non-default codec in the following ways if desired.
    // vidRecorder->setVideoCodec("QTCompressionOptionsJPEGVideo");
    // vidRecorder->setVideoCodec(videoCodecs[2]);
    
    // 5. Initialize the grabber.
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
    
    videoGrabberRect.set(0,0,vidGrabber.getWidth()/3,vidGrabber.getHeight()/3);
    previewWindow.set(20, 20, vidGrabber.getWidth()/3,vidGrabber.getHeight()/3);
    fullwidth.set(0,0,vidGrabber.getWidth(),vidGrabber.getHeight());
    
    
    
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
    
    
    
    // draw the background boxes
    ofPushStyle();
    ofSetColor(0);
    ofFill();
    ofDrawRectangle(previewWindow);
    ofPopStyle();
    
    // draw the preview if available
    if(vidRecorder->hasPreview()){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        ofPushMatrix();
        if(isSmall){
            videoGrabberRect.scaleTo(previewWindow);
            vidGrabber.draw(videoGrabberRect);
        } else {
            videoGrabberRect.scaleTo(fullwidth);
            vidGrabber.draw(videoGrabberRect);
        }
        ofPopMatrix();
        ofPopStyle();
        
        
    }
    
        
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(3);
    if(vidRecorder->isRecording()){
        //make a nice flashy red record color
        int flashRed = powf(1 - (sin(ofGetElapsedTimef()*10)*.5+.5),2)*255;
        ofSetColor(255, 255-flashRed, 255-flashRed);
    }
    else{
        ofSetColor(255,80);
    }
    ofDrawRectangle(previewWindow);
    ofPopStyle();
    
    
    //draw instructions
    ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString("' ' space bar to toggle recording", 680, 540);
    ofDrawBitmapString("'v' switches video device", 680, 560);
    ofDrawBitmapString("'a' switches audio device", 680, 580);
    
    //draw video device selection
    ofDrawBitmapString("VIDEO DEVICE", 20, 540);
    for(int i = 0; i < videoDevices.size(); i++){
        if(i == vidRecorder->getVideoDeviceID()){
            ofSetColor(255, 100, 100);
        }
        else{
            ofSetColor(255);
        }
        ofDrawBitmapString(videoDevices[i], 20, 560+i*20);
    }
    
    //draw audio device;
    int startY = 580+20*videoDevices.size();
    ofDrawBitmapString("AUDIO DEVICE", 20, startY);
    startY += 20;
    for(int i = 0; i < audioDevices.size(); i++){
        if(i == vidRecorder->getAudioDeviceID()){
            ofSetColor(255, 100, 100);
        }
        else{
            ofSetColor(255);
        }
        ofDrawBitmapString(audioDevices[i], 20, startY+i*20);
    }
    ofPopStyle();
    
}

//--------------------------------------------------------------
void Videorecorder::videoSaved(ofVideoSavedEventArgs& e){
    // the ofQTKitGrabber sends a message with the file name and any errors when the video is done recording
    
    //  XMLclips.addTag("");
    
    
    // lastSessionNumber	= XMLclips->addTag("SESSION");
    recordingSession=XMLclips->getNumTags("RECORDINGSESSION")-1;
    cout<<"--------------recordingSession "<<recordingSession<<endl;
    if( XMLclips->pushTag("RECORDINGSESSION", recordingSession) ){
        int tagNum = XMLclips->addTag("CLIP");
        XMLclips->setValue("CLIP:Number",  tagNum,tagNum);
        XMLclips->setValue("CLIP:Filename",  myFileName,tagNum);
        
        XMLclips->popTag();
        XMLclips->saveFile("clips.xml");
        
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
}
//--------------------------------------------------------------
void Videorecorder::stopRecording(){
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
        recordingSession=XMLclips->getNumTags("RECORDINGSESSION")-1;
        XMLclips->pushTag("RECORDINGSESSION", recordingSession);
        clipNumber=XMLclips->getNumTags("CLIP");
        XMLclips->popTag();
        XMLclips->saveFile("clips.xml");
        
        cout<<"Recordingsession "<<recordingSession<<" "<<clipNumber<<endl;
        
        myFileName="Recodings/MyMovieFile_"+ofToString(recordingSession)+"_"+ofToString(clipNumber)+".mov";
        vidRecorder->startRecording(myFileName);
        bIsPaused=true;
    }
    
    
}
//--------------------------------------------------------------


void Videorecorder::addRecordingSession(){
    
    
    int session=XMLclips->addTag("RECORDINGSESSION");
    XMLclips->pushTag("RECORDINGSESSION", session);
    int tagNum = XMLclips->addTag("SESSIONID");
    XMLclips->setValue("SESSIONID",  ofGetTimestampString("%Y%m%d%H%M%S%i") ,tagNum);
    XMLclips->setValue("SESSION_TIME",  ofGetTimestampString() ,tagNum);
    XMLclips->setValue("SESSION_NUMBER",session,tagNum);
    
    
    XMLclips->popTag();
    XMLclips->saveFile("clips.xml");
    
    
}
void Videorecorder::pauseRecording(bool p){
    bIsPaused=p;
}




