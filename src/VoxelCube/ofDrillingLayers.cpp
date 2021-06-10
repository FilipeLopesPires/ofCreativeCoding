#include "ofDrillingLayers.h"

//--------------------------------------------------------------
void ofDrillingLayers::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetColor(239, 39, 39);
	ofEnableDepthTest();

	this->len = 15;
	for (auto x = -150; x <= 150; x += this->len) {
		for (auto y = -150; y <= 150; y += this->len) {
			for (auto z = -150; z <= 150; z += this->len) {
				this->box_location_list.push_back(glm::vec3(x, y, z));
				this->alpah_param_list.push_back(255);
			}
		}
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofDrillingLayers::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
	auto noise_location = glm::vec3(ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.01), 0, 1, -300, 300), ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.01), 0, 1, -300, 300), ofMap(ofNoise(noise_seed.z, ofGetFrameNum() * 0.01), 0, 1, -300, 300));
	this->log_list.push_back(noise_location);
	while (this->log_list.size() > 30) { this->log_list.erase(this->log_list.begin()); }

	for (int i = 0; i < this->box_location_list.size(); i++) {

		if (glm::distance(this->box_location_list[i], noise_location) < 60) {
			this->alpah_param_list[i] = 0;
		}

		if (this->alpah_param_list[i] > 200) {
			this->setBoxToMesh(this->face, this->frame, this->box_location_list[i], this->len, ofColor(39, ofMap(this->alpah_param_list[i], 200, 255, 0, 255)));
		}

		this->alpah_param_list[i] = this->alpah_param_list[i] >= 255 ? 255 : this->alpah_param_list[i] + 0.5;
	}
}

//--------------------------------------------------------------
void ofDrillingLayers::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.85);
	ofRotateX(ofGetFrameNum() * 0.5);

	ofNoFill();
	ofBeginShape();
	ofVertices(this->log_list);
	ofEndShape();

	ofFill();
	ofDrawSphere(this->log_list.back(), 10);

	this->face.drawFaces();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofDrillingLayers::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size, ofColor color) {
	this->setBoxToMesh(face_target, frame_target, location, size, size, size, color);
}

//--------------------------------------------------------------
void ofDrillingLayers::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth, ofColor color) {

	int index = face_target.getVertices().size();

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addIndex(index + 0); face_target.addIndex(index + 1); face_target.addIndex(index + 2);
	face_target.addIndex(index + 0); face_target.addIndex(index + 2); face_target.addIndex(index + 3);

	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 4); face_target.addIndex(index + 6); face_target.addIndex(index + 7);

	face_target.addIndex(index + 0); face_target.addIndex(index + 4); face_target.addIndex(index + 1);
	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 1);

	face_target.addIndex(index + 1); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 6); face_target.addIndex(index + 2); face_target.addIndex(index + 1);

	face_target.addIndex(index + 2); face_target.addIndex(index + 6); face_target.addIndex(index + 7);
	face_target.addIndex(index + 7); face_target.addIndex(index + 3); face_target.addIndex(index + 2);

	face_target.addIndex(index + 3); face_target.addIndex(index + 7); face_target.addIndex(index + 4);
	face_target.addIndex(index + 4); face_target.addIndex(index + 0); face_target.addIndex(index + 3);

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 1);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 0);

	frame_target.addIndex(index + 4); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 5); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 7);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 7);

	for (int i = 0; i < 8; i++) {
		face_target.addColor(color);
		frame_target.addColor(ofColor(239));
	}
}
