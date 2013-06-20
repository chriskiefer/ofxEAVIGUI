//
//  EAVIGUI.checkbox.h
//  SonicTag3
//
//  Created by Chris on 20/06/2013.
//
//

#ifndef __SonicTag3__EAVIGUI_checkbox__
#define __SonicTag3__EAVIGUI_checkbox__

#include "EAVIGUI.label.h"
#include <iostream>


namespace EAVIGUI {
    
    class CheckBox : public Label {
    public:
        enum checkBoxEvents {STATECHANGED};
        CheckBox(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofTrueTypeFont *_font, string _text, ofColor _colour);
        void setColour(ofColor col);
        void fitToText();
        void setChecked(bool newVal);
        bool isChecked(){return checked;}
        void drawToBuffer();
        void touchUp(ofTouchEventArgs &touch);
    protected:
        ofColor colour, borderColour;
        int checkBoxSize;
        int gap;
        bool checked;
        
    };
    
}

#endif /* defined(__SonicTag3__EAVIGUI_checkbox__) */
