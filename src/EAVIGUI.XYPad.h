//
//  EAVIGUI.XYPad.h
//  FishPolice
//
//  Created by Chris on 24/06/2013.
//
//

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
    protected:
        float xValue, yValue;
        ofColor markerColour;
        void drawToBuffer();
        
        void touchDown(ofTouchEventArgs &touch);
        void touchMoved(ofTouchEventArgs &touch);
        void moveMarker(ofTouchEventArgs &touch);

        
    };
    
}
