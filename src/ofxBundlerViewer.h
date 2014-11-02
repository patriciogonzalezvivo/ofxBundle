//
//  ofxBundler.h
//
//  Created by Patricio Gonzalez Vivo on 10/29/14.
//
//

#pragma once

#include "ofMain.h"

#include "sfmCamera.h"

class ofxBundlerViewer {
public:
    
    ofxBundlerViewer();
    virtual ~ofxBundlerViewer();
    
    void    load(string _bundle_rd_out = "bundle.rd.out");
    void    loadList(string _list_txt = "list.txt");
    void    loadCameras(string _cameras_v2_txt = "cameras_v2.txt");
    
    void    draw();
    
    GeoLoc  geoCamerasCentroid;
    
    ofVboMesh           points;
    vector<sfmCamera>   cameras;
    
    vector<int>         geoCamerasIndex;
private:
    
};