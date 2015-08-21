//
//  AnimationObj.h
//  leapSample
//
//  Created by 大聖堂 on 2015/08/20.
//
//

#ifndef __leapSample__AnimationObj__
#define __leapSample__AnimationObj__

#include "ofMain.h"
#include "ofxTween.h"

class AnimationObj{
public:
    void setup();
    void update();
    void draw();
    
    void reset();
    
    
    ofxEasingExpo easeExpo;
    float clamp;
    ofxTween::ofxEasingType easingType;
    string easestring;
    
    ofxTween twn;
};


#endif /* defined(__leapSample__AnimationObj__) */
