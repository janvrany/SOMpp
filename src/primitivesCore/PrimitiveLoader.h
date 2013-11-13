#pragma once
#ifndef PRIMITIVESLOADER_H_
#define PRIMITIVESLOADER_H_

/*
 *
 *
 Copyright (c) 2007 Michael Haupt, Tobias Pape, Arne Bergmann
 Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
 http://www.hpi.uni-potsdam.de/swa/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include <map>
#include <misc/defs.h>

class PrimitiveContainer;
class PrimitiveRoutine;

///Core class for primitive loading.
//In order to implement new primitive libraries, you can use this class
//to implement the "create" factory method (of course you can do something
//totally different, if you chose...)
//
//Functions that are expected to be exported by the library are:
//bool supportsClass(const char* name)
//void tearDown()
//PrimitiveRoutine* create(const StdString& cname, const StdString& fname)
//
//The expected file extension is ".csp".
//Libraries have to take care of initializing any needed data or data structures.
//When using the PrimitiveLoader class that is the the std::map primitiveObjects.
//Initialize it by calling the AddPrimitiveObject method, in order to map the 
//name of the smalltalk class to the corresponding PrimitiveContainer object.
class PrimitiveLoader {
public:
    PrimitiveLoader();
    virtual ~PrimitiveLoader();
    virtual PrimitiveRoutine*
    GetPrimitiveRoutine(const std::string& cname, const std::string& mname);
    virtual void AddPrimitiveObject(const char*, PrimitiveContainer*);
    virtual bool SupportsClass(const char*);
private:
    std::map<StdString, PrimitiveContainer*> primitiveObjects;
};

#endif // PRIMITIVESCORE_H_
