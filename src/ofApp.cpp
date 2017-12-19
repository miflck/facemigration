#include "ofApp.h"
#include "SessionController.hpp"

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/USBSpec.h>




//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(255);
    ofSetVerticalSync(true);
    CGDisplayHideCursor(NULL);
    
    reenumerateWebcam();
    
    ofSleepMillis(1000);

    SC->initialize();
    SC->setup();
    ofSleepMillis(1000);
    cout<<"sleep"<<endl;
    
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
    system("osascript -e 'set Volume 10'");
    
 
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(requestRead)
    {
        serial.sendRequest();
        requestRead = false;
    }
    
    SC->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    SC->draw();
}

//--------------------------------------------------------------


void ofApp::reenumerateWebcam(){
    
    cout<<"REENUMERATE! "<<endl;
    mach_port_t             masterPort;
    CFMutableDictionaryRef  matchingDict;
    kern_return_t           kr;
    SInt32                  usbVendor =0x046d;
    SInt32                  usbProduct = 0x0825;
    
    io_service_t                usbDevice;
    IOCFPlugInInterface         **plugInInterface = NULL;
    IOUSBDeviceInterface187     **deviceInterface;
    HRESULT                     result;
    SInt32                      score;
    
    // to get vendorid and productid in termina: system_profiler SPUSBDataType
    
    //Create a master port for communication with the I/O Kit
    kr = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if (kr || !masterPort)
    {
        return -1;
    }
    //Set up matching dictionary for class IOUSBDevice and its subclasses
    matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (!matchingDict)
    {
        mach_port_deallocate(mach_task_self(), masterPort);
        return -1;
    }
    // Only get the Device you are looking for:
    //Add the vendor and product IDs to the matching dictionary.
    CFDictionarySetValue(matchingDict, CFSTR(kUSBVendorName),
                         CFNumberCreate(kCFAllocatorDefault,
                                        kCFNumberSInt32Type, &usbVendor));
    CFDictionarySetValue(matchingDict, CFSTR(kUSBProductName),
                         CFNumberCreate(kCFAllocatorDefault,
                                        kCFNumberSInt32Type, &usbProduct));
    
    io_iterator_t iterator;
    /*Get an iterator.*/
    kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iterator);
    if (kr != KERN_SUCCESS)
    {
        return -1;// fail
    }
    
    
    while (usbDevice = IOIteratorNext(iterator))
    {
        io_name_t deviceName;
        IORegistryEntryGetName( usbDevice, deviceName );
        printf("\ndeviceName:%s",deviceName);
        
        //Create an intermediate plug-in
        kr = IOCreatePlugInInterfaceForService(usbDevice,
                                               kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID,
                                               &plugInInterface, &score);
        //DonÕt need the device object after intermediate plug-in is created
        kr = IOObjectRelease(usbDevice);
        
        if ((kIOReturnSuccess != kr) || !plugInInterface)
        {
            printf("Unable to create a plug-in (%08x)\n", kr);
            continue;
        }
        //Now create the device interface
        result = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID187), (LPVOID *)&deviceInterface);
        // Now done with the plugin interface.
        (*plugInInterface)->Release(plugInInterface);
        
        
        kr = (*deviceInterface)->USBDeviceOpen(deviceInterface);
        if(kr == kIOReturnSuccess)
        {
            kr = (*deviceInterface)->USBDeviceReEnumerate(deviceInterface, 0);
        }
        
        (*deviceInterface)->USBDeviceClose(deviceInterface);
        (*deviceInterface)->Release(deviceInterface);
    }
    IOObjectRelease(iterator);



}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
  // SC->videorecorder.toggleRecording();
    }

    //no data gets saved unless you hit the s key
    if(key == 's'){
     //   recordedClips->saveFile("clips.xml");
        SC->saveBackground();
    }

    
    if(key=='s'){
       // SC->makeNewSession();
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
    cout<<"********** msg ********* "<<msg.message<<endl;

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



void ofApp::onNewMessage(string & message)
{
    cout << "onNewMessage, message: " << message << "\n";
    if(ofToInt(message)==1)SC->buttonPushed();
    if(ofToInt(message)==-1)SC->reset();


}

