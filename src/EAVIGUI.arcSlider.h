//
//  EAVIGUI.arcSlider.h
//  FishPolice
//
//  Created by Chris on 30/08/2013.
//
//

#ifndef __FishPolice__EAVIGUI_arcSlider__
#define __FishPolice__EAVIGUI_arcSlider__

#include <iostream>

#include "EAVIGUI.imageSlider.h"
#include <iostream>

using namespace std;

namespace EAVIGUI {
    
    class ArcSlider : public ImageSlider {
    public:
        ArcSlider(InterfaceListener *_listener, int _id, int _x, int _y, string barImageFileName, string sliderImageFileName, string barTouchImageFileName, string sliderTouchImageFileName, ofPoint arcStart, ofPoint arcEnd, float arcSize);
//        bool canInteractAt(int x, int y);
        void moveSlider(ofTouchEventArgs &touch);
        bool canInteractAt(int x, int y);
    protected:
        void drawToBuffer();
        ofVec2f arcStart, arcEnd;
        float arcAngle;
        float C,R;
        ofVec2f midPoint;
        ofVec2f Q1, Q2, N;
        ofVec2f intersectPoint;
        float bound1, bound2;
        float angleFromCenter;
        float highDist, lowDist;
        float sliderAngleWidth;
    };
    
}

#endif /* defined(__FishPolice__EAVIGUI_arcSlider__) */
