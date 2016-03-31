//
//  Timeline.hpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 3/30/16.
//
//

#ifndef Timeline_hpp
#define Timeline_hpp

#include "ofMain.h"
#include "AnimationNode.hpp"

class Timeline{
public:
    
    void setup(int yOrder, float _maxTime, string _limbName);
    void makeStarterNodes();
    
    void update(float _curTime);
    
    void setNodeRotation(float newAngle);
    void setNodePosition(float xPos, float yPos);
    
    void draw(bool isSelected);
    
    float mousePressed(int x, int y, int button);
    float mouseDragged(int x, int y, int button);
    void mouseReleased();
    
    void changeMaxTime(float newMaxTime);
    
    void addNode();
    void sortNodes();
    void setLockToGrid(float timeStep);
    void disableLockToGrid();
    
    //info
    float maxTime;
    float curTime;
    vector<AnimationNode> nodes;
    
    int selectedNode;
    int nextNode;
    
    //dragging nodes
    bool nodeBeingDragged;
    int dragFrameTimer;
    int framesToDragNode;
    
    //drawing
    string limbName;
    int drawW, drawH;
    ofVec2f offset;
    float nodeCircleSize;
    
    bool showTime;
    
    //mouse stuff
    bool mouseStartedInside;
    
    //locking to grid
    bool lockToGrid;
    float gridTimeStep;
    
};

#endif /* Timeline_hpp */
