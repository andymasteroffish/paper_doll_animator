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
    
    void setup(int yOrder, float _maxTime);
    
    void update(float _curTime);
    
    void setNodeRotation(float newAngle);
    void setNodePosition(float xPos, float yPos);
    
    void draw(bool isSelected);
    
    float mousePressed(int x, int y, int button);
    float mouseDragged(int x, int y, int button);
    void mouseReleased();
    
    
    //info
    float maxTime;
    float curTime;
    vector<AnimationNode> nodes;
    
    int selectedNode;
    int nextNode;
    
    //drawing
    int drawW, drawH;
    ofVec2f offset;
    
    //mouse stuff
    bool mouseStartedInside;
    
};

#endif /* Timeline_hpp */
