//
//  EAVIGUI.arcSlider.cpp
//  FishPolice
//
//  Created by Chris on 30/08/2013.
//
//

#include "EAVIGUI.arcSlider.h"

namespace EAVIGUI {
    ArcSlider::ArcSlider(InterfaceListener *_listener, int _id, int _x, int _y, string barImageFileName, string sliderImageFileName, string barTouchImageFileName, string sliderTouchImageFileName, ofPoint _arcStart, ofPoint _arcEnd, float _arcSize) : ImageSlider(_listener, _id, _x, _y, barImageFileName, sliderImageFileName, barTouchImageFileName, sliderTouchImageFileName) {
        arcStart = _arcStart;
        arcEnd = _arcEnd;
        arcSize = _arcSize;
    }
    
    void ArcSlider::drawToBuffer() {
        if(touchTarget > -1) {
            barTouchIm.draw(0,0);
            if (isEnabled()) {
//                sliderTouchIm.draw(sliderLeft, 0);
            }
        }else{
            barIms[currentBarIm].draw(0,0);
            if (isEnabled()) {
//                sliderIm.draw(sliderLeft, 0);
            }
        }
        ofColor(255,0,0);
        ofFill();
        ofCircle(arcStart.x, arcStart.y, 10);
        ofCircle(arcEnd.x, arcEnd.y, 10);
        
        //find center of the circle that the arc is part of
        ofPoint midPoint = (arcStart + arcEnd) / 2.0;
        ofCircle(midPoint, 10);
        float k = ofDist(arcStart.x, arcStart.y, midPoint.x, midPoint.y);
        float distToCentre = k / tan(arcSize/2.0);
    }

};