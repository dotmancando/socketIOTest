#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxSocketIO.h"
#include "ofxSocketIOData.h"
#include "ofxHttpUtils.h"


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void gotEvent(std::string& name);
    void onServerEvent(ofxSocketIOData& data);
    void onPingEvent(ofxSocketIOData& data);
    
    ofxSocketIO socketIO;
    
    bool isConnected;
    void onConnection();
    void bindEvents();
    ofEvent<ofxSocketIOData&> serverEvent;
    ofEvent<ofxSocketIOData&> pingEvent;
    
    std::string address;
    std::string status;
    
    void newResponse(ofxHttpResponse & response);
    
    ofxHttpUtils httpUtils;
    string responseStr;
    string token = "";
    void requestToken();
    const string SERVER_ADDRESS = "localhost";
    const int SERVER_PORT = 5000;
    const string MACHINE_ID = "OF_CAT";
};