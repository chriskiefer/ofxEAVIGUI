//
//  EAVIGUI.multiImageButton.cpp
//  FishPolice
//
//  Created by Chris on 30/08/2013.
//
//

#include "EAVIGUI.multiImageButton.h"


namespace EAVIGUI {
    MultiImageButton::MultiImageButton(InterfaceListener *_listener, int _id, int _x, int _y, string state0FileName, string touchOverlayImageFilename)  : InterfaceObject(_listener, _id, _x, _y) {
        states.resize(1);
        states[0].loadImage(state0FileName);
        touchStates.resize(1);
        touchStates[0].loadImage(touchOverlayImageFilename);
        canTouchTranparency = false;
        currentState = currentTouchState = 0;
        disabledIm = states[0];
        init();
    }
    
    void MultiImageButton::init() {
        setWidth(states[0].width);
        setHeight(states[0].height);
        setIsInteractive(true);
        isTouched = false;
    }
    
    void MultiImageButton::drawToBuffer() {
        if (isEnabled()) {
            states[currentState].draw(0,0,w,h);
            if (isTouched) {
                touchStates[currentTouchState].draw(0,0,w,h);
            }
        }else{
            disabledIm.draw(0,0,w,h);
        }
    }

    void MultiImageButton::setCanTouchTranparency(bool newVal) {
        canTouchTranparency = newVal;
    }
    
    bool MultiImageButton::getCanTouchTranparency() {
        return canTouchTranparency;
    }
    
    bool MultiImageButton::canInteractAt(int x, int y) {
        bool ok = true;
        if (!canTouchTranparency) {
            ofPixels pix = states[currentState].getPixelsRef();
            int idx = pix.getPixelIndex(x, y);
            unsigned char* pixel = pix.getPixels();
            //check alpha value
            ok = pixel[idx+3] > 0;
        }
        return ok;
    }
    
    void MultiImageButton::touchDown(ofTouchEventArgs &touch) {
        InterfaceObject::touchDown(touch);
        invalidate();
    }
    
    void MultiImageButton::touchUp(ofTouchEventArgs &touch) {
        InterfaceObject::touchUp(touch);
        invalidate();
    }
    
    void MultiImageButton::touchExit(ofTouchEventArgs &touch) {
        InterfaceObject::touchExit(touch);
        invalidate();
    }
    
    void MultiImageButton::setEnabled(bool newEnabled) {
        if(!newEnabled) {
            isTouched = false;
        }
        InterfaceObject::setEnabled(newEnabled);        
    }
    
    void MultiImageButton::setState(int val) {
        if (val >= 0 && val < states.size()) {
            currentState = val;
            invalidate();
        }
    }

    void MultiImageButton::setTouchState(int val) {
        if (val >= 0 && val < touchStates.size()) {
            currentTouchState = val;
            invalidate();
        }
    }

    int MultiImageButton::addState(string imageFileName) {
        ofImage im;
        im.loadImage(ofToDataPath(imageFileName));
        states.push_back(im);
        if (im.width > w || im.height > h) {
            setWidth(im.width);
            setHeight(im.height);
        }
        return states.size() - 1;
    }

    int MultiImageButton::addTouchState(string imageFileName) {
        ofImage im;
        im.loadImage(ofToDataPath(imageFileName));
        touchStates.push_back(im);
        if (im.width > w || im.height > h) {
            setWidth(im.width);
            setHeight(im.height);
        }
        return touchStates.size() - 1;
    }

    void MultiImageButton::addDisabledState(string filename) {
        disabledIm.loadImage(ofToDataPath(filename));
    }

    
};