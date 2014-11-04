//
//  sfmCamera.cpp
//  mapBundler
//
//  Created by Patricio Gonzalez Vivo on 11/2/14.
//
//

#include "sfmCamera.h"

sfmCamera::sfmCamera():imgPath("NONE"),R(1.0),t(0.,0.,0.),lat(0.0),lon(0.0),alt(0.0),f(0.0),k1(0.0),k2(0.0){
    
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
    setOrientation(ofQuaternion(q[0], q[1], q[2], q[3]));
    
    glm::vec3 p = -R * t;
    setPosition(p[0],p[1],p[2]);
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
//        cout << "Focal lenght match on " << imgFile << endl;
    } else {
        cout << "ERROR: Focal lenght DON'T match on " << imgFile << endl;
    }
    
    if(ofFile(ofToDataPath(imgFile)).exists()){
        imgPath = imgFile;
    }
    
    vector<string> sizeValues = ofSplitString(_buffer.getNextLine()," ");
    width = ofToFloat(sizeValues[0]);
    height = ofToFloat(sizeValues[1]);
    
    for (int i = 0; i < 8; i++) {
        string tmp = _buffer.getNextLine();
    }
    
    string geoString = _buffer.getNextLine();
    if(geoString != "0 0 0"){
        vector<string> geoValues = ofSplitString(geoString," ");
        lat = ofToDouble(geoValues[0]);
        lon = ofToDouble(geoValues[1]);
        alt = ofToDouble(geoValues[2]);
    }
    
}


void sfmCamera::draw(){
    
    float w = width/f;
    float h = height/f;
    float d = 0.5;

    glm::vec3 points[4] = { R * glm::vec3(-w,h,-d),
                            R * glm::vec3(w,h,-d),
                            R * glm::vec3(w,-h,-d),
                            R * glm::vec3(-w,-h,-d) };
    
    ofPoint center = getPosition();
    ofPoint topLeft = center+ofPoint(points[0].x,points[0].y,points[0].z);
    ofPoint topRight = center+ofPoint(points[1].x,points[1].y,points[1].z);
    ofPoint bottomRight = center+ofPoint(points[2].x,points[2].y,points[2].z);
    ofPoint bottomLeft = center+ofPoint(points[3].x,points[3].y,points[3].z);
    
    
    if(lat != 0 || lon != 0){
        ofSetColor(0,255,0);
    } else {
        ofSetColor(255,0,0);
    }
    
    ofLine(center,topLeft);
    ofLine(topLeft,topRight);
    ofLine(center,topRight);
    ofLine(topRight,bottomRight);
    ofLine(center,bottomRight);
    ofLine(bottomRight,bottomLeft);
    ofLine(center,bottomLeft);
    ofLine(bottomLeft,topLeft);
}

void sfmCamera::drawPhotoBillboard(){
    
    glm::vec3 points[4] = { R * glm::vec3(-width,height,-f),
                            R * glm::vec3(width,height,-f),
                            R * glm::vec3(width,-height,-f),
                            R * glm::vec3(-width,-height,-f) };
    
    ofPoint center = getPosition();
    ofPoint topLeft = center+ofPoint(points[0].x,points[0].y,points[0].z);
    ofPoint topRight = center+ofPoint(points[1].x,points[1].y,points[1].z);
    ofPoint bottomRight = center+ofPoint(points[2].x,points[2].y,points[2].z);
    ofPoint bottomLeft = center+ofPoint(points[3].x,points[3].y,points[3].z);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3fv(&topLeft.x);
    glTexCoord2f(width*2.0, 0); glVertex3fv(&topRight.x);
    glTexCoord2f(width*2.0, height*2.0); glVertex3fv(&bottomRight.x);
    glTexCoord2f(0,height*2.0);  glVertex3fv(&bottomLeft.x);
    glEnd();
}