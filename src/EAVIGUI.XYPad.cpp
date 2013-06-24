//
//  EAVIGUI.XYPad.cpp
//  FishPolice
//
//  Created by Chris on 24/06/2013.
//
//

#include "EAVIGUI.XYPad.h"


namespace EAVIGUI {

    XYPad::XYPad(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofTrueTypeFont *_font, string _text, ofColor _textcolour, ofColor _backgroundColour, ofColor _markerColour)
    : Label(_listener, _id, _x, _y, _w, _h, _font, _text, _textcolour)
    {
        setBackgroundColour(_backgroundColour);
        markerColour = _markerColour;
        setIsInteractive(true);
        xValue = 0.5;
        yValue = 0.5;
    }

    
    void XYPad::drawToBuffer() {
        ofFill();
        ofSetColor(backgroundColour);
        ofRect(0,0,w,h);
        ofSetColor(markerColour);
        ofNoFill();
        float xpos = xValue * w;
        float ypos = yValue * h;
        ofCircle(xpos, ypos, 40);
        ofSetColor(230,230,230,100);
        ofLine(xpos, 0, xpos, h);
        ofLine(0,ypos, w, ypos);
    }
    
    void XYPad::touchDown(ofTouchEventArgs &touch) {
        moveMarker(touch);
        Label::touchDown(touch);
    }
    
    void XYPad::touchMoved(ofTouchEventArgs &touch) {
        moveMarker(touch);
        Label::touchMoved(touch);
    }
    
    void XYPad::moveMarker(ofTouchEventArgs &touch) {
        xValue = touch.x / static_cast<float>(w);
        yValue = touch.y / static_cast<float>(h);
        invalidate();
    }


};