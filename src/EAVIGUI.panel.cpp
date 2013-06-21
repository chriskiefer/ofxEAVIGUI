//
//  panel.cpp
//
//  Created by Chris on 20/06/2013.
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

#include "EAVIGUI.panel.h"

namespace EAVIGUI {

    Panel::Panel(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofColor _colour, ofColor _borderColour) :
        InterfaceObject(_listener, _id, _x, _y)
    {
        colour = _colour;
        borderColour = _borderColour;
        setIsInteractive(false);
        setWidth(_w);
        setHeight(_h);
        cornerRadius = 30;
    }
    
    void Panel::setCornerRadius(float val) {
        cornerRadius = val;
        invalidate();
    }

    void Panel::setColour(ofColor col) {
        colour = col;
        invalidate();
    }
    
    void Panel::drawToBuffer() {
        ofFill();
        ofSetColor(colour);
        roundedRect(0,0, w-1, h-1, cornerRadius);
        ofNoFill();
        ofSetColor(borderColour);
        roundedRect(0,0, w-1, h-1, cornerRadius);
    }
    
}
