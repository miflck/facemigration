//
//  Videoplayer.hpp
//  facemigration
//
//  Created by Flückiger Michael on 12.06.17.
//
//

#ifndef Videoplayer_hpp
#define Videoplayer_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxXmlSettings.h"


#define INIT 100
#define IDLE 200
#define SESSION 300





class Videoplayer: public ofBaseApp{
    
public:
    
    Videoplayer();
    virtual ~Videoplayer();
    
    void setup();
    void update();
    void draw();
    
    void setState(int state);
    void setVideo(int id);
    void setInitVideo(int index);

    void loadStory(int num);
    
    
    int getNumberOfVideos();
    bool getIsVideoLoaded();
    
    void forward();
    

private:
    
    ofVideoPlayer *movieclip;
    ofVideoPlayer idle;
    
    vector<ofVideoPlayer> videos;
    vector<ofVideoPlayer> initVideos;

    
    int state;
    int videoid;
    
    ofPtr<ofxXmlSettings>	storyline;

    int numberOfVideos;
    bool bIsVideoLoaded;

    
    
    
};



#endif /* Videoplayer_hpp */
