//
//  AnimationState.cpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 4/1/16.
//
//

#include "AnimationState.hpp"


void AnimationState::setup(string _name, float _animationTime, float _transitionTime, bool _doesLoop){
    name = _name;
    animationTime = _animationTime;
    transitionTime = _transitionTime;
    doesLoop = _doesLoop;
    
    timelines.clear();
}

void AnimationState::addTimeline(string timelineName, bool makeStarterNodes){
    Timeline newTimeline;
    newTimeline.setup(timelines.size(), animationTime, timelineName, doesLoop);
    if (makeStarterNodes){
        newTimeline.makeStarterNodes();
    }
    timelines.push_back(newTimeline);
}


AnimationNode AnimationState::getCurNode(int timelineIndex){
    int selectedNode = timelines[timelineIndex].selectedNode;
    return timelines[timelineIndex].nodes[selectedNode];
}

AnimationNode AnimationState::getNextNode(int timelineIndex){
    int nextNode = timelines[timelineIndex].nextNode;
    return timelines[timelineIndex].nodes[nextNode];
}