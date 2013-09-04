//
//  EAVIGUI.multiImageButton.h
//  FishPolice
//
//  Created by Chris on 30/08/2013.
//
//

#ifndef __FishPolice__EAVIGUI_multiImageButton__
#define __FishPolice__EAVIGUI_multiImageButton__

#include <iostream>

#include "EAVIGUI.interfaceObject.h"
#include <iostream>
#include <list>
#include <vector>
using namespace std;

namespace EAVIGUI {
    
    /**
     a button with multiple states. The touch image is an overlay.
     */
    class MultiImageButton : public InterfaceObject {
    public:
        MultiImageButton(InterfaceListener *_listener, int _id, int _x, int _y, string state0FileName, string touchOverlayImageFilename);
        void setCanTouchTranparency(bool newVal);
        bool getCanTouchTranparency();
        bool canInteractAt(int x, int y);
        void touchDown(ofTouchEventArgs &touch);
        void touchUp(ofTouchEventArgs &touch);
        void touchExit(ofTouchEventArgs &touch);
        void setEnabled(bool newEnabled);
        void setState(int val);
        int getState(){return currentState;}
        int addState(string imageFileName);
        void addDisabledState(string filename);
    protected:
        void init();
        virtual void drawToBuffer();
        ofImage touchIm;
        vector<ofImage> states;
        int currentState;
        bool canTouchTranparency;  //if false, reject touches on transparent pixels
        ofImage disabledIm;
    };
    
};

#endif /* defined(__FishPolice__EAVIGUI_multiImageButton__) */
