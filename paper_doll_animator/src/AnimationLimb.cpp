//
//  AnimationLimb.cpp
//  paper_doll_animator
//
//  Created by Andrew Wallace on 3/30/16.
//
//

#include "AnimationLimb.hpp"

void AnimationLimb::setup(){
    setup("none", 0, 0, 0, 0, 0);
}
void AnimationLimb::setup(string imgFile){
    setup(imgFile, 0, 0, 0, 0, 0);
}
void AnimationLimb::setup(string imgFile, float x, float y, float _angle){
    setup(imgFile, x, y, angle, 0, 0);
}
void AnimationLimb::setup(string imgFile, float x, float y, float _angle, float pivotX, float pivotY){
    
    if (imgFile != "none"){
        img.load(imgFile);
    }
    pos.set(x,y);
    angle = _angle;
    
    pivotPoint.set(pivotX, pivotY);
    
}

void AnimationLimb::setPosFromPrc(float xPrc, float yPrc){
    pos.x = ofGetWidth() * xPrc;
    pos.y = ofGetHeight() * yPrc;
}

void AnimationLimb::setAngle(float newAngle){
    angle = newAngle;
}

void AnimationLimb::setPivotFromPrc(float xPrc, float yPrc){
    //incoming values are 0-1, but we want -2 to 2
    float xPrcAdjust = xPrc*4 - 2;
    float yPrcAdjust = yPrc*4 - 2;
    
    pivotPoint.x = img.getWidth() * xPrcAdjust;
    pivotPoint.y = img.getHeight() * yPrcAdjust;
}

void AnimationLimb::loadImge(string fileName){
    img.load(fileName);
}

void AnimationLimb::update(float time, AnimationNode prevNode, AnimationNode nextNode){
    
    float prc = (time - prevNode.time) / (nextNode.time-prevNode.time);
    
    //lerp between the points
    pos = (1-prc) * prevNode.pos + prc * nextNode.pos;
    angle = (1-prc) * prevNode.angle + prc * nextNode.angle;
    
}

void AnimationLimb::draw(bool isSelected){
    ofSetColor(255);
    
    
    ofPushMatrix();
    
    ofTranslate(pos.x, pos.y);
    ofRotate(angle);
    
    ofTranslate(pivotPoint.x, pivotPoint.y);
    
    if (img.isAllocated()){
        img.draw(-img.getWidth()/2, -img.getHeight()/2);
    }else{
        ofNoFill();
        ofSetColor(0,0,255);
        float demoSize = 30;
        ofDrawRectangle(-demoSize/2, -demoSize/2, demoSize, demoSize);
    }
    
    if (isSelected){
        ofSetColor(100,100,255);
        ofNoFill();
        ofDrawRectangle(-img.getWidth()/2, -img.getHeight()/2, img.getWidth(), img.getHeight());
    }
    
    ofPopMatrix();
    
    //testing pivot point
    ofPushMatrix();
    ofNoFill();
    ofTranslate(pos.x, pos.y);
    
    ofSetColor(0,0,255);
    ofDrawCircle(0,0, 3);
    
    ofPopMatrix();
    
}