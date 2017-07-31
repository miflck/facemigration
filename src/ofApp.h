#pragma once

#include "ofMain.h"
#include "Videorecorder.hpp"
#include "Videoplayer.hpp"
#include "ofxXmlSettings.h"
#include "SessionController.hpp"



#define IDLE 100
#define ACTIVE_SESSION_START 110
#define ACTIVE_SESSION_RECORD 120
#define ACTIVE_SESSION_END 130


#define START 110


class ofApp : public ofBaseApp{

	public:
    
    
    
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
};
