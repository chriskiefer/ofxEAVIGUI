//
//  EAVIGUI.XYPad.cpp
//  FishPolice
//
//  Created by Chris on 24/06/2013.
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
        markerDiameter = min(w,h) * 0.2;
    }

    
    void XYPad::drawToBuffer() {
        ofFill();
        ofSetColor(backgroundColour);
        ofRect(0,0,w,h);
        ofSetColor(markerColour);
        ofNoFill();
        float xpos = xValue * w;
        float ypos = yValue * h;
        ofCircle(xpos, ypos, markerDiameter);
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

    void XYPad::setXValue(float val) {
        xValue = val;
        invalidate();
    }
    
    void XYPad::setYValue(float val) {
        yValue = val;
        invalidate();
    }


};