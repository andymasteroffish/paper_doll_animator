//
//  AnimationLimb.hpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 3/30/16.
//
//

#ifndef AnimationLimb_hpp
#define AnimationLimb_hpp

#include "ofMain.h"
#include "AnimationNode.hpp"

class AnimationLimb{
public:
    
    void setup();
    void setup(string imgFile);
    void setup(string imgFile, float x, float y, float _angle);
    void setup(string imgFile, float x, float y, float _angle, float pivotX, float pivotY);
    
    void setPosFromPrc(float xPrc, float yPrc);
    void setAngle(float newAngle);
    void setPivotFromPrc(float xPrc, float yPrc);
    void loadImge(string fileName);
    
    void update(float time, AnimationNode prevNode, AnimationNode nextNode);
    void draw(bool isSelected);
    
    ofVec2f pos;
    float angle;
    
    ofVec2f pivotPoint;
    
    ofImage img;
    
};

#endif /* AnimationLimb_hpp */
