//
//  EAVIGUI.XYPad.h
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

#ifndef __FishPolice__EAVIGUI_XYPad__
#define __FishPolice__EAVIGUI_XYPad__

#include <iostream>

#endif /* defined(__FishPolice__EAVIGUI_XYPad__) */


#include "EAVIGUI.label.h"
#include <iostream>
//#include "ofmain.h"

using namespace std;

namespace EAVIGUI {
    
    class XYPad : public Label {
    public:
        XYPad(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofTrueTypeFont *_font, string _text, ofColor _textcolour, ofColor _backgroundColour, ofColor markerColour);
        inline float getXValue() {
            return xValue;
        }
        inline float getYValue() {
            return yValue;
        }
        void setXValue(float val);
        void setYValue(float val);
        void setBackground(string filename);
        void setDisabledBackground(string filename);
    protected:
        float xValue, yValue;
        ofColor markerColour;
        void drawToBuffer();
        
        void touchDown(ofTouchEventArgs &touch);
        void touchMoved(ofTouchEventArgs &touch);
        void moveMarker(ofTouchEventArgs &touch);
        float markerDiameter;
        ofImage background, disabledBackground;
        bool hasBackground, hasDisabldBackground;
        
    };
    
}
