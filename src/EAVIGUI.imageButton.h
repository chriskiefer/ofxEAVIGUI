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

#ifndef sonicTag2_imageButton_h
#define sonicTag2_imageButton_h

#include "EAVIGUI.interfaceObject.h"
#include <iostream>

using namespace std;

namespace EAVIGUI {
  
    class ImageButton : public InterfaceObject {
    public:
        ImageButton(InterfaceListener *_listener, int _id, int _x, int _y, string imageFileName, string touchImageFilename);
        ImageButton(InterfaceListener *_listener, int _id, int _x, int _y, ofImage _im, ofImage _touchIm);
        void setColor(ofColor newCol) {imColor = newCol; invalidate();}
        ofImage getImage() {return im;}
        void loadNewImage(string imageFileName);
        void setCanTouchTranparency(bool newVal);
        bool getCanTouchTranparency();
        bool canInteractAt(int x, int y);
        void touchDown(ofTouchEventArgs &touch);
        void touchUp(ofTouchEventArgs &touch);
        void touchExit(ofTouchEventArgs &touch);
        void setEnabled(bool newEnabled);
        void setDisabledState(string filename);
    protected:
        void init();
        virtual void drawToBuffer();
        ofImage im, touchIm;
        ofColor imColor;
        bool canTouchTranparency;  //if false, reject touches on transparent pixels
        ofImage disabledIm;
        bool hasDisabledIm;
    };
    
};

#endif
