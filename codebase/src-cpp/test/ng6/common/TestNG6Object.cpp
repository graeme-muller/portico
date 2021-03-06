/*
 *   Copyright 2008 The Portico Project
 *
 *   This file is part of portico.
 *
 *   portico is free software; you can redistribute it and/or modify
 *   it under the terms of the Common Developer and Distribution License (CDDL)
 *   as published by Sun Microsystems. For more information see the LICENSE file.
 *
 *   Use of this software is strictly AT YOUR OWN RISK!!!
 *   If something bad happens you do not have permission to come crying to me.
 *   (that goes for your lawyer as well)
 *
 */
#include "TestNG6Object.h"

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Constructors ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
TestNG6Object::TestNG6Object( RTI::ObjectHandle objectHandle,
                              RTI::ObjectClassHandle classHandle,
                              const char *objectName )
{
	this->objectHandle = objectHandle;
	this->classHandle = classHandle;
	this->objectName = new char[strlen(objectName)+1];
	strcpy( this->objectName, objectName );
	
	// create a place to store the attribute values
	this->attributes = new map<RTI::AttributeHandle,char*>();
}

TestNG6Object::~TestNG6Object()
{
	if( this->objectName != NULL )
		delete this->objectName;
	
	MAP_CLEANUP( RTI::AttributeHandle, char*, attributes );
	delete this->attributes;
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Instance Methods //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
RTI::ObjectHandle TestNG6Object::getHandle()
{
	return this->objectHandle;
}

RTI::ObjectClassHandle TestNG6Object::getClassHandle()
{
	return this->classHandle;
}

char* TestNG6Object::getName()
{
	return this->objectName;
}

char* TestNG6Object::getAttribute( RTI::AttributeHandle attribute )
{
	return (*attributes)[attribute];
}

bool TestNG6Object::containsAttribute( RTI::AttributeHandle attribute )
{
	return attributes->find(attribute) != attributes->end();
}

/*
 * Store the value for the given attribute handle. If there is already a value for that handle,
 * delete the existing value and replace it with the new value. This method will take a COPY of
 * the provided value, so it is the callers responsibility to delete the given newValue once it
 * is finished.
 */
void TestNG6Object::setAttribute( RTI::AttributeHandle attribute, char *newValue )
{
	// attempt to erase the existing value, no effect if it doesn't exist
	attributes->erase( attribute );

	// create a copy of the string and store it
	char *copy = new char[strlen(newValue)+1];
	strcpy( copy, newValue );
	(*attributes)[attribute] = copy;
}

/*
 * This will take the given AHVPS and update the local instance with the provided data.
 */
void TestNG6Object::updateAttributes( const RTI::AttributeHandleValuePairSet& theAttributes )
{
	for( RTI::ULong i = 0; i < theAttributes.size(); i++ )
	{
		RTI::AttributeHandle attributeHandle = theAttributes.getHandle( i );
		RTI::ULong length;
		setAttribute( attributeHandle, theAttributes.getValuePointer(i,length) );
	}
}
