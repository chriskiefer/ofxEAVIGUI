//
//  EAVIGUI.imagePanel.cpp
//  FishPolice
//
//  Created by Chris on 27/08/2013.
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

#include "EAVIGUI.imagePanel.h"

namespace EAVIGUI {
    ImagePanel::ImagePanel(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofColor _colour, ofColor _borderColour,
                           unsigned int _borderWidth, string fileName) : Panel(_listener, _id, _x, _y, _w, _h, _colour, _borderColour), borderWidth(_borderWidth)  {
        im.loadImage(ofToDataPath(fileName));
        
    }

    void ImagePanel::drawToBuffer() {
        Panel::drawToBuffer();
        ofSetColor(colour);
        im.draw(0,0,w,h);
    }
    
};


