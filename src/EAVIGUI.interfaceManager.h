/*
 *
 *  interfaceManager.h
 *
 *  Created by chris on 01/07/2010.
 *  
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

#ifndef _INTERFACE_MANAGER_H
#define _INTERFACE_MANAGER_H

#include "ofMain.h"
#include "EAVIGUI.interfaceObject.h"
#include "EAVIGUI.label.h"
#include "EAVIGUI.imageButton.h"
#include "EAVIGUI.panel.h"
#include "EAVIGUI.checkbox.h"
#include "EAVIGUI.slider.h"
#include "EAVIGUI.XYPad.h"

#include <map>

#ifdef TARGET_OF_IPHONE
#if OF_VERSION_MINOR == 8
#include "ofxIOSAlerts.h"
#else
#include "ofxiPhoneAlerts.h"
#endif
#endif

namespace EAVIGUI {

    typedef vector<InterfaceObject*> InterfaceObjectGroup;
    typedef InterfaceObjectGroup* interfaceObjectVector;

    class touchListenerProxy;
    class eventProxy;
    class iPhoneEventsProxy;

    class InterfaceManager {
    public:
        
        enum rotationLockModes {
            NOROTATIONLOCK, LANDSCAPEROTATIONLOCK, PORTRAITROTATIONLOCK, ALLROTATIONLOCK
        };
        
        
        static InterfaceObject* addObject(InterfaceObject* obj);
        static void addObjects(InterfaceObjectGroup &objs);
        static InterfaceObject* addPanel(InterfaceObject* obj);
        static InterfaceObject* lastObject();
        static void setup();
        static void draw();
        static void update();
        static void exit();
        static void mouseMoved(int x, int y);
        static void mouseDragged(int x, int y, int button);
        static void mousePressed(int x, int y, int button);
        static void mouseReleased(int x, int y, int button);
        static void touchDown(ofTouchEventArgs &touch);
        static void touchMoved(ofTouchEventArgs &touch);
        static void touchUp(ofTouchEventArgs &touch);
        static void touchDoubleTap(ofTouchEventArgs &touch);
        static void touchCancelled(ofTouchEventArgs &touch);
        static bool touchExitWithKeepCheck(ofTouchEventArgs &touch);
        
        static void showHideModalGroup(InterfaceObjectGroup *modalGroup, bool visible);
        static bool modalShowing() {return currentModalGroup != NULL;}
        
//        static InterfaceObject *beingTouched();
        static touchListenerProxy touchListener;
        static eventProxy eventListener;
        static iPhoneEventsProxy iPhoneListener;
        static map<string, ofTrueTypeFont> fontList;
        static void addFont(string identifier, string fontname, int size);
        
        static void setGroupVisibility(InterfaceObjectGroup &objects, bool visibility);
        static float deviceScaleMod;
        
        static void deviceOrientationChanged(int newOrientation);
        static void setRotationLock(rotationLockModes newlock);
        static rotationLockModes getRotationLock();
        static void setScreenRotations(screenRotations newMode);
        static int touchedObjectCount();
        static void enableDoubleTap(bool val);
        static void setRedirectMouseToTouch(bool val) {redirectMouseToTouch = val;}

    private:
        static bool redirectMouseToTouch;
        static InterfaceObjectGroup intObjs;
        static InterfaceObjectGroup panels;
        static interfaceObjectVector getLiveObjectList();
        static InterfaceObject* draggingTarget;
        static InterfaceObject* getTargetObject(ofTouchEventArgs &touch);
        static interfaceObjectVector currentModalGroup;
        static rotationLockModes rotationLock;
        typedef map<int, InterfaceObject*> touchObjectMap;
        static touchObjectMap touchedObjects;
        static touchObjectMap externalTouches;
        static InterfaceObject* queryTouchObjectMap(touchObjectMap& m, int key);
        static bool doubleTapFlag;
        static bool doubleTapEnabled;
    };

    class touchListenerProxy {
    public:
        void setup();
        void touchDown(ofTouchEventArgs &touch);
        void touchMoved(ofTouchEventArgs &touch);
        void touchUp(ofTouchEventArgs &touch);
        void touchDoubleTap(ofTouchEventArgs &touch);
        void touchCancelled(ofTouchEventArgs &touch);
    };

    class eventProxy {
    public:
        void draw(ofEventArgs &data);
        void update(ofEventArgs &data);
        void exit(ofEventArgs &data);
        void mousePressed(ofMouseEventArgs &args);
        void mouseReleased(ofMouseEventArgs &args);
        void mouseDragged(ofMouseEventArgs &args);
        void mouseMoved(ofMouseEventArgs &args);
    };
    
#ifdef TARGET_OF_IPHONE

    class iPhoneEventsProxy : public ofxiPhoneAlertsListener {
    public:
        void deviceOrientationChanged(int newOrientation);
    };
#endif
    

};

#endif