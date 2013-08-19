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

#ifndef _INTERFACE_OBJECT_H
#define _INTERFACE_OBJECT_H

#include "ofMain.h"
#include "TargetConditionals.h"
#include <list>
#include <map>

namespace EAVIGUI {

    class InterfaceObject;
    class baseEffect;


    class InterfaceListener {
    public:
        virtual void handleInterfaceEvent(int id, int eventTypeId, InterfaceObject *object) {};
    };

    enum mouseStates {
        SOMEWHERELSE, OVER, PRESSED
    };

    enum screenRotations {
        ROT0, ROT90, ROT180, ROT270
    };

    enum screenRotationModes {
        ROTMODE_NONE, ROTMODE_REFLECT, ROTMODE_ROTATE, ROTMODE_SPECIFY
    };

    class InterfaceObject
    {

    public:

        enum InterfaceManagerEvents {
            TOUCHDOUBLETAP, TOUCHDOWN, TOUCHMOVED, TOUCHMOVED_EXTERNAL, TOUCHUP, TOUCHEXIT, TOUCHUP_EXTERNAL, MOUSEPRESSED, MOUSERELEASED, MOUSEDRAGGED, MOUSEMOVED, TOUCHEXITFLICK
        };
        
        
        InterfaceObject(InterfaceListener *_listener, int _id, int _x, int _y);
        InterfaceObject(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h);
        ~InterfaceObject();

        virtual void setup();
        virtual void update();
        void draw();
        virtual void mouseMoved( int x, int y );
        virtual void mouseExit();
        virtual void mousePressed( int x, int y, int button=0 );
        virtual void mouseReleased();
        virtual void mouseReleased(int x, int y, int button );
        virtual void mouseDragged(int x, int y, int button);
        virtual void setEnabled(bool newEnabled);
        virtual bool isEnabled();
        virtual void touchDown(ofTouchEventArgs &touch);
        virtual void touchUp(ofTouchEventArgs &touch);
        virtual void touchUpExternal(ofTouchEventArgs &touch);
        virtual void touchMovedExternal(ofTouchEventArgs &touch);
        virtual void touchDoubleTap(ofTouchEventArgs &touch);
        virtual void touchMoved(ofTouchEventArgs &touch);
        virtual void touchExit(ofTouchEventArgs &touch);
        virtual bool keepThisTouch(ofTouchEventArgs &touch); //don't exit, continue piping this touch to the control when out of its bounds
        
        
        void addChild(InterfaceObject* child);
        bool hasChildren();
        void invalidate();
        void setVisible(bool val, bool noFade = false);
        bool isVisible();
        bool isInteractive();
        void setIsInteractive(bool val) {interactive = val;}
        InterfaceObject* getParent();
        void setParent(InterfaceObject* newParent);
        void setScreenRotation(screenRotations rot);
        int getWidth();
        int getHeight();
        int getScaledWidth();
        int getScaledHeight();
        void setWidth(int width);
        void setHeight(int height);
        ofRectangle getRectWithScreenRotation();
        
        virtual void deviceOrientationChanged(int newOrientation);
        void updatePosition();
        InterfaceObject* setNoScreenRotation(){screenRotMode = ROTMODE_NONE; return this;}
        InterfaceObject* setScreenRotationPoints(ofPoint r90, ofPoint r180, ofPoint r270);
        InterfaceObject* setScreenRotationAroundCenter();
        InterfaceObject* setRelativePositioning(float xpos, float ypos);
        InterfaceObject* setRelativePositioning(float xpos, int xPixelOffset, float ypos, int yPixelOffset);
        void transformTouchForScreenRotation(ofTouchEventArgs &touch);
        
        virtual bool canInteractAt(int x, int y); //accept touch/mouse event here? or pass on to next object

        
        screenRotationModes screenRotMode;
        ofPoint rotPt0, rotPt90, rotPt180, rotPt270;
        vector<InterfaceObject*>* getChildren();
        int x, y;
        InterfaceListener *listener;
        int id;
        int red,green,blue,alpha;
        ofTouchEventArgs *touchArgs;
        int screenRotation;
        int xScreenRotShift, yScreenRotShift;
        int fadeTime;
        int fadeInTimeStamp, fadeOutTimeStamp;
        
        void setScale(float newScale);
        void setAnchorPoint(float ax, float ay);
        void getAnchorPoint(float &ax, float &ay);

        bool pulsate;
        float pulsateSpeed;
        float pulsateMin, pulsateMax;
        
        void addEffect(baseEffect *newEffect);
        void dropEffects();
        
        void setRotation(float angle);
        float getRotation();
        void getTouchDownPoint(int &x, int &y);
        
        void enableExitFlickDetection(bool val);
        void enableExternalTouchUp(bool val);
        virtual void touchMovingToExternal(ofTouchEventArgs &touch);
    protected:
        InterfaceObject();
        void init(InterfaceListener *_listener, int _id, int _x, int _y);
        virtual void drawToBuffer();
        virtual void postDrawToBuffer();  //use this to do something with fbo?
        void sendCallback(int eventTypeId);
        void sendTouchCallback(int eventTypeId, ofTouchEventArgs &touch);
        void calcRotShifts();
        void calcScaleMods();
        ofFbo *tex;
        bool invalidated;
        bool enabled;
        bool exitFlickDetection;
        bool externalTouchUp;
        void recordTouchMoved(ofTouchEventArgs &touch);
        
        vector<InterfaceObject*> children;
        bool visible;
        mouseStates mouseState;
        InterfaceObject* parent;
        bool interactive;
        int w, h, orgW, orgH, cx, cy, lx, ly, tx, ty, ex, ey;
        float scale, totalScale, xScaleMod, yScaleMod;
        int fboWidth, fboHeight;
        float anchorX, anchorY;
        bool deviceScalingOn;
        void setDeviceScaling(bool val);
        screenRotations currentScreenOrientation;
        std::map<int, vector<ofPoint> > touchHistory;
        std::map<int, float> touchVelocity;
        
        std::vector<baseEffect* > effects;
        float rotation;
        float rotatedSize; //square dimension of touch area when the object is rotated
        void updateRotatedSize();
        void changeWidth(float neww);
        void changeHeight(float newh);
        
        list<int> touches;
        bool isTouched;
        int exitGestureStartIdx;

    private:
        void show();
        void hide();
        void allocateFBO();
        void deallocateFBO();
        bool allocated;



        
    };
    
   
    //from http://forum.openframeworks.cc/index.php?topic=4448.0
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY);
    void roundedRect(float x, float y, float w, float h, float r);
};


#endif

