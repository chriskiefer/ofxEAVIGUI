//
//  EAVIGUI.imageSlider.h
//  FishPolice
//
//  Created by Chris on 30/08/2013.
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

#ifndef __FishPolice__EAVIGUI_imageSlider__
#define __FishPolice__EAVIGUI_imageSlider__

#include <iostream>

#include "EAVIGUI.interfaceObject.h"


namespace EAVIGUI {
    
    class ImageSlider : public InterfaceObject {
    public:
        enum sliderEvents {SLIDERMOVED};
        ImageSlider(InterfaceListener *_listener, int _id, int _x, int _y, std::string barImageFileName, std::string sliderImageFileName, std::string barTouchImageFileName, std::string sliderTouchImageFileName);
        void setValue(float val);
        float getValue() {return value;}
        bool canInteractAt(int x, int y);
        void setBarImage(int val);
        int addBarImage(string filename);
    protected:
        void drawToBuffer();
        void touchDown(ofTouchEventArgs &touch);
        void touchMoved(ofTouchEventArgs &touch);
        void touchUp(ofTouchEventArgs &touch);
        void touchUpExternal(ofTouchEventArgs &touch);
        void touchExit(ofTouchEventArgs &touch);
        virtual void moveSlider(ofTouchEventArgs &touch);
        
        ofImage sliderIm, sliderTouchIm, barTouchIm;
        int currentBarIm;
        std::vector<ofImage> barIms;
        float value;
        float sliderWidth, sliderLeft;
        float barLeft, barWidth;
        int touchTarget;        
    };
    
}

#endif /* defined(__FishPolice__EAVIGUI_imageSlider__) */
