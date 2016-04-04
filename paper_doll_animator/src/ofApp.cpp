#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //animationLength = 2.96;
    curTime = 0;
    
    isPlaying = false;
    
    notificationTextAlpha = 0;
    notificationtextStartAlpha = 500;
    notificationFadeSpeed = 255;
    
    referencePicPos.set(0,0);
    referencePicRotation = 0;
    referencePicScale = 1;
    
    renamingLimb = false;
    renamingAnimation = false;
    
    showHelpText = false;
    
    zoom =1;
    
    //setup control panel
    int panelW = 300;
    panel.setup("settings", ofGetWidth()-panelW, 0, panelW, 700);
    
    //limb settings
    panel.addLabel("Limb settings");
    
    vector<string> names;
    animationSelectorDropDown = panel.addTextDropDown("SELECTED_ANIMATION", 0, names);
    panel.addSlider("ANIMATION_LENGTH", 3, 0.1, 10);
    panel.addSlider("ANIMATION_TRANSITION_TIME", 0.2, 0, 3);
    panel.addToggle("ANIMATION_LOOPS", false);
    
    panel.addSlider2D("PIVOT_POINT_PRC", 0.5, 0.5, 0, 1, 0, 1);
    imgLister.listDir("parts/");
    panel.addFileLister("LIMB_IMG", &imgLister, 200, 100);
    
    
    
    //node settings
    panel.addPanel("node settings");
    panel.addLabel("Node settings");
    
    
    panel.addSlider("ZOOM", 1, 0.1, 4);
    
    panel.addSlider("LIMB_ROTATION", 0, -360, 720);
    panel.addSlider2D("LIMB_POS_PRC", 0.5, 0.5, 0, 1, 0, 1);
    
    //node settings
    panel.addPanel("ref pic settings");
    panel.addLabel("Ref pic");
    
    panel.addSlider("REF_ROTATION", 0, -360, 360);
    panel.addSlider("REF_SCALE", 1, 0, 2);
    panel.addSlider2D("REF_POS_PRC", 0.5, 0.5, 0, 1, 0, 1);
    

    
    //finish setting up the control panel
    panel.setWhichPanel(0);
    panel.setupEvents();
    panel.enableEvents();
    ofAddListener(panel.guiEvent, this, &ofApp::eventsIn);
    
    //try to load an existing file
    loadFromXML("animation_data.xml");
    
    //give us one limb if nothign else is there
    if (limbs.size() == 0){
        addLimb(true);
    }
    
    //give us one animation if nothing else is there
    if (animations.size() == 0){
        addAnimation(true);
    }
    
    
    
    //defaults
    selectedLimb = 0;
    selectedAnimation = 0;
    
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
    
    
    if (data.getDisplayName() == "ZOOM"){
        zoom = data.getFloat(0);
    }
    if (data.getDisplayName() == "SELECTED_ANIMATION"){
        setSelectedAnimation( data.getInt(0) );
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
    
    if (data.getDisplayName() == "LIMB_ROTATION"){
        animations[selectedAnimation].timelines[selectedLimb].setNodeRotation(data.getFloat(0));
    }
    if (data.getDisplayName() == "LIMB_POS_PRC"){
        float spacingDist = ofGetWidth()/zoom;
        float xPrc = data.getFloat(0) * spacingDist - spacingDist/2;
        float yPrc = data.getFloat(1) * spacingDist - spacingDist/2;
        animations[selectedAnimation].timelines[selectedLimb].setNodePosition(xPrc, yPrc);
    }
    
    if (data.getDisplayName() == "ANIMATION_LENGTH"){
        float newTime = data.getFloat(0);
        animations[selectedAnimation].animationTime = newTime;
        for (int i=0; i<limbs.size(); i++){
            animations[selectedAnimation].timelines[i].changeMaxTime(newTime);
        }
    }
    
    if (data.getDisplayName() == "ANIMATION_LOOPS"){
        animations[selectedAnimation].doesLoop = data.getFloat(0) == 1.00;
        cout<<"loop:"<<animations[selectedAnimation].doesLoop<<endl;
        for (int i=0; i<limbs.size(); i++){
            animations[selectedAnimation].timelines[i].animationLoops = animations[selectedAnimation].doesLoop;
        }
    }
    
    if (data.getDisplayName() == "ANIMATION_TRANSITION_TIME"){
        animations[selectedAnimation].transitionTime = data.getFloat(0);
    }
    
    if (data.getDisplayName() == "REF_ROTATION"){
        referencePicRotation = data.getFloat(0);
    }
    if (data.getDisplayName() == "REF_SCALE"){
        referencePicScale = data.getFloat(0);
    }
    if (data.getDisplayName() == "REF_POS_PRC"){
        referencePicPos.x = data.getFloat(0) * ofGetWidth() - ofGetWidth()/2;
        referencePicPos.y = data.getFloat(1) * ofGetHeight() - ofGetHeight()/2;
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    panel.update();
    
    deltaTime = ofGetElapsedTimef()-prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    if (isPlaying){
        curTime += deltaTime;
        if (curTime > animations[selectedAnimation].animationTime){
            curTime -= animations[selectedAnimation].animationTime;
        }
        
    }
    
    
    for (int i=0; i<limbs.size(); i++){
       //cout<<"updating animaiton "<<selectedAnimation<<" timeline "<<i<<endl;
        animations[selectedAnimation].timelines[i].update(curTime);
        
        AnimationNode curNode = animations[selectedAnimation].getCurNode(i);
        AnimationNode nextNode = animations[selectedAnimation].getNextNode(i);
        
        //have the limb set itself based on the previous(current) node and the next node
        limbs[i].update(curTime, curNode, nextNode);
    }
    
    
    notificationTextAlpha -= notificationFadeSpeed * deltaTime;
    
    if (renamingLimb){
        setNotificationtext("EDITING LIMB NAME");
    }
    if (renamingAnimation){
        setNotificationtext("EDITING ANIMATION NAME");
    }
    
    //have the panel rotaiton value match the current limb
    panel.setValueF("LIMB_ROTATION", limbs[selectedLimb].angle);
    panel.setValueF("ANIMATION_TRANSITION_TIME", animations[selectedAnimation].transitionTime);
    panel.setValueB("ANIMATION_LOOPS", animations[selectedAnimation].doesLoop);
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    //draw the reference pic if we have one
    if (referencePic.isAllocated()){
        ofPushMatrix();
        ofTranslate(referencePicPos.x, referencePicPos.y);
        ofRotate(referencePicRotation);
        ofScale(referencePicScale, referencePicScale);
        ofSetColor(255);
        referencePic.draw(-referencePic.getWidth()/2, -referencePic.getHeight()/2);
        
        ofPopMatrix();
    }
    
    //draw the limbs
    ofPushMatrix();
    ofScale(zoom,zoom);
    for (int i=0; i<limbs.size(); i++){
        limbs[i].draw( i==selectedLimb );
    }
    ofPopMatrix();
    
    ofPopMatrix();
    
    
    //draw the timelines
    for (int i=0; i<limbs.size(); i++){
        animations[selectedAnimation].timelines[i].draw( i==selectedLimb );
    }
    
    ofSetColor(255,100,100);
    ofDrawLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
    ofDrawLine(0, ofGetHeight()/2, ofGetWidth(), ofGetHeight()/2);
    
    panel.draw();
    
    ofSetColor(0, notificationTextAlpha);
    ofDrawBitmapString(notificationText, ofGetWidth()-300, ofGetHeight()-20);
    
    if (showHelpText){
        string helpText = "";
        helpText += "space - play/pause\n";
        helpText += "l/L - new limb\n";
        helpText += "n/N - new animation\n";
        helpText += "a/A - add node\n";
        helpText += "tab - cycle limbs\n";
        helpText += "s   - save\n";
        helpText += "S   - save as\n";
        helpText += "o/O - open\n";
        helpText += "c/C - clear\n";
        helpText += "del - delete node\n";
        helpText += "r/R - rename animation\n";
        helpText += "ent - rename limb\n";
        helpText += "ent again to end rename\n";
        helpText += "hold shift to lock to grid\n";
        helpText += "h/H - toggle help text\n";
        
        int xPos = 12;
        int yPos = animations[0].timelines[limbs.size()-1].offset.y + 60;
        ofSetColor (255,0,0);
        ofDrawBitmapString(helpText, xPos, yPos);
        
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (renamingLimb || renamingAnimation){
        updateName( key);
        return;
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
    
    if (key == 'l' || key == 'L'){
        addLimb(true);
    }
    if (key == 'n' || key == 'N'){
        addAnimation(true);
    }
    if (key == 'a' || key == 'A'){
        animations[selectedAnimation].timelines[selectedLimb].addNode();
    }
    
    //tab to cycle limbs
    if (key == 9){
        setSelectedLimb( (selectedLimb+1)%limbs.size() );
    }
    
    
    if (key == 's'){
        saveToXML(curFileName);
    }
    if (key == 'S'){
        ofFileDialogResult saveFileResult = ofSystemSaveDialog(curFileName, "Save your file");
        if (saveFileResult.bSuccess){
            saveToXML(saveFileResult.fileName);
        }
    }
    if (key == 'o' || key == 'O'){
        //Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select an xml");
        cout<<"load:"<<openFileResult.fileName<<endl;
        loadFromXML(openFileResult.fileName);
    }
    if (key == 'c' || key == 'C'){
        clearAnimations();
        addLimb(true);
        addAnimation(true);
    }
    
    
    
    //delete to kill the selected node
    if (key == 127){
        animations[selectedAnimation].timelines[selectedLimb].deleteCurrentNode();
    }
    
    //enter to rename a limb
    if (key == 13){
        renamingLimb = true;
    }
    //r to rename an animation
    if (key == 'r' || key == 'R'){
        renamingAnimation = true;
    }
    
    //shift to lock to grid
    if (key == 2305){
        for (int i=0; i<limbs.size(); i++){
            animations[selectedAnimation].timelines[i].setLockToGrid(gridStep);
        }
    }
    
    if (key == 'h'){
        showHelpText = !showHelpText;
    }
    
    //cout<<"pressed "<<key<<endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    //shift to lock to grid
    if (key == 2305){
        for (int i=0; i<limbs.size(); i++){
            animations[selectedAnimation].timelines[i].disableLockToGrid();
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    for (int i=0; i<limbs.size(); i++){
        float val = animations[selectedAnimation].timelines[i].mouseDragged(x, y, button);
        if (val >= 0){
            curTime = val;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for (int i=0; i<limbs.size(); i++){
        float val = animations[selectedAnimation].timelines[i].mousePressed(x, y, button);
        if (val >= 0){
            curTime = val;
            setSelectedLimb(i); //select the limb asociated with this timeline
        }
    }
    
    if (button == 2){
        animations[selectedAnimation].timelines[selectedLimb].update(curTime);
        animations[selectedAnimation].timelines[selectedLimb].addNode();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    for (int i=0; i<limbs.size(); i++){
        animations[selectedAnimation].timelines[i].mouseReleased();
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
void ofApp::updateName(int input){
    
    //number, lower case, upper case and underscore all valid chars to add
    if ( (input >= 48 && input <= 57) || (input >= 65 && input <= 90) || (input >= 97 && input <= 122) || input == 95){
        if (renamingLimb){
            limbs[selectedLimb].name += input;
        }
        if (renamingAnimation){
            animations[selectedAnimation].name += input;
        }
    }
    
    //del removes the last char if there is one
    if (input == 127){
        if (renamingLimb && limbs[selectedLimb].name.length() > 0){
            limbs[selectedLimb].name = limbs[selectedLimb].name.substr(0, limbs[selectedLimb].name.size()-1);
        }
        if (renamingAnimation){
            animations[selectedAnimation].name = animations[selectedAnimation].name.substr(0, animations[selectedAnimation].name.size()-1);
        }
    }
    
    //enter ends it
    if (input == 13){
        renamingAnimation = false;
        renamingLimb = false;
        setNotificationtext("Done editing name");
    }
    
    //if we're updating the limb, update all timelines of all animaitonf
    if (renamingLimb){
        for (int i=0; i<animations.size(); i++){
            animations[i].timelines[selectedLimb].limbName = limbs[selectedLimb].name;
        }
    }
    
    //if we're updating the animaiton name, update the dorpown
    if (renamingAnimation){
        animationSelectorDropDown->vecDropList[selectedAnimation] = animations[selectedAnimation].name;
    }
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    if (dragInfo.files.size() > 0){
        cout<<"I got "<<dragInfo.files[0]<<endl;
        referencePic.load(dragInfo.files[0]);
    }
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
    
    //and a timeline for it for every animation
    for (int i=0; i<animations.size(); i++){
        animations[i].addTimeline(name, makeStarterNodes);
    }
    
    //set this new limb as selected
    setSelectedLimb(limbs.size()-1);
}

//--------------------------------------------------------------
void ofApp::setSelectedLimb(int index){
    selectedLimb = index;
}

//--------------------------------------------------------------
void ofApp::addAnimation(bool makeStarterNodes){
    addAnimation("ANIM_"+ofToString(animations.size()), 3, 0.3f, true, makeStarterNodes);
}
//--------------------------------------------------------------
void ofApp::addAnimation(string _name, float _animationTime, float _transitionTime, bool _doesLoop, bool makeStarterNodes){
    AnimationState newAnimation;
    newAnimation.setup( _name,  _animationTime, _transitionTime, _doesLoop);
    
    //create a timeline for every limb
    for (int i=0; i<limbs.size(); i++){
        newAnimation.addTimeline(limbs[i].name, makeStarterNodes);
    }
    
    animations.push_back(newAnimation);
    
    //update the limb selection tool
    animationSelectorDropDown->vecDropList.push_back(newAnimation.name);
    setSelectedAnimation(animations.size()-1);
}

//--------------------------------------------------------------
void ofApp::setSelectedAnimation(int index){
    //make sure that nothing in the current animaiton is still selected
    for (int i=0; i<limbs.size(); i++){
        animations[selectedAnimation].timelines[i].mouseReleased();
    }
    
    //actually change the animaiton
    panel.setValueI("SELECTED_ANIMATION", index);
    selectedAnimation = index;
    curTime = 0;
    
    panel.setValueF("ANIMATION_LENGTH", animations[selectedAnimation].animationTime);
}


//--------------------------------------------------------------
void ofApp::saveToXML(string fileName){
    ofxXmlSettings xml;
    xml.clear();
    
    xml.addTag("LIMBS");
    xml.pushTag("LIMBS");
    
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
        
        //pop out of this limb
        xml.popTag();
    }
    
    //pop out of LIMBS
    xml.popTag();
    
    
    xml.addTag("ANIMATIONS");
    xml.pushTag("ANIMATIONS");
    
    //make a tag for each animaiton
    for (int a=0; a<animations.size(); a++){
        AnimationState thisAnimation = animations[a];
        string animationTagName = "ANIMATION_"+ofToString(a);
        xml.addTag(animationTagName);
        xml.pushTag(animationTagName);
        
        xml.setValue("name", thisAnimation.name);
        xml.setValue("length", thisAnimation.animationTime);
        xml.setValue("transition_time", thisAnimation.transitionTime);
        xml.setValue("does_loop", thisAnimation.doesLoop);
    
        //make a tag for each timeline
        for (int i=0; i<thisAnimation.timelines.size(); i++){
            Timeline thisTimeline = thisAnimation.timelines[i];
            string timelineTag = "TIMELINE_"+ofToString(i);
            xml.addTag(timelineTag);
            xml.pushTag(timelineTag);
            
            //go through that timeline and save all node info
            for (int k=0; k<thisTimeline.nodes.size(); k++){
                string nodeTagName = "NODE_"+ofToString(k);
                AnimationNode thisNode = thisTimeline.nodes[k];
                
                xml.addTag(nodeTagName);
                xml.pushTag(nodeTagName);
                
                xml.setValue("time", thisNode.time);
                xml.setValue("pos_x", thisNode.pos.x);
                xml.setValue("pos_y", thisNode.pos.y);
                xml.setValue("angle", thisNode.angle);
                
                //pop out of this node
                xml.popTag();
            }
            
            //pop out of this timeline
            xml.popTag();
        }
        
        //pop out of this animation
        xml.popTag();
    }
    
    //pop out of ANIMATIONS
    xml.popTag();
    
    xml.saveFile(fileName);
    
    cout<<"saved "<<curFileName<<endl;
    setNotificationtext("Saved "+curFileName);
    
}

//--------------------------------------------------------------
void ofApp::loadFromXML(string fileName){
    ofxXmlSettings xml;
    
    clearAnimations();
    
    if (xml.loadFile(fileName)){
        cout<<"start file load"<<endl;
        curFileName = fileName;
    }else{
        cout<<"could not load file"<<endl;
        setNotificationtext("Error loading file");
        return;
    }
    
    xml.pushTag("LIMBS");
    
    //go through and make limbs
    string limbTagName = "LIMB_"+ofToString(0);
    while (xml.tagExists(limbTagName)){
        xml.pushTag(limbTagName);
        
        string limbName = xml.getValue("name", "none");
        string fileName = xml.getValue("file", "none");
        float pivotX = xml.getValue("pivot_x", 0);
        float pivotY = xml.getValue("pivot_y", 0);
        addLimb(limbName, fileName, pivotX, pivotY, false);
        
        //pop out of this limb
        xml.popTag();
        //setup the next limb
        limbTagName = "LIMB_"+ofToString(limbs.size());
    }
    
    //pop out of LIMBS
    xml.popTag();
    
    //cout<<"loaded limbs"<<endl;
    
    
    xml.pushTag("ANIMATIONS");
    string animationTagName = "ANIMATION_"+ofToString(0);
    while (xml.tagExists(animationTagName)){
        xml.pushTag(animationTagName);
        
        //cout<<"push "<<animationTagName<<endl;
        
        string animationName = xml.getValue("name", "none");
        float animationTime = xml.getValue("length", 3.0f);
        float transitionTime = xml.getValue("transition_time", 0.3f);
        bool doesLoop = xml.getValue("does_loop", 0) == 1;
        
        addAnimation(animationName, animationTime, transitionTime, doesLoop, false);
    
        //go through the timelines of this animaiton
        //unlike most tags, we know exactly how many there will be because every limb has a timeline
        for (int t=0; t<limbs.size(); t++){
            string timelineTagName = "TIMELINE_"+ofToString(t);
            xml.pushTag(timelineTagName);
            
            //cout<<"push "<<timelineTagName<<endl;
            
            animations[animations.size()-1].timelines[t].limbName = limbs[t].name;
            
            //animations[animations.size()-1].addTimeline(limbs[animations.size()-1].name, false);
            
            //go through the nodes of this timeline
            string nodeTagName = "NODE_"+ofToString(0);
            while (xml.tagExists(nodeTagName)){
                xml.pushTag(nodeTagName);
                
                //cout<<"push "<<nodeTagName<<endl;
                
                AnimationNode thisNode;
                thisNode.time = xml.getValue("time", 0.0f);
                thisNode.pos.x = xml.getValue("pos_x", 0.0f);
                thisNode.pos.y = xml.getValue("pos_y", 0.0f);
                thisNode.angle = xml.getValue("angle", 0.0f);
                
                //cout<<"adding node to animaiton "<<animations.size()-1<<" timeline "<<t<<endl;
                //cout<<"   node time "<<thisNode.time<<endl;
                animations[animations.size()-1].timelines[t].nodes.push_back(thisNode);
                
                //pop out of this node
                xml.popTag();
                //get the next node
                nodeTagName = "NODE_"+ofToString(animations[animations.size()-1].timelines[t].nodes.size());
            }
        
            
            animations[animations.size()-1].timelines[t].sortNodes();
            
            //pop out of this timeline
            xml.popTag();
        }
        
        //pop out of this animation
        xml.popTag();
        
        //set up the next one
        animationTagName = "ANIMATION_"+ofToString(animations.size());
    
    }
    
    //pop out of ANIMATIONS
    xml.popTag();
    
    setSelectedAnimation(0);
    
    cout<<"loaded "<<curFileName<<endl;
    setNotificationtext("Loaded "+curFileName);
}

//--------------------------------------------------------------
void ofApp::clearAnimations(){
    limbs.clear();
    for (int i=0; i<animations.size(); i++){
        animations[i].timelines.clear();
    }
    animations.clear();
    
}

//--------------------------------------------------------------
void ofApp::setNotificationtext(string notification){
    notificationText = notification;
    notificationTextAlpha = notificationtextStartAlpha;
}