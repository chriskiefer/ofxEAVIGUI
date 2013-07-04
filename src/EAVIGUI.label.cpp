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

#include "EAVIGUI.label.h"

namespace EAVIGUI {

    Label::Label(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofTrueTypeFont *_font, string _text, ofColor _color) : InterfaceObject(_listener, _id, _x, _y) {
        font = _font;
        text = _text;
        colour = _color;
        setWidth(_w);
        setHeight(_h);
        setIsInteractive(false);
        backgroundColour = ofColor(0,0,0,0);
        touchedColour = backgroundColour;
        hjustify = JUSTIFYLEFT;
        vjustify = JUSTIFYTOP;
        roundedCorners = false;
    }
    
    void Label::fitToText() {
        setWidth(font->stringWidth(text));
        setHeight(font->getLineHeight() * 1.3);
    }

    void Label::drawToBuffer() {
        ofFill();
        ofSetColor(isTouched ? touchedColour : backgroundColour);
        if (roundedCorners) {
            roundedRect(0, 0, w, h, 10);
        }else{
            ofRect(0,0, w, h);
        }
        ofSetColor(colour);
        float textleft = 0;
        switch (hjustify) {
            case JUSTIFYRIGHT:
                textleft = w - font->stringWidth(text);
                break;
            case JUSTIFYCENTER:
                textleft = (w - font->stringWidth(text)) / 2.0;
                break;
            case JUSTIFYLEFT:
                break;
        }
        float textTop = font->getLineHeight();
        switch (vjustify) {
            case JUSTIFYCENTERV:
                textTop = cy + (font->getLineHeight() / 2.0);
                break;
            case JUSTIFYBOTTOM:
                textTop = h - font->getLineHeight();
                break;
            default:
                break;
        }

        font->drawString(text, textleft, textTop);
        
    }

    void Label::setText(string t) {
        text = t ;
        invalidate();
    }

    void Label::setColour(ofColor col) {
        colour = col;
        invalidate();
    }
    
    void Label::setBackgroundColour(ofColor col) {
        backgroundColour = col;
        invalidate();
    }
    
    void Label::setTouchDownColour(ofColor col) {
        touchedColour = col;
        invalidate();
    }
    
    void Label::touchDown(ofTouchEventArgs &touch) {
        InterfaceObject::touchDown(touch);
        invalidate();
    }
    
    void Label::touchUp(ofTouchEventArgs &touch) {
        InterfaceObject::touchUp(touch);
        invalidate();
    }
    
    void Label::touchExit(ofTouchEventArgs &touch) {
        InterfaceObject::touchExit(touch);
        invalidate();
    }
    
    void Label::setHorizontalTextJustification(textHorizontalJustifyOptions newOption) {
        hjustify = newOption;
        invalidate();
    }

    void Label::setVerticalTextJustification(textVerticalJustifyOptions newOption) {
        vjustify = newOption;
        invalidate();
    }

    void Label::setRoundedCorners(bool val) {
        roundedCorners = val;
        invalidate();
    }


}
