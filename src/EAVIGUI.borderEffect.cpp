//
//  EAVIGUI.borderEffect.cpp
//  FishPolice
//
//  Created by Chris on 01/07/2013.
//
//

#include "EAVIGUI.borderEffect.h"

namespace EAVIGUI {
    
    borderEffect::borderEffect(ofColor newcol) : col(newcol) {
        baseEffect::baseEffect();
    }
    
    void borderEffect::draw() {
        ofNoFill();
        ofSetLineWidth(5);
        ofSetColor(col);
        ofRect(1, 1, this->parentObject->getWidth()-1, this->parentObject->getHeight()-1);
        ofSetLineWidth(1);
    }
    
};