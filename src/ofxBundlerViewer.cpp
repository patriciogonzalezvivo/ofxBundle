//
//  ofxBundler.cpp
//  mapBundler
//
//  Created by Patricio Gonzalez Vivo on 10/29/14.
//
//

#include "ofxBundlerViewer.h"


ofxBundlerViewer::ofxBundlerViewer(){
    
}

ofxBundlerViewer::~ofxBundlerViewer(){
    
}

void ofxBundlerViewer::load(string _bundleRdOut){
    
    ofBuffer buffer = ofBufferFromFile(_bundleRdOut);
    
    int counter = 0;
    int totalCameras = 0;
    int totalPoints = 0;
    while(!buffer.isLastLine()) {
        if(counter == 0){
            string temp = buffer.getNextLine();
            if(temp == "# Bundle file v0.3"){
                cout << "Is a valid Bundle file format" << endl;
            } else {
                cout << "Is NOT a valid Bundle file format" << endl;
                return;
            }
        } else if (counter == 1){
            string temp = buffer.getNextLine();
            vector<string> values = ofSplitString(temp, " ");
            totalCameras = ofToInt(values[0]);
            totalPoints = ofToInt(values[1]);
            cout << "About to load " << totalCameras << " and " << totalPoints << " points"<< endl;
        } else {
            if(totalCameras != cameras.size()){
                cameras.push_back(getCameraIntrinsics(buffer));
            } else {
                points.push_back(getPoint(buffer));
            }
        }
        counter++;
    }
    
    if(totalCameras == cameras.size() && totalPoints == points.size()){
        cout << "Bundle file LOADED" << endl;
    }
}

sfmCamera ofxBundlerViewer::getCameraIntrinsics(ofBuffer &_buffer){
    sfmCamera rta;
    
    vector<string> intrinsics = ofSplitString(_buffer.getNextLine(), " ");
    rta.f = ofToDouble(intrinsics[0]);
    rta.k1 = ofToDouble(intrinsics[1]);
    rta.k2 = ofToDouble(intrinsics[2]);
    
    for (int i = 0; i < 3; i++) {
        vector<string> row = ofSplitString(_buffer.getNextLine(), " ");
        rta.R[i][0] = ofToFloat(row[0]);
        rta.R[i][1] = ofToFloat(row[1]);
        rta.R[i][2] = ofToFloat(row[2]);
    }
    
    vector<string> pos = ofSplitString(_buffer.getNextLine(), " ");
    rta.t.x = ofToFloat(pos[0]);
    rta.t.y = ofToFloat(pos[1]);
    rta.t.z = ofToFloat(pos[2]);
    
    //  Convert values to OF
    //
    glm::quat q = glm::quat_cast(rta.R);
    rta.rot.set(q[0], q[1], q[2], q[3]);
    
    glm::vec3 p = -rta.R * rta.t;
    rta.pos.set(p.x,p.y,p.z);
    
    return rta;
}

sfmPoint ofxBundlerViewer::getPoint(ofBuffer &_buffer){
    sfmPoint rta;
    
    vector<string> pos = ofSplitString(_buffer.getNextLine(), " ");
    rta.position.x = ofToFloat(pos[0]);
    rta.position.y = ofToFloat(pos[1]);
    rta.position.z = ofToFloat(pos[2]);
    
    vector<string> col = ofSplitString(_buffer.getNextLine(), " ");
    rta.color.r = (float)ofToInt(col[0])/255.;
    rta.color.g = (float)ofToInt(col[1])/255.;
    rta.color.b = (float)ofToInt(col[2])/255.;
    
    vector<string> vList = ofSplitString(_buffer.getNextLine(), " ");
    for (int i = 0; i < vList.size(); i++) {
        rta.viewList.push_back(ofToDouble(vList[i]));
    }
    
    return rta;
}

void ofxBundlerViewer::loadList(string _list_txt){
    //  TODO
    //
}


void ofxBundlerViewer::loadCameras(string _cameras_v2_txt){
    
    if(cameras.size()==0){
        cout << "Please load the bundle file first" << endl;
        return;
    }
    
    ofBuffer buffer = ofBufferFromFile(_cameras_v2_txt);
    
    int counter = 0;
    bool bStart = false;
    while(!buffer.isLastLine()) {
        if(!bStart){
            string temp = buffer.getNextLine();
            if(temp == ofToString(cameras.size())){
                cout << "The file match the number of cameras. Proceding to load the extra data" << endl;
                bStart = true;
            }
        } else {
            if(counter == cameras.size()){
                break;
            }
            getCameraExtrinsics(cameras[counter],buffer);
            
            if(cameras[counter].loc.lon != 0 || cameras[counter].loc.lat != 0){
                geoCamerasIndex.push_back(counter);
            }
            counter++;
        }
    }
    
    if(geoCamerasIndex.size() > 0){
        
        //  Calculate GEO-Centroid and SfM
        //
        geoCamerasCentroid.lat = 0.0;
        geoCamerasCentroid.lon = 0.0;
        geoCamerasCentroid.alt = 0.0;
        
        for(unsigned int i = 0; i < geoCamerasIndex.size(); i++) {
            geoCamerasCentroid.lat += cameras[geoCamerasIndex[i]].loc.lat;
            geoCamerasCentroid.lon += cameras[geoCamerasIndex[i]].loc.lon;
            geoCamerasCentroid.alt += cameras[geoCamerasIndex[i]].loc.alt;
        }
        geoCamerasCentroid.lat /= geoCamerasIndex.size();
        geoCamerasCentroid.lon /= geoCamerasIndex.size();
        geoCamerasCentroid.alt /= geoCamerasIndex.size();
        
    }
}

void ofxBundlerViewer::getCameraExtrinsics(sfmCamera &_cam, ofBuffer &_buffer){
    
    //  Space
    //  vis image
    for (int i = 0; i < 2; i++) {
        string tmp = _buffer.getNextLine();
    }
    
    string imgFile = _buffer.getNextLine();
    double focalLength = ofToDouble(_buffer.getNextLine());
    if(focalLength == _cam.f){
        cout << "Focal lenght match on " << imgFile << endl;
    } else {
        for (int i = 0; i < 4; i++) {
            string tmp = _buffer.getNextLine();
        }
    }
    
    if(ofFile(ofToDataPath(imgFile)).exists()){
//        _cam.img.loadImage(ofToDataPath(imgFile));
        _cam.imgPath = imgFile;
    }
    
    for (int i = 0; i < 9; i++) {
        string tmp = _buffer.getNextLine();
    }
    
    string geoString = _buffer.getNextLine();
    if(geoString != "0 0 0"){
        vector<string> geoValues = ofSplitString(geoString," ");
        _cam.loc.lat = ofToDouble(geoValues[0]);
        _cam.loc.lon = ofToDouble(geoValues[1]);
        _cam.loc.alt = ofToDouble(geoValues[2]);
        cout << "Geo location captured at " << _cam.loc.lat << "," << _cam.loc.lon << endl;
    }
    
}

void ofxBundlerViewer::draw(){
    for (int i = 0; i < cameras.size(); i++) {

        glm::vec3 dir = cameras[i].R * glm::vec3(0,0,-0.2);
        if(cameras[i].loc.lat != 0 || cameras[i].loc.lon != 0){
            ofSetColor(0,255,0);
        } else {
            ofSetColor(255,0,0);
        }
        
        ofLine(ofPoint(cameras[i].pos.x,cameras[i].pos.y,cameras[i].pos.z),
               ofPoint(cameras[i].pos.x,cameras[i].pos.y,cameras[i].pos.z)+ofPoint(dir.x,dir.y,dir.z));

    }
    
    glBegin(GL_POINTS);
    for (int i = 0; i < points.size(); i++) {
        glColor3fv(&points[i].color.r);
        glVertex3fv(&points[i].position.x);
    }
    glEnd();
}