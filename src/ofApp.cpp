#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // ÁîªÈù¢Ë®≠ÂÆö
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofBackground(31);
    // ÁÖßÊòé„Å®„Ç´„É°„É©
    ofEnableLighting();
    light.setPosition(200, 300, 50);
    light.enable();
    cam.setOrientation(ofPoint(0, 0,0));
    cam.setDistance(-100);
    // Leap MotionÈñãÂßã
    leap.open();
    
    gui.setup();
    gui.add(guiFPS.set("average FPS", 0, 0, 60));
    gui.add(guiMinFPS.set("minimum FPS", 0, 0, 60));
    gui.add(leapSpeed.set("leapSpeed", 0, 0, 1500));
    gui.add(bpm.set("BPM", 0, 0, 360));
    gui.add(videoSpeed.set("speed", 0, 0, 2));
    gui.add(videoVolume.set("volume", 1, 0, 1));

    gui.add(leapmotionStrength.set("lp strength", 0, 0, 1000));
    gui.add(hitCircle.setup("hit radius",200.0,10.0,1000.0));
    gui.add(hitCircleY.setup("hit Y",0,-1000,1000));
    
    
    backVideos.push_back(new ofxHapPlayer());
    backVideos.back()->loadMovie("mp4/learge/wait_hap.mov");
    backVideos.push_back(new ofxHapPlayer());
    backVideos.back()->loadMovie("mp4/learge/ready_hap.mov");
    
    addVideo("mp4/learge/01_sample_hap.mov");
    addVideo("mp4/learge/02_sample_hap.mov");
    addVideo("mp4/learge/03_sample_hap.mov");
    addVideo("mp4/learge/04_sample_hap.mov");
    for (int i = 0;i < frontVideos.size();i++){
        frontVideos[i]->play();
        frontVideos[i]->setFrame(1);
        frontVideos[i]->setVolume(0.0);
    }
    
    
    
    videoIndex = 0;
    
    textImg[0].loadImage("texture/leaptext_cool.png");
    textImg[1].loadImage("texture/leaptext_good.png");
    textImg[2].loadImage("texture/leaptext_bad.png");
    
    ofEnableArbTex();
    ofFbo::Settings settings;
    settings.width = frontVideos[0]->getWidth();
    settings.height = frontVideos[0]->getHeight();
    settings.internalformat	= GL_RGBA;
    settings.useDepth		= true;
    settings.useStencil		= false;
    settings.textureTarget	= ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
    settings.depthStencilInternalFormat = GL_DEPTH_COMPONENT16;
    settings.depthStencilAsTexture = false;
    settings.minFilter = GL_LINEAR;
    settings.maxFilter = GL_LINEAR;
    settings.numSamples = 8;
    
    buffer.allocate(settings);
    
    
    ofFbo::Settings settings2;
    settings2.width = settings.width;
    settings2.height = settings.height;
    effect.setup(&buffer, settings2);
    
    mask.set(0,0,1920,1080);
    
    ofDisableArbTex();
    texs.push_back(new ofTexture());
    ofLoadImage(*texs.back(), "textures/bokeh1b.png");
    
    texs.push_back(new ofTexture());
    ofLoadImage(*texs.back(), "textures/bokeh3b.png");
    
    texs.push_back(new ofTexture());
    ofLoadImage(*texs.back(), "textures/bokeh6.png");
    
    texs.push_back(new ofTexture());
    ofLoadImage(*texs.back(), "textures/bokeh10.png");
    
    texs.push_back(new ofTexture());
    ofLoadImage(*texs.back(), "textures/bokeh9.png");
    basecolor.setHsb(180, 200, 180);
    
    particle.setup("texture/maru.png");
    particle.setAttenuation(0.90);
    
    pathLines.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    
    isTapped = false;
    isHandOut = false;
    isHandOutSoon = false;
    isStop = false;
    isWaiting = false;
    isReady = false;
    isReadyFinish = false;
    
    fxEnable = true;
    
    prevBPM = 80.00;
    goodCount = 0;
    
    anime.setup();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //FPS計算
    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
    
    //BPM
    bpmTapper.update();
    
    //particle
    particle.update();
    
    //video
    if(isWaiting){
        backVideos[0]->update();
    }else if (isReady && !isReadyFinish){
        backVideos[1]->update();
        if(backVideos[1]->getPosition()>=0.96){
           
            isReadyFinish = true;
            backVideos[1]->stop();
        }
        
    }else{
        frontVideos[videoIndex]->update();
    }
    
    
    // Leap Motion
    vector <Hand> hands = leap.getLeapHands();
    Frame frameprev10 = contoroller.frame(10);
    Vector currentPosition[2];
    
    //leapmotion 手を検知したら
    if( leap.isFrameNew() && hands.size()){
        isHandOut = false;
        isHandOutSoon = false;
        isReady = true;
        if (isReady && isReadyFinish && isWaiting) {
            isWaiting = false;
            videoIndex = (int)ofRandom(frontVideos.size());
            if(prevVideoIndex != videoIndex){
                frontVideos[videoIndex]->setFrame(1);
                int currentPos = frontVideos[videoIndex]->getCurrentFrame();
                
            }
            prevVideoIndex = videoIndex;
        }
        fingerPos.clear();
        spherePos.clear();
        sphereSize.clear();
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        int ct = 0;
        for(int i = 0; i < hands.size(); i++){
            ofVec3f pt;
            pt = leap.getMappedofPoint(hands[i].palmPosition());
            translateValue[i] = hands[i].translation(frameprev10);
            currentPosition[i] = hands[i].palmPosition();
            speedVars[i] = hands[i].palmVelocity().magnitudeSquared()/1000;
            distance[i] = translateValue[i].magnitude();
            fingerPos.push_back(pt);
        }
        leapmotionStrength = distance[0]>=distance[1]?distance[0]>0?distance[0]:0:distance[1]>0?distance[1]:0;
        
        leapSpeed = speedVars[0];
        previous = current;
        current = fingerPos[0];
        ofColor colorMesh;
        colorMesh.setHsb(ofRandom(80, 230), 230, 255);
        pathVertices.push_back(current);
        colorMeshes.push_back(colorMesh);
        while(pathVertices.size() > 15) {
            pathVertices.pop_front();
            
            colorMeshes.pop_front();
        }
        pathLines.clear();
        for(unsigned int i = 0; i < pathVertices.size(); i++) {
            pathLines.addVertex(pathVertices[i]);
            pathLines.addColor(colorMeshes[i]);
        }
        
    }else{
        //isHandOut = true;
        isHandOutSoon = true;
        leapSpeed = 0;
        pathLines.clear();
        pathVertices.clear();
    }
        //
    //change uniform parameter
    effect.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->u_Volume = ofNoise(ofGetElapsedTimef())*5000.0;
    leap.markFrameAsOld();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    guiFPS = (int)(ofGetFrameRate() + 0.5);
    
    // calculate minimum fps
    deltaTimeDeque.push_back(deltaTime);
    
    
    while (deltaTimeDeque.size() > guiFPS.get())
        deltaTimeDeque.pop_front();
    
    float longestTime = 0;
    for (int i=0; i<deltaTimeDeque.size(); i++){
        if (deltaTimeDeque[i] > longestTime)
            longestTime = deltaTimeDeque[i];
    }
    
    guiMinFPS.set(1.0 / longestTime);
    
    
    //早さ感知ちょっと微妙
    if(leapSpeed > 1000){
        if(!isTapped){
            
            bpmTapper.tap();
            isTapped = true;
            bpmDownCount.clear();
        }
        
    }else{
        
        if(bpmTapper.beatPerc()>0.9){
            bpmDownCount.push_back(bpmTapper.beatPerc());
            
            if(bpmDownCount.size()>2){
                bpmTapper.tap();
                bpmDownCount.clear();
                if(isHandOutSoon){
                    isHandOut = true;
                }
                
            }
        }
        isTapped = false;
    }
    
    //leapmotionhand検知外にいってもBPMはかる
    if (isHandOutSoon) {
        //bpmTapper.tap();
    }
    
    //////////////////////

    
    buffer.begin();
    
    ofClear(0, 0, 0);
    
    ofSetColor(255);
    
    ofTexture *vidtexture;
    /*
    vidtexture->bind();
    glBegin(GL_TRIANGLE_STRIP);
    
    glTexCoord2d(mask.x, mask.y);
    glVertex2d(mask.x, mask.y);
    
    glTexCoord2d(mask.x+mask.width, mask.y);
    glVertex2d(mask.x+mask.width, mask.y);
    
    glTexCoord2d(mask.x, mask.y+mask.height);
    glVertex2d(mask.x, mask.y+mask.height);
    
    glTexCoord2d(mask.x+mask.width, mask.y+mask.height);
    glVertex2d(mask.x+mask.width, mask.y+mask.height);
    
    glEnd();
    
    vidtexture->unbind();
    */
    ofShader *shader;
    if (isWaiting) {
        vidtexture = backVideos[0]->getTexture();
        shader = backVideos[0]->getShader();
    }else if(isReady && !isReadyFinish){
        vidtexture = backVideos[1]->getTexture();
        shader = backVideos[1]->getShader();
    }else{
        vidtexture = frontVideos[videoIndex]->getTexture();
        shader = frontVideos[videoIndex]->getShader();
    }
    
    // the result of getShader() will be NULL if the movie is not Hap Q
    if (shader)
    {
        shader->begin();
    }
    vidtexture->draw(0,0);
    if (shader)
    {
        shader->end();
    }
    
    
    if (fxEnable){
        
        int step = 150;
        
        ofSetColor(255);
        /*
        for (int i = 0;i < buffer.getWidth();i+=step){
            for (int j = 0;j < buffer.getHeight();j+=step){
                ofPushMatrix();
                
                ofTranslate(i + ((int)buffer.getWidth() % step) / 2,
                            j + ((int)buffer.getHeight()% step) / 2);
                
                float scl = 5 + videoVolume*15.0;
                ofLine(-scl, 0, scl, 0);
                ofLine(0, -scl, 0, scl);
                ofPopMatrix();
            }
        }
        */
        ofPushMatrix();
        ofTranslate(buffer.getWidth()-30, buffer.getHeight());
        float bpmAverage = ofMap(videoSpeed, 0, 2.0, 0, buffer.getHeight());
        if(videoSpeed>=1.2){
            ofSetColor(255, 0, 0);
        }else if(videoSpeed<=0.8){
            ofSetColor(0, 0, 255);
        }else{
            ofSetColor(0, 255, 0);
        }

        ofRect(0, -bpmAverage, 30, bpmAverage);
        ofPopMatrix();
        
        ofPushMatrix();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetColor(255, 255, 255);
        ofTranslate(buffer.getWidth()/2, buffer.getHeight()/2);
        if(isTapped){
            if(videoSpeed>=1.2){
                //ofSetColor(255, 0, 0);
                textImg[2].draw(-320, -240);
                goodCount = 0;
            }else if(videoSpeed<=0.8){
                //ofSetColor(0, 0, 255);
                textImg[2].draw(-320, -240);
                goodCount = 0;
            }else{
                
                //ofRectangle rect = face.getStringBoundingBox(result, 0,0);
                //face.drawString(result, -rect.getWidth()/2, -rect.getHeight()/2);
                
                if(goodCount>10){
                    textImg[0].draw(-320, -240);
                    particleShow();
                }else{
                    textImg[1].draw(-320, -240);
                    
                }
                goodCount++;
            }

        
        }
        ofPopMatrix();

    }
    
    ofEnableDepthTest();

    ///////////////
    ///デバッグ表示
    cam.begin();
    int ct = 0;
    for(int i = 0; i < fingerPos.size(); i++){
        if(isTapped){
            
            ofSetColor(255, 127, 0, 127);
            ofEnableBlendMode(OF_BLENDMODE_ADD);
           
        }else{
            ofSetColor(255, 255, 0, 127);
        }
        //sphere.draw();
    }
    
    
    pathLines.drawWireframe();
    ofSetColor(255, 255, 0, 127);
    // „É°„ÉÉ„Ç∑„É•ÊèèÁîª
    ofSetColor(255);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    particle.drawMultiSplite(texs);
    
    /*
    glLineWidth(2.0);//チャネル
    vector<ofVec3f> vercs;
    vercs = pathLines.getVertices();
    if (pathLines.getNumVertices() > 2){
        ofSetColor(basecolor);
        for (int i = 0;i < 2;i++){
            int idx[3];
            idx[0] = ofNoise(i/3.5+ofGetElapsedTimef()/34.3)*pathLines.getNumVertices();
            idx[1] = ofNoise(i/6.5+ofGetElapsedTimef()/27.4)*pathLines.getNumVertices();
            idx[2] = ofNoise(i/8.5+ofGetElapsedTimef()/49.7)*pathLines.getNumVertices();
            
            glBegin(GL_LINE_STRIP);
            glVertex2d(vercs[idx[0]].x, vercs[idx[0]].y);
            glVertex2d(vercs[idx[1]].x, vercs[idx[1]].y);
            glVertex2d(vercs[idx[2]].x, vercs[idx[2]].y);
            glVertex2d(vercs[idx[0]].x, vercs[idx[0]].y);
            glEnd();
        }
    }
    glLineWidth(1.0);
     */
    
    ofDisableBlendMode();
    cam.end();
    buffer.end();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofClear(0,0,0,255);
    ofBackground(0, 0, 0);
    ofSetColor(255);
    //buffer.draw(0, 0, 1280, 720);
    effect.applyFx();
    buffer.draw(0, 0, 1280, 720);
    
    
    ofPopStyle();
    ofDisableDepthTest();
    gui.draw();
    ofPushStyle();
    
    if (isHandOut) {
        
        if(isReadyFinish && isReady){
            if (bpm>10) {
                bpm.set((bpm-2)*0.98);
            }else{
                bpm.set(1);
            }
            bpmTapper.setBpm(bpm);
            
            if(leapmotionStrength>0.2){
                float volDown = (leapmotionStrength-0.2)*0.98;
                leapmotionStrength = volDown;
            }else{
                leapmotionStrength = 0;
            }
            
            if(bpm<10){
                isReady = false;
                isReadyFinish = false;
                isWaiting = true;
                frontVideos[videoIndex]->stop();
                frontVideos[videoIndex]->setFrame(1);
                //backVideos[0]->setFrame(1);
                backVideos[0]->play();
                effect.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable		= false;
                effect.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable	= false;
                effect.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable	= false;
                bpm.set(60);

            }
            if(bpm<20){
                effect.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable		= true;
                effect.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable	= true;
                effect.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable	= true;
            
            }
        }

    }else{
        effect.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable		= false;
        effect.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable	= false;
        effect.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable	= false;
        if(isReady && !isReadyFinish){
            if (!backVideos[1]->isPlaying()) {
                backVideos[0]->stop();
                backVideos[1]->setFrame(1);
                backVideos[1]->play();
            }
            
        }
        if(isReady && isReadyFinish){
            backVideos[1]->stop();
            if(!frontVideos[videoIndex]->isPlaying()){
                frontVideos[videoIndex]->play();
            }
        }
    }
    
    if(abs(bpm-prevBPM)>5){
        videoSpeed.set(ofMap(bpm,0,160,0,2));
        prevBPM = bpm;
    }else{
        
        
    }
    
    float cacheBPM = bpmTapper.bpm();
    
    if(cacheBPM>160){
        cacheBPM = 160;
    }else if(cacheBPM<0){
        cacheBPM = 0;
    }
    
    bpm.set(cacheBPM);
    videoVolume.set(ofMap(leapmotionStrength, 0, 300.0, 0, 1));
    
    if(isReadyFinish && isReady){
        frontVideos[videoIndex]->setSpeed(videoSpeed);
        frontVideos[videoIndex]->setVolume(videoVolume);
    }else{
        bpmTapper.setBpm(60.0);
        backVideos[0]->setSpeed(1.0);
        backVideos[0]->setVolume(0);
    }
    
}

void ofApp::exit(){
    for (int i = 0;i < frontVideos.size();i++){
        frontVideos[i]->close();
    }
    for (int i = 0;i < backVideos.size();i++){
        backVideos[i]->close();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case 'G':
        case 'g': toggleGuiDraw = !toggleGuiDraw; break;
            
        default: break;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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

bool ofApp::handInArea(Vector pos){
    bool isit = true;
    isit = pos.x > centerSpherePos.x+(hitCircle/2) ? false : isit;
    isit = pos.y > centerSpherePos.y+(hitCircle/2) ? false : isit;
    isit = pos.z > centerSpherePos.z+(hitCircle/2) ? false : isit;
    isit = pos.x < centerSpherePos.x-(hitCircle/2) ? false : isit;
    isit = pos.y < centerSpherePos.y-(hitCircle/2) ? false : isit;
    isit = pos.z < centerSpherePos.z-(hitCircle/2) ? false : isit;
    return isit;
}

void ofApp::addVideo(string file){
    frontVideos.push_back(new ofxHapPlayer());
    frontVideos.back()->loadMovie(file);

}

void ofApp::particleShow(){
    for (int i=0; i<fingerPos.size(); i++) {
        for(int j =0;j<4;j++){
        ofVec3f v = ofVec3f(ofRandomuf(),
                            ofRandomuf(),
                            ofRandomuf());
        basecolor.setHue(ofRandom(160, 220));
        particle.emitParticle(fingerPos[i], (ofVec3f(translateValue[i].x,translateValue[i].y,translateValue[i].z)-fingerPos[i])*v, j,basecolor);
        }
    }
}
