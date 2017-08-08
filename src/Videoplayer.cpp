//
//  Videoplayer.cpp
//  facemigration
//
//  Created by Flückiger Michael on 12.06.17.
//
//

#include "Videoplayer.hpp"
#include "SessionController.hpp"


Videoplayer::Videoplayer(){
}
//--------------------------------------------------------------

Videoplayer::~Videoplayer(){
    
}
//--------------------------------------------------------------


void Videoplayer::setup(){
    movieclip= new Video();
    // state=-1;
    bIsVideoLoaded=false;
    setState(IDLE);
}
//--------------------------------------------------------------

void Videoplayer::update(){
    
    
    switch (state) {
        case SESSION:
            if(bShowVideo){
                if(movieclip->isLoaded()){
                    movieclip->update();
                    if(movieclip->getIsMovieDone()){
                        ofSendMessage("CLIP is done");
                        videoid++;
                        setVideo(videoid);
                        SC->setClipIsDone(true);

                    }
                }
            }
            break;
            
            
        case INIT:
            if(bShowVideo){
                if(movieclip->isLoaded()){
                    movieclip->update();
                    if(movieclip->getIsMovieDone()){
                        ofSendMessage("Init CLIP is done");
                        SC->setClipIsDone(true);

                    }
                }
            }
            
            break;
        default:
            break;
    }
    
    
    
}
//--------------------------------------------------------------


void Videoplayer::draw(){
    if(bShowVideo){
        movieclip->draw(0,0);
    }
}



void Videoplayer::setState(int _state){
    state=_state;
}


void Videoplayer::setVideo(int _id){
    
    if(_id>videos.size()-1){
        SC->setState(ACTIVE_SESSION_END);
        setState(IDLE);
        return;
    }
    
    videoid=_id;
    setState(SESSION);
    
    cout<<"State "<<state<<" videoid: "<<videoid<<" videos: "<<videos.size()<<endl;
    
    movieclip->setPaused(true);
    movieclip=&videos[videoid];
    movieclip->setPaused(false);
    
    cout<<"Loopstate "<<movieclip->getLoopstate()<<endl;
    if(movieclip->getLoopstate()=="OF_LOOP_NONE"){
        SC->setClipIsDone(false);
    }else{
        SC->setClipIsDone(true);
    }
    showVideo(true);
    
}


void Videoplayer::setInitVideo(int _index){
    setState(INIT);
    
    cout<<"SET INIT VIDEO "<<_index<<endl;
    
    
    
    videoid=_index;
    movieclip->setPaused(true);
    movieclip=&initVideos[videoid];
    movieclip->setPaused(false);
}



//--------------------------------------------------------------

void Videoplayer::loadStory(int num){
    videos.clear();
    initVideos.clear();
    
    bIsVideoLoaded=false;
    state=-1;
    videoid=-1;
    
    ofxXmlSettings storylines;
    if(storylines.loadFile("stories.xml")){
        storylines.pushTag("STORIES");
        int numberOfSavedPoints = storylines.getNumTags("STORIES");
        storylines.pushTag("STORY", num);
        
        int numInitVids = storylines.getNumTags("INIT_VIDEO");
        for(int i = 0; i < numInitVids; i++){
            storylines.pushTag("INIT_VIDEO", i);
            string myPath = storylines.getValue("PATH", "");
            string loopstate = storylines.getValue("LOOPSTATE", "");
            
            Video mc;
            mc.load("movies/"+myPath);
            mc.play();
            mc.setPaused(true);
            mc.setVolume(0);
            if(loopstate=="OF_LOOP_NONE") mc.setLoopState(OF_LOOP_NONE);
            if(loopstate=="OF_LOOP_NORMAL") mc.setLoopState(OF_LOOP_NORMAL);
            if(loopstate=="OF_LOOP_PALINDROME") mc.setLoopState(OF_LOOP_PALINDROME);
            mc.setLoopstate(loopstate);
            initVideos.push_back(mc);
            storylines.popTag();
        }
        
        cout<<".... init videos... "<<initVideos.size()<<endl;
        
        int numVids = storylines.getNumTags("VIDEO");
        numberOfVideos=numVids;
        for(int i = 0; i < numberOfVideos; i++){
            storylines.pushTag("VIDEO", i);
            
            string myPath = storylines.getValue("PATH", "");
            int myloopstate = storylines.getValue("LOOPSTATE", 0);
            string loopstate = storylines.getValue("LOOPSTATE", "");
            
            cout<<"-------------------"+myPath<<" "<<myloopstate<<" "<<loopstate<<endl;
            Video mc;
            mc.load("movies/"+myPath);
            mc.play();
            mc.setPaused(true);
            if(loopstate=="OF_LOOP_NONE") mc.setLoopState(OF_LOOP_NONE);
            if(loopstate=="OF_LOOP_NORMAL"){
                mc.setLoopState(OF_LOOP_NORMAL);
                mc.setVolume(0);
            }
            
            if(loopstate=="OF_LOOP_PALINDROME"){
                mc.setLoopState(OF_LOOP_PALINDROME);
                mc.setVolume(0);
            }
            mc.setLoopstate(loopstate);
            videos.push_back(mc);
            storylines.popTag();
            
        }
        storylines.popTag();
        storylines.popTag();
        bIsVideoLoaded=true;
    }
    else{
        ofLogError("Story file did not load!");
    }
    
    }

int Videoplayer::getNumberOfVideos(){
    return numberOfVideos;
}


bool Videoplayer::getIsVideoLoaded(){
    return bIsVideoLoaded;
}

void Videoplayer::forward(){
    if(videoid<numberOfVideos)videoid++;
    setVideo(videoid);
    
}

void Videoplayer::stop(){
    movieclip->stop();
}

void Videoplayer::showVideo(bool _showVideo){
    bShowVideo=_showVideo;
    cout<<"show video "<<_showVideo<<bShowVideo<<endl;
}
