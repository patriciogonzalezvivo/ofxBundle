//
//  ofxBundler.cpp
//  mapBundler
//
//  Created by Patricio Gonzalez Vivo on 10/29/14.
//
//

#include "ofxBundlerViewer.h"


ofxBundlerViewer::ofxBundlerViewer(){
    points.setMode(OF_PRIMITIVE_POINTS);
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
                ofPoint position;
                vector<string> pos = ofSplitString(buffer.getNextLine(), " ");
                position.x = ofToFloat(pos[0]);
                position.y = ofToFloat(pos[1]);
                position.z = ofToFloat(pos[2]);
                
                ofFloatColor color;
                vector<string> col = ofSplitString(buffer.getNextLine(), " ");
                color.r = (float)ofToInt(col[0])/255.;
                color.g = (float)ofToInt(col[1])/255.;
                color.b = (float)ofToInt(col[2])/255.;
                color.a = 1.0;
                
                vector<string> vList = ofSplitString(buffer.getNextLine(), " ");
//                for (int i = 0; i < vList.size(); i++) {
//                    viewList.push_back(ofToDouble(vList[i]));
//                }
                
                points.addColor(color);
                points.addVertex(position);
            }
        }
        counter++;
    }
    
    if(totalCameras == cameras.size() && totalPoints == points.getVertices().size()){
        cout << "Bundle file LOADED" << endl;
    }
}

//struct sfmPoint{
//    ofFloatColor    color;      // [a 3-vector describing the RGB color of the point]
//    ofPoint         position;   // [a 3-vector describing the 3D position of the point]
//    vector<double>  viewList; // [a list of views the point is visible in]
//};

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
    
    points.drawVertices();
}