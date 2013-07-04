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

#ifndef _LABEL_H
#define _LABEL_H

#include "EAVIGUI.interfaceObject.h"
#include <iostream>
//#include "ofmain.h"

using namespace std;

namespace EAVIGUI {

    class Label : public InterfaceObject {
    public:
        enum textHorizontalJustifyOptions {JUSTIFYLEFT, JUSTIFYCENTER, JUSTIFYRIGHT};
        enum textVerticalJustifyOptions {JUSTIFYTOP, JUSTIFYCENTERV, JUSTIFYBOTTOM};
        Label(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofTrueTypeFont *_font, string _text, ofColor _colour);
        void setText(string t);
        void fitToText();
        void setColour(ofColor col);
        void setBackgroundColour(ofColor col);
        void setTouchDownColour(ofColor col);
        void touchDown(ofTouchEventArgs &touch);
        void touchUp(ofTouchEventArgs &touch);
        void touchExit(ofTouchEventArgs &touch);
        void setHorizontalTextJustification(textHorizontalJustifyOptions newOption);
        void setVerticalTextJustification(textVerticalJustifyOptions newOption);
        void setRoundedCorners(bool val);
    protected:
        ofTrueTypeFont *font;
        ofColor colour, backgroundColour, touchedColour;
        void drawToBuffer();
        string text;
        textHorizontalJustifyOptions hjustify;
        textVerticalJustifyOptions vjustify;
        bool roundedCorners;
        
    };

}
#endif

