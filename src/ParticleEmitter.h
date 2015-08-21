//
//  ParticleEmitter.h
//  leapSample
//
//  Created by 大聖堂 on 2015/08/06.
//
//

#ifndef __leapSample__ParticleEmitter__
#define __leapSample__ParticleEmitter__

#include "ofMain.h"

#define MAX_PARTICLE_NUM 2500
#define EMIT_UNIT_NUM 4

class ParticleEmitter{
public:
    void setup(string imagepath);
    void update();
    void draw();
    void drawMultiSplite(vector<ofTexture*> textures);
    
    void setAttenuation(float val);
    void setLife(float val);
    void emitParticle(ofVec3f pos,ofVec3f vec,int unit, ofFloatColor color);
    
    ofVbo pVbo[EMIT_UNIT_NUM];
    vector<ofVec3f>			pts[EMIT_UNIT_NUM];
    vector<ofVec3f>			vecs[EMIT_UNIT_NUM];
    vector<ofFloatColor>	colors[EMIT_UNIT_NUM];
    
    float attenuation;
    float life;
    ofImage texture;
};


#endif /* defined(__leapSample__ParticleEmitter__) */
