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
    
    void onOneshotTimerEvent(int&);
    void onRepeatTimerEvent(int&);

    
    void setup(ofImage *orgImg);
    void update();
    void draw(float volume);
    
    void reset();
    
    
    ofxEasingExpo easeExpo;
    float clamp;
    ofxTween::ofxEasingType easingType;
    string easestring;
    
    ofxTween twn;
    
    float speed;
    float _alpha;
    bool isRunning;
    
    unsigned char* orgPixels;
    vector<ofVec2f> dotPoints;
    vector<ofVec2f> orgPoints;
    vector<ofColor> orgColors;
    
    int IMG_W;
    int IMG_H;
};


#endif /* defined(__leapSample__AnimationObj__) */
