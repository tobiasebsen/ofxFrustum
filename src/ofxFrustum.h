//
//  ofxFrustum.h
//
//  Created by Tobias Ebsen on 30/03/15.
//
//

#pragma once

#include "ofMain.h"

class ofxFrustum : public ofNode {
public:
    ofxFrustum();

    void set(double fovx, double fovy, float clipNear, float clipFar);
    void set(ofCamera & camera);
    
    void setFov(double fov);
    float getFov();

    void setViewport(int width, int height);
    float getAspectRatio();
    
    float getWidth(float relativeZ);
    float getHeight(float relativeZ);
    
    ofVec3f relativeToAbsolute(ofVec3f & relative);
    ofVec3f absoluteToRelative(ofVec3f & absolute);
    
    void setNearClip(float clipNear);
    void setFarClip(float clipFar);
    
    float getNearClip();
    float getFarClip();
    
    void setShowNear(bool bShowNear);
    
    bool inside(ofVec3f & vec, bool absolute = true);
    
    void draw();

private:
    void set();

    ofMesh mesh;

    double fovy;
    double ratio;
    float clipNear;
    float clipFar;
    bool bShowNear;
};