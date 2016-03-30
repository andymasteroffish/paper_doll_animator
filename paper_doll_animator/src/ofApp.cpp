#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    animationLength = 3;
    curTime = 0;
    
    isPlaying = false;
    
    //setup control panel
    int panelW = 300;
    panel.setup("settings", ofGetWidth()-panelW, 0, panelW, 700);
    
    //limb settings
    panel.addLabel("Limb settings");
    
    vector<string> names;
    limbSelectorDropDown = panel.addTextDropDown("SELECTED_LIMB", 0, names);
    
    //limbSelectorSlider = panel.addSliderInt("SELECTED_LIMB", 0, 0, 5);
    
    panel.addSlider2D("PIVOT_POINT_PRC", 0.5, 0.5, 0, 1, 0, 1);
    imgLister.listDir("parts/");
    panel.addFileLister("LIMB_IMG", &imgLister, 200, 100);
    
    //node settings
    panel.addPanel("node settings");
    panel.addLabel("Node settings");
    
    panel.addSlider("ROTATION", 0, -360, 720);
    panel.addSlider2D("LIMB_POS_PRC", 0.5, 0.5, 0, 1, 0, 1);

    
    //finish setting upt he control panel
    panel.setWhichPanel(0);
    panel.setupEvents();
    panel.enableEvents();
    ofAddListener(panel.guiEvent, this, &ofApp::eventsIn);
    
    //give us one limb if nothign else is there
    if (limbs.size() == 0){
        addLimb();
    }
    
    //defaults
    selectedLimb = 0;
    
    prevFrameTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::eventsIn(guiCallbackData & data){
    // print to terminal if you want to
    //this code prints out the name of the events coming in and all the variables passed
    printf("ofApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
    if( data.getDisplayName() != "" ){
        printf(" element name is %s \n", data.getDisplayName().c_str());
    }
    for(int k = 0; k < data.getNumValues(); k++){
        if( data.getType(k) == CB_VALUE_FLOAT ){
            printf("%i float  value = %f \n", k, data.getFloat(k));
        }
        else if( data.getType(k) == CB_VALUE_INT ){
            printf("%i int    value = %i \n", k, data.getInt(k));
        }
        else if( data.getType(k) == CB_VALUE_STRING ){
            printf("%i string value = %s \n", k, data.getString(k).c_str());
        }
    }
    
    printf("\n");
    
    //dealing with the limbs
    if (data.getDisplayName() == "SELECTED_LIMB"){
        setSelectedLimb( data.getInt(0) );
        cout<<"selected: "<<selectedLimb<<endl;
    }
    
    if (data.getDisplayName() == "LIMB_IMG"){
        limbs[selectedLimb].loadImge(data.getString(1));
    }
    if (data.getDisplayName() == "PIVOT_POINT_PRC"){
        float xPrc = data.getFloat(0);
        float yPrc = data.getFloat(1);
        limbs[selectedLimb].setPivotFromPrc(xPrc, yPrc);
    }
    
    if (data.getDisplayName() == "ROTATION"){
        timelines[selectedLimb].setNodeRotation(data.getFloat(0));
        //limbs[selectedLimb].setAngle();
    }
    if (data.getDisplayName() == "LIMB_POS_PRC"){
        float xPrc = data.getFloat(0) * ofGetWidth() - ofGetWidth()/2;
        float yPrc = data.getFloat(1) * ofGetHeight() - ofGetHeight()/2;
        timelines[selectedLimb].setNodePosition(xPrc, yPrc);
        //limbs[selectedLimb].setPosFromPrc(xPrc, yPrc);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    panel.update();
    
    deltaTime = ofGetElapsedTimef()-prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    if (isPlaying){
        curTime += deltaTime;
        if (curTime > animationLength){
            curTime -= animationLength;
        }
        cout<<"d horse do it "<<curTime<<endl;
        
    }
    
    for (int i=0; i<timelines.size(); i++){
        timelines[i].update(curTime);
        
        //have the limb set itself based on the previous node and current node
        limbs[i].update(curTime, timelines[i].nodes[timelines[i].selectedNode], timelines[i].nodes[timelines[i].nextNode]);
    }
    
    
    //keep the node panel in line with the current node
    //FIGURE OUT HOW TO DO THIS
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    //testLimb.draw();
    
    for (int i=0; i<timelines.size(); i++){
        timelines[i].draw( i==selectedLimb );
    }
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    for (int i=0; i<limbs.size(); i++){
        limbs[i].draw( i==selectedLimb );
    }
    ofPopMatrix();
    
    ofSetColor(255,100,100);
    ofDrawLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
    ofDrawLine(0, ofGetHeight()/2, ofGetWidth(), ofGetHeight()/2);
    
    panel.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'a'){
        addLimb();
    }
    if (key == ' '){
        if (!isPlaying){
            isPlaying = true;
            timeOnPlay = curTime;
        }else{
            isPlaying = false;
            curTime = timeOnPlay;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    for (int i=0; i<timelines.size(); i++){
        float val = timelines[i].mouseDragged(x, y, button);
        if (val >= 0){
            curTime = val;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for (int i=0; i<timelines.size(); i++){
        float val = timelines[i].mousePressed(x, y, button);
        if (val >= 0){
            curTime = val;
            setSelectedLimb(i); //select the limb asociated with this timeline
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    for (int i=0; i<timelines.size(); i++){
        timelines[i].mouseReleased();
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::addLimb(){
    
    AnimationLimb newLimb;
    newLimb.setup("none", 0,0, 0, 0, 0);
    limbs.push_back(newLimb);
    
    Timeline newTimeline;
    newTimeline.setup(timelines.size(), animationLength);
    timelines.push_back(newTimeline);
    
    limbSelectorDropDown->vecDropList.push_back("limb_"+ofToString(limbs.size()));
    
    setSelectedLimb(limbs.size()-1);
}


//--------------------------------------------------------------
void ofApp::setSelectedLimb(int index){
    panel.setValueI("SELECTED_LIMB", index);
    selectedLimb = index;
}
