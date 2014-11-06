#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxSimpleGuiToo.h"
#include "surflib.h"
#include "ofxOpenSurf.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofVideoGrabber movie;
    
        ofxCvColorImage rgb, hsb;
        ofxCvGrayscaleImage hue, sat, bri, filtered;
        ofxCvContourFinder contours;
    
        // Feature points definitions
        IpPairVec matches;
        IpPairVec vmatches;
    
        ofPoint src_corners[4];
        ofPoint dst_corners[4];
    
        ofxSurfImage sImg;
        ofxSurfImage dImg;
        ofxSurfImage camImg;
        ofxSurfImage dvImg;
    
        ofxSurfMotion mov;
        // Feature points end
    
        int w, h;
        int findHue;

        ofImage img;
        ofSerial mySerial;
    
};
