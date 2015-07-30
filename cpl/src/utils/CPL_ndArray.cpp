/*

    ________/\\\\\\\\\__/\\\\\\\\\\\\\____/\\\_____________
     _____/\\\////////__\/\\\/////////\\\_\/\\\_____________
      ___/\\\/___________\/\\\_______\/\\\_\/\\\_____________
       __/\\\_____________\/\\\\\\\\\\\\\/__\/\\\_____________ 
        _\/\\\_____________\/\\\/////////____\/\\\_____________
         _\//\\\____________\/\\\_____________\/\\\_____________
          __\///\\\__________\/\\\_____________\/\\\_____________
           ____\////\\\\\\\\\_\/\\\_____________\/\\\\\\\\\\\\\\\_
            _______\/////////__\///______________\///////////////__


                         C P L  -  L I B R A R Y

           Copyright (C) 2012-2015 Edward Smith & David Trevelyan

License

    This file is part of CPL-Library.

    CPL-Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CPL-Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CPL-Library.  If not, see <http://www.gnu.org/licenses/>.


Description

    See CPL_ndArray.h

Author(s)
    
    David Trevelyan

*/

#include "CPL_ndArray.h"
//#include<exception>
//#include<iostream>
//#include<string>
//#include<sstream>
//#include<vector>

template<class T>
CPL::ndArray<T>::ndArray()
{
    nDims = 0;
    nElements = 0;
    order = defaultOrder;
}


// Constructor taking in number of dimensions, shape and optional order
template<class T>
CPL::ndArray<T>::ndArray
(
    const int nd,
    const int shape[],
    const char ordering//=defaultOrder
)
{
    this->resize(nd, shape);
    order = ordering;
    if (order != 'F' && order != 'C')
    {
        throw orderEx;
    }
}
    
// Construct from pointer to existing data, number of dimensions and
// shape, with optional ordering
template<class T>
CPL::ndArray<T>::ndArray
(
    const T* existingData,
    const int nd,
    const int shape[],
    const char ordering//=defaultOrder
)
    : CPL::ndArray<T>(nd, shape, ordering)
{
    ndArrayData.assign(existingData, existingData + size());
}

//// Destructor automatically generated by compiler
//~ndArray()
//{
//    this->clear();
//}

// Deallocate data, resizes to 0
template<class T>
void CPL::ndArray<T>::clear()
{
    shapeVector.clear();
    ndArrayData.clear();
    nElements = 0;
}

// Number of elements of the ndArray along the dim axis
template<class T>
int CPL::ndArray<T>::shape (const int dim)
{
    return shapeVector.at(dim);
}

// Pointer to the front of the shapeVector array
template<class T>
int* CPL::ndArray<T>::shapeData()
{
    return shapeVector.data();
}

// Pointer to front of internal array
template<class T>
T* CPL::ndArray<T>::data()
{
    return ndArrayData.data();
}

// Number of elements
template<class T>
int CPL::ndArray<T>::size()
{
    return nElements;
}

// Allocation after empty construction
template<class T>
void CPL::ndArray<T>::resize (const int nd, const int shape[])
{
    nDims = nd;

    // Resize shapeVector and populate it, store nElements
    shapeVector.clear();
    nElements = 1;
    for (int i=0; i<nDims; i++)
    {
        shapeVector.push_back(shape[i]);
        nElements *= shape[i];
    }

    // Create empty vector with enough memory for nElements
    ndArrayData.resize(nElements);

}

// Element access - support up to 4 (for now) dimensions
// 1D
template<class T>
    T CPL::ndArray<T>::operator () (const int i0) const
    {
        if (checkDimsEquals(1)) return ndArrayData[flatIndex(i0)];
    }
template<class T>
    T& CPL::ndArray<T>::operator () (const int i0)
    {
        if (checkDimsEquals(1)) return ndArrayData[flatIndex(i0)];
    }

// 2D
template<class T>
    T CPL::ndArray<T>::operator () (const int i0, const int i1) const
    {
        if (checkDimsEquals(2)) return ndArrayData[flatIndex(i0, i1)];
    }
template<class T>
    T& CPL::ndArray<T>::operator () (const int i0, const int i1)
    {
        if (checkDimsEquals(2)) return ndArrayData[flatIndex(i0, i1)];
    }

// 3D
template<class T>
    T CPL::ndArray<T>::operator ()
    (
        const int i0,
        const int i1,
        const int i2
    ) const
    {
        if (checkDimsEquals(3))
        {
            return ndArrayData[flatIndex(i0, i1, i2)];
        }
    }
template<class T>
    T& CPL::ndArray<T>::operator ()
    (
        const int i0,
        const int i1,
        const int i2
    )
    {
        if (checkDimsEquals(3))
        {
            return ndArrayData[flatIndex(i0, i1, i2)];
        }
    }

// 4D
template<class T>
    T CPL::ndArray<T>::operator ()
    (
        const int i0,
        const int i1,
        const int i2,
        const int i3
    ) const
    {
        if (checkDimsEquals(4))
        {
            return ndArrayData[flatIndex(i0, i1, i2, i3)];
        }
    }
template<class T>
    T& CPL::ndArray<T>::operator ()
    (
        const int i0,
        const int i1,
        const int i2,
        const int i3
    )
    {
        if (checkDimsEquals(4))
        {
            return ndArrayData[flatIndex(i0, i1, i2, i3)];
        }
    }

// Assignment operator from single value
template<class T>
CPL::ndArray<T>& CPL::ndArray<T>::operator= (const T &rhs)
{
    for (T &e : ndArrayData)
    {
        e = rhs;
    }
    return *this;
}

// Arithmetic operators from single value
template<class T>
CPL::ndArray<T>& CPL::ndArray<T>::operator+= (const T &rhs)
{
    for (T &e : ndArrayData) e += rhs;
    return *this;
}
template<class T>
CPL::ndArray<T>& CPL::ndArray<T>::operator-= (const T &rhs)
{
    for (T &e : ndArrayData) e -= rhs;
    return *this;
}
template<class T>
CPL::ndArray<T>& CPL::ndArray<T>::operator*= (const T &rhs)
{
    for (T &e : ndArrayData) e *= rhs;
    return *this;
}
template<class T>
CPL::ndArray<T> CPL::ndArray<T>::operator+ (const T &rhs)
{
    CPL::ndArray<T> result(*this);
    result += rhs;
    return result;
}
template<class T>
CPL::ndArray<T> CPL::ndArray<T>::operator- (const T &rhs)
{
    ndArray result(*this);
    result -= rhs;
    return result;
}
template<class T>
CPL::ndArray<T> CPL::ndArray<T>::operator* (const T &rhs)
{
    CPL::ndArray<T> result(*this);
    result *= rhs;
    return result;
}

// Check dimensionality is a certain value
template<class T>
bool CPL::ndArray<T>::checkDimsEquals (const int dims) const
{
    if (nDims != dims)
    {
        throw shapeEx;
        return false;
    }
    return true;
}

// Return information about the object
template<class T>
std::string CPL::ndArray<T>::info()
{
    std::ostringstream os;
    os << "<CPL::ndArray of shape (";
    for (int i=0; i<nDims-1; ++i)
    {
        os << shapeVector[i] << ", ";
    }
    os << shapeVector[nDims-1] << "), order=" << order << ">";
    
    return os.str();
}

// Return string all the elements of the internal data
template<class T>
std::string CPL::ndArray<T>::infoData()
{
    std::ostringstream os;
    os << "Internal data elements: " << std::endl;
    for (int i=0; i<nElements; i++)
    {
        os << "ndArrayData["<< i << "]: "
           << ndArrayData[i] << std::endl;
    }
    return os.str();
}

template<class T>
std::string CPL::ndArray<T>::infoArray()
{

    if (order == 'F')
    {
        return "infoArray() for F-ordered ndArrays not implemented";
    }

    std::ostringstream os;
    int modArray[nDims];
  
    // Calculate modArray
    int prev = 1;
    for (int dim=nDims-1; dim>-1; dim--)
    {
        modArray[dim] = shapeVector[dim]*prev;
        prev = modArray[dim];
    }

    // Loop over elements
    int pos = 0;
    while (pos<nElements)
    {
        // Open all lines necessary
        for (int dim=0; dim<nDims; dim++)
        {
            if (pos%modArray[dim] == 0)
            {
                os << std::endl;
                for (int i=0; i<dim; i++)
                {
                    os << "    ";
                }
                os << "(" ;
            }
        }

        os << "    ";
        os << ndArrayData[pos];
        pos++;
     
        // Close final line if necessary
        if (pos%modArray[nDims-1] == 0)
        {
            os << ")";
        }
        // Close other lines if necessary
        for (int dim=nDims-2; dim>-1; dim--)
        {
            if (pos%modArray[dim] == 0)
            {
                os << std::endl;
                for (int i=0; i<dim; i++)
                {
                    os << "    ";
                }
                os << ")";
            }
            
        }

    }
    return os.str();

}

// Convert nd indices to 1D index position
template<class T>
int CPL::ndArray<T>::flatIndex(int i0) const
{
    return    i0;
}
template<class T>
int CPL::ndArray<T>::flatIndex(int i0, int i1) const
{
    if (order == 'F')
    {
        return    i1*shapeVector[0]
                + i0;
    }
    else
    {
        return    i0*shapeVector[1]
                + i1;
    }
}
template<class T>
int CPL::ndArray<T>::flatIndex(int i0, int i1, int i2) const
{
    if (order == 'F')
    {
        return    i2*shapeVector[0]*shapeVector[1]
                + i1*shapeVector[0]
                + i0;
    }
    else
    {
        return    i0*shapeVector[2]*shapeVector[1]
                + i1*shapeVector[2]
                + i2;
    }
}
template<class T>
int CPL::ndArray<T>::flatIndex(int i0, int i1, int i2, int i3) const
{
    if (order == 'F')
    {
        return     i3*shapeVector[0]*shapeVector[1]*shapeVector[2]
                 + i2*shapeVector[0]*shapeVector[1]
                 + i1*shapeVector[0]
                 + i0;
    }
    else
    {
        return     i0*shapeVector[3]*shapeVector[2]*shapeVector[1]
                 + i1*shapeVector[3]*shapeVector[2]
                 + i2*shapeVector[3]
                 + i3;
    }
}

// Explicit instantiations
template class CPL::ndArray<short>;
template class CPL::ndArray<int>;
template class CPL::ndArray<long>;
template class CPL::ndArray<long long>;
template class CPL::ndArray<unsigned short>;
template class CPL::ndArray<unsigned int>;
template class CPL::ndArray<unsigned long>;
template class CPL::ndArray<unsigned long long>;
template class CPL::ndArray<float>;
template class CPL::ndArray<double>;
template class CPL::ndArray<long double>;

