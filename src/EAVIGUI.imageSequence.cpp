//
//  EAVIGUI.imageSequence.cpp
//  FishPolice
//
//  Created by Chris on 04/09/2013.
//
//

#include "EAVIGUI.imageSequence.h"
#include <sstream>

namespace EAVIGUI {
    ImageSequence::ImageSequence(std::string folder, string rootname, string filetype, int start, int end) {
        folder = ofToDataPath(folder);
//        ofDirectory dir(folder);
//        dir.allowExt(filetype);
//        dir.listDir();
//        for(int i = 0; i < dir.numFiles(); i++){
//            ofLogNotice(dir.getPath(i));
//        }
        ims.resize(end - start + 1);
        for(int i=start; i <= end; i++) {
            stringstream fname;
            fname << folder << "/" << rootname << i << "." << filetype;
            ims[i-start].loadImage(fname.str());
            cout << "Loaded " << fname.str() << endl;
        }
    }
    
    ofImage& ImageSequence::getImage(int idx) {
        return ims[idx];
    }
    
};