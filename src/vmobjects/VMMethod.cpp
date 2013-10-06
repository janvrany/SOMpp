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

#include "VMMethod.h"
#include "VMFrame.h"
#include "VMClass.h"
#include "VMSymbol.h"
#include "VMArray.h"
#include "VMObject.h"
#include "VMInteger.h"
#include "Signature.h"

#include "../vm/Universe.h"

#include "../compiler/MethodGenerationContext.h"

//this method's bytecodes
#define _BC ((uint8_t*)&FIELDS[this->GetNumberOfFields() + this->GetNumberOfIndexableFields()])

//this method's literals (-> VMArray)
#define theEntries(i) FIELDS[this->GetNumberOfFields()+i]

const long VMMethod::VMMethodNumberOfFields = 5;

VMMethod::VMMethod(long bcCount, long numberOfConstants, long nof) :
        VMInvokable(nof + VMMethodNumberOfFields) {
    _HEAP->StartUninterruptableAllocation();
    bcLength = _UNIVERSE->NewInteger( bcCount );
    numberOfLocals = _UNIVERSE->NewInteger(0);
    maximumNumberOfStackElements = _UNIVERSE->NewInteger(0);
    numberOfArguments = _UNIVERSE->NewInteger(0);
    this->numberOfConstants = _UNIVERSE->NewInteger(numberOfConstants);
    for (long i = 0; i < numberOfConstants; ++i) {
        this->SetIndexableField(i, nilObject);
    }
    _HEAP->EndUninterruptableAllocation();
}

void VMMethod::SetSignature(pVMSymbol sig) {
    VMInvokable::SetSignature(sig);

    SetNumberOfArguments(Signature::GetNumberOfArguments(signature));
}

void VMMethod::MarkReferences() {
    if (gcfield)
        return;
    VMInvokable::MarkReferences();
    long numIndexableFields = GetNumberOfIndexableFields();
    for (long i = 0; i < numIndexableFields; ++i) {
        if (theEntries(i) != NULL)
            theEntries(i)->MarkReferences();
    }
}

long VMMethod::GetNumberOfLocals() const {
    return numberOfLocals->GetEmbeddedInteger();
}

void VMMethod::SetNumberOfLocals(long nol) {
    numberOfLocals->SetEmbeddedInteger(nol);
}

long VMMethod::GetMaximumNumberOfStackElements() const {
    return maximumNumberOfStackElements->GetEmbeddedInteger();
}

void VMMethod::SetMaximumNumberOfStackElements(long stel) {
    maximumNumberOfStackElements->SetEmbeddedInteger(stel);
}

long VMMethod::GetNumberOfArguments() const {
    return numberOfArguments->GetEmbeddedInteger();
}

void VMMethod::SetNumberOfArguments(long noa) {
    numberOfArguments->SetEmbeddedInteger(noa);
}

long VMMethod::GetNumberOfBytecodes() const {
    return bcLength->GetEmbeddedInteger();
}

void VMMethod::operator()(pVMFrame frame) {
    pVMFrame frm = _UNIVERSE->GetInterpreter()->PushNewFrame(this);
    frm->CopyArgumentsFrom(frame);
}

void VMMethod::SetHolderAll(pVMClass hld) {
    long numIndexableFields = GetNumberOfIndexableFields();
    for (long i = 0; i < numIndexableFields; ++i) {
        pVMObject o = GetIndexableField(i);
        pVMInvokable vmi = dynamic_cast<pVMInvokable>(o);
        if (vmi != NULL) {
            vmi->SetHolder(hld);
        }
    }
}

pVMObject VMMethod::GetConstant(long indx) const {
    uint8_t bc = _BC[indx+1];
    if (bc >= this->GetNumberOfIndexableFields()) {
        cout << "Error: Constant index out of range" << endl;
        return NULL;
    }
    return this->GetIndexableField(bc);
}

uint8_t VMMethod::GetBytecode(long indx) const {
    return _BC[indx];
}

void VMMethod::SetBytecode(long indx, uint8_t val) {
    _BC[indx] = val;
}

//VMArray Methods
pVMArray VMMethod::CopyAndExtendWith(pVMObject item) const {
    size_t fields = this->GetNumberOfIndexableFields();
    pVMArray result = _UNIVERSE->NewArray(fields+1);
    this->CopyIndexableFieldsTo(result);
    result->SetIndexableField(fields, item);
    return result;
}

pVMObject VMMethod::GetIndexableField(long idx) const {
    if (idx > this->GetNumberOfIndexableFields()-1 || idx < 0) {
        cout << "Array index out of bounds: Accessing " << idx
        << ", but only " << GetNumberOfIndexableFields()-1
        << " entries are available\n";
        _UNIVERSE->ErrorExit("Array index out of bounds exception");
    }
    return theEntries(idx);
}

void VMMethod::CopyIndexableFieldsTo(pVMArray to) const {
    long numIndexableFields = GetNumberOfIndexableFields();
    for (int i = 0; i < numIndexableFields; ++i) {
        to->SetIndexableField(i, GetIndexableField(i));
    }

}

void VMMethod::SetIndexableField(long idx, pVMObject item) {
    if (idx > this->GetNumberOfIndexableFields()-1 || idx < 0) {
        cout << "Array index out of bounds: Accessing " << idx
        << ", but there is only space for "
        << this->GetNumberOfIndexableFields()
        << " entries available\n";
        _UNIVERSE->ErrorExit("Array index out of bounds exception");
    }
    theEntries(idx) = item;
}

long VMMethod::GetNumberOfIndexableFields() const {
    //cannot be done using GetAdditionalSpaceConsumption,
    //as bytecodes need space, too, and there might be padding
    return this->numberOfConstants->GetEmbeddedInteger();
}

