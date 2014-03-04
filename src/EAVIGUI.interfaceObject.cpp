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

#include "EAVIGUI.interfaceObject.h"
#include "EAVIGUI.interfaceManager.h"
#include "EAVIGUI.baseEffect.h"
#include "EAVIGUI.geomFunctions.h"

//#define GUIDEGBUG

namespace EAVIGUI {
    
    InterfaceObject::InterfaceObject(InterfaceListener *_listener, int _id, int _x, int _y) {
        init(_listener, _id, _x, _y);
    }
    
    InterfaceObject::InterfaceObject(InterfaceListener *_listener, int _id, int _x, int _y, int _w, int _h) {
        init(_listener, _id, _x, _y);
        setWidth(_w);
        setHeight(_h);
    }
    
    void InterfaceObject::init(InterfaceListener *_listener, int _id, int _x, int _y) {
        tex = NULL;
        allocated = false;
        listener = _listener;
        id = _id;
        x = _x;
        y = _y;
        currentScreenOrientation = ROT0;
        setWidth(0);
        setHeight(0);
        invalidated = false;
        alpha = red = green = blue = 255;
        visible = false;
        mouseState = SOMEWHERELSE;
        parent = NULL;
        setIsInteractive(true);
        screenRotation = 0;
        screenRotMode = ROTMODE_NONE;
        rotPt0.x = x;
        rotPt0.y = y;
        rotation = 0;
        rotatedSize = 0;
        fadeTime = 300;  //ms
        fadeInTimeStamp = -1;
        fadeOutTimeStamp = -1;
        deviceScalingOn = true;
        setScale(1.0);
        setAnchorPoint(0.5, 0.5);
        calcScaleMods();
//        pulsate = false;
//        pulsateSpeed = 0.007;
//        pulsateMin = 0.5;
//        pulsateMax = 1.0;
        enabled = true;
        isTouched = false;
        anchorX = anchorY = 0;
        lx=ly=0;
        tx = ty = 0; //touchDown Point
        ex = ey = 0;
        exitFlickDetection = false;
        draggable = false;
        touchAndHoldTime = 500; //milliseconds
        touchAndHoldSent = false;
        touchAndHoldEnabled = false;
        exitGestureStartIdx = 0;
        externalTouchUp = false;
    }
    
    InterfaceObject::~InterfaceObject() {
        if (NULL != tex)
            delete tex;
    }
    
    
    void InterfaceObject::setup() {
    }
    
    void InterfaceObject::allocateFBO() {
        ofFbo::Settings settings;
#ifdef TARGET_OPENGLES
        settings.width			= ofNextPow2(w);
        settings.height			= ofNextPow2(h);
#else
        settings.width			= w;
        settings.height			= h;
#endif
        fboWidth = settings.width;
        fboHeight = settings.height;
        settings.useDepth		= false;
        settings.useStencil		= false;
        tex = new ofFbo();
        tex->allocate(settings);
        allocated = true;
        invalidated = true;
    }
    
    void InterfaceObject::deallocateFBO() {
        allocated = false;
        delete tex;
    }
    
    void InterfaceObject::update(){
        if (touchAndHoldEnabled && touches.size() > 0) {
            if (!touchAndHoldSent && ofGetElapsedTimeMillis() - touchAndHoldTS > touchAndHoldTime) {
                sendCallback(TOUCHANDHOLD);
                touchAndHoldSent = true;
            }
        }
    }
    
    void InterfaceObject::setScale(float newScale) {
        scale = newScale;
        totalScale = deviceScalingOn ? scale * InterfaceManager::deviceScaleMod : scale;
        calcScaleMods();
    }
    
    void InterfaceObject::setDeviceScaling(bool val) {
        deviceScalingOn = val;
        setScale(scale);
    }

    
    void InterfaceObject::setAnchorPoint(float ax, float ay) {
        anchorX = ax;
        anchorY = ay;
        calcScaleMods();
    }
    
    void InterfaceObject::getAnchorPoint(float &ax, float &ay) {
        ax = anchorX;
        ay = anchorY;
    }
    
    void InterfaceObject::drawToBuffer() {
        
    }
    
    void InterfaceObject::draw(){	
        if (visible) {
            bool effectInvalidated = false;
            for(int i=0; i < effects.size(); i++) {
                if (effects.at(i)->isInvalidated()) {
                    effectInvalidated = true;
                }
            }
            if (invalidated || effectInvalidated) {
                invalidated = false;
                tex->begin();
                ofEnableAlphaBlending();
                ofClear(255);
                ofBackground(0,0);

                drawToBuffer();
                
                for(int e=0; e < effects.size(); e++) {
                    effects.at(e)->draw();
                    effects.at(e)->validate();
                }
//                if (!enabled) {
//                    ofSetColor(150, 150, 150, 200);
//                    ofFill();
//                    ofRect(0,0,w,h);
//                }
                tex->end();
                postDrawToBuffer();
            }
            
            int fadeAlpha = alpha;
            if (fadeInTimeStamp != -1) {
                int elapsed = ofGetElapsedTimeMillis() - fadeInTimeStamp;
                if (elapsed > fadeTime) {
                    fadeInTimeStamp = -1;
                }
                else {
                    fadeAlpha = round((float) elapsed / (float) fadeTime * (float) alpha);
                }
                
                
            }
            if (fadeOutTimeStamp != -1) {
                int elapsed = ofGetElapsedTimeMillis() - fadeOutTimeStamp;
                if (elapsed > fadeTime) {
                    fadeOutTimeStamp = -1;
                    hide();
                    fadeAlpha = 0;
                }
                else {
                    fadeAlpha = alpha - round((float) elapsed / (float) fadeTime * (float) alpha);
                }
                
            }
            //visible now?
            if (visible) {
//                float pulsateVal = 0;
//                if (pulsateSpeed > 0) {
//                    pulsateVal = (fabs(sin(ofGetElapsedTimeMillis() * pulsateSpeed)) * (pulsateMax - pulsateMin)) + pulsateMin;
//                }
                ofSetColor(red, green, blue, fadeAlpha);
                
                glPushMatrix();

                //rotate to match screen orientation
                glTranslatef(x,y,0);
                glRotatef(screenRotation,0,0,1);
                glTranslatef(-x,-y,0);

                //put in position
                glTranslatef(x + xScaleMod, y + yScaleMod,0);
                
                if (rotation != 0) {
                    //rotate FBO around the center
                    glTranslatef(getScaledWidth()/2.0, getScaledHeight()/2.0,0);
                    glRotatef(rotation, 0, 0, 1);
                    glTranslatef(-getScaledWidth()/2.0, -getScaledHeight()/2.0,0);
                }

                if (enableAlphaWhenDrawing()) {
                    ofEnableAlphaBlending();
                }else{
                    ofDisableAlphaBlending();
                }
                //#ifndef TARGET_OPENGLES
//				glBlendEquation(GL_FUNC_ADD);
//#endif
//                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//                ofDisableAlphaBlending();
//                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
                tex->draw(0, 0, fboWidth * totalScale, fboHeight *totalScale);
                
#ifdef GUIDEGBUG
                //undo rotation for debug rects
                glTranslatef(getScaledWidth()/2.0, getScaledHeight()/2.0,0);
                glRotatef(-rotation, 0, 0, 1);
                glTranslatef(-getScaledWidth()/2.0, -getScaledHeight()/2.0,0);
                ofNoFill();
                ofSetColor(255,0,0,255);
                ofRect(0, 0, fboWidth * totalScale, fboHeight *totalScale);
                ofSetColor(100,100,100,255);
                ofRect(0, 0, w * totalScale, h *totalScale);
                if (rotation != 0) {
                    //expanded area to capture touch on rotated FBOs
                    ofSetColor(0,255,0,255);
                    ofRect(-(rotatedSize - w)/2.0 * totalScale, -(rotatedSize - h) /2.0 * totalScale, rotatedSize * totalScale, rotatedSize * totalScale);
                }
#endif            
                glPopMatrix();
                
                for(int i=0; i < children.size(); i++) {
                    children[i]->draw();
                }
            }
        }
    }
    
    void InterfaceObject::postDrawToBuffer() {
        
    }
    
    bool InterfaceObject::canInteractAt(int x, int y) {
        bool res = true;
        if (rotation != 0) {
            res = x >= 0 && y >= 0 && x < (w * totalScale) && y < (h * totalScale);
        }
        return res;
    }


    
    void InterfaceObject::mouseMoved( int x, int y ){	
        mouseState = OVER;
        sendCallback(InterfaceObject::MOUSEMOVED);
    }
    
    void InterfaceObject::mouseExit() {	
        mouseState = SOMEWHERELSE;
    }
    
    void InterfaceObject::mousePressed( int x, int y, int button ){	
        mouseState = PRESSED;
        sendCallback(InterfaceObject::MOUSEPRESSED);
    }
    
    void InterfaceObject::mouseReleased(){	
        mouseState = OVER;
        sendCallback(InterfaceObject::MOUSERELEASED);
    }
    
    void InterfaceObject::mouseReleased(int x, int y, int button ){	
        mouseState = OVER;
        sendCallback(InterfaceObject::MOUSERELEASED);
    }
    
    
    void InterfaceObject::mouseDragged(int x, int y, int button) {
        mouseState = PRESSED;
        sendCallback(InterfaceObject::MOUSEDRAGGED);
    }
    
    void InterfaceObject::setEnabled(bool newEnabled) {
        enabled = newEnabled;
        invalidate();
    }
    
    InterfaceObject::InterfaceObject(){
        
    }
    
    void InterfaceObject::sendCallback(int eventTypeId) {
        listener->handleInterfaceEvent(id, eventTypeId, this);
    }
    
    void InterfaceObject::invalidate() {
        invalidated = true;
    }
    
    
    void InterfaceObject::addChild(InterfaceObject* child) {
        child->x += x;
        child->y += y;
        children.push_back(child);
        child->setParent(this);
        InterfaceManager::addObject(child);
    }
    
    bool InterfaceObject::hasChildren() {
        return (children.size() > 0);
    }
    
    void InterfaceObject::show() {
        if (!visible)
            allocateFBO();
        visible = true;
        invalidate();
    }
    
    void InterfaceObject::hide() {
        if (visible)
            deallocateFBO();
        visible = false;
    }
    
    void InterfaceObject::moveTo(float newx, float newy) {
        x = newx;
        y = newy;
    }
    
    void InterfaceObject::moveTo(ofPoint newPos) {
        x = newPos.x;
        y = newPos.y;
    }

    
    void InterfaceObject::setVisible(bool val, bool noFade) {
        if (!val) { //hiding
            mouseState = SOMEWHERELSE;
            if (noFade) 
                hide();
            else {
                fadeOutTimeStamp = ofGetElapsedTimeMillis();
                fadeInTimeStamp = -1;
            }
            
            //and cancel all touches
            isTouched = false;
            touches.clear();
        }
        else { //showing
            show();
            if (!noFade) {
                fadeInTimeStamp = ofGetElapsedTimeMillis();
                fadeOutTimeStamp = -1;
            }
        }
    }
    
    bool InterfaceObject::isVisible() {
        return visible;
    }
    
    InterfaceObject* InterfaceObject::getParent() {
        return parent;
    }
    
    void InterfaceObject::setParent(InterfaceObject* newParent) {
        parent = newParent;
    }
    
    vector<InterfaceObject*>* InterfaceObject::getChildren() {
        return &children;
    }
    
    bool InterfaceObject::isInteractive() {
        return interactive;
    }
    
    void InterfaceObject::sendTouchCallback(int eventTypeId, ofTouchEventArgs &touch) {
        touchArgs = &touch;
        sendCallback(eventTypeId);
    }
    
    void InterfaceObject::touchDown(ofTouchEventArgs &touch) {
        if (touchAndHoldEnabled && touches.size() == 0) {
            touchAndHoldTS = ofGetElapsedTimeMillis();
            touchAndHoldSent = false;
        }
        touches.push_back(touch.id);
        touchHistory[touch.id].clear();
        isTouched = true;
        tx = touch.x;
        ty = touch.y;
        touchHistory[touch.id].push_back(ofPoint(touch.x, touch.y));
        touchVelocity[touch.id] = 0;
        if (draggable) {
            dragPoint = ofPoint(tx, ty);
        }
        sendTouchCallback(InterfaceObject::TOUCHDOWN, touch);
    }
    
    void InterfaceObject::touchDoubleTap(ofTouchEventArgs &touch) {
        sendTouchCallback(InterfaceObject::TOUCHDOUBLETAP, touch);
    }
    
    void InterfaceObject::touchMoved(ofTouchEventArgs &touch) {
        recordTouchMoved(touch);
        if (isTouched && draggable) {
            if (touchAndHoldEnabled) {
                touchAndHoldTS = ofGetElapsedTimeMillis(); //postpone press and hold
            }
            moveTo(x + touch.x - dragPoint.x, y + touch.y - dragPoint.y);
        }
        sendTouchCallback(InterfaceObject::TOUCHMOVED, touch);
    }
    
    void InterfaceObject::touchMovedExternal(ofTouchEventArgs &touch) {
        recordTouchMoved(touch);
        sendTouchCallback(InterfaceObject::TOUCHMOVED_EXTERNAL, touch);
    }
    
    void InterfaceObject::recordTouchMoved(ofTouchEventArgs &touch) {
        if (touches.size() > 0) {
            touchVelocity[touch.id] = ofDist(lx, ly, touch.x, touch.y);
        }else{
            touchVelocity[touch.id] = 0;
        }
        touches.remove(touch.id);
        touches.push_back(touch.id);
        touchHistory[touch.id].push_back(ofPoint(touch.x, touch.y));
        lx = touch.x;
        ly = touch.y;
    }

    
    void InterfaceObject::touchExit(ofTouchEventArgs &touch) {
        touches.remove(touch.id);
        isTouched = touches.size() > 0;
        ex = touch.x;
        ey = touch.y;
        sendTouchCallback(InterfaceObject::TOUCHEXIT, touch);
    }
    
    void InterfaceObject::touchUp(ofTouchEventArgs &touch) {
        touches.remove(touch.id);
        isTouched = touches.size() > 0;
        if (!touchAndHoldEnabled || (touchAndHoldEnabled && !touchAndHoldSent)) {
            sendTouchCallback(InterfaceObject::TOUCHUP, touch);
        }
    }
    
    void InterfaceObject::touchCancelled(ofTouchEventArgs &touch) {
        touches.remove(touch.id);
        sendTouchCallback(InterfaceObject::TOUCHCANCELLED, touch);
    }

    void InterfaceObject::touchUpExternal(ofTouchEventArgs &touch) {
        touches.remove(touch.id);
        isTouched = touches.size() > 0;
        if (exitFlickDetection) {
            bool flick = false;
    //        float flickangle = 0;
            float overallAvgVel=0, lateAvgVel=0;
            if (touchHistory[touch.id].size() - exitGestureStartIdx > 3) {
                for(int i=exitGestureStartIdx+1; i < touchHistory[touch.id].size(); i++) {
                    float vel = ofDist(touchHistory[touch.id][i-1].x, touchHistory[touch.id][i-1].y, touchHistory[touch.id][i].x, touchHistory[touch.id][i].x);
                    overallAvgVel += vel;
                    cout << vel << ", ";
                }
                overallAvgVel /= touchHistory[touch.id].size();
                int pt = (touchHistory[touch.id].size() - exitGestureStartIdx) / 2;
                for(int i=pt; i < touchHistory[touch.id].size(); i++) {
                    lateAvgVel += ofDist(touchHistory[touch.id][i-1].x, touchHistory[touch.id][i-1].y, touchHistory[touch.id][i].x, touchHistory[touch.id][i].x);
                }
                lateAvgVel /= pt;
                float velRatio = overallAvgVel > 0 ? lateAvgVel / overallAvgVel : 0;
                cout << endl << "Avg: " << overallAvgVel << ", " << lateAvgVel << ", " << velRatio << endl;
    //            for(int i=0; i < touchHistory[touch.id].size(); i++) {
    //                cout << touchHistory[touch.id].at(i) << " -- ";
    //            }
                cout << endl;
                flick = velRatio > 1.4;
            }
            if (flick) {
                sendCallback(TOUCHEXITFLICK);
            }else{
                sendCallback(TOUCHEXIT);
            }
        }else{
            sendTouchCallback(InterfaceObject::TOUCHUP_EXTERNAL, touch);            
        }
    }

    
    
    
    void InterfaceObject::calcRotShifts() {
        xScreenRotShift = (w/2.0) - (h / 2.0);
        yScreenRotShift =  (h/2.0) - (w / 2.0);
    }

    void InterfaceObject::setWidth(int width){
        orgW = width;
        changeWidth(width);
        updateRotatedSize();
    }
    void InterfaceObject::setHeight(int height){
        orgH = height;
        changeHeight(height);
        updateRotatedSize();
    }
    
    void InterfaceObject::changeWidth(float neww) {
        w = neww;
        cx = w / 2;
        if (allocated) {
            deallocateFBO();
            allocateFBO();
        }
    }
    
    void InterfaceObject::changeHeight(float newh) {
        h = newh;
        cy = h / 2;
        if (allocated) {
            deallocateFBO();
            allocateFBO();
        }
    }

    int InterfaceObject::getWidth(){
        return w; 
    }
    
    int InterfaceObject::getHeight(){
        return h; 
    }
    
    int InterfaceObject::getScaledWidth(){
        return w * totalScale; 
    }
    
    int InterfaceObject::getScaledHeight(){
        return h * totalScale; 
    }
    
    void InterfaceObject::setScreenRotation(screenRotations rot) {
        if (ROTMODE_NONE != screenRotMode) {
            switch(rot) {
                case ROT0:
                    screenRotation = 0;
                    xScreenRotShift = 0;
                    yScreenRotShift = 0;
                    break;
                case ROT90:
                    screenRotation = 90;
                    xScreenRotShift = h;
                    yScreenRotShift = 0;
                    break;
                case ROT180:
                    screenRotation = 180;
                    xScreenRotShift = w;
                    yScreenRotShift = h;
                    break;
                case ROT270:
                    screenRotation = 270;
                    xScreenRotShift = 0;
                    yScreenRotShift = w;
                    break;
            };
            currentScreenOrientation = rot;
        }
    }
    
    void InterfaceObject::calcScaleMods() {
        xScaleMod = ((totalScale - 1.0) * -w) * anchorX;
        yScaleMod = ((totalScale - 1.0) * -h) * anchorY;                
    }
    
    
    ofRectangle InterfaceObject::getRectWithScreenRotation() {
        ofRectangle res;
        if (rotation == 0) {
            switch(screenRotation) {
                case 0:
                    res = ofRectangle(x + xScaleMod, y + yScaleMod, w * totalScale, h * totalScale);
                    break;
                case 90:
                    res = ofRectangle(x- (h * totalScale) - yScaleMod, y + xScaleMod, h * totalScale, w * totalScale);
                    break;
                case 180:
                    res = ofRectangle(x - (w * totalScale) - xScaleMod, y - (h * totalScale) - yScaleMod, w * totalScale, h * totalScale);
                    break;
                case 270:
                    res = ofRectangle(x + yScaleMod, y - (w * totalScale) - xScaleMod, h * totalScale, w * totalScale);
                    break;
            }
        }else{
            float scaledSize = rotatedSize * totalScale;
            float rotAdjustW = -((rotatedSize - w)/2.0 * totalScale);
            float rotAdjustH = -((rotatedSize - h)/2.0 * totalScale);
            switch(screenRotation) {
                case 0:
                    res = ofRectangle(x + xScaleMod + rotAdjustW,
                                      y + yScaleMod + rotAdjustH,
                                      scaledSize, scaledSize);
                    break;
                case 90:
                    res = ofRectangle(x - (h * totalScale) - yScaleMod + rotAdjustH, y + xScaleMod + rotAdjustW, scaledSize, scaledSize);
                    break;
                case 180:
                    res = ofRectangle(x - (w * totalScale) - xScaleMod + rotAdjustW, y - (h * totalScale) - yScaleMod + rotAdjustH, scaledSize, scaledSize);
                    break;
                case 270:
                    res = ofRectangle(x + yScaleMod + rotAdjustH, y - (w * totalScale) - xScaleMod + rotAdjustW, scaledSize, scaledSize);
                    break;
            }
            
        }
        return res;
    }
    
    
    InterfaceObject* InterfaceObject::setScreenRotationPoints(ofPoint r90, ofPoint r180, ofPoint r270) {
        rotPt90 = r90;
        rotPt180 = r180;
        rotPt270 = r270;
        screenRotMode = ROTMODE_SPECIFY;
        return this;
    }
    
    
    //convert from screen coordinate system to object coordinate system
    void InterfaceObject::transformTouchForScreenRotation(ofTouchEventArgs &touch) {
        int tx = touch.x;
        int ty = touch.y;
        //fbo rotation?
        if (rotation == 0) {
            switch(screenRotation) {
                case 0: 
                    touch.x -= x;
                    touch.y -= y;
                    break;
                case 90: 
                    touch.x = ty - y;
                    touch.y = x - tx;
                    break;
                case 180: 
                    touch.x = (w * totalScale) - (touch.x - (x - (w * totalScale)));
                    touch.y = y - touch.y;
                    break;
                case 270: 
                    touch.x = y - ty;
                    touch.y = tx - x;
                    break;
            };
        }else{
            float rotAdjustW = -((rotatedSize - w)/2.0 * totalScale);
            float rotAdjustH = -((rotatedSize - h)/2.0 * totalScale);
            
            switch(screenRotation) {
                case 0:
                    touch.x -= x + rotAdjustW;
                    touch.y -= y + rotAdjustH;
                    break;
                case 90:
                    touch.x = ty - y - rotAdjustW;
                    touch.y = x - tx - rotAdjustH;
                    break;
                case 180:
                    touch.x = (w * totalScale) - (touch.x - (x - (w * totalScale))) - rotAdjustW;
                    touch.y = y - touch.y - rotAdjustH;
                    break;
                case 270:
                    touch.x = y - ty + rotAdjustW;
                    touch.y = tx - x - rotAdjustH;
                    break;
            };
            
            //rotate back around to original box, center as origin
            //move origin
            touch.x -= rotatedSize /2.0;
            touch.y -= rotatedSize /2.0;
            //rotate back
            float theta = -rotation / 360.0 * 2.0 * PI;;
            float rotx = (touch.x * cosf(theta)) - (touch.y * sinf(theta));
            float roty = (touch.x * sinf(theta)) + (touch.y * cosf(theta));
            //translate to origin as top left of box
            rotx += w /2.0;
            roty += h /2.0;
            touch.x = rotx;
            touch.y = roty;

            
        }
    }
    
    void InterfaceObject::deviceOrientationChanged(int newOrientation) {
        if (ROTMODE_NONE != screenRotMode) {
            switch (newOrientation) {
                case 1: //portrait
                    setScreenRotation(ROT0);
                    break;
                case 2: // OFXIPHONE_ORIENTATION_UPSIDEDOWN
                    setScreenRotation(ROT180);
                    break;
                case 4: //OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT 
                    setScreenRotation(ROT270);
                    break;
                case 3: //OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT
                    setScreenRotation(ROT90);
            }
            updatePosition();
        }
    };
    
    void InterfaceObject::updatePosition() {
        switch (screenRotation) {
            case 0: //portrait
                x = rotPt0.x;
                y = rotPt0.y;
                break;
            case 180: // OFXIPHONE_ORIENTATION_UPSIDEDOWN
                x = rotPt180.x + xScreenRotShift;
                y = rotPt180.y + yScreenRotShift;
                break;
            case 270: //OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT 
                x = rotPt270.x + xScreenRotShift;
                y = rotPt270.y + yScreenRotShift;
                break;
            case 90: //OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT
                x = rotPt90.x  + xScreenRotShift;
                y = rotPt90.y + yScreenRotShift;
                break;
        }
        invalidate();
    };
    
    ofPoint rotate(ofPoint n, float angle) {
        ofPoint m;
        m.x = ((n.x * cos(angle)) - (n.y*sin(angle)));
        m.y = ((n.x * sin(angle)) + (n.y*cos(angle)));	
        return m;
    }
    
    InterfaceObject* InterfaceObject::setScreenRotationAroundCenter() {
        screenRotMode = ROTMODE_ROTATE;
        int scx = ofGetScreenWidth() / 2 ;
        int scy = ofGetScreenHeight() / 2 ;
        ofPoint z;
        z.x = x - scx;
        z.y = scy - y;
        ofPoint r;
        r= rotate(z, -PI/2);
        rotPt90.x =  r.x + scx - h;
        rotPt90.y = scy - r.y;
        r= rotate(z, PI);
        rotPt180.x =  r.x + scx - w;
        rotPt180.y = scy - r.y - h;
        r= rotate(z, PI/2);
        rotPt270.x =  r.x + scx;
        rotPt270.y = scy - r.y - w;
        setScreenRotation(currentScreenOrientation);
        updatePosition();        
        return this;
    }
    
    InterfaceObject* InterfaceObject::setRelativePositioning(float xpos, float ypos) {
        screenRotMode = ROTMODE_ROTATE;
        rotPt0.x = xpos * (float) ofGetWidth();
        rotPt0.y = ypos * (float) ofGetHeight();
        rotPt90.x =  ofGetWidth() - (ypos * (float) ofGetWidth()) - h;
        rotPt90.y = xpos * (float) ofGetHeight();
        rotPt180.x =  ofGetWidth() - (xpos * (float) ofGetWidth()) - w;
        rotPt180.y = ofGetHeight() - (ypos * (float) ofGetHeight()) - h;
        rotPt270.x =  (ypos * (float) ofGetWidth());
        rotPt270.y = ofGetHeight() - (xpos * (float) ofGetHeight()) - w;
        setScreenRotation(currentScreenOrientation);
        updatePosition();
        return this;
        
    }
    
    InterfaceObject* InterfaceObject::setRelativePositioning(float xpos, int xPixelOffset, float ypos, int yPixelOffset)
    {
        screenRotMode = ROTMODE_ROTATE;
        rotPt0.x = (xpos * (float) ofGetWidth()) + xPixelOffset;
        rotPt0.y = (ypos * (float) ofGetHeight()) + yPixelOffset;
        rotPt90.x =  ofGetWidth() - ((ypos * (float) ofGetWidth()) + yPixelOffset) - h;
        rotPt90.y = (xpos * (float) ofGetHeight()) + xPixelOffset;
        rotPt180.x =  ofGetWidth() - ((xpos * (float) ofGetWidth()) + xPixelOffset) - w;
        rotPt180.y = ofGetHeight() - ((ypos * (float) ofGetHeight()) + yPixelOffset) - h;
        rotPt270.x =  (ypos * (float) ofGetWidth()) + yPixelOffset;
        rotPt270.y = ofGetHeight() - ((xpos * (float) ofGetHeight()) + xPixelOffset) - w;
        setScreenRotation(currentScreenOrientation);
        updatePosition();
        return this;
    }
    
    
    //effects
    void InterfaceObject::addEffect(baseEffect *newEffect) {
        effects.push_back(newEffect);
        newEffect->setParent(this);
    }
    
    void InterfaceObject::dropEffects() {
        for(int i=0; i < effects.size(); i++) {
            delete effects.at(i);
        }
        effects.clear();
        invalidate();
    }

    void InterfaceObject::setRotation(float angle) {
        rotation = angle;
        updateRotatedSize();
        invalidate();
    }

    void InterfaceObject::updateRotatedSize() {
        //set the size of the touch capture area to the hypotenuse of the fbo, to make sure to capture touch at any rotation
        if (rotation != 0) {
            rotatedSize = sqrtf(powf(w,2) + powf(h,2));
        }
    }
    
    float InterfaceObject::getRotation() {
        return rotation;
    }
    
    void InterfaceObject::getTouchDownPoint(int &_x, int &_y) {
        _x = tx;
        _y = ty;
    }

    
    bool InterfaceObject::keepThisTouch(ofTouchEventArgs &touch) {
        //could this be the start of a flick? or are we monitoring for external touch up
        return externalTouchUp || (exitFlickDetection && touchVelocity[touch.id] > min(getScaledWidth(), getScaledHeight()) / 5.0);
    }
    
    void InterfaceObject::enableExitFlickDetection(bool val) {
        exitFlickDetection = val;
    }
    
    void InterfaceObject::setTouchAndHoldTime(int newVal) {
        touchAndHoldTime = newVal;
    }

    void InterfaceObject::enableTouchAndHold(bool val) {
        touchAndHoldEnabled = val;
    }
    void InterfaceObject::enableExternalTouchUp(bool val) {
        externalTouchUp = val;
    }

    
    void InterfaceObject::touchMovingToExternal(ofTouchEventArgs &touch) {
        exitGestureStartIdx = MAX(0, touchHistory[touch.id].size() - 5);
    }
    
    bool InterfaceObject::isEnabled() {
        return enabled;
    }

    bool InterfaceObject::enableAlphaWhenDrawing() {
        return true;
    }

    void InterfaceObject::saveEnabledState() {
        wasEnabled = enabled;
    } //used when showing modal groups
    
    void InterfaceObject::restoreEnabledState() {
        setEnabled(wasEnabled);
    }


    
    
    //from http://forum.openframeworks.cc/index.php?topic=4448.0
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
        float cp1x = prevX + 2.0/3.0*(cpx - prevX);
        float cp1y = prevY + 2.0/3.0*(cpy - prevY);
        float cp2x = cp1x + (x - prevX)/3.0;
        float cp2y = cp1y + (y - prevY)/3.0;
        
        // finally call cubic Bezier curve function
        ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
    };
    void roundedRect(float x, float y, float w, float h, float r) {
        ofBeginShape();
        ofVertex(x+r, y);
        ofVertex(x+w-r, y);
        quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
        ofVertex(x+w, y+h-r);
        quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);
        ofVertex(x+r, y+h);
        quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
        ofVertex(x, y+r);
        quadraticBezierVertex(x, y, x+r, y, x, y+r);
        ofEndShape();
    }


    
};