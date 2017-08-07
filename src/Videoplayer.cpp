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
    /*  movieclip.load("movies/fingers.mov");
     movieclip.setLoopState(OF_LOOP_NORMAL);
     movieclip.play();
     movieclip.setPaused(true);
     
     idle.load("movies/idle_b.mov");
     idle.setLoopState(OF_LOOP_PALINDROME);
     idle.play();
     idle.setPaused(true);*/
    
    
    
    movieclip= new ofVideoPlayer();
    
    //loadStory();
    
    state=-1;
    bIsVideoLoaded=false;
}
//--------------------------------------------------------------

void Videoplayer::update(){
    
    if(movieclip->isLoaded()){
    movieclip->update();
    if(movieclip->getIsMovieDone()){
        string message="clip is done";
        ofSendMessage(message);
        videoid++;
        setVideo(videoid);
    }
    }
    
  /*  if(state>=0){
        //      cout<<videoid<<endl;
        videos[videoid].update();
        if(videos[videoid].getIsMovieDone()){
            ofSendMessage("Movie is done");
            videoid++;
            setVideo(videoid);
            videos[videoid].setLoopState(OF_LOOP_PALINDROME);
            cout<<"*************** is done *************"<<endl;
        };
    }*/
    
    
    
    
    
    
    /*
     switch (state) {
     
     case 1:
     videos[0].update();
     break;
     
     case 2:
     videos[1].update();
     break;
     
     default:
     break;
     }*/
    
    
    
}
//--------------------------------------------------------------


void Videoplayer::draw(){
    movieclip->draw(0,0);
    
    /*if(state>=0){
        videos[videoid].draw(0,0);
    }*/
    
    
    /*
     switch (state) {
     case 1:
     videos[0].draw(20,20);
     break;
     
     case 2:
     videos[1].draw(50,50);
     break;
     
     default:
     break;
     }*/
    
    
    
}



void Videoplayer::setState(int _state){
    state=_state;
    
    /*
     
     for (auto video:videos){
     video.setPaused(true);
     
     }*
     
     videos[state].setPaused(false);
     */
    /*switch (state) {
     case 1:
     videos[0].setPaused(false);
     videos[1].setPaused(true);
     break;
     
     case 2:
     videos[0].setPaused(true);
     videos[1].setPaused(false);
     break;
     
     default:
     break;
     }*/
    
    
}


void Videoplayer::setVideo(int _id){
    
    if(_id>videos.size()-1){
        SC->setState(ACTIVE_SESSION_END);
    return;
    }
    
    videoid=_id;
    setState(0);
    
    cout<<"State "<<state<<" videoid: "<<videoid<<" videos: "<<videos.size()<<endl;
    
    movieclip->setPaused(true);
    
    //movieclip->close();
    movieclip=&videos[videoid];
    movieclip->setPaused(false);
    
    //movieclip=videos[videoid];
    //movieclip.setPaused(false);
    
    
   /* movieclip.setPaused(true);
    movieclip.close();

    movieclip=initVideos[videoid];
    movieclip.setPaused(false);
    */
    
    /*
    for (auto video:videos){
        video.setPaused(true);
        
    }
    
    videos[videoid].setPaused(false);
    */
    
    
    /*switch (state) {
     case 1:
     videos[0].setPaused(false);
     videos[1].setPaused(true);
     break;
     
     case 2:
     videos[0].setPaused(true);
     videos[1].setPaused(false);
     break;
     
     default:
     break;
     }*/
    
    
}


void Videoplayer::setInitVideo(int _index){
    setState(0);

    cout<<"SET INIT VIDEO "<<_index<<endl;
    videoid=_index;
    movieclip->setPaused(true);

    //movieclip->close();
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
        ofVideoPlayer mc;
        mc.load("movies/"+myPath);
        mc.play();
        mc.setPaused(true);
        mc.setVolume(0);
        if(loopstate=="OF_LOOP_NONE") mc.setLoopState(OF_LOOP_NONE);
        if(loopstate=="OF_LOOP_NORMAL") mc.setLoopState(OF_LOOP_NORMAL);
        if(loopstate=="OF_LOOP_PALINDROME") mc.setLoopState(OF_LOOP_PALINDROME);
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
            ofVideoPlayer mc;
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
    
    
    /*
     
     ofVideoPlayer mc;
     mc.load("movies/fingers.mov");
     mc.play();
     mc.setPaused(true);
     videos.push_back(mc);
     ofVideoPlayer mc2;
     
     mc2.load("movies/idle_b.mov");
     mc2.play();
     mc2.setPaused(true);
     videos.push_back(mc2);
     */
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

