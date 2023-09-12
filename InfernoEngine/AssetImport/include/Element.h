#ifndef Element_H_
#define Element_H_

class Element
{
public:
	Element();
	Element(const Element&);
	~Element();

	void operator= (const Element&);

	int index_v1;
	int index_v2;
	int index_v3;

	float restlength1;
	float restlength2;
	float restlength3;
};

#endif