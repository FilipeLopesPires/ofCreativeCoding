#pragma once

#include "ofNoisySurfaceColorful.h"

//--------------------------------------------------------------
void ofNoisySurfaceColorful::setup() {

	ofSetWindowTitle("Voxel Cube - Noisy Surface Colorful");
	ofSetFrameRate(60);		// change application's framerate
	ofSetLineWidth(2);		// change line width of small and large cubes
	ofBackground(255);		// change background color (value between 0 and 255)
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	ofColor color;
	vector<int> hex_list = { 0x003060, 0x055C9D, 0x0E86D4, 0x68BBE3 }; // change hex values and add or remove elements at your will
	vector<ofColor> base_color_list;
	for (auto hex : hex_list) {
		color.setHex(hex);
		base_color_list.push_back(color);
	}

	int span = 20;			// change distance between small cubes 

	// change values of x,y,z on each loop to create different patterns
	for (int x = -220; x <= 220; x += span) {
		for (int y = -220; y <= 220; y += span) {
			for (int z = -220; z <= 220; z += 440) {
				this->location_list.push_back(glm::vec3(x, y, z));
				this->color_list.push_back(base_color_list[(int)ofRandom(base_color_list.size())]);
			}
		}
	}

	for (int x = -220; x <= 220; x += 440) {
		for (int y = -220; y <= 220; y += span) {
			for (int z = -220 + span; z <= 220 - span; z += span) {
				this->location_list.push_back(glm::vec3(x, y, z));
				this->color_list.push_back(base_color_list[(int)ofRandom(base_color_list.size())]);
			}
		}
	}

	for (int x = -220 + span; x <= 220 - span; x += span) {
		for (int y = -220; y <= 220; y += 440) {
			for (int z = -220 + span; z <= 220 - span; z += span) {
				this->location_list.push_back(glm::vec3(x, y, z));
				this->color_list.push_back(base_color_list[(int)ofRandom(base_color_list.size())]);
			}
		}
	}
}

//--------------------------------------------------------------
void ofNoisySurfaceColorful::update() {

	this->face.clear();
	this->frame.clear();

	float size;
	for (int i = 0; i < this->location_list.size(); i++) {
		auto noise_value = ofNoise(glm::vec4(this->location_list[i] * 0.005, ofGetFrameNum() * 0.01));
		if (noise_value < 0.5) { size = 0; }
		else if (noise_value > 0.55) { size = 20; }
		else { size = ofMap(noise_value, 0.5, 0.55, 0, 20); }

		this->setBoxToMesh(this->face, this->frame, this->location_list[i], size, this->color_list[i]);
	}
}

//--------------------------------------------------------------
void ofNoisySurfaceColorful::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.21);
	ofRotateY(ofGetFrameNum() * 0.37);

	this->face.draw();

	ofSetColor(39);					// change wireframe color (value between 0 and 255)
	this->frame.drawWireframe();

	ofNoFill();
	ofSetColor(0);
	ofDrawBox(440 + 16);

	this->cam.end();
}

//--------------------------------------------------------------
void ofNoisySurfaceColorful::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size, ofColor face_color) {
	this->setBoxToMesh(face_target, frame_target, location, size, size, size, face_color);
}

//--------------------------------------------------------------
void ofNoisySurfaceColorful::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth, ofColor face_color) {

	int face_index = face_target.getNumVertices();
	int frame_index = frame_target.getNumVertices();

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 1);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

	face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
	face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 1);

	face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);
	face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 2);

	face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 4);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 3);

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
	frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
	frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
	frame_target.addIndex(frame_index + 3); frame_target.addIndex(frame_index + 0);

	frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
	frame_target.addIndex(frame_index + 5); frame_target.addIndex(frame_index + 6);
	frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);
	frame_target.addIndex(frame_index + 7); frame_target.addIndex(frame_index + 4);

	frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 4);
	frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
	frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 6);
	frame_target.addIndex(frame_index + 3); frame_target.addIndex(frame_index + 7);

	for (int i = 0; i < 8; i++) {
		face_target.addColor(face_color);
	}
}
