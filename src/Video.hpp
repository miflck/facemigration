//
//  Video.hpp
//  facemigration
//
//  Created by Flückiger Michael on 07.08.17.
//
//

#ifndef Video_hpp
#define Video_hpp

#include <stdio.h>
#include "ofMain.h"

class Video:  public ofVideoPlayer{

public:
    
    Video();
    virtual ~Video();
    
    void setLoopstate(string _loopstate);
    string getLoopstate();
    bool getAutoRecording();
    void setAutoRecording(bool _autorecording);
    
    
private:
    string loopstate="";
    bool bHasAutoRecording=false;


};
#endif /* Video_hpp */
