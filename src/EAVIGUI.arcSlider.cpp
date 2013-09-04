//
//  EAVIGUI.arcSlider.cpp
//  FishPolice
//
//  Created by Chris on 30/08/2013.
//
//

#include "EAVIGUI.arcSlider.h"
#include "geomFunctions.h"

namespace EAVIGUI {
    ArcSlider::ArcSlider(InterfaceListener *_listener, int _id, int _x, int _y, string barImageFileName, string sliderImageFileName, string barTouchImageFileName, string sliderTouchImageFileName, ofPoint _arcStart, ofPoint _arcEnd, float _arcSize) : ImageSlider(_listener, _id, _x, _y, barImageFileName, sliderImageFileName, barTouchImageFileName, sliderTouchImageFileName) {
        arcStart = _arcStart;
        arcEnd = _arcEnd;
        arcAngle = _arcSize;

        //find center of the circle that the arc is part of
        midPoint = (arcStart + arcEnd) / 2.0;
        C = ofDist(arcStart.x, arcStart.y, arcEnd.x, arcEnd.y); //chord length
        R = C / sqrt(2 - (2 * cos(arcAngle)));
        N = ((arcEnd - arcStart) / 2.0).getPerpendicular();
        float d = ofDist(arcStart.x, arcStart.y, midPoint.x, midPoint.y);
        float t = sqrt((R*R) - (d * d));
//        Q1 = midPoint - (t * N);
        Q2 = midPoint + (t * N);
        
        float a1 = angleBetween(arcStart.x, arcStart.y, Q2.x, Q2.y);
        float a2 = angleBetween(arcEnd.x, arcEnd.y, Q2.x, Q2.y);
        if (a1 < 0) a1 += TWO_PI;
        if (a2 < 0) a2 += TWO_PI;
        bound1 = min(a1, a2);
        bound2 = max(a1, a2);
//        cout << "Bounds 1: " << bound1 << ", " << bound2 << endl;
        if (bound1 < arcAngle && bound2 > TWO_PI - arcAngle) {
            angleFromCenter = bound1 - (arcAngle / 2.0);
        }else{
            angleFromCenter = bound1 + (arcAngle / 2.0);
        }
        intersectPoint = Q2 + ofVec2f(cos(angleFromCenter) * R, sin(angleFromCenter) * R);
        
        float sliderDistToCenter = ofDist(0, 0, sliderIm.getWidth()/2.0, sliderIm.getHeight()/2.0);
        highDist = R + sliderDistToCenter;
        lowDist = R - sliderDistToCenter;
        
        //calc circumference
        float circumference = TWO_PI * R;
        sliderAngleWidth = sliderIm.getWidth() / circumference * TWO_PI;

    }
    
    void ArcSlider::drawToBuffer() {
        if(touchTarget > -1) {
            barTouchIm.draw(0,0);
        }else{
            barIms[currentBarIm].draw(0,0);
        }
//        ofColor(255,0,0);
//        ofFill();
//        ofCircle(arcStart.x, arcStart.y, 10);
//        ofCircle(arcEnd.x, arcEnd.y, 10);
//        
//        ofCircle(midPoint, 10);
//        ofCircle(Q2, 7);
//        ofLine(midPoint, midPoint + (100 * N));
        
//        ofSetColor(ofColor::red);
//        ofLine(intersectPoint, Q2);
//        ofNoFill();
//        ofCircle(intersectPoint, 15);
        if (isEnabled()) {
            ofSetColor(255);
            glPushMatrix();
            ofTranslate(intersectPoint);
            ofTranslate(-sliderIm.getWidth()/2.0, -sliderIm.getHeight()/2.0);
            
            ofTranslate(ofVec2f(sliderIm.getWidth() / 2.0, sliderIm.getHeight() / 2.0));
            ofRotateZ((angleFromCenter / TWO_PI * 360.0) + 90);
            ofTranslate(-ofVec2f(sliderIm.getWidth()/2.0, sliderIm.getHeight()/2.0));
            if (touchTarget > -1) {
                sliderTouchIm.draw(0,0);
            }else{
                sliderIm.draw(0,0);
            }
            ofPopMatrix();
        }
    }

    void ArcSlider::moveSlider(ofTouchEventArgs &touch) {
        float theta = angleBetween(touch.x, touch.y, Q2.x, Q2.y);
        //wrap?
        if (theta < 0) theta += TWO_PI;
        cout << theta << endl;
        bool validAngle = false;
        if (bound1 < arcAngle && bound2 > TWO_PI - arcAngle) {
            validAngle = theta <= bound1 || theta >= bound2;
        }else{
            validAngle = theta >= bound1 && theta <= bound2;
        }
        if (validAngle) {
            intersectPoint = Q2 + ofVec2f(cos(angleFromCenter) * R, sin(angleFromCenter) * R);
            angleFromCenter = theta;
            if (bound1 < arcAngle && bound2 > TWO_PI - arcAngle) {
                if (angleFromCenter > bound2) {
                    value = (angleFromCenter - bound2) / arcAngle;
                }else{
                    value = (angleFromCenter + (TWO_PI - bound2)) / arcAngle;
                }
                cout << value << endl;
            }else{
                value = (angleFromCenter - bound1) / arcAngle;
            }
            sendCallback(SLIDERMOVED);
            invalidate();
        }
    }

    bool ArcSlider:: canInteractAt(int x, int y) {
        float distToCenter =  ofDist(x, y, Q2.x, Q2.y);
        bool distOK = distToCenter < highDist && distToCenter > lowDist;
        float theta = angleBetween(x, y, Q2.x, Q2.y);
        //wrap?
        if (theta < 0) theta += TWO_PI;
        bool angleOK = false;
        if (angleFromCenter < sliderAngleWidth) {
            float lb = angleFromCenter - sliderAngleWidth + TWO_PI;
            float hb = angleFromCenter + sliderAngleWidth;
            angleOK = theta > lb || theta < hb;
        }else if (angleFromCenter > TWO_PI - sliderAngleWidth) {
            float lb = angleFromCenter - sliderAngleWidth;
            float hb = angleFromCenter + sliderAngleWidth - TWO_PI;
            angleOK = theta > lb || theta < hb;
        }else{
            angleOK = theta > angleFromCenter - sliderAngleWidth && theta < angleFromCenter + sliderAngleWidth;
        }
        return angleOK && distOK;
    }

};