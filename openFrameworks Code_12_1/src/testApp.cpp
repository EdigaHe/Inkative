#include "testApp.h"

int orx = 0;
int ory = 0;
int ors = 0;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(100);
    ofBackground(0,0,0);
    w = 640;
    h = 480;
    // Featured points begin
    ofSetVerticalSync(true);
    camImg.allocate(w,h);
    dvImg.loadImage("grabScreen.jpg");
    dvImg.calcSurf(OFX_SURF_VIDEOMATCH);
    mov.setup(&camImg);
    
    src_corners[0].set(0,0);
    src_corners[1].set(hsb.width,0);
    src_corners[2].set(hsb.width,hsb.height);
    src_corners[3].set(0,hsb.height);
    
    movie.initGrabber(w, h, true);
    
    //reserve memory for cv images
    rgb.allocate(w,h);
    hsb.allocate(w,h);
    hue.allocate(w,h);
    sat.allocate(w,h);
    bri.allocate(w,h);
    filtered.allocate(w,h);
    
    mySerial.setup("/dev/tty.usbserial-A601EE8V",9600);
}

//--------------------------------------------------------------
void testApp::update(){
    movie.update();
    
    /*unsigned char myByte = 255;
    bool byteWasWritten = mySerial.writeByte(myByte);
    if(!byteWasWritten)
        printf("The byte was not written to serial port!");*/
    
    if(movie.isFrameNew()){
        
        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels(),w,h);
        
        //mirror horizontal
        rgb.mirror(false,true);
        
        //duplicate rgb
        hsb = rgb;
        
        //covert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for(int i = 0; i< w*h; i++){
            filtered.getPixels()[i] = ofInRange(hue.getPixels()[i], findHue-5, findHue+5)?255:0;
        }
        filtered.flagImageChanged();
        
        //run the contour finder on the filtered image to tfind blobs with a certain hue
        contours.findContours(filtered, 20, w*h/4, 1, true);
        
        // Calc feature points
        dvImg.loadImage("grabScreen.jpg");
        dvImg.calcSurf(OFX_SURF_VIDEOMATCH);
        ofxSurfVideoMatch(&camImg,&dvImg,&vmatches);
        
        
        // Send data package
        int x = contours.blobs[0].centroid.x;
        int y = contours.blobs[0].centroid.y;
        int w = contours.blobs[0].boundingRect.width;
        int h = contours.blobs[0].boundingRect.height;
        
        if(x != orx){
            orx = x;
            bool byteWasWritten_x = mySerial.writeByte('x');
            if(!byteWasWritten_x)
                printf("x is not written!");
        }
        if(y != ory){
            ory = y;
            bool byteWasWritten_y = mySerial.writeByte('y');
            if(!byteWasWritten_y)
                printf("y is not written!");
        }
        if(w*h != ors){
            ors = w*h;
            bool byteWasWritten_s = mySerial.writeByte('s');
            if(!byteWasWritten_s)
                printf("s is not written!");
            
        }
        
        printf("X = %d\n", x);
        printf("Y = %d\n", y);
        printf("W = %d\n", w);
        printf("H = %d\n", h);
        
        
    }

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255,255);
    rgb.draw(0,0);
    filtered.draw(640,0);
    contours.draw(640,0);
    
    /*ofSetColor(255, 0, 0);
    ofFill();
    
    //draw red circles for found blobs
    for(int i=0; i<contours.nBlobs; i++){
        ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
    }*/
    
    // Draw featured points part
    ofSetColor(0xffffff);
    //video match
    camImg.draw(0,480);
    dvImg.draw(camImg.width,480);
    ofxDrawIpoints(0,0,1,camImg.ipts);
    ofxDrawIpoints(camImg.width,0,1,dvImg.ipts);
    ofSetColor(0xffff00);
    if(ofxSurfObjCorners(vmatches,src_corners,dst_corners)){
        
    }
    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //calculate local mouse x, y in image
    int mx = x % w;
    int my = y % h;
    
    //get hue value on mouse position
    findHue = hue.getPixels()[my*w+mx];
    
    movie.update();
    
    if(movie.isFrameNew()){
        
        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels(),w,h);
        
        //mirror horizontal
        rgb.mirror(false,true);
        
        //duplicate rgb
        hsb = rgb;
        
        //covert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for(int i = 0; i< w*h; i++){
            filtered.getPixels()[i] = ofInRange(hue.getPixels()[i], findHue-5, findHue+5)?255:0;
        }
        filtered.flagImageChanged();
        
        //run the contour finder on the filtered image to tfind blobs with a certain hue
        contours.findContours(filtered, 20, w*h/4, 1, true);
        
        // Save contour Image
        img.grabScreen(contours.blobs[0].centroid.x-contours.blobs[0].boundingRect.width/2, contours.blobs[0].centroid.y-contours.blobs[0].boundingRect.height/2, contours.blobs[0].boundingRect.width, contours.blobs[0].boundingRect.height);
        img.saveImage("grabScreen.jpg");
      
        // Featured points end
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
