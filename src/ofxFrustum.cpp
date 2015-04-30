//
//  ofxFrustum.cpp
//
//  Created by Tobias Ebsen on 30/03/15.
//
//

#include "ofxFrustum.h"

ofxFrustum::ofxFrustum() {
    
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    for (int i=0; i<9; i++)
        mesh.addVertex(ofPoint());

    // Near to far cone lines
    mesh.addIndex(1); mesh.addIndex(5);
    mesh.addIndex(2); mesh.addIndex(6);
    mesh.addIndex(3); mesh.addIndex(7);
    mesh.addIndex(4); mesh.addIndex(8);

    // Near rect
    mesh.addIndex(1); mesh.addIndex(2);
    mesh.addIndex(2); mesh.addIndex(3);
    mesh.addIndex(3); mesh.addIndex(4);
    mesh.addIndex(4); mesh.addIndex(1);

    // Far rect
    mesh.addIndex(5); mesh.addIndex(6);
    mesh.addIndex(6); mesh.addIndex(7);
    mesh.addIndex(7); mesh.addIndex(8);
    mesh.addIndex(8); mesh.addIndex(5);

    setShowNear(true);

    set(70, 60, 500, 5000);
}

void ofxFrustum::set() {
    
    double fovx = fovy;
    double fovx_tan = tan(fovx * 0.5 * DEG_TO_RAD) * ratio;
    double fovy_tan = tan(fovy * 0.5 * DEG_TO_RAD);

    vector<ofPoint> & vertices = mesh.getVertices();
    vertices[1] = ofPoint(-fovx_tan*clipNear, -fovy_tan*clipNear, -clipNear);
    vertices[2] = ofPoint(fovx_tan*clipNear, -fovy_tan*clipNear, -clipNear);
    vertices[3] = ofPoint(fovx_tan*clipNear, fovy_tan*clipNear, -clipNear);
    vertices[4] = ofPoint(-fovx_tan*clipNear, fovy_tan*clipNear, -clipNear);

    vertices[5] = ofPoint(-fovx_tan*clipFar, -fovy_tan*clipFar, -clipFar);
    vertices[6] = ofPoint(fovx_tan*clipFar, -fovy_tan*clipFar, -clipFar);
    vertices[7] = ofPoint(fovx_tan*clipFar, fovy_tan*clipFar, -clipFar);
    vertices[8] = ofPoint(-fovx_tan*clipFar, fovy_tan*clipFar, -clipFar);
}

void ofxFrustum::set(double fovx, double fovy, float clipNear, float clipFar) {
    
    this->fovy = fovy;
    this->ratio = fovx / fovy;
    this->clipNear = clipNear;
    this->clipFar = clipFar;

    set();
}

void ofxFrustum::set(ofCamera &camera) {
    this->ratio = (double)ofGetWidth() / (double)ofGetHeight();
    this->fovy = camera.getFov();
    this->clipNear = camera.getNearClip();
    this->clipFar = camera.getFarClip();
    set();
    ofNode::setTransformMatrix(camera.getGlobalTransformMatrix());
}

void ofxFrustum::setFov(double fov) {
    this->fovy = fov;
    set();
}

float ofxFrustum::getFov() {
    return fovy;
}

void ofxFrustum::setViewport(int width, int height) {
    ratio = (double)width / (double)height;
    set();
}

float ofxFrustum::getAspectRatio() {
    return ratio;
}

float ofxFrustum::getWidth(float z) {
    return tan(fovy*0.5f*DEG_TO_RAD) * fabs(z) * ratio;
}

float ofxFrustum::getHeight(float z) {
    return tan(fovy*0.5f*DEG_TO_RAD) * fabs(z);
}

ofVec3f ofxFrustum::relativeToAbsolute(ofVec3f &relative) {
    return ofNode::getGlobalTransformMatrix().preMult(relative);
}

ofVec3f ofxFrustum::absoluteToRelative(ofVec3f &absolute) {
    return ofNode::getGlobalTransformMatrix().getInverse().preMult(absolute);
}

void ofxFrustum::setNearClip(float clipNear) {
    this->clipNear = clipNear;
    set();
}

void ofxFrustum::setFarClip(float clipFar) {
    this->clipFar = clipFar;
    set();
}

float ofxFrustum::getNearClip() {
    return clipNear;
}

float ofxFrustum::getFarClip() {
    return clipFar;
}

void ofxFrustum::setShowNear(bool bShowNear) {
    this->bShowNear = bShowNear;
    vector<ofIndexType> & indices = mesh.getIndices();
    indices.resize(24);
    if (bShowNear) {
        // Camera to near cone lines
        mesh.addIndex(0); mesh.addIndex(1);
        mesh.addIndex(0); mesh.addIndex(2);
        mesh.addIndex(0); mesh.addIndex(3);
        mesh.addIndex(0); mesh.addIndex(4);
    }
}

void ofxFrustum::draw() {
    ofPushMatrix();
    ofMultMatrix(ofNode::getGlobalTransformMatrix());
    mesh.drawWireframe();
    ofPopMatrix();
}

bool ofxFrustum::inside(ofVec3f &vec, bool absolute) {
    
    ofVec3f relative = vec;

    if (absolute)
        relative = ofNode::getGlobalTransformMatrix().getInverse().preMult(vec);
    
    float width = getWidth(relative.z);
    float height = getHeight(relative.z);

    return (relative.x < width) && (relative.x > -width) && (relative.y < height) && (relative.y > -height);
}