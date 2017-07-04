#include "ofApp.h"






//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    recordedClips = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );

    recordedClips->loadFile("clips.xml");
    recordingSession=recordedClips->getNumTags("RECORDINGSESSION");
    cout<<"Session: "<<recordingSession<<endl;
    videorecorder.setup(recordedClips);
    videoplayer.setup();

    story=0;

}

//--------------------------------------------------------------
void ofApp::update(){
    videorecorder.update();
    videoplayer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    videorecorder.draw();
    videoplayer.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        videorecorder.toggleRecording();
    }
    
    //no data gets saved unless you hit the s key
    if(key == 's'){
        recordedClips->saveFile("clips.xml");
    }

    
    if(key=='S'){
        videorecorder.addRecordingSession();
    }
    
    if(key=='r'){
        videorecorder.pauseRecording(false);
    }
    
    if(key=='0'){
       // videoplayer.setState(0);
    }
    if(key=='1'){
        //videoplayer.setState(1);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key=='r'){
        videorecorder.pauseRecording(true);
    }
    
    
    
    if(key=='l'){
        videoplayer.loadStory(story);
        story++;
    }
    
    if(key=='n'){
        videoplayer.forward();
    }
    
    if(isdigit(key)) {
        
     
        char c = (char) key;
        int i = c - '0';
        
        cout<<"+++++++++++++++ "<<videoplayer.getNumberOfVideos()<<endl;
        
        videoplayer.setVideo(i);
    }
    

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
