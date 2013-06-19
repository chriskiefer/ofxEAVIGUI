//
//  highLightEffect.cpp
//  FishPolice
//
//  Created by Chris on 11/06/2013.
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

#include "highLightEffect.h"

namespace EAVIGUI {

    highLightEffect::highLightEffect(ofColor newcol) : col(newcol) {
        baseEffect::baseEffect();
    }
    
    void highLightEffect::draw() {
        ofFill();
        ofSetColor(col);
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
//        ofSetColor(0,255,0,fabs(sin(ofGetFrameNum() / 3.0)) * 10.0);
//        ofNoFill();
//        for (int i = 0; i < 400; i++){
//            ofLine(ofRandom(0,500), ofRandom(0,500), ofRandom(0,500), ofRandom(0,500));
//        }

        ofRect(0, 0, this->parentObject->getWidth(), this->parentObject->getHeight());
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

};