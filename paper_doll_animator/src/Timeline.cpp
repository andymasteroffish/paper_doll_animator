//
//  Timeline.cpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 3/30/16.
//
//

#include "Timeline.hpp"


void Timeline::setup(int yOrder, float _maxTime){
    
    maxTime = _maxTime;
    curTime = 0;
    
    drawW = ofGetWidth() - 400;
    drawH = 20;
    
    offset.x = 20;
    offset.y = 10 + yOrder*(drawH+5);
    
    nodes.clear();
    
    AnimationNode startNode;
    startNode.setup(0, 0, 0, 0);
    nodes.push_back(startNode);
    
    AnimationNode nodeTest;
    nodeTest.setup(maxTime*0.6, ofRandom(-200,200), ofRandom(-200,200), ofRandom(360));
    nodes.push_back(nodeTest);
    
    startNode.time = maxTime;
    nodes.push_back(startNode);
    
    selectedNode = 0;
    
}

void Timeline::update(float _curTime){
    curTime = _curTime;
    
    //selected node should be the one before the current point on the timeline
    selectedNode = 0;
    for (int i=0; i<nodes.size(); i++){
        if (nodes[i].time <= curTime){
            selectedNode = i;
        }else{
            break;
        }
    }
    
    nextNode = selectedNode+1;
    if (nextNode >= nodes.size()){
        nextNode = 0;
    }
    
    
    //make sure the firts and last node are the same
    if (selectedNode != nodes.size()-1){
        nodes[ nodes.size()-1 ] = nodes[0];
        nodes[ nodes.size()-1 ].time =maxTime;
    }else{
        nodes[0] = nodes[nodes.size()-1];
        nodes[0].time = 0;
    }
    
}

void Timeline::setNodeRotation(float newAngle){
    nodes[selectedNode].angle = newAngle;
}
void Timeline::setNodePosition(float xPos, float yPos){
    nodes[selectedNode].pos.set(xPos, yPos);
}

void Timeline::draw(bool isSelected){
    
    float prc = curTime/maxTime;
    
    ofSetColor(0);
    if (isSelected){
        ofSetColor(100, 100, 200);
    }
    
    //the line
    ofDrawLine(offset.x, offset.y+drawH/2, offset.x+drawW, offset.y+drawH/2);
    
    //the playhead
    float playheadX = offset.x+drawW*prc;
    ofDrawLine(playheadX, offset.y, playheadX, offset.y+drawH);
    
    //draw the nodes
    for (int i=0; i<nodes.size(); i++){
        ofSetColor(0);
        if (isSelected && selectedNode==i){
            ofSetColor(100, 100, 200);
        }
        float nodePrc = nodes[i].time/maxTime;
        float nodeX = offset.x+drawW*nodePrc;
        ofDrawCircle(nodeX, offset.y+drawH/2, drawH*0.4);
    }
    
}


float Timeline::mousePressed(int x, int y, int button){
    if (x > offset.x && x < offset.x+drawW && y > offset.y && y < offset.y+drawH){
        mouseStartedInside = true;
    }else{
        mouseStartedInside = false;
    }
    
    return mouseDragged(x, y, button);
}

float Timeline::mouseDragged(int x, int y, int button){
    //if the mosue was not inside, just return -1
    if (!mouseStartedInside){
        return -1;
    }
    
    //otherwise, figure out where we are in the timeline
    float prc = (x-offset.x) / drawW;
    prc = CLAMP(prc, 0, 1);
    return maxTime * prc;
    
}

void Timeline::mouseReleased(){
    mouseStartedInside = false;
}