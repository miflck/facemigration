//
//  Video.cpp
//  facemigration
//
//  Created by Flückiger Michael on 07.08.17.
//
//

#include "Video.hpp"


Video::Video(){
    cout<<"init Video"<<endl;
    bHasAutoRecording=false;
}
//--------------------------------------------------------------

Video::~Video(){
    cout<<"destruct Video"<<endl;
    
}
//--------------------------------------------------------------



void Video::setLoopstate(string _loopstate){
    loopstate=_loopstate;
}

string Video::getLoopstate(){
    return loopstate;
}

void Video::setAutoRecording(bool _autorecording){
    bHasAutoRecording=_autorecording;
    cout<<" Set Autorecord to "<<bHasAutoRecording<<" "<<_autorecording<<endl;
}

bool Video::getAutoRecording(){
    return bHasAutoRecording;
}