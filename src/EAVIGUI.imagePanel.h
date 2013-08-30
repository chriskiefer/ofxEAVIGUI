//
//  EAVIGUI.imagePanel.h
//  FishPolice
//
//  Created by Chris on 27/08/2013.
//
//

#ifndef __FishPolice__EAVIGUI_imagePanel__
#define __FishPolice__EAVIGUI_imagePanel__

#include "EAVIGUI.panel.h"
#include <iostream>

using namespace std;

namespace EAVIGUI {
    
    class ImagePanel : public Panel {
    public:
        ImagePanel(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h, ofColor _colour, ofColor _borderColour,
                   unsigned int borderWidth, string fileName);
    protected:
        void drawToBuffer();
        ofImage im;
        unsigned borderWidth;

    };
    
}

#endif /* defined(__FishPolice__EAVIGUI_imagePanel__) */
