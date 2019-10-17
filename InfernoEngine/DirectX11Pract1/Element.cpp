#include "Element.h"


Element::Element()
{
	index_v1 = 0;
	index_v2 = 0;
	index_v3 = 0;
	restlength1 = 0;
	restlength2 = 0;
	restlength3 = 0;

}
Element::Element(const Element& other)
{
	index_v1 = other.index_v1;
	index_v2 = other.index_v2;
	index_v3 = other.index_v3;

	restlength1 = other.restlength1;
	restlength2 = other.restlength2;
	restlength3 = other.restlength3;
}
Element::~Element()
{

}

void Element::operator= (const Element& other)
{
	index_v1 = other.index_v1;
	index_v2 = other.index_v2;
	index_v3 = other.index_v3;

	restlength1 = other.restlength1;
	restlength2 = other.restlength2;
	restlength3 = other.restlength3;
}
