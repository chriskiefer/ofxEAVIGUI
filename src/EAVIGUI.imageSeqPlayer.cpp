//
//  EAVIGUI.imageSeqPlayer.cpp
//  FishPolice
//
//  Created by Chris on 05/09/2013.
//
//

#include "EAVIGUI.imageSeqPlayer.h"

namespace EAVIGUI {
    ImageSequencePlayer::ImageSequencePlayer(InterfaceListener *_listener, int _id, int _x, int _y, ImageSequence *seq)
    : InterfaceObject(_listener, _id, _x, _y) {
        imSeq = seq;
        framePeriod =50; //milliseconds
        lastFrameTS = ofGetElapsedTimeMillis();
        currIm = &imSeq->getImage(0);
        setWidth(currIm->getWidth()-1);
        setHeight(currIm->getHeight()-1);
        seqIndex = 0;
        setIsInteractive(false);
        backgroundColor = ofColor::black;
        alpha = 255;
    }
    
    void ImageSequencePlayer::setFrameTimeInMillis(float val) {
        framePeriod = val;
        invalidate();
    }

    void ImageSequencePlayer::update() {
        InterfaceObject::update();
        long now = ofGetElapsedTimeMillis();
        long elapsed = now - lastFrameTS;
        if (elapsed >= framePeriod) {
            seqIndex++;
            if (seqIndex == imSeq->size()) {
                seqIndex = 0;
            }
            lastFrameTS = now - (elapsed % framePeriod);
            currIm = &imSeq->getImage(seqIndex);
            invalidate();
        }
    }
    
    void ImageSequencePlayer::drawToBuffer() {
        ofFill();
        ofSetColor(backgroundColor);
        ofRect(0,0,w,h);
        ofSetColor(ofColor::white, alpha);
        currIm->draw(0,0);
    }
};