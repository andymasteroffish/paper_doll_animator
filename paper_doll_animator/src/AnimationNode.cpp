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

//this funciton is called if the total time of the animaiton changes and we want to move all nodes to remain relative to eahcother
float AnimationNode::adjustTime(float oldMaxTime, float newMaxTime){
    float prc = time/oldMaxTime;
    time = newMaxTime * prc;
}