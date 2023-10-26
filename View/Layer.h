// Layer.h
// Interface for a render layer; manages references to objects that will be rendered i.e. render list
// Code by Derek Bliss
// Created September 20, 2007
// Last updated October 7, 2008
//
#ifndef LAYER_H
#define LAYER_H

class BaseObject;

class Layer
{
	bool active;									// Set to false to not render referenced objects
	std::vector<const BaseObject *> objects;		// Objects in this layer

	// copy constructor and assignment operator are disabled for this class
	Layer(const Layer &ref);
	const Layer &operator=(const Layer &ref);

public:

	// constructor
	Layer();

	// destructor
	~Layer();

	// mutators
	void SetActive(bool _active);

	// accessors
	bool GetActive() const;

	// interface methods
	void AddObject(const BaseObject *_object);		// Add an object to this layer
	void RemoveObject(const BaseObject *_object);	// Remove object from this layer; if exists

	// The traverse method passes each BaseObject contained within to the method given;
	// if there are 15 BaseObjects contained within this layer, it will invoke the given
	// method 15 times each time passing in a different BaseObject.  This method is used
	// to render / display all referenced objects.
	void Traverse(void (* method)(const BaseObject *_object));
};


#endif