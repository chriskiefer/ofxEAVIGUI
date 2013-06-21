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
    }
    
    void Slider::drawToBuffer() {
        ofFill();
        ofSetColor(backgroundColour);
        ofRect(0,0, w, h);
        ofSetColor(sliderColour);
        ofRect(0,0, w * value, h);
        ofSetColor(colour);
        font->drawString(text, 0, font->getLineHeight());
    }

    void Slider::touchDown(ofTouchEventArgs &touch) {
        Label::touchDown(touch);
        moveSlider(touch);
    }
    
    void Slider::touchMoved(ofTouchEventArgs &touch) {
        Label::touchMoved(touch);
        moveSlider(touch);
    }
  
    void Slider::moveSlider(ofTouchEventArgs &touch) {
        cout << touch.x << endl;
        value = (float)touch.x / (float)w;
        sendCallback(SLIDERMOVED);
        invalidate();
    }

};