#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    bundler.load();
    bundler.loadCameras();
//    mesh.load("clean.ply");
    
    nCamera = -1;
}

//--------------------------------------------------------------
void ofApp::update(){

    float lerpPct = 0.5;
    if(nCamera >= 0 ){
        if(fCameraPct>0.01){
            cam.setPosition(cam.getPosition().getInterpolated(bundler.cameras[nCamera].getPosition(), 1.0-fCameraPct) );
            ofQuaternion q;
            q.slerp(1.0-fCameraPct,cam.getOrientationQuat(),bundler.cameras[nCamera].getOrientationQuat() );
            cam.setGlobalOrientation(q);
            fCameraPct *= (1.0- powf(10.0, (1.0-lerpPct)*-3.0 ) );
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    
    cam.begin();
    ofPushMatrix();
    
    bundler.draw();
    mesh.drawVertices();
    
    if(nCamera != -1 && bundler.cameras[nCamera].imgPath != "NONE"){
        ofSetColor(255,255.*(1.-fCameraPct));
        img.bind();
        bundler.cameras[nCamera].drawPhotoBillboard();
        img.unbind();
    }
    
    ofPopMatrix();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' ' || key == OF_KEY_RIGHT){
        nCamera++;
        if(nCamera >= bundler.cameras.size()){
            nCamera = -1;
        }
    } if(key == OF_KEY_LEFT){
        nCamera--;
        if(nCamera == -1){
            nCamera = bundler.cameras.size()-1;
        }
    }
    
    if(nCamera != -1 && bundler.cameras[nCamera].imgPath != "NONE"){
        ofLoadImage(img, bundler.cameras[nCamera].imgPath);
    }
    
    fCameraPct = 1.0;

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
