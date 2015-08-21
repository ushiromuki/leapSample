#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "ofxGui.h"
#include "MSABPMTapper.h"
#include "ParticleEmitter.h"
#include "ofxKsmrFragmentFx.h"
#include "ofxTrueTypeFontUL2.h"
#include "ofxPDF.h"
#include "AnimationObj.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    bool handInArea(Vector pos);
    void addVideo(string file);
    void particleShow();
    
    ofxLeapMotion leap; // Leap Motion„ÅÆ„É°„Ç§„É≥„ÇØ„É©„Çπ„Çí„Ç§„É≥„Çπ„Çø„É≥„ÇπÂåñ
    ofEasyCam cam; //„Ç´„É°„É©
    ofLight light; //„É©„Ç§„Éà
    vector <ofVec3f> fingerPos;     // Êåá„ÅÆ‰ΩçÁΩÆ„ÅÆÈÖçÂàó
    vector <ofVec3f> spherePos;     // Êâã„ÅåÂèñ„ÇäÂõ≤„ÇÄÁêÉ‰Ωì„ÅÆ‰ΩçÁΩÆ„ÅÆÈÖçÂàó
    vector <float> sphereSize;      // Êâã„ÅåÂèñ„ÇäÂõ≤„ÇÄÁêÉ‰Ωì„ÅÆÂ§ß„Åç„Åï„ÅÆÈÖçÂàó
    float distance[2];
    float speedVars[2];
    Vector translateValue[2];
    
    bool isHandOut;
    
    //Users/ushiromuki/of_v0.8.4_osx_release/apps/myApps/leapSample/src/ofApp.cpp/leap motion
    Controller contoroller;
    ofVec3f centerSpherePos;
    
    //video
    int videoIndex;
    vector<ofVideoPlayer> videos;
    ofFbo buffer;
    ofRectangle mask;
    bool isStop;
    float prevBPM;
    
    
    //effect
    bool fxEnable;
    int goodCount;
    ofxKsmrFragmentFx effect;
    ofVec3f previous, current;
    deque<ofVec3f> pathVertices;
    ofMesh pathLines;
    deque<ofColor> colorMeshes;
    
    //image
    ofImage textImg[3];
    AnimationObj anime;
        
    
    //particle
    ParticleEmitter particle;
    vector<ofTexture*> texs;
    
    ofColor basecolor;
    
    // Time
    float				lastTime;
    float				deltaTime;
    msa::BPMTapper bpmTapper;
    bool isTapped;
    vector<float> bpmDownCount;
    
    
    //gui
    bool toggleGuiDraw;
    ofxPanel gui;
    ofParameter <float> guiFPS;
    ofParameter<float> guiMinFPS;
    deque<float>		deltaTimeDeque;
    ofParameter<float> leapmotionStrength;
    ofParameter<float> bpm;
    ofParameter<float> leapSpeed;
    ofParameter<float> videoSpeed;
    ofParameter<float> videoVolume;

    ofxFloatSlider hitCircle;
    ofxFloatSlider hitCircleY;
};
