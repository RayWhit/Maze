#include "Cell.h"
#include <SFML/Graphics.hpp>

Cell::Cell() {
	this->n = false;
	//this->n = true;
	this->s = false;
	this->e = false;
	//this->e = true;
	this->w = false;
	this->u = false;
	//this->u = true;
	this->d = false;
	this->a = false;
	//this->a = true;
	this->k = false;
	this->visited = false;
}

Cell::~Cell() {

}
