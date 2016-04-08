#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"

#include "AnimationLimb.hpp"
#include "AnimationState.hpp"
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
    
    void updateName(int input);
    
    void addLimb(bool makeStarterNodes);
    void addLimb(string name, string imgFile, float pivotX, float pivotY, bool makeStarterNodes);
    void setSelectedLimb(int index);
    
    void addAnimation(bool makeStarterNodes);
    void addAnimation(string _name, float _animationTime, float _transitionTime, bool _doesLoop, bool makeStarterNodes);
    void setSelectedAnimation(int index);
    
    void saveToXML(string fileName);
    void loadFromXML(string fileName);
    void clearAnimations();
    
    void setNotificationtext(string notification);
    
    float deltaTime;
    float prevFrameTime;
    
    ofxControlPanel panel;
    simpleFileLister imgLister;
    guiTypeTextDropDown * animationSelectorDropDown;
    
    
    //float animationLength;
    float curTime;
    
    bool isPlaying;
    float timeOnPlay;
    
    int selectedLimb;
    vector<AnimationLimb>   limbs;
    bool showLimbDebugColor;
    
    int selectedAnimation;
    vector<AnimationState>  animations;
    
    float zoom;
    
    //renaming limbs or animations
    bool renamingLimb, renamingAnimation;
    
    //keeping the nodes at exact times when a button is held
    float gridStep;
    
    //refrence image
    ofImage referencePic;
    ofVec2f referencePicPos;
    float referencePicRotation;
    float referencePicScale;
    
    //saving/loading
    string curFileName;
    
    bool showHelpText;
    
    //notifications
    string notificationText;
    float notificationtextStartAlpha;
    float notificationFadeSpeed;
    float notificationTextAlpha;
    
    
    
    
};
