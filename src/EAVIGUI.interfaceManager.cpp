
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

#include "EAVIGUI.interfaceManager.h"
#include "EAVIGUI.geomFunctions.h"

namespace EAVIGUI {

    vector<InterfaceObject*> InterfaceManager::intObjs;
    vector<InterfaceObject*> InterfaceManager::panels;
    InterfaceObject* InterfaceManager::draggingTarget = NULL;
    interfaceObjectVector InterfaceManager::currentModalGroup = NULL;
    touchListenerProxy InterfaceManager::touchListener;
    eventProxy InterfaceManager::eventListener;
    iPhoneEventsProxy InterfaceManager::iPhoneListener;
    map<string, ofTrueTypeFont> InterfaceManager::fontList;
    float InterfaceManager::deviceScaleMod = 1.0;
    bool InterfaceManager::redirectMouseToTouch = false;
    InterfaceManager::rotationLockModes InterfaceManager::rotationLock = InterfaceManager::NOROTATIONLOCK;
    map<int, InterfaceObject*> InterfaceManager::touchedObjects;
    map<int, InterfaceObject*> InterfaceManager::externalTouches;

    InterfaceObject* InterfaceManager::addObject(InterfaceObject* obj) {
        intObjs.push_back(obj);
        return intObjs.back();
    }

    void InterfaceManager::addObjects(InterfaceObjectGroup &objs) {
        for(int i=0; i < objs.size(); i++)
            intObjs.push_back(objs[i]);
    }

    InterfaceObject* InterfaceManager::lastObject() {
        return intObjs.back();
    }

    InterfaceObject* InterfaceManager::addPanel(InterfaceObject* obj) {
        panels.push_back(obj);
        return panels.back();
    }

    void InterfaceManager::setup() {
        ofAddListener(ofEvents().update, &eventListener, &eventProxy::update);
        ofAddListener(ofEvents().draw, &eventListener, &eventProxy::draw);
        ofAddListener(ofEvents().exit, &eventListener, &eventProxy::exit);
#ifndef TARGET_OF_IPHONE
        ofAddListener(ofEvents().mousePressed, &eventListener, &eventProxy::mousePressed);
        ofAddListener(ofEvents().mouseReleased, &eventListener, &eventProxy::mouseReleased);
        ofAddListener(ofEvents().mouseDragged, &eventListener, &eventProxy::mouseDragged);
        ofAddListener(ofEvents().mouseMoved, &eventListener, &eventProxy::mouseMoved);
#endif
#ifdef TARGET_OF_IPHONE
        ofxiPhoneAlerts.addListener(&InterfaceManager::iPhoneListener);
#endif
        
        for(int i=0; i < intObjs.size(); i++) {
            intObjs[i]->setup();
        }
        for(int i=0; i < panels.size(); i++) {
            panels[i]->setup();
        }
        touchListener.setup();
    }

    void InterfaceManager::draw() {
        for(int i=0; i < intObjs.size(); i++) {
            if (intObjs[i]->getParent() == NULL) {
                intObjs[i]->draw();
            }
        }	
        for(int i=0; i < panels.size(); i++) {
            if (panels[i]->isVisible()) {
                ofSetColor(200, 200, 200, 200);
                ofFill();
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                panels[i]->draw();
                break;
            }
        }
    }

    vector<InterfaceObject*>* InterfaceManager::getLiveObjectList() {
        interfaceObjectVector liveObjectList;
        liveObjectList = &intObjs;
        
        if (currentModalGroup != NULL) {
            liveObjectList = currentModalGroup;
        }
        return liveObjectList;
    }
    
    ofTouchEventArgs createTouch(int x, int y) {
        ofTouchEventArgs touch;
        touch.x = x;
        touch.y = y; 
        touch.numTouches = 1;
        touch.id=0;
        return touch;
    }
        

    void InterfaceManager::mouseMoved(int x, int y) {
        
        
        static InterfaceObject* mouseOverObject = NULL;
        
        interfaceObjectVector liveObjectList = InterfaceManager::getLiveObjectList();
        bool found = false;
        int i = liveObjectList->size();
        while(i--) {
            if (liveObjectList->at(i)->isInteractive() && geom::pointInRect(x,y,liveObjectList->at(i)->x, liveObjectList->at(i)->y, liveObjectList->at(i)->getWidth(), liveObjectList->at(i)->getHeight())) {
                found = true;
                if ((mouseOverObject != liveObjectList->at(i)) && (mouseOverObject != NULL)) {
                    mouseOverObject->mouseExit();
                }
                mouseOverObject = liveObjectList->at(i);
                liveObjectList->at(i)->mouseMoved(x - liveObjectList->at(i)->x, y - liveObjectList->at(i)->y);
                break;
            }
        }
        if (!found && (mouseOverObject != NULL)) {
            mouseOverObject->mouseExit();
            mouseOverObject = NULL;
        }	
        
        
    }

    void InterfaceManager::mouseDragged(int x, int y, int button){
        if (redirectMouseToTouch) {
            ofTouchEventArgs touch = createTouch(x, y);
            InterfaceManager::touchDown(touch);
        }else{
            if (NULL == InterfaceManager::draggingTarget) {
                vector<InterfaceObject*>* liveObjectList = InterfaceManager::getLiveObjectList();
                int i = liveObjectList->size();
                while(i--) {
                    if (liveObjectList->at(i)->isInteractive() && geom::pointInRect(x,y,liveObjectList->at(i)->x, liveObjectList->at(i)->y, liveObjectList->at(i)->getWidth(), liveObjectList->at(i)->getHeight())) {
                        //liveObjectList->at(i)->mouseDragged(x - liveObjectList->at(i)->x, y - liveObjectList->at(i)->y, button);
                        InterfaceManager::draggingTarget = liveObjectList->at(i);
                        break;
                    }
                }
            }
            if (NULL != InterfaceManager::draggingTarget) {
                InterfaceManager::draggingTarget->mouseDragged(x - InterfaceManager::draggingTarget->x, y - InterfaceManager::draggingTarget->y, button);
            }
        }
    }

    void InterfaceManager::mousePressed(int x, int y, int button){
        if (redirectMouseToTouch) {
            ofTouchEventArgs touch = createTouch(x, y);
            InterfaceManager::touchDown(touch);
        }else{
        
            vector<InterfaceObject*>* liveObjectList = InterfaceManager::getLiveObjectList();
            int i = liveObjectList->size();
            while(i--) {
                if (liveObjectList->at(i)->isInteractive() && geom::pointInRect(x,y,liveObjectList->at(i)->x, liveObjectList->at(i)->y, liveObjectList->at(i)->getWidth(), liveObjectList->at(i)->getHeight())) {
                    liveObjectList->at(i)->mousePressed(x - liveObjectList->at(i)->x, y - liveObjectList->at(i)->y, button);
                    break;
                }
            }
        }
    }

    void InterfaceManager::mouseReleased(int x, int y, int button){
        if (redirectMouseToTouch) {
            ofTouchEventArgs touch = createTouch(x, y);
            InterfaceManager::touchUp(touch);
        }else{

            if (NULL != InterfaceManager::draggingTarget) {
                InterfaceManager::draggingTarget->mouseReleased(x - InterfaceManager::draggingTarget->x, y - InterfaceManager::draggingTarget->y, button);
                InterfaceManager::draggingTarget = NULL;
            }else{
                vector<InterfaceObject*>* liveObjectList = InterfaceManager::getLiveObjectList();
                int i = liveObjectList->size();
                while(i--) {
                    if (liveObjectList->at(i)->isInteractive() && geom::pointInRect(x,y,liveObjectList->at(i)->x, liveObjectList->at(i)->y, liveObjectList->at(i)->getWidth(), liveObjectList->at(i)->getHeight())) {
                        liveObjectList->at(i)->mouseReleased(x - liveObjectList->at(i)->x, y - liveObjectList->at(i)->y, button);
                        break;
                    }
                }
            }
        }
        
    }

    InterfaceObject* InterfaceManager::getTargetObject(ofTouchEventArgs &touch) {
    //	cout << "testing " << touch.x << ", " << touch.y << endl;
        InterfaceObject *target = NULL;
        interfaceObjectVector liveObjectList = InterfaceManager::getLiveObjectList();
        int i = liveObjectList->size();
        while(i--) {
            ofRectangle rotatedRect = liveObjectList->at(i)->getRectWithScreenRotation();
            if (liveObjectList->at(i)->isInteractive() && liveObjectList->at(i)->isVisible() &&
                geom::pointInRect(touch.x,touch.y,rotatedRect.x, rotatedRect.y, rotatedRect.width-1, rotatedRect.height-1))
            {
//                cout << touch.x << ", " << touch.y << ", " << rotatedRect.x << ", " << rotatedRect.y << ", " <<  rotatedRect.width << ", " << rotatedRect.height << endl;
                //convert from screen coordinate system to object coordinate system
                ofTouchEventArgs tempTouch = touch;
                liveObjectList->at(i)->transformTouchForScreenRotation(tempTouch);
                if(liveObjectList->at(i)->canInteractAt(tempTouch.x, tempTouch.y)) {
                    target = liveObjectList->at(i);
                    touch = tempTouch;
                    //cout << "found" << endl;
                    break;
                }
            }
        }
        return target;
    }


    InterfaceObject* InterfaceManager::queryTouchObjectMap(touchObjectMap& m, int key) {
        InterfaceObject *res = NULL;
        touchObjectMap::iterator it = m.find(key);
        if (it != m.end()) {
            res = it->second;
        }
        return res;
    }

    void InterfaceManager::touchDown(ofTouchEventArgs &touch) {
        //cout << "Touch down " << touch.x << "," << touch.y << endl;
        InterfaceObject *obj = InterfaceManager::getTargetObject(touch);
        if (NULL != obj) {
//            cout << "touch down: " << obj->id << endl;
            touchedObjects[touch.id] = obj;
            obj->touchDown(touch);
        }
        
    }
    
    void InterfaceManager::touchMoved(ofTouchEventArgs &touch) {
        //has an object kept this touch?
        InterfaceObject* trackingObject = queryTouchObjectMap(externalTouches, touch.id);
        if (trackingObject != NULL) {
            //pipe the touch to this object
            trackingObject->touchMovedExternal(touch);
        }else{
    //        cout << "Touch moved " << touch.id << endl;
            InterfaceObject *obj = getTargetObject(touch);
            //in an object?
            if (NULL != obj) {
    //            cout << "Touch " << touch.id << " moved to " << obj->id << endl;
                //has this touch just exited from another object?
                bool touchKept = false;
                InterfaceObject* touchedObj = queryTouchObjectMap(touchedObjects, touch.id);
                if (touchedObj != NULL && touchedObj != obj) {
                    //cout << "Touch exit: " << touchingObject->id << endl;
                    touchKept = touchExitWithKeepCheck(touch);
    //                touchedObjects[touch.id]->touchExit(touch);
                }
                if (!touchKept) {
                    touchedObjects[touch.id] = obj;
                    obj->touchMoved(touch);
                }
            }else{
                //not on an object
    //            cout << "Touch " << touch.id << " moved, no object found\n";
                //trigger an exit if this touch was on an object and that object isn't still being touched
                if (queryTouchObjectMap(touchedObjects, touch.id) != NULL) {
    //                cout << "Touch exit to space: " << touchedObjects[touch.id]->id << endl;
    //                touchedObjects[touch.id]->touchExit(touch);
                    touchExitWithKeepCheck(touch);
                    touchedObjects[touch.id] = NULL;
                }
            }
        }
    }
    
    bool InterfaceManager::touchExitWithKeepCheck(ofTouchEventArgs &touch) {
        bool keepTouch = touchedObjects[touch.id]->keepThisTouch(touch);
        if (keepTouch) {
            externalTouches[touch.id] = touchedObjects[touch.id];
            touchedObjects[touch.id]->touchMovingToExternal(touch);
        }else{
            touchedObjects[touch.id]->touchExit(touch);
        }
        return keepTouch;
    }


    void InterfaceManager::touchUp(ofTouchEventArgs &touch) {
//        cout << "Touch up " << touch.x << "," << touch.y << endl;
        //has an object kept this touch?
        InterfaceObject* trackingObject = queryTouchObjectMap(externalTouches, touch.id);
        if (trackingObject != NULL) {
            //pipe the event to this object
            trackingObject->touchUpExternal(touch);
            touchedObjects[touch.id] = NULL;
            externalTouches[touch.id] = NULL;
        }else{
            InterfaceObject *obj = InterfaceManager::getTargetObject(touch);
            if (NULL != obj) {
    //            cout << "touch up: " << obj->id << endl;
                touchedObjects[touch.id] = NULL;
                obj->touchUp(touch);
            }
        }
    }

    void InterfaceManager::touchDoubleTap(ofTouchEventArgs &touch) {
        InterfaceObject *obj = InterfaceManager::getTargetObject(touch);
        if (NULL != obj) {
            obj->touchDoubleTap(touch);
        }
    }

    void InterfaceManager::touchCancelled(ofTouchEventArgs &touch) {
    }


    void InterfaceManager::showHideModalGroup(InterfaceObjectGroup *modalGroup, bool visible) {
        //cancel all touches
        for (std::map<int,InterfaceObject*>::iterator it=touchedObjects.begin(); it!=touchedObjects.end(); ++it) {
            if (NULL != it->second) {
                it->second = NULL;
            }
        }
        //disable other controls
        for (int i=0; i < intObjs.size(); i++) {
            if (std::find(modalGroup->begin(), modalGroup->end(), intObjs[i]) == modalGroup->end()) {
                intObjs[i]->setEnabled(!visible);
            }
        }
        for(int i=0; i < modalGroup->size(); i++) {
            modalGroup->at(i)->setVisible(visible);
        }
        currentModalGroup = visible ? modalGroup : NULL;
    }

    void InterfaceManager::update() {
        for (int i=0; i < intObjs.size(); i++) {
            intObjs[i]->update();
        }
    }

//    InterfaceObject* InterfaceManager::beingTouched() {
//        return InterfaceManager::touchingObject;
//    }

    void InterfaceManager::addFont(string identifier, string fontname, int size) {
        InterfaceManager::fontList[identifier] = ofTrueTypeFont();
        InterfaceManager::fontList[identifier].loadFont(fontname, size);
    }
    
    
    void InterfaceManager::setGroupVisibility(InterfaceObjectGroup &objects, bool visibility) {
        for(int i=0; i < objects.size(); i++) {
            objects[i]->setVisible(visibility);
        }
    }
    
    void InterfaceManager::deviceOrientationChanged(int newOrientation) {
        if (rotationLock != ALLROTATIONLOCK ) {
            bool doRotation = true;
            if (rotationLock == PORTRAITROTATIONLOCK && (newOrientation == 3 || newOrientation == 4)) {
                doRotation = false;
            }
            if (rotationLock == LANDSCAPEROTATIONLOCK && (newOrientation == 1 || newOrientation == 2)) {
                doRotation = false;
            }
            if (doRotation) {
                interfaceObjectVector liveObjectList = InterfaceManager::getLiveObjectList();
                int i = liveObjectList->size();
                while(i--) {
                    liveObjectList->at(i)->deviceOrientationChanged(newOrientation);
                }
            }
        }
    }

    void InterfaceManager::setRotationLock(InterfaceManager::rotationLockModes newlock) {
        rotationLock = newlock;
    }
    
    InterfaceManager::rotationLockModes InterfaceManager::getRotationLock() {
        return rotationLock;
    }
    
    void InterfaceManager::setScreenRotations(screenRotations newMode) {
        for(int i=0; i < intObjs.size(); i++) {
            intObjs[i]->setScreenRotation(newMode);
            intObjs[i]->invalidate();
        }
    }

    
    void InterfaceManager::exit() {
        for (int i=0; i < intObjs.size(); i++) {
            delete intObjs[i];
        }
    }
    
    ///////////////////////////////////////////////////////////
    
    void touchListenerProxy::setup() {
        ofRegisterTouchEvents(this);
    };

    void touchListenerProxy::touchDown(ofTouchEventArgs &touch) {
        InterfaceManager::touchDown(touch);
    }
    void touchListenerProxy::touchMoved(ofTouchEventArgs &touch) {
        InterfaceManager::touchMoved(touch);
    }
    void touchListenerProxy::touchUp(ofTouchEventArgs &touch) {
        InterfaceManager::touchUp(touch);
    }
    void touchListenerProxy::touchDoubleTap(ofTouchEventArgs &touch) {
        InterfaceManager::touchDoubleTap(touch);
    }

    void touchListenerProxy::touchCancelled(ofTouchEventArgs &touch) {
        
    }


    void eventProxy::draw(ofEventArgs &data) {
        InterfaceManager::draw();
    }

    void eventProxy::update(ofEventArgs &data) {
        InterfaceManager::update();
    }
    
    void eventProxy::exit(ofEventArgs &data) {
        InterfaceManager::exit();
    }
    
    void eventProxy::mousePressed(ofMouseEventArgs &args) {
        InterfaceManager::mousePressed(args.x, args.y, args.button);
    }

    void eventProxy::mouseReleased(ofMouseEventArgs &args) {
        InterfaceManager::mouseReleased(args.x, args.y, args.button);
    }
    
    void eventProxy::mouseDragged(ofMouseEventArgs &args) {
        InterfaceManager::mouseDragged(args.x, args.y, args.button);
    }
    
    void eventProxy::mouseMoved(ofMouseEventArgs &args) {
        InterfaceManager::mouseMoved(args.x, args.y);
    }
    
#ifdef TARGET_OF_IPHONE
    
    void iPhoneEventsProxy::deviceOrientationChanged(int newOrientation) {
        cout << "Orientation change: " << newOrientation << endl;
        InterfaceManager::deviceOrientationChanged(newOrientation);
    }
    
#endif

};