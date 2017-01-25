// File Sparse.cpp
// Joo Park
// Foothill CS 2C
// January 22, 2017
// Instructor: Dave Harden

// Sparce Matrix is created using SparseMat class that uses a vector
// and list to create the sparse matrix.  
// Input to the program is the value to hold for a given row and column through the 
// set method.  
// The output will display a subset of the matrix through the showSubSquare methohd.

#include <iostream>
#include <iomanip>
using namespace std;

#include "FHvector.h"
#include "FHlist.h"

const int MAT_SIZE = 100000;

// The Node class that is used to store
// the data in a matrix.

//MatNode(int cl = 0, Object dt = Object())
//Precondition:  the column # where this node should be placed along with the data object.
//Postcondition: an object of class MatNode will be created

//int getCol() const
//Precondition: N/A
//Postcondition: returns the column # for this node

//const Object & operator=(const Object &x)
//Precondition: The data to compare for equality
//Postcondition: returns true if this node has the same data as the one passed in

template <class Object>
class MatNode
{
protected:
    int col;

public:
    Object data;
    // we need a default constructor for lists
    MatNode(int cl = 0, Object dt = Object()) : col(cl), data(dt) {}
    int getCol() const { return col; }
    const Object & operator=(const Object &x) { return (data = x); }
};

// The Spare Matrix class that is used to store
// the rows and columns of a sparce matrix.

// SparseMat(int r, int c, const Object & defaultVal)
//Precondition:  r is the # of rows and c is the number of columns in the matrix.  
// defaultVal is the default instance that the matrix would deem as the zero value or null value.
//Postcondition: an object of class SparseMat will be created

//const Object  get(int r, int c) const
//Precondition: the row and column that is needed to get the data in that cell.
//Postcondition: N/A

//bool set(int r, int c, const Object &x);
//Precondition: the row and column to place the data into the matrix
//Postcondition: the data will now be in the row and column given

//void showSubSquare(int start, int size)
//Precondition: the start row and column number to start display until the size is reached
//Postcondition: it will display a subset of the matrix in row/column format for the given start and size

//void clear();
//Precondition: N/A
//Postcondition: it will clear out the matrix by setting all existing row/column to the defaultVal.

template <class Object>
class SparseMat
{
public:
    SparseMat(int r, int c, const Object & defaultVal);
    const Object  get(int r, int c) const;
    bool set(int r, int c, const Object &x);
    void showSubSquare(int start, int size);
    void clear();

protected:
    typedef FHlist< MatNode<Object> > MatRow;
    typedef FHvector<MatRow> MasterCol;

    MasterCol rows;
    Object defaultVal;
    int rowSize, colSize;
};

template <class Object>
SparseMat<Object>::SparseMat(int r, int c, const Object & defaultVal) : rowSize(r), colSize(c), defaultVal(defaultVal)
{
    rows = FHvector<MatRow>(r);
}

template <class Object>
void SparseMat<Object>::showSubSquare(int start, int size)
{
    int end = start + size - 1;
    typename MatRow::const_iterator iter;
    MatRow row;
  
    for (int i = start; i <= end; i++)
    {
        row = rows[i];
        if (row.size() == 0)
        {
            for (int i = start; i <= end; i++)
            {
                cout << setw(3) << "0";
            }
        }
        else
        {
            iter = row.begin();
            for (int i = start; i <= end || iter != row.end(); i++)
            {
                if ((*iter).getCol() == i)
                {
                    cout << setw(3) << (*iter).data;
                    ++iter;
                }
                else
                {
                    cout << setw(3) << "0";
                }
               
            }
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}

template <class Object>
void SparseMat<Object>::clear()
{
    MatRow row;
    typename MatRow::const_iterator iter;
    for (int i = 0; i < MAT_SIZE; i++)
    {
        row = rows[i];
        for (iter = row.begin(); iter != row.end(); ++iter)
        {
            set(i, (*iter).getCol(), defaultVal);
        }
    }

}

template <class Object>
bool SparseMat<Object>::set(int r, int c, const Object &x)
{
    MatNode<Object> node = MatNode<Object>(c, x);
    if (r >= MAT_SIZE || r < 0)
    {
        return false;
    }

    MatRow row = rows[r];
    row.size();
    bool isDefaultValue = x == defaultVal ? true : false;
    bool exists = false;

    typename MatRow::iterator iter;
    iter = row.begin();
    if (row.size() == 0)
    {
        row.insert(iter, node);
    }
    for (iter; iter != row.end(); ++iter)
    {
        if ((*iter).getCol() == c)
        {
            exists = true;
            break;
        }
        if ((*iter).getCol() < c)
        {
            row.insert(iter, node);
            break;
        }
    }

    if (exists)
    {
        (*iter).data = x;
    }

    if (isDefaultValue && exists)
    {
        row.erase(iter);
    }
    
    rows[r] = row;
    return true;
}


template <class Object>
const Object SparseMat<Object>::get(int r, int c) const
{
    Object returnVal = defaultVal;
    typename MatRow::const_iterator iter;

    if (r > MAT_SIZE || r < 0)
        throw FHlist<Object>::IteratorMismatchException();

    MatRow row = rows[r];
    for (iter = row.begin(); iter != row.end(); ++iter) 
    {
        if ((*iter).getCol() == c)
        {
            returnVal = (*iter).data;
        }
    }

    return returnVal;
}


// --------------- main ---------------
int main()
{
    SparseMat<float> mat(MAT_SIZE, MAT_SIZE, 0); // 100000 x 100000 filled with 0

    // test mutators
    mat.set(2, 5, 10);
    mat.set(2, 5, 35);   // should overwrite the 10
    mat.set(3, 9, 21);
    mat.set(MAT_SIZE, 1, 5);  // should fail silently
    mat.set(9, 9, mat.get(3, 9));  // should copy the 21 here
    mat.set(4, 4, -9);
    mat.set(4, 4, 0);   // should remove the -9 node entirely
    mat.set(MAT_SIZE - 1, MAT_SIZE - 1, 99);

    //// test accessors and exceptions
    try
    {
        cout << mat.get(7, 8) << endl;
        cout << mat.get(2, 5) << endl;
        cout << mat.get(4, 4) << endl;
        cout << mat.get(9, 9) << endl;
        cout << mat.get(-4, 7) << endl;  // should throw an exception
    }
    catch (...)
    {
        cout << "oops" << endl;
    }

    //// show top left 15x15
    mat.showSubSquare(0, 15);

    //// show bottom right 15x15
    mat.showSubSquare(MAT_SIZE - 15, 15);

    mat.clear();

    mat.showSubSquare(MAT_SIZE - 15, 15);
}

// Output

//0
//35
//0
//21
//oops
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0 35  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0 21  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0 21  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//
//
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0 99
//
//
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
//0  0  0  0  0  0  0  0  0  0  0  0  0  0  0