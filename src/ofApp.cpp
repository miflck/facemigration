#include "ofApp.h"
#include "SessionController.hpp"






//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(255);
    
    /*
    recordedClips = ofPtr<ofxXmlSettings>( new ofxXmlSettings() );

    recordedClips->loadFile("clips.xml");
    recordingSession=recordedClips->getNumTags("RECORDINGSESSION");
    cout<<"Session: "<<recordingSession<<endl;
    
    
    videorecorder.setup(recordedClips);
    videoplayer.setup();

    story=0;
    
    ofImage img;
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen.jpg");
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen2.png");
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen3.jpg");
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen4.jpg");
    startScreens.push_back(ofImage(img));
    startScreens.back().load("startscreens/startscreen5.jpg");
    */
    
    SC->initialize();
    SC->setup();
    
    
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    // this should be set to whatever com port your serial device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
    // arduino users check in arduino app....
    int baud = 9600;//115200;
    serial.setup(0, baud); //open the first device
    
   // serial.setup("/dev/tty.usbserial-A70060V8", 9600);
    serial.startContinuousRead();
    ofAddListener(serial.NEW_MESSAGE,this,&ofApp::onNewMessage);
    
    message = "";
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(requestRead)
    {
        serial.sendRequest();
        requestRead = false;
    }
    
    SC->update();

   /* switch (state) {
        case IDLE:
            
            break;
        
        case ACTIVE_SESSION_START:
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
    }*/
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    SC->draw();

  /*  switch (state) {
        case IDLE:
            
            break;
            
        case ACTIVE_SESSION_START:
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
    }*/

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
   SC->videorecorder.toggleRecording();
    }

    //no data gets saved unless you hit the s key
    if(key == 's'){
     //   recordedClips->saveFile("clips.xml");
    }

    
    if(key=='s'){
        SC->makeNewSession();
    }
    
    if(key=='r'){
       // videorecorder.pauseRecording(false);
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
      //  videorecorder.pauseRecording(true);
    }
    
    
    
    if(key=='l'){
       // videoplayer.loadStory(story);
       // story++;
    }
    
    if(key=='n'){
        //videoplayer.forward();
        //next();
        
        SC->next();
        
    }
    
    
    if(key=='b'){
        //screenInd++;
    }
    
   /*
    if(isdigit(key)) {
        
     
        char c = (char) key;
        int i = c - '0';
        
        cout<<"+++++++++++++++ "<<videoplayer.getNumberOfVideos()<<endl;
        
        videoplayer.setVideo(i);
    }*/
    

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
    cout<<"********** msg ********* "<<&msg<<endl;

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



void ofApp::onNewMessage(string & message)
{
    cout << "onNewMessage, message: " << message << "\n";
    if(ofToInt(message)==1)SC->buttonPushed();

}

