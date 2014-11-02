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
                sfmCamera cam;
                cam.setIntrinsics(buffer);
                cameras.push_back(cam);
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
            cameras[counter].setExtrinsics(buffer);
            
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

void ofxBundlerViewer::draw(){
    for (int i = 0; i < cameras.size(); i++) {
        cameras[i].draw();
    }
    
    glBegin(GL_POINTS);
    for (int i = 0; i < points.size(); i++) {
        glColor3fv(&points[i].color.r);
        glVertex3fv(&points[i].position.x);
    }
    glEnd();
}