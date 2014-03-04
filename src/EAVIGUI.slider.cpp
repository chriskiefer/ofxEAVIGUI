//
//  EAVIGUI.slider.cpp
//  FishPolice
//
//  Created by Chris on 21/06/2013.
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

#include "EAVIGUI.slider.h"

namespace EAVIGUI {
    
    Slider::Slider(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofTrueTypeFont *_font, string _text, ofColor _textcolour, ofColor _backgroundColour, ofColor _sliderColour)
    : Label(_listener, _id, _x, _y, _w, _h, _font, _text, _textcolour) {
        backgroundColour = _backgroundColour;
        sliderColour = _sliderColour;
        value = 0;
        setIsInteractive(true);
        touchTarget = -1;
        sliderWidth = 50;
        sliderType = TOUCHANYWHERE;
        barLeft = sliderWidth / 2.0;
        barWidth = w - sliderWidth;
        sliderLeft = 0;
    }
    
    void Slider::setSliderType(sliderTypes val) {
        sliderType = val;
        invalidate();
    }
    
    void Slider::drawToBuffer() {
        ofFill();
        ofSetColor(backgroundColour);
        ofRect(0,0, w, h);
        
        switch(sliderType) {
            case TOUCHANYWHERE:
                ofSetColor(sliderColour);
                ofRect(0,0, w * value, h);
                break;
            case PICKUP:
                ofSetColor(sliderColour);
                sliderLeft = barLeft + (barWidth * value) - (sliderWidth / 2.0);
                ofRect(sliderLeft, 0, sliderWidth, h);
                break;
        }
        ofSetColor(colour);
        font->drawString(text, 0, font->getLineHeight());
    }

    void Slider::touchDown(ofTouchEventArgs &touch) {
        touchTarget = touch.id;
        moveSlider(touch);
        Label::touchDown(touch);
    }
    
    void Slider::touchMoved(ofTouchEventArgs &touch) {
        if (touch.id == touchTarget) {
            Label::touchMoved(touch);
            moveSlider(touch);
        }
    }
  
    void Slider::moveSlider(ofTouchEventArgs &touch) {
        bool validMove = false;
        switch(sliderType) {
            case TOUCHANYWHERE:
                value = (float)touch.x / (float)w;
                validMove = true;
                break;
            case PICKUP:
                if (touch.x >= sliderLeft && touch.x < sliderLeft + sliderWidth) {
                    float newValue = (touch.x - (sliderWidth / 2.0)) / barWidth;
                    if (newValue >=0 && newValue <= 1.0) {
                        value = newValue;
                        validMove = true;
                    }
                }
                break;
        }
        if (validMove) {
            sendCallback(SLIDERMOVED);
            invalidate();
        }
        
    }
    
    void Slider::touchUp(ofTouchEventArgs &touch) {
        Label::touchUp(touch);
        touchTarget = -1;
    }
    
    void Slider::touchUpExternal(ofTouchEventArgs &touch) {
        Label::touchUpExternal(touch);
        touchTarget = -1;
    }
    
    void Slider::touchExit(ofTouchEventArgs &touch) {
        Label::touchExit(touch);
        touchTarget = -1;
    }

    
    void Slider::setValue(float val) {
        value = val;
        invalidate();
    }

    bool Slider::keepThisTouch(ofTouchEventArgs &touch) {
        return exitFlickDetection; //don't check vel, as slider is probably quite thin in one dimension, making measurement difficult
    }


};