#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class CMatrix
{
private:
    std::vector<float> m_data;
    int m_columns;
    int m_rows;
public:
    int total_columns() const { return m_columns;}
    int total_rows() const {return m_rows;}

    CMatrix(int rows, int columns);

    CMatrix transpose();
    CMatrix addColumn(std::vector<float> elements);

    float &get(int row, int column);
    CMatrix add(CMatrix other);
    CMatrix mul(float d);
    CMatrix mul(CMatrix right);
};


#endif