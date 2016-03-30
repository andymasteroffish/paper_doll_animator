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
    void setup(string _imgFile, string _name, float pivotX, float pivotY);
    
    void setPosFromPrc(float xPrc, float yPrc);
    void setAngle(float newAngle);
    void setPivotFromPrc(float xPrc, float yPrc);
    void loadImge(string _imgFile);
    
    void update(float time, AnimationNode prevNode, AnimationNode nextNode);
    void draw(bool isSelected);
    
    string name;
    
    ofVec2f pos;
    float angle;
    
    ofVec2f pivotPoint;
    
    string imgFile;
    ofImage img;
    
};

#endif /* AnimationLimb_hpp */
