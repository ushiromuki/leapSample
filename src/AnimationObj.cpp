//
//  AnimationObj.cpp
//  leapSample
//
//  Created by 大聖堂 on 2015/08/20.
//
//

#include "AnimationObj.h"

void AnimationObj::setup(){
    
    clamp = false;
    easingType = ofxTween::easeInOut;
    easestring = "ease in/out";
    
    twn.setParameters(1,easeExpo, ofxTween::easeInOut, 100, 200, 800, 0);
    
}

void AnimationObj::update(){
    
}

void AnimationObj::draw(){
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofTranslate((ofGetWidth()/2)-twn.update(), ofGetHeight()/2);
    ofCircle((ofGetWidth()/2)-twn.update(), ofGetHeight()/2, twn.update());
    ofPopMatrix();
}

void AnimationObj::reset(){
    if(twn.isCompleted()){
        twn.setParameters(1,easeExpo, ofxTween::easeInOut, 100, 200, 800, 0);
        twn.start();
    }
   }
