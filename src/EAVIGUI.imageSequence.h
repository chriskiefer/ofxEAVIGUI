//
//  EAVIGUI.imageSequence.h
//  FishPolice
//
//  Created by Chris on 04/09/2013.
//
//

#ifndef __FishPolice__EAVIGUI_imageSequence__
#define __FishPolice__EAVIGUI_imageSequence__

#include <iostream>
#include "ofMain.h"

namespace EAVIGUI {
    class ImageSequence {
    public:
        ImageSequence(std::string folder, string rootname, string filetype, int start, int end);
        ofImage& getImage(int idx);
        int size(){return ims.size();}
    protected:
        std::vector<ofImage> ims;
    };
};

#endif /* defined(__FishPolice__EAVIGUI_imageSequence__) */
