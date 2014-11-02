//
//  ofxBundler.h
//
//  Created by Patricio Gonzalez Vivo on 10/29/14.
//
//

#pragma once

#include "ofMain.h"

#include "sfmCamera.h"

struct sfmPoint{
    glm::vec3   color;      // [a 3-vector describing the RGB color of the point]
    glm::vec3   position;   // [a 3-vector describing the 3D position of the point]
    vector<double> viewList; // [a list of views the point is visible in]
};

class ofxBundlerViewer {
public:
    
    ofxBundlerViewer();
    virtual ~ofxBundlerViewer();
    
    void    load(string _bundle_rd_out = "bundle.rd.out");
    void    loadList(string _list_txt = "list.txt");
    void    loadCameras(string _cameras_v2_txt = "cameras_v2.txt");
    
    void    draw();
    
    GeoLoc  geoCamerasCentroid;
    
    vector<sfmPoint>    points;
    vector<sfmCamera>   cameras;
    
    vector<int>         geoCamerasIndex;
private:
    
    sfmPoint    getPoint(ofBuffer &_buffer);
};