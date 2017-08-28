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
    
     grabberWidth=1920;
     grabberHeight=1080;
    

    
    
    
    // 1. Create a new recorder object.  ofPtr will manage this
    // pointer for us, so no need to delete later.
    vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    vidRecorder->setup(1920, 1080);

    // 2. Set our video grabber to use this source.
    vidGrabber.setGrabber(vidRecorder);
    
    // 3. Make lists of our audio and video devices.
    videoDevices = vidRecorder->listVideoDevices();
    audioDevices = vidRecorder->listAudioDevices();
    
    for(size_t i = 0; i < audioDevices.size(); i++){
   //     ofLogVerbose("Available Audio Devices") << audioDevices[i];
    }
    
    // 3a. Optionally add audio to the recording stream.
     vidRecorder->setAudioDeviceID(1);
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
    // vidRecorder->setVideoCodec(videoCodecs[0]);
    
    // 5. Initialize the grabber.
    
    vidGrabber.setup(grabberWidth, grabberHeight);
   // vidGrabber.setup(grabberWidth, grabberHeight);
 //   cvGrabber.setup(grabberWidth/3, grabberHeight/3);

    
    // If desired, you can disable the preview video.  This can
    // help help speed up recording and remove recording glitches.
     //vidRecorder->setupWithoutPreview();
    
    // 6. Initialize recording on the grabber.  Call initRecording()
    // once after you've initialized the grabber.
    vidRecorder->initRecording();
    
    // 7. If you'd like to launch the newly created video in Quicktime
    // you can enable it here.
    bLaunchInQuicktime = false;
    
    
 
    
    videoGrabberRect.set(0,0,grabberWidth/2,grabberHeight/2);
    previewWindow.set(10,10, grabberWidth/3,grabberHeight/3);
    bigpreview.set(0,0,grabberWidth/2,grabberHeight/2);
    fullwidth.set(0,0,grabberWidth,grabberHeight);
    
    recordRect.set(5, 5, grabberWidth/3+10,grabberHeight/3+10);
    
    colorImg.allocate(grabberWidth,grabberHeight);
    
    grayImage.allocate(grabberWidth/3,grabberHeight/3);
    grayBg.allocate(grabberWidth/3,grabberHeight/3);
    grayDiff.allocate(grabberWidth/3,grabberHeight/3);
    
    bLearnBakground = false;
    threshold = 80;
    
    
    //load background;
    ofImage fileImage;
    fileImage.loadImage("background.jpg");
    grayBg.setFromPixels(fileImage.getPixels());
    
    
    
}
//--------------------------------------------------------------


void Videorecorder::update(){
    //if(!bIsPaused)vidGrabber.update();
    
    
    bool bNewFrame = false;

    
    vidGrabber.update();
   /* cvGrabber.update();
    bNewFrame = cvGrabber.isFrameNew();

    if (bNewFrame){
        grayImage.resetROI();

        colorImg.setFromPixels(cvGrabber.getPixels());
        grayImage = colorImg;
        
        int roiY=grabberHeight/3/3*2;
        int roiX=grabberWidth/3/3*2;

        grayImage.setROI(roiX,0,grabberWidth/3-roiX,grabberHeight/3);
        grayImage.blur(21);
        grayImage.threshold(threshold);
        grayImage.invert();
        grayImage.dilate();
        grayImage.erode();
        
        if (bLearnBakground == true){
            grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
            bLearnBakground = false;
        }
        
        // take the abs value of the difference between background and incoming and then threshold:

       /* grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);
        grayDiff.dilate();
        grayDiff.erode();
        */

        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
       // contourFinder.findContours(grayDiff, 20, (grabberWidth/2*grabberHeight/2)/3, 10, true);	// find holes
        
     //   contourFinder.findContours(grayImage, 20, (grabberWidth/2*grabberHeight/2)/3, 10, true);	// find holes

        
    //}

    
    
}
//--------------------------------------------------------------

void Videorecorder::draw(){
    
    
    
    if(vidRecorder->isRecording()){
        ofPushStyle();
        //make a nice flashy red record color
        int flashRed = powf(1 - (sin(ofGetElapsedTimef()*10)*.5+.5),2)*255;
        ofSetColor(255, 255-flashRed, 255-flashRed);
        ofDrawCircle(previewWindow.getWidth()/2, previewWindow.getHeight()/2, 20);
        ofDrawRectangle(recordRect);
        ofPopStyle();
        
    }

    
    
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
        
        if(bHasBigPreview){
            videoGrabberRect.scaleTo(bigpreview);
            vidGrabber.draw(videoGrabberRect);
        }
        ofPopMatrix();
        ofPopStyle();
    }
    
    
    if(debug){
    grayImage.draw(0,0);
    ofPushStyle();
    ofFill();
    ofSetColor(255,100);
    ofPushMatrix();
    ofTranslate(grabberWidth/3/3*2,0);
    ofSetColor(255,0,0);
       for (int i = 0; i < contourFinder.nBlobs; i++){
         contourFinder.blobs[i].draw(0,0);
    }
    ofPopMatrix();
    ofPopStyle();
    }
    
    
    /*
    ofPushStyle();
    ofFill();
    if(vidRecorder->isRecording()){
        //make a nice flashy red record color
        int flashRed = powf(1 - (sin(ofGetElapsedTimef()*10)*.5+.5),2)*255;
        ofSetColor(255, 255-flashRed, 255-flashRed);
        ofDrawCircle(previewWindow.getWidth()/2, previewWindow.getHeight()/2, 20);
    }
    ofPopStyle();
 */
    
}

//--------------------------------------------------------------
void Videorecorder::videoSaved(ofVideoSavedEventArgs& e){
    sessions = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );
    sessions->loadFile("sessions.xml");
    
    recordingSession=sessions->getNumTags("RECORDINGSESSION");
    if( sessions->pushTag("RECORDINGSESSION", recordingSession-1) ){
        int tagNum = sessions->addTag("CLIP");
        sessions->setValue("CLIP:Number",  tagNum,tagNum);
        sessions->setValue("CLIP:Filename",  myFileName,tagNum);
        sessions->popTag();
        sessions->saveFile("sessions.xml");
        
    }
    
    
    if(e.error.empty()){
        cout<<"-------------- SAVED "<<myFileName<< "----------------"<<endl;
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
   // cout<<"Recordingsession "<<recordingSession<<" "<<clipNumber<<endl;
    
    
    sessions->pushTag("RECORDINGSESSION", recordingSession);
    clipNumber=sessions->getNumTags("CLIP");
    sessions->popTag();
    sessions->saveFile("sessions.xml");
    
    
    myFileName="Recodings/"+ofToString(recordingSession)+"/Session_"+ofToString(recordingSession)+"_Clip_"+ofToString(clipNumber)+".mov";
    cout<<"Recordingsession "<<recordingSession<<" "<<clipNumber<<" "<<myFileName<<endl;

    vidRecorder->startRecording(myFileName);
   // bIsPaused=true;
    
 
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

void Videorecorder::setBigPreview(bool _bigpreview){
    cout<<"set big preview "<<_bigpreview<<endl;
    bHasBigPreview=_bigpreview;

}

void Videorecorder::saveBackground(){
    colorImg.setFromPixels(vidGrabber.getPixels());
    colorImg.resize(grabberWidth/3,grabberHeight/3);
    grayImage = colorImg;
    grayImage.blur(19);
    grayBg = grayImage;
ofSaveImage(grayBg.getPixels(),"background.jpg");

}


