//
//  EAVIGUI.slider.cpp
//  FishPolice
//
//  Created by Chris on 21/06/2013.
//
//

#include "EAVIGUI.slider.h"

namespace EAVIGUI {
    
    Slider::Slider(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofTrueTypeFont *_font, string _text, ofColor _textcolour, ofColor _backgroundColour, ofColor _sliderColour)
    : Label(_listener, _id, _x, _y, _w, _h, _font, _text, _textcolour) {
        backgroundColour = _backgroundColour;
        sliderColour = _sliderColour;
        value = 0;
        setIsInteractive(true);
        touchTarget = -1;
        barWidth = 50;
        sliderType = BASIC;
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
            case BASIC:
                ofSetColor(sliderColour);
                ofRect(0,0, w * value, h);
                break;
            case BAR:
                ofSetColor(sliderColour);
                float barLeft = MAX(0,(w * value) - (barWidth / 2));
                float actualBarWidth = barWidth;
                if (value > 0.5) {
                    actualBarWidth = min(barWidth, w - barLeft);
                }else{
                    actualBarWidth = min(barWidth, barWidth + (barLeft - (barWidth / 2)));
                }
                ofRect(barLeft, 0, actualBarWidth, h);
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
            case BASIC:
                validMove = true;
                break;
            case BAR:
                float sliderx = value * static_cast<float>(w);
                if (fabs(sliderx - touch.x) < (barWidth / 2.0)) {
                    validMove = true;
                }
                break;
        }
        if (validMove) {
            value = (float)touch.x / (float)w;
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