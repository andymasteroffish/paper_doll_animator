//
//  Timeline.cpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 3/30/16.
//
//

#include "Timeline.hpp"



void Timeline::setup(int yOrder, float _maxTime, string _limbName){
    
    maxTime = _maxTime;
    curTime = 0;
    
    limbName =  _limbName;
    
    drawW = ofGetWidth() - 420;
    drawH = 20;
    
    offset.x = 70;
    offset.y = 20 + yOrder*(drawH+5);
    
    nodeCircleSize = drawH*0.4;
    framesToDragNode = 10;
    
    nodes.clear();
    
    showTime = yOrder == 0;
    
    selectedNode = 0;
    
}

void Timeline::makeStarterNodes(){
    AnimationNode startNode;
    startNode.setup(0, 0, 0, 0);
    nodes.push_back(startNode);
    
    AnimationNode nodeTest;
    nodeTest.setup(maxTime*0.6, ofRandom(-200,200), ofRandom(-200,200), ofRandom(360));
    nodes.push_back(nodeTest);
    
    startNode.time = maxTime;
    nodes.push_back(startNode);
}

void Timeline::update(float _curTime){
    curTime = _curTime;
    
    dragFrameTimer++;
    
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
    
    
    //make sure the first and last node are the same
    if (selectedNode != nodes.size()-1){
        nodes[ nodes.size()-1 ] = nodes[0];
        nodes[ nodes.size()-1 ].time =maxTime;
    }else{
        nodes[0] = nodes[nodes.size()-1];
        nodes[0].time = 0;
    }
    
    //lockNodesToGrid(0.2);
    
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
    
    //the name
    ofDrawBitmapString(limbName, 5, offset.y+drawH-5);
    
    //the playhead
    float playheadX = offset.x+drawW*prc;
    ofDrawLine(playheadX, offset.y, playheadX, offset.y+drawH);
    
    if (showTime){
        ofSetColor(0);
        ofDrawBitmapString(ofToString(curTime), playheadX, offset.y-3);
    }
    
    //draw the nodes
    for (int i=0; i<nodes.size(); i++){
        ofSetColor(0);
        if (isSelected && selectedNode==i){
            ofSetColor(100, 100, 200);
        }
        float nodePrc = nodes[i].time/maxTime;
        float nodeX = offset.x+drawW*nodePrc;
        ofDrawCircle(nodeX, offset.y+drawH/2, nodeCircleSize);
        //ofDrawBitmapString(ofToString(i), nodeX, offset.y);
    }
    
}


float Timeline::mousePressed(int x, int y, int button){
    if (x > offset.x-nodeCircleSize && x < offset.x+drawW+nodeCircleSize && y > offset.y && y < offset.y+drawH){
        mouseStartedInside = true;
        
        //did they click a node?
        for (int i=0; i<nodes.size(); i++){
            float nodePrc = nodes[i].time/maxTime;
            float nodeX = offset.x+drawW*nodePrc;
            if ( ofDistSquared(x, y, nodeX, offset.y+drawH/2) < nodeCircleSize*nodeCircleSize){
                selectedNode = i;
                nodeBeingDragged = true;
                dragFrameTimer = 0;
                return nodes[i].time;   //set the playhead to this position
            }
        }
        
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
    
    //figure out where we are in the timeline
    float prc = (x-offset.x) / drawW;
    prc = CLAMP(prc, 0, 1);
    float time = maxTime * prc;
    
    //lock to grid if it is on
    if (lockToGrid){
        int numTime = (time+gridTimeStep*0.5) / gridTimeStep;
        time = numTime * gridTimeStep;
        time = MIN(time, maxTime);
    }
    
    //if they just clicked a node, they may ahve just been wanting to set the playhead there, so we wait a bit before dragging or changing the time
    if (nodeBeingDragged){
        
        if (dragFrameTimer < framesToDragNode){
            return nodes[selectedNode].time;
        }
        
        //if we're past that time, we can set the node's time and move on
        //do not do this to the first or last node
        if (selectedNode > 0 && selectedNode < nodes.size()-1){
            nodes[selectedNode].time = time;
            sortNodes();
        }
    }
    
    //if nothing else stoppped us, return the time they dragged the playhead to
    return time;
    
}

void Timeline::mouseReleased(){
    mouseStartedInside = false;
    nodeBeingDragged = false;
}

void Timeline::addNode(){
    
    AnimationNode thisNode;
    thisNode.setup(nodes[selectedNode]);
    thisNode.time = curTime;
    if (lockToGrid){
        int numTime = (curTime+gridTimeStep*0.5) / gridTimeStep;
        thisNode.time = numTime * gridTimeStep;
        thisNode.time = MIN(thisNode.time, maxTime);
    }
    nodes.insert(nodes.begin()+selectedNode+1, thisNode);

    sortNodes();
}

//this is poorly written, but whatever. Chances are pretty low that there will ever be enough nodes to matter
void Timeline::sortNodes(){

    bool isDone = false;
    while (!isDone){
        isDone = true;
        
        for (int i=0; i<nodes.size()-1; i++){
            if (nodes[i].time > nodes[i+1].time){
                AnimationNode temp = nodes[i];
                nodes[i] = nodes[i+1];
                nodes[i+1] = temp;
                isDone = false;
            }
        }
    }

}

void Timeline::setLockToGrid(float timeStep){
    lockToGrid = true;
    gridTimeStep = timeStep;
}
void Timeline::disableLockToGrid(){
    lockToGrid = false;
}