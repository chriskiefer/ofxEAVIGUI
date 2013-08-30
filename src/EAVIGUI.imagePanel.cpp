//
//  EAVIGUI.imagePanel.cpp
//  FishPolice
//
//  Created by Chris on 27/08/2013.
//
//

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


