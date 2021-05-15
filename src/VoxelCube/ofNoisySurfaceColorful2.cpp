#include "ofNoisySurfaceColorful2.h"

//--------------------------------------------------------------
void ofNoisySurfaceColorful2::setup() {

	ofSetWindowTitle("Voxel Cube - Noisy Surface Colorful");
	ofSetFrameRate(60);		// change application's framerate
	ofBackground(255);		// change background color (value between 0 and 255)
	//ofSetLineWidth(2);	// change line width of small and large cubes
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	int span = 20;			// change distance between small cubes 

	for (int x = -220; x <= 220; x += span) {
		for (int y = -220; y <= 220; y += span) {
			for (int z = -220; z <= 220; z += 440) {
				this->location_list.push_back(glm::vec3(x, y, z));
				this->param_list.push_back(0);
				this->param_color_list.push_back(ofColor());
			}
		}
	}

	for (int x = -220; x <= 220; x += 440) {
		for (int y = -220; y <= 220; y += span) {
			for (int z = -220 + span; z <= 220 - span; z += span) {
				this->location_list.push_back(glm::vec3(x, y, z));
				this->param_list.push_back(0);
				this->param_color_list.push_back(ofColor());
			}
		}
	}

	for (int x = -220 + span; x <= 220 - span; x += span) {
		for (int y = -220; y <= 220; y += 440) {
			for (int z = -220 + span; z <= 220 - span; z += span) {
				this->location_list.push_back(glm::vec3(x, y, z));
				this->param_list.push_back(0);
				this->param_color_list.push_back(ofColor());
			}
		}
	}

	for (auto& location : this->location_list) {
		vector<int> route_info = vector<int>();
		int index = -1;

		for (auto& other : this->location_list) {
			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {
				route_info.push_back(index);
			}
		}

		this->route_info_list.push_back(route_info);
	}

	ofColor color;
	vector<int> hex_list = { 0x402220, 0x08122B, 0xE4D4C8, 0x53001D };	// change hex values and add or remove elements at your will
	vector<ofColor> base_color_list;

	for (auto hex : hex_list) {
		color.setHex(hex);
		base_color_list.push_back(color);
	}

	for (int i = 0; i < 230; i++) {
		this->index_list.push_back((int)ofRandom(this->location_list.size()));
		this->color_list.push_back(base_color_list[(int)ofRandom(base_color_list.size())]);
	}
}

//--------------------------------------------------------------
void ofNoisySurfaceColorful2::update() {

	this->face.clear();
	this->frame.clear();

	for (int i = 0; i < this->index_list.size(); i++) {
		int next_index = this->route_info_list[this->index_list[i]][(int)ofRandom(this->route_info_list[this->index_list[i]].size())];
		
		for (int k = 0; k < this->route_info_list[this->index_list[i]].size(); k++) {
			if (this->param_list[next_index] <= 0) {
				this->param_list[next_index] = 30;
				this->param_color_list[next_index] = this->color_list[i];
				this->index_list[i] = next_index;
				break;
			}
		}
	}

	for (int i = 0; i < this->location_list.size(); i++) {
		if (this->param_list[i] > 0) { this->setBoxToMesh(this->face, this->frame, this->location_list[i], 20, this->param_color_list[i]); }
		if (this->param_list[i] > 0) { this->param_list[i] -= 1; }
	}
}

//--------------------------------------------------------------
void ofNoisySurfaceColorful2::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.21);
	ofRotateY(ofGetFrameNum() * 0.37);

	this->face.draw();

	ofSetColor(239);
	this->frame.drawWireframe();

	ofNoFill();
	ofSetColor(239);
	ofDrawBox(440 + 15);

	this->cam.end();
}

//--------------------------------------------------------------
void ofNoisySurfaceColorful2::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size, ofColor face_color) {
	this->setBoxToMesh(face_target, frame_target, location, size, size, size, face_color);
}

//--------------------------------------------------------------
void ofNoisySurfaceColorful2::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth, ofColor face_color) {

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
