//
//  videoThread.h
//  leapSample
//
//  Created by 大聖堂 on 2015/08/16.
//
//

#ifndef leapSample_videoThread_h
#define leapSample_videoThread_h

#pragma once

const unsigned int MULTI_THREAD_NUM = 6;

typedef struct{
    string fName;
    ofPixels pixs;
} QueImg;

class videoThreadObj : public ofThread
{
public:
    deque<QueImg> ques;
    int renderFrame = 0;
    
    void close(){ stop(); }
    bool running(){ return isThreadRunning(); }
    
    void start() {
        renderFrame = 0;
        this->startThread(false,true);   // blocking, verbose
    }
    void stop() {
        this->stopThread();
    }
    
    void threadedFunction()
    {
        while(isThreadRunning()) {
            if(!ques.empty())
            {
                QueImg i = ques.front();
                ofSaveImage( i.pixs, i.fName);
                ques.pop_front();
                
                if( lock() ){
                    renderFrame++;
                    unlock();
                }
            }
        }
    }
};

class videoThread
{
    SequenceExport expos[ MULTI_THREAD_NUM ];
    
    string outpath,format;
    int addFrame, renderFrame;
    bool isThreadRunning = false;
    
public:
    
    void setup( string outpath_, string format_ ) {
        addFrame = 0;
        renderFrame = 0;
        outpath = outpath_;
        format = format_;
    }
    
    void close(){
        stop();
    }
    
    void start()
    {
        addFrame = 0;
        renderFrame = 0;
        isThreadRunning = true;
        
        for (int i=0; i<MULTI_THREAD_NUM; i++) {
            expos[i].start();
        }
    }
    void stop(){
        isThreadRunning = false;
        for (int i=0; i<MULTI_THREAD_NUM; i++) {
            expos[i].stop();
        }
    }
    
    void addQue(ofFbo *fbo){
        ofPixels pixs;
        fbo->readToPixels( pixs );
        addQue( pixs );
    }
    
    void addQue(ofPixels pixs)
    {
        assert( outpath.length()>0 );
        assert( format.length()>0 );
        
        char fName[100];
        sprintf(fName,  "%s%.6i.%s" , outpath.c_str(), addFrame, format.c_str() );
        
        QueImg que;
        que.fName = fName;
        que.pixs = pixs;
        expos[ addFrame%MULTI_THREAD_NUM ].ques.push_back( que );
        
        addFrame++;
        getRenderFrame();
    }
    
    int getRenderFrame() {
        renderFrame = 0;
        for (int i=0; i<MULTI_THREAD_NUM; i++) {
            renderFrame += expos[i].renderFrame;
        }
        return renderFrame;
    }
    int getAddFrame() { return addFrame; }
    
    bool running(){ return isThreadRunning; }
};
#endif
