#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < 2; i++) {

		auto deg = ofGetFrameNum() * 2.88 + i * 180;
		auto location = glm::vec2();
		auto next_location = glm::vec2(cos(deg * DEG_TO_RAD), sin(deg * DEG_TO_RAD));

		for (int k = 0; k < 2; k++) {

			vector<glm::vec2> log;
			log.push_back(location);
			this->log_list.push_back(log);

			auto deg = glm::atan(next_location.y - location.y, next_location.x - location.x) * RAD_TO_DEG + 180 + ofRandom(-35, 35);
			glm::vec2 velocity = glm::vec2(cos(deg * DEG_TO_RAD), sin(deg * DEG_TO_RAD));
			this->velocity_list.push_back(velocity);

			this->noise_seed_list.push_back(ofRandom(1000));
		}
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		auto future = this->velocity_list[i] * 200;
		auto random_deg = ofMap(ofNoise(glm::vec2(this->noise_seed_list[i], ofGetFrameNum() * 0.01)), 0, 1, 0, 360);
		future += glm::vec2(50 * cos(random_deg * DEG_TO_RAD), 50 * sin(random_deg * DEG_TO_RAD));

		auto next = this->log_list[i].back() + glm::normalize(future) * 2;
		next = glm::length(next) < 1000 ? next : glm::normalize(next) * 1000;
		this->log_list[i].push_back(next);

		if (this->log_list[i].size() > 300) {

			this->log_list.erase(this->log_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->noise_seed_list.erase(this->noise_seed_list.begin() + i);
		}
	}

	this->line.clear();

	for (int i = 0; i < this->log_list.size(); i++) {

		this->line.addVertex(glm::vec3(this->log_list[i].back(), 0));
		this->line.addColor(ofColor(255, this->log_list[i].size() < 50 ? 255 : ofMap(this->log_list[i].size(), 50, 300, 255, 0)));
    }

	for (int i = 0; i < this->line.getNumVertices(); i++) {

		for (int k = i + 1; k < this->line.getNumVertices(); k++) {

			auto distance = glm::distance(this->line.getVertex(i), this->line.getVertex(k));
			if (distance < 30) {

				this->line.addIndex(i); this->line.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	for (int i = 0; i < this->line.getNumVertices(); i++) {

		ofSetColor(this->line.getColor(i));
		ofDrawCircle(this->line.getVertex(i), 3);
	}

	this->line.drawWireframe();

	/*
	int start = 300;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}