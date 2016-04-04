//
//  AnimationState.hpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 4/1/16.
//
//

#ifndef AnimationState_hpp
#define AnimationState_hpp

#include "ofMain.h"
#include "Timeline.hpp"

class AnimationState{
public:
    
    void setup(string _name, float _animationTime, float _transitionTime, bool _doesLoop);
    
    void addTimeline(string timelineName, bool makeStarterNodes);
    
    AnimationNode getCurNode(int timelineIndex);
    AnimationNode getNextNode(int timelineIndex);
    
    
    string name;
    bool doesLoop;
    float animationTime;
    float transitionTime;
    
    vector<Timeline> timelines;
    
    
    
};

#endif /* AnimationState_hpp */
