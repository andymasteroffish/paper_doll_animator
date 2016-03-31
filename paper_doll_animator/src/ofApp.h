#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"

#include "AnimationLimb.hpp"
#include "Timeline.hpp"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void eventsIn(guiCallbackData & data);
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
    
    void addLimb(bool makeStarterNodes);
    void addLimb(string name, string imgFile, float pivotX, float pivotY, bool makeStarterNodes);
    void setSelectedLimb(int index);
    
    void saveToXML();
    void loadFromXML();
    void clearAnimation();
    
    float deltaTime;
    float prevFrameTime;
    
    ofxControlPanel panel;
    simpleFileLister imgLister;
    guiTypeTextDropDown * limbSelectorDropDown;
    
    
    float animationLength;
    float curTime;
    
    bool isPlaying;
    float timeOnPlay;
    
    int selectedLimb;
    vector<AnimationLimb>   limbs;
    vector<Timeline>        timelines;
    
    //keeping the nodes at exact times
    //bool lockNodesToGrid;
    float gridStep;
    
    
};
