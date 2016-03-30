//
//  AnimationNode.cpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 3/30/16.
//
//

#include "AnimationNode.hpp"

void AnimationNode::setup(AnimationNode source){
    setup(source.time, source.pos.x, source.pos.y, source.angle);
}

void AnimationNode::setup(float _time, float xPos, float yPos, float _angle){
    time = _time;
    pos.set(xPos, yPos);
    angle = _angle;
}