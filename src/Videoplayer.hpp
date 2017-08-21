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
#include "Video.hpp"


#define INIT 000
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
    int getVideoIndex();
    
    void forward();
    void stop();
    void showVideo(bool _showVideo);
        

private:
    /*
    ofVideoPlayer *movieclip;
    
    ofVideoPlayer idle;
    
    vector<ofVideoPlayer> videos;
    vector<ofVideoPlayer> initVideos;*/

    
    //Video *movieclip;
    
    
    
    shared_ptr<Video> movieclip;

    
    //vector<Video> videos;
    vector<shared_ptr<Video> > videos;

    
    //vector<Video> initVideos;
    vector<shared_ptr<Video> > initVideos;

    
    
    int state;
    int videoid;
    
    ofPtr<ofxXmlSettings>	storyline;

    int numberOfVideos;
    bool bIsVideoLoaded;
    
    bool bShowVideo=false;

    
    
    
};



#endif /* Videoplayer_hpp */
