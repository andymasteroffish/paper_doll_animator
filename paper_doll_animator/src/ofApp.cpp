#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    animationLength = 2.96;
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
    
    panel.addSlider("ANIMATION_LENGTH", 3, 0.1, 10);
    
    //node settings
    panel.addPanel("node settings");
    panel.addLabel("Node settings");
    
    panel.addSlider("ROTATION", 0, -360, 720);
    panel.addSlider2D("LIMB_POS_PRC", 0.5, 0.5, 0, 1, 0, 1);

    
    //finish setting up the control panel
    panel.setWhichPanel(0);
    panel.setupEvents();
    panel.enableEvents();
    ofAddListener(panel.guiEvent, this, &ofApp::eventsIn);
    
    //try to load an existing file
    loadFromXML();
    
    //give us one limb if nothign else is there
    if (limbs.size() == 0){
        addLimb(true);
    }
    
    //defaults
    selectedLimb = 0;
    
    gridStep = 0.1;
    
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
    
    if (data.getDisplayName() == "ANIMATION_LENGTH"){
        float newTime = data.getFloat(0);
        animationLength = newTime;
        for (int i=0; i<timelines.size(); i++){
            timelines[i].changeMaxTime(newTime);
        }
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
    if (key == 'l'){
        addLimb(true);
        
    }
    if (key == 'a' || key == 'A'){
        timelines[selectedLimb].addNode();
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
    if (key == 's'){
        saveToXML();
    }
    
    //shift to lock to grid
    if (key == 2305){
        for (int i=0; i<timelines.size(); i++){
            timelines[i].setLockToGrid(gridStep);
        }
    }
    
    //tab to cycle limbs
    if (key == 9){
        setSelectedLimb( (selectedLimb+1)%limbs.size() );
    }
    
    //cout<<"pressed "<<key<<endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    //shift to lock to grid
    if (key == 2305){
        for (int i=0; i<timelines.size(); i++){
            timelines[i].disableLockToGrid();
        }
    }
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
void ofApp::addLimb(bool makeStarterNodes){
    addLimb("limb_"+ofToString(limbs.size()), "none", 0, 0, makeStarterNodes);
}
//--------------------------------------------------------------
void ofApp::addLimb(string name, string imgFile, float pivotX, float pivotY, bool makeStarterNodes){
    
    //create a limb
    AnimationLimb newLimb;
    newLimb.setup(imgFile, name, pivotX, pivotY);
    newLimb.name = name;
    limbs.push_back(newLimb);
    
    //and a timeline for it
    Timeline newTimeline;
    newTimeline.setup(timelines.size(), animationLength, name);
    if (makeStarterNodes){
        newTimeline.makeStarterNodes();
    }
    timelines.push_back(newTimeline);
    
    //update the limb selection tool
    limbSelectorDropDown->vecDropList.push_back(newLimb.name);
    
    //set this new limb as selected
    setSelectedLimb(limbs.size()-1);
}

//--------------------------------------------------------------
void ofApp::setSelectedLimb(int index){
    panel.setValueI("SELECTED_LIMB", index);
    selectedLimb = index;
}


//--------------------------------------------------------------
void ofApp::saveToXML(){
    ofxXmlSettings xml;
    xml.clear();
    
    //make a tag for the animaiton
    xml.addTag("ANIMATION_0");
    xml.pushTag("ANIMATION_0");
    
    //make a tag for each limb
    for (int i=0; i<limbs.size(); i++){
        string limbTagName = "LIMB_"+ofToString(i);
        xml.addTag(limbTagName);
        xml.pushTag(limbTagName);
        
        //save the name and image file used
        xml.setValue("name", limbs[i].name);
        xml.setValue("file", limbs[i].imgFile);
        xml.setValue("pivot_x", limbs[i].pivotPoint.x);
        xml.setValue("pivot_y", limbs[i].pivotPoint.y);
        
        //go through that timeline and save all node info
        for (int k=0; k<timelines[i].nodes.size(); k++){
            string nodeTagName = "NODE_"+ofToString(k);
            AnimationNode thisNode = timelines[i].nodes[k];
            
            xml.addTag(nodeTagName);
            xml.pushTag(nodeTagName);
            
            xml.setValue("time", thisNode.time);
            xml.setValue("pos_x", thisNode.pos.x);
            xml.setValue("pos_y", thisNode.pos.y);
            xml.setValue("angle", thisNode.angle);
            
            xml.popTag();
        }
        
        xml.popTag();
    }
    
    xml.popTag();
    
    xml.saveFile("animation_data.xml");
    
    cout<<"saved!"<<endl;
    
}

//--------------------------------------------------------------
void ofApp::loadFromXML(){
    ofxXmlSettings xml;
    
    clearAnimation();
    
    if (xml.loadFile("animation_data.xml")){
        cout<<"file loaded"<<endl;
    }else{
        cout<<"could not load file"<<endl;
    }
    
    xml.pushTag("ANIMATION_0");
    
    //go through and make limbs
    string limbTagName = "LIMB_"+ofToString(0);
    while (xml.tagExists(limbTagName)){
        xml.pushTag(limbTagName);
        
        string limbName = xml.getValue("name", "none");
        string fileName = xml.getValue("file", "none");
        float pivotX = xml.getValue("pivot_x", 0);
        float pivotY = xml.getValue("pivot_y", 0);
        addLimb(limbName, fileName, pivotX, pivotY, false);
        
        //go through that timeline and add all of the nodes
        string nodeTagName = "NODE_0";
        while (xml.tagExists(nodeTagName)){
            
            xml.pushTag(nodeTagName);
            
            AnimationNode thisNode;
            thisNode.time = xml.getValue("time", 0.0f);
            thisNode.pos.x = xml.getValue("pos_x", 0.0f);
            thisNode.pos.y = xml.getValue("pos_y", 0.0f);
            thisNode.angle = xml.getValue("angle", 0.0f);
            
            timelines[timelines.size()-1].nodes.push_back(thisNode);
            
            //pop out of this node
            xml.popTag();
            
            nodeTagName = "NODE_"+ofToString(timelines[timelines.size()-1].nodes.size());
        }
        
        timelines[timelines.size()-1].sortNodes();
        
        //pop out of this limb
        xml.popTag();
        
        //set up the next one
        limbTagName = "LIMB_"+ofToString(limbs.size());
    
    }
    
    xml.popTag();
}

//--------------------------------------------------------------
void ofApp::clearAnimation(){
    limbs.clear();
    timelines.clear();
    
}
