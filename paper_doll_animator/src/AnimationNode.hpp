//
//  AnimationNode.hpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 3/30/16.
//
//

#ifndef AnimationNode_hpp
#define AnimationNode_hpp

#include "ofMain.h"

class AnimationNode{
public:
    
    void setup(AnimationNode source);
    void setup(float _time, float xPos, float yPos, float _angle);
    
    
    float time;
    ofVec2f pos;
    float angle;
};

#endif /* AnimationNode_hpp */
