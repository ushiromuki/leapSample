//
//  AnimationObj.cpp
//  leapSample
//
//  Created by 大聖堂 on 2015/08/20.
//
//

#include "AnimationObj.h"
#include "ofxAnimationPrimitives.h"

ofxAnimationPrimitives::Timer oneshot;
ofxAnimationPrimitives::Timer repeat;

void AnimationObj::onOneshotTimerEvent(int&)
{
    ofBackground(255, 0, 0);
    cout << "oneshot: " << ofGetElapsedTimef() << endl;
}

void AnimationObj::onRepeatTimerEvent(int&)
{
    ofBackground(255);
    cout << "repeat: " << ofGetElapsedTimef() << endl;
}


void AnimationObj::setup(ofImage *orgImg){
    
    clamp = false;
    easingType = ofxTween::easeOut;
    easestring = "ease in/out";
    
    ofAddListener(oneshot.timerEvent, this, &AnimationObj::onOneshotTimerEvent);
    ofAddListener(repeat.timerEvent, this, &AnimationObj::onRepeatTimerEvent);
    
    //とばす
    int step = 5;
    speed = 1.2;
    
    IMG_W = (int)orgImg->getWidth();
    IMG_H = (int)orgImg->getHeight();
    
    //orgImg->setImageType(OF_IMAGE_GRAYSCALE);
    orgPixels = orgImg->getPixels();
    for(int y = 0; y < IMG_H; y = y + step){
        for(int x = 0; x < IMG_W; x= x+step){
            int red = orgPixels[(IMG_W * y + x) * 4];
            int green = orgPixels[(IMG_W * y + x )* 4 + 1];
            int blue = orgPixels[(IMG_W * y + x )* 4 + 2];
            int alpha = orgPixels[y*IMG_W*4+x*4+3];
            
            if(alpha > 10){
                dotPoints.push_back(ofVec2f(x ,y));
                orgColors.push_back(ofColor(red,green,blue));
            }
        }
    }
    //twn.setParameters(1,easeExpo,easingType,255,0,1000,500);
    isRunning = false;
    
}

void AnimationObj::update(){
    
}

void AnimationObj::draw(float volume){
    ofPushMatrix();
    ofScale(1, -1);
    ofTranslate(-(IMG_W/2), -(IMG_H/2));
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    for(int i = 0; i < dotPoints.size(); i++){
        ofSetColor(orgColors[i]);
        ofCircle(dotPoints[i].x + speed * ofRandom(-1.0 , 1.0) , dotPoints[i].y + speed * ofRandom(-1.0 , 1.0) , ofRandom(2, 10)*volume);
    }
    ofPopMatrix();
    

}

void AnimationObj::reset(){
    oneshot.start(1,false);
}
