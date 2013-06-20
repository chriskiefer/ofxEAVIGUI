//
//  EAVIGUI.checkbox.cpp
//  SonicTag3
//
//  Created by Chris on 20/06/2013.
//
//

#include "EAVIGUI.checkbox.h"

namespace EAVIGUI {
    CheckBox::CheckBox(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofTrueTypeFont *_font, string _text, ofColor _colour)
    : Label(_listener, _id, _x, _y, _w, _h, _font, _text, _colour)
    {
        int mindim = min(_w, _h);
        checkBoxSize = mindim * 0.8;
        gap = (mindim - checkBoxSize) / 2.0;
        setIsInteractive(true);
        checked = false;
    }

    void CheckBox::fitToText() {
        Label::fitToText();
        setHeight(font->getLineHeight() * 1.3);
    }

    void CheckBox::drawToBuffer() {
        Label::drawToBuffer();
        ofFill();
        ofSetColor(255, 255, 255, 255);
        ofRect(w - checkBoxSize - gap, gap, checkBoxSize, checkBoxSize);
        if (checked) {
            ofSetColor(100, 100, 100, 255);
            ofRect(w - checkBoxSize - gap + 2, gap + 2, checkBoxSize - 4, checkBoxSize - 4);
        }
        
    }

    void CheckBox::setChecked(bool newVal) {
        checked = newVal;
        invalidate();
        sendCallback(STATECHANGED);
    }

    void CheckBox::touchUp(ofTouchEventArgs &touch) {
        Label::touchUp(touch);
        checked = !checked;
        invalidate();
    }

}
