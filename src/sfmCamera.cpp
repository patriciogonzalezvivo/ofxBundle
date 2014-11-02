//
//  sfmCamera.cpp
//  mapBundler
//
//  Created by Patricio Gonzalez Vivo on 11/2/14.
//
//

#include "sfmCamera.h"

sfmCamera::sfmCamera(){
    
}

sfmCamera::~sfmCamera(){
    
}

void sfmCamera::setIntrinsics(ofBuffer &_buffer){
    
    vector<string> intrinsics = ofSplitString(_buffer.getNextLine(), " ");
    f = ofToDouble(intrinsics[0]);
    k1 = ofToDouble(intrinsics[1]);
    k2 = ofToDouble(intrinsics[2]);
    
    for (int i = 0; i < 3; i++) {
        vector<string> row = ofSplitString(_buffer.getNextLine(), " ");
        R[i][0] = ofToFloat(row[0]);
        R[i][1] = ofToFloat(row[1]);
        R[i][2] = ofToFloat(row[2]);
    }
    
    vector<string> transfor = ofSplitString(_buffer.getNextLine(), " ");
    t.x = ofToFloat(transfor[0]);
    t.y = ofToFloat(transfor[1]);
    t.z = ofToFloat(transfor[2]);
    
    //  Convert values to OF
    //
    glm::quat q = glm::quat_cast(R);
    rot.set(q[0], q[1], q[2], q[3]);
    
    glm::vec3 p = -R * t;
    pos.set(p[0],p[1],p[2]);
}

void sfmCamera::setExtrinsics(ofBuffer &_buffer){
    
    //  Space
    //  vis image
    for (int i = 0; i < 2; i++) {
        string tmp = _buffer.getNextLine();
    }
    
    string imgFile = _buffer.getNextLine();
    double focalLength = ofToDouble(_buffer.getNextLine());
    if(focalLength == f){
        cout << "Focal lenght match on " << imgFile << endl;
    } else {
        cout << "Focal lenght DON'T match on " << imgFile << endl;
    }
    
    if(ofFile(ofToDataPath(imgFile)).exists()){
        //        _cam.img.loadImage(ofToDataPath(imgFile));
        imgPath = imgFile;
    }
    
    for (int i = 0; i < 9; i++) {
        string tmp = _buffer.getNextLine();
    }
    
    string geoString = _buffer.getNextLine();
    if(geoString != "0 0 0"){
        vector<string> geoValues = ofSplitString(geoString," ");
        loc.lat = ofToDouble(geoValues[0]);
        loc.lon = ofToDouble(geoValues[1]);
        loc.alt = ofToDouble(geoValues[2]);
        cout << "Geo location captured at " << loc.lat << "," << loc.lon << endl;
    }
    
}


void sfmCamera::draw(){
    
    float w = 0.1;
    float h = 0.1;
    float d = 0.2;
    
    glm::vec3 topLeft = R * glm::vec3(-w,-h,-d);
    glm::vec3 topRight = R * glm::vec3(w,-h,-d);
    glm::vec3 bottomRight = R * glm::vec3(w,h,-d);
    glm::vec3 bottomLeft = R * glm::vec3(-w,h,-d);
    
    if(loc.lat != 0 || loc.lon != 0){
        ofSetColor(0,255,0);
    } else {
        ofSetColor(255,0,0);
    }
    
    ofLine(pos,pos+ofPoint(topLeft.x,topLeft.y,topLeft.z));
    ofLine(pos+ofPoint(topLeft.x,topLeft.y,topLeft.z),pos+ofPoint(topRight.x,topRight.y,topRight.z));
    ofLine(pos,pos+ofPoint(topRight.x,topRight.y,topRight.z));
    ofLine(pos+ofPoint(topRight.x,topRight.y,topRight.z),pos+ofPoint(bottomRight.x,bottomRight.y,bottomRight.z));
    ofLine(pos,pos+ofPoint(bottomRight.x,bottomRight.y,bottomRight.z));
    ofLine(pos+ofPoint(bottomRight.x,bottomRight.y,bottomRight.z),pos+ofPoint(bottomLeft.x,bottomLeft.y,bottomLeft.z));
    ofLine(pos,pos+ofPoint(bottomLeft.x,bottomLeft.y,bottomLeft.z));
    ofLine(pos+ofPoint(bottomLeft.x,bottomLeft.y,bottomLeft.z),pos+ofPoint(topLeft.x,topLeft.y,topLeft.z));
}