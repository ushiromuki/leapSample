//
//  ParticleEmitter.cpp
//  leapSample
//
//  Created by 大聖堂 on 2015/08/06.
//
//

#include "ParticleEmitter.h"

void ParticleEmitter::setup(string imagepath){
    
    for (int i = 0;i < EMIT_UNIT_NUM;i++){
        pts[i]		.assign(MAX_PARTICLE_NUM, ofVec3f(0,0,0));
        vecs[i]		.assign(MAX_PARTICLE_NUM, ofVec3f(0,0,0));
        colors[i]	.assign(MAX_PARTICLE_NUM, ofFloatColor(0,0,0));
        
        pVbo[i].setVertexData(&pts[i][0], pts[i].size(), GL_DYNAMIC_DRAW);
        pVbo[i].setColorData(&colors[i][0], colors[i].size(), GL_DYNAMIC_DRAW);
        
    }
    
    ofDisableArbTex();
    texture.loadImage(imagepath);
}

void ParticleEmitter::update(){
    for (int o = 0;o < EMIT_UNIT_NUM;o++){
        
        for (int i = 0;i < MAX_PARTICLE_NUM;i++){
            
            pts[o][i] += vecs[o][i];
            vecs[o][i] *= attenuation;
            ofFloatColor base = colors[o][i];
            colors[o][i].set( base.r,  base.g,  base.b, vecs[o][i].lengthSquared()/30.0);
            
        }
        
        pVbo[o].updateVertexData(&pts[o][0], pts[o].size());
        pVbo[o].updateColorData(&colors[o][0], colors[o].size());
        
    }
    
}

void ParticleEmitter::draw(){
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofDisableArbTex();
    ofEnablePointSprites();
    
    texture.getTextureReference().bind();
    for (int o = 0;o < EMIT_UNIT_NUM;o++){
        glPointSize(30 + o*2);
        pVbo[o].draw(GL_POINTS, 0, pts[o].size());
    }
    texture.getTextureReference().unbind();
    
    ofDisablePointSprites();
    ofEnableArbTex();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

void ParticleEmitter::drawMultiSplite(vector<ofTexture *> textures){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
    
    for (int o = 0;o < EMIT_UNIT_NUM;o++){
        textures[o % EMIT_UNIT_NUM]->bind();
        glPointSize(10 + o*5);
        pVbo[o].draw(GL_POINTS, 0, pts[o].size());
        textures[o % EMIT_UNIT_NUM]->unbind();
    }
    
    ofDisablePointSprites();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

void ParticleEmitter::setAttenuation(float val){
    attenuation = val;
}

void ParticleEmitter::emitParticle(ofVec3f pos, ofVec3f vec, int unit, ofFloatColor color){
    for (int i = 0;i < MAX_PARTICLE_NUM;i++){
        if (vecs[unit][i].lengthSquared() < 0.01){
            pts[unit][i]  = pos;
            vecs[unit][i] = vec;
            colors[unit][i] = color;
            break;
        }
    }
}