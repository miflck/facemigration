//
//  Video.cpp
//  facemigration
//
//  Created by Flückiger Michael on 07.08.17.
//
//

#include "Video.hpp"


Video::Video(){
    bHasAutoRecording=false;
}
//--------------------------------------------------------------

Video::~Video(){
    
}
//--------------------------------------------------------------



void Video::setLoopstate(string _loopstate){
    loopstate=_loopstate;
    //cout<<" Set Loopstate to "<<loopstate<<endl;

}

string Video::getLoopstate(){
    return loopstate;
}

void Video::setAutoRecording(bool _autorecording){
    bHasAutoRecording=_autorecording;
    //cout<<" Set Autorecord to "<<bHasAutoRecording<<" "<<_autorecording<<endl;
}

bool Video::getAutoRecording(){
    return bHasAutoRecording;
}