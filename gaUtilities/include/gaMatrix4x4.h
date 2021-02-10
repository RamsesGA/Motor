#pragma once

template<class T, int rows, int columns>
class Matrix4x4
{
  public:
    /*************************************************************************/
    /**
    * Constructor and Destructor
    */
    /*************************************************************************/

    Matrix4x4() {}

    /**
    * @brief Construct a Matrix from another Matrix copying each element.
    * @param m Matrix that the data will be copied from.
    */
    Matrix4x4(const Matrix4x4<T, rows, columns>& m);

    /**
    * @brief Create 4x4 Matrix from 4, 4 element vectors.
    * @param column0 Vector used for the first column.
    * @param column1 Vector used for the second column.
    * @param column2 Vector used for the third column.
    * @param column3 Vector used for the fourth column.
    */
    Matrix4x4(const Vector<T, 4>& column0, const Vector<T, 4>& column1,
              const Vector<T, 4>& column2, const Vector<T, 4>& column3);

    ~Matrix4x4() = default;

    /*************************************************************************/
    /**
    * Operator overload
    */
    /*************************************************************************/
  
    /**
    * @brief Access an element of the Matrix.
    * @param row Index of the row to access.
    * @param column Index of the column to access.
    * @return Reference to the data that can be modified by the caller.
    */
    T& 
    operator()(const int row, const int column);

    /**
    * @brief Access an element of the Matrix, where
    *        the column accessed is i / Rows.
    * @param i Index of the element to access in memory.
    * @return Reference to the data that can be modified by the caller.
    */
    T&
    operator()(const int i);

    /*************************************************************************/
    /**
    * Mathematical normal methods..
    */
    /*************************************************************************/



    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/

    /**
    * @brief Access a column vector of the Matrix.
    * @param i Index of the column to access.
    * @return Reference to the data that can be modified by the caller.
    */
    Vector<T, Rows>&
    GetColumn(const int i);

  private:
  
};