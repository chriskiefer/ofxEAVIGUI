//
//  imageButton.cpp
//  sonicTag2
//
//  Created by Chris Kiefer on 25/07/2012.
/*
 *  EAVIGUI
 *  Copyright 2010 Chris Kiefer. All rights reserved.
 *
 *	Permission is hereby granted, free of charge, to any person
 *	obtaining a copy of this software and associated documentation
 *	files (the "Software"), to deal in the Software without
 *	restriction, including without limitation the rights to use,
 *	copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the
 *	Software is furnished to do so, subject to the following
 *	conditions:
 *
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *	OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include "EAVIGUI.imageButton.h"

namespace EAVIGUI {
    
    ImageButton::ImageButton(InterfaceListener *_listener, int _id, int _x, int _y, string imageFileName, string touchImageFilename) : InterfaceObject(_listener, _id, _x, _y) {
        im.loadImage(imageFileName);
        touchIm.loadImage(touchImageFilename);
        canTouchTranparency = false;
        init();
    }
    
    ImageButton::ImageButton(InterfaceListener *_listener, int _id, int _x, int _y, ofImage _im, ofImage _touchIm) : InterfaceObject(_listener, _id, _x, _y) {
        im = _im;
        touchIm = _touchIm;
        init();
    }
    
    void ImageButton::init() {
        setWidth(im.width);
        setHeight(im.height);
        setIsInteractive(true);
        imColor = ofColor(255,255,255);
        isTouched = false;
    }
    
    void ImageButton::loadNewImage(string imageFileName) {
        im.loadImage(imageFileName);
        init();
        invalidate();
    }

 
    void ImageButton::drawToBuffer() {
        ofEnableAlphaBlending();
        ofSetColor(imColor);
        if (isTouched) {
            touchIm.draw(0,0,w,h);
        }else{
            im.draw(0,0,w,h);
        }
    }
    
    void ImageButton::touchDown(ofTouchEventArgs &touch) {
        InterfaceObject::touchDown(touch);
        invalidate();
    }
    
    void ImageButton::touchUp(ofTouchEventArgs &touch) {
        InterfaceObject::touchUp(touch);
        invalidate();
    }
    
    void ImageButton::touchExit(ofTouchEventArgs &touch) {
        InterfaceObject::touchExit(touch);
        invalidate();
    }


    
    void ImageButton::setCanTouchTranparency(bool newVal) {
        canTouchTranparency = newVal;
    }
    
    bool ImageButton::getCanTouchTranparency() {
        return canTouchTranparency;
    }
    
    bool ImageButton::canInteractAt(int x, int y) {
        bool ok = true;
        if (!canTouchTranparency) {
            ofPixels pix = im.getPixelsRef();
            int idx = pix.getPixelIndex(x, y);
            unsigned char* pixel = pix.getPixels();
            //check alpha value
            ok = pixel[idx+3] > 0;
        }
        return ok;
    }
    
    void ImageButton::setEnabled(bool newEnabled) {
        if(!newEnabled) {
            isTouched = false;
        }
        InterfaceObject::setEnabled(newEnabled);
    }



    
}
