#include "Cell.h"
#include <SFML/Graphics.hpp>

Cell::Cell() {
	this->n = false;
	this->s = false;
	this->e = false;
	this->w = false;
	this->u = false;
	this->d = false;
	this->a = false;
	this->k = false;
	this->visited = false;
}

Cell::~Cell() {

}
