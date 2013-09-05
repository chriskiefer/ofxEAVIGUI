//
//  EAVIGUI.imageSeqPlayer.h
//  FishPolice
//
//  Created by Chris on 05/09/2013.
//
//

#ifndef __FishPolice__EAVIGUI_imageSeqPlayer__
#define __FishPolice__EAVIGUI_imageSeqPlayer__

#include <iostream>


#include "EAVIGUI.interfaceObject.h"
#include "EAVIGUI.imageSequence.h"

using namespace std;

namespace EAVIGUI {
    
    class ImageSequencePlayer : public InterfaceObject {
    public:
        ImageSequencePlayer(InterfaceListener *_listener, int _id, int _x, int _y, ImageSequence *seq);
        void update();
        void setFrameTimeInMillis(float val);
        void setAlpha(float newVal) {alpha = newVal; invalidate();}
        void setBackgroundColor(ofColor newCol) {backgroundColor = newCol; invalidate();}
    protected:
        void drawToBuffer();
        ImageSequence *imSeq;
        int framePeriod;
        long lastFrameTS;
        ofImage *currIm;
        int seqIndex;
        ofColor backgroundColor;
        float alpha;
    };

}


#endif /* defined(__FishPolice__EAVIGUI_imageSeqPlayer__) */
