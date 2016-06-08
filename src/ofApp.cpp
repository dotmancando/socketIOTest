#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    isConnected = false;
    address = "http://localhost:5000";
    status = "not connected";
    
    //  socketIO.setup(address);
    ofAddListener(socketIO.notifyEvent, this, &ofApp::gotEvent);
    
    ofAddListener(socketIO.connectionEvent, this, &ofApp::onConnection);
    
    httpUtils.start();
    ofAddListener(httpUtils.newResponseEvent,this,&ofApp::newResponse);
    requestToken();
}

void ofApp::onConnection () {
    isConnected = true;
    bindEvents();
}

void ofApp::bindEvents () {
    std::string serverEventName = "server-event";
    socketIO.bindEvent(serverEvent, serverEventName);
    ofAddListener(serverEvent, this, &ofApp::onServerEvent);
    
    std::string pingEventName = "echo";
    socketIO.bindEvent(pingEvent, pingEventName);
    ofAddListener(pingEvent, this, &ofApp::onPingEvent);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapStringHighlight(ofApp::status, 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //    ofLogNotice("ofxSocketIO", "echo");
    ofLog() << "sending";
    std::string pong = "echo";
    std::string param = "hello";
    socketIO.emit(pong, param);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::gotEvent(string& name) {
    status = name;
}

//--------------------------------------------------------------
void ofApp::onServerEvent (ofxSocketIOData& data) {
    ofLogNotice("ofxSocketIO", data.getStringValue("stringData"));
    ofLogNotice("ofxSocketIO", ofToString(data.getIntValue("intData")));
    ofLogNotice("ofxSocketIO", ofToString(data.getFloatValue("floatData")));
    ofLogNotice("ofxSocketIO", ofToString(data.getBoolValue("boolData")));
}

void ofApp::onPingEvent (ofxSocketIOData& data) {
    ofLog() << "getting: " << data.getStringValue("msg");

    //  ofLogNotice("ofxSocketIO", "echo");
    //  std::string pong = "pong";
    //  std::string param = "foo";
    //  socketIO.emit(pong, param);
}

void ofApp::requestToken(){
    ofLog() << "requesting token";
    ofBuffer data;
    data.set("id=" + MACHINE_ID);
    httpUtils.postData("http://" + SERVER_ADDRESS + ":" + ofToString(SERVER_PORT)+ "/token", data, "application/x-www-form-urlencoded");
    
}

void ofApp::newResponse(ofxHttpResponse & response){
    
    ofLog() << "getting = " << ofToString(response.status);
    
    if(response.status == 200){
        ofLog() << "got result 200 OK";
        string txt = (string)response.responseBody;
        ofLog() << "txt = " << txt;
        // this is extremely hacky rightnow, basically we find the third " which i know that it's after location 11
        token = txt.substr (txt.find('"',11)+1, strlen(txt.c_str()) - txt.find('"',11) - 4);
        ofLog() << "token = " << token;
        
        address = "http://127.0.0.1:5000";
        ofLog() << "address = " << address;

        std::map<std::string,std::string> query = {{"token", token}};
        
        socketIO.setup(address, query);
    }
    
}
