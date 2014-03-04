//
//  EAVIGUI.imageSlider.cpp
//  FishPolice
//
//  Created by Chris on 30/08/2013.
//
//
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

#include "EAVIGUI.imageSlider.h"

using namespace std;

namespace EAVIGUI {
    ImageSlider::ImageSlider(InterfaceListener *_listener, int _id, int _x, int _y, string barImageFileName, string sliderImageFileName,
                             string barTouchImageFileName, string sliderTouchImageFileName) : InterfaceObject(_listener, _id, _x, _y) {
        sliderIm.loadImage(ofToDataPath(sliderImageFileName));
        barIms.resize(1);
        barIms[0].loadImage(ofToDataPath(barImageFileName));
        sliderTouchIm.loadImage(ofToDataPath(sliderTouchImageFileName));
        barTouchIm.loadImage(ofToDataPath(barTouchImageFileName));
        setWidth(barIms[0].width);
        setHeight(barIms[0].height);
        value = 0.0;
        setIsInteractive(true);
        touchTarget = -1;
        sliderWidth = sliderIm.width;
        barLeft = sliderWidth / 2.0;
        barWidth = barIms[0].width - sliderWidth;
        sliderLeft = 0;
        currentBarIm = 0;
    }

    void ImageSlider::drawToBuffer() {
        if(touchTarget > -1) {
            barTouchIm.draw(0,0);
            if (isEnabled()) {
                sliderTouchIm.draw(sliderLeft, 0);
            }
        }else{
            barIms[currentBarIm].draw(0,0);
            if (isEnabled()) {
                sliderIm.draw(sliderLeft, 0);
            }
        }
    }
    
    void ImageSlider::setValue(float val) {
        value = val;
        invalidate();
    }

    void ImageSlider::touchDown(ofTouchEventArgs &touch) {
        touchTarget = touch.id;
        moveSlider(touch);
        InterfaceObject::touchDown(touch);
    }
    
    void ImageSlider::touchMoved(ofTouchEventArgs &touch) {
        if (touch.id == touchTarget) {
            InterfaceObject::touchMoved(touch);
            moveSlider(touch);
        }
    }
    
    void ImageSlider::moveSlider(ofTouchEventArgs &touch) {
        if (isEnabled()) {
            bool validMove = false;
            if (touch.x >= sliderLeft && touch.x < sliderLeft + sliderWidth) {
                float newValue = (touch.x - (sliderWidth / 2.0)) / barWidth;
                if (newValue >=0 && newValue <= 1.0) {
                    value = newValue;
                    validMove = true;
                    sliderLeft = barLeft + (barWidth * value) - (sliderWidth / 2.0);
                }
            }
            if (validMove) {
                sendCallback(SLIDERMOVED);
                invalidate();
            }
        }
    }
    
    void ImageSlider::touchUp(ofTouchEventArgs &touch) {
        InterfaceObject::touchUp(touch);
        touchTarget = -1;
        invalidate();
    }
    
    void ImageSlider::touchUpExternal(ofTouchEventArgs &touch) {
        InterfaceObject::touchUpExternal(touch);
        touchTarget = -1;
        invalidate();
    }
    
    void ImageSlider::touchExit(ofTouchEventArgs &touch) {
        InterfaceObject::touchExit(touch);
        touchTarget = -1;
        invalidate();
    }
  
    void ImageSlider::setBarImage(int val) {
        if (val >=0 && val < barIms.size()) {
            currentBarIm = val;
            invalidate();
        }
    }
    
    int ImageSlider::addBarImage(string filename) {
        ofImage im;
        im.loadImage(ofToDataPath(filename));
        barIms.push_back(im);
        return barIms.size() - 1;
    }

    bool ImageSlider::canInteractAt(int x, int y) {
        return x >= sliderLeft && x < sliderLeft + sliderWidth;
    }

};