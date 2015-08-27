//
//  ParticleEmitter.cpp
//  leapSample
//
//  Created by 大聖堂 on 2015/08/06.
//
//

#include "ParticleEmitter.h"

void ParticleEmitter::setup(string imagepath){
    
    shader.load("shaders/Billboard");
    
    for (int i = 0;i < EMIT_UNIT_NUM;i++){
        pts[i]		.assign(MAX_PARTICLE_NUM, ofVec3f(0,0,0));
        vecs[i]		.assign(MAX_PARTICLE_NUM, ofVec3f(0,0,0));
        colors[i]	.assign(MAX_PARTICLE_NUM, ofFloatColor(0,0,0));
        angles[i]   .assign(MAX_PARTICLE_NUM, ofRandom(0, 360));
        pointsz[i]   .assign(MAX_PARTICLE_NUM, ofRandom(26, 32));
        
        pVbo[i].setVertexData(&pts[i][0], pts[i].size(), GL_DYNAMIC_DRAW);
        pVbo[i].setColorData(&colors[i][0], colors[i].size(), GL_DYNAMIC_DRAW);
        
    }
    
    ofDisableArbTex();
    texture.loadImage(imagepath);
    for (int o=0; o<EMIT_UNIT_NUM; o++) {
        shader.begin();
        int sizeLoc = shader.getAttributeLocation("size");
        pVbo[o].setAttributeData(sizeLoc , &pointsz[o][0], 1, pointsz[o].size(), GL_DYNAMIC_DRAW);
        // rotate the snow based on the velocity
        int angleLoc = shader.getAttributeLocation("angle");
        pVbo[o].setAttributeData(angleLoc, &angles[o][0], 1, angles[o].size(), GL_DYNAMIC_DRAW);
        shader.end();
    }
    
}

void ParticleEmitter::update(){
    for (int o = 0;o < EMIT_UNIT_NUM;o++){
        
        for (int i = 0;i < MAX_PARTICLE_NUM;i++){
            
            pts[o][i] += vecs[o][i];
            vecs[o][i] *= attenuation;
            ofFloatColor base = colors[o][i];
            colors[o][i].set( base.r,  base.g,  base.b, vecs[o][i].lengthSquared()/10.0);
            angles[o][i] += vecs[o][i].lengthSquared()/360;
            
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
    shader.begin();
    ofEnablePointSprites();
    
    for (int o = 0;o < EMIT_UNIT_NUM;o++){
        textures[o % EMIT_UNIT_NUM]->bind();
        int sizeLoc = shader.getAttributeLocation("size");
        pVbo[o].updateAttributeData(sizeLoc , &pointsz[o][0], pointsz[o].size());
        int angleLoc = shader.getAttributeLocation("angle");
        pVbo[o].updateAttributeData(angleLoc, &angles[o][0], angles[o].size());
        pVbo[o].draw(GL_POINTS, 0, pts[o].size());
        textures[o % EMIT_UNIT_NUM]->unbind();
    }
    
    ofDisablePointSprites();
    shader.end();
    ofDisableBlendMode();
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