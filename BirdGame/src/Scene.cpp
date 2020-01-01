#include "Scene.h"
#include <iostream>

Scene::Scene() {
	this->ID = 0;
}

Scene::Scene(int scene_id) {
	this->ID = scene_id;
}

void Scene::update() {

}

void Scene::render() {
	std::cout << "old drawing" << std::endl;
}

void Scene::handleEvent(int type, void * data) {
}

void Scene::initialize() {

}
