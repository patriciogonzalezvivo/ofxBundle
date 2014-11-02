//
//  sfmCamera.h
//
//  Created by Patricio Gonzalez Vivo on 11/2/14.
//
//

#pragma once

#include "ofMain.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

struct GeoLoc {
    GeoLoc():lon(0),lat(0),alt(0){};
    double lon,lat,alt;
};

//  Using he reference from: https://github.com/snavely/bundler_sfm
//
class sfmCamera {
public:
    
    sfmCamera();
    virtual ~sfmCamera();
    
    void    setIntrinsics(ofBuffer &_buffer);
    void    setExtrinsics(ofBuffer &_buffer);
    
    void    draw();
    
    ofQuaternion    rot;        //  Camera Orientation
    ofPoint         pos;        //  Camera Position
    
    //  From cameras_v2.txt
    //
    std::string     imgPath;    //  Image;
    GeoLoc          loc;        //  Geo location
    
    //  From boundle.rd.out
    //
    glm::mat3   R;          //  [a 3x3 matrix representing the camera rotation]
    glm::vec3   t;          //  [a 3-vector describing the camera translation]
    double      f, k1, k2;  //  [the focal length, followed by two radial distortion coeffs]
    
    
};