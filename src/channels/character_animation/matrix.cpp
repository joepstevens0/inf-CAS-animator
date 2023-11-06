#include "matrix.h"
#include <stdio.h>

CMatrix::CMatrix(int rows, int columns)
{
    m_rows = rows;
    m_columns = columns;
    m_data.resize(rows * columns, 0);
}

CMatrix CMatrix::transpose()
{
    CMatrix result{m_columns, m_rows};
    for (int c = 0; c < m_columns; ++c)
        for (int r = 0; r < m_rows; ++r)
        {
            result.get(c, r) = get(r, c);
        }
    return result;
}
CMatrix CMatrix::addColumn(std::vector<float> elements)
{
    if ((int)elements.size() != m_rows){
        fprintf(stderr, "Matrix error adding column: incorrects size %d vs %d\n", (int)elements.size(), m_rows);
        throw;
    }
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        m_data.push_back(elements[i]);
    }
    m_columns += 1;
    return *this;
}

float &CMatrix::get(int row, int column)
{
    return m_data[column * m_rows + row];
}

CMatrix CMatrix::add(CMatrix other)
{
    for (int r = 0; r < m_rows; ++r)
    {
        for (int c = 0; c < m_columns; ++c)
        {
            get(r, c) += other.get(r, c);
        }
    }
    return *this;
}
CMatrix CMatrix::mul(float d)
{
    for (int r = 0; r < m_rows; ++r)
    {
        for (int c = 0; c < m_columns; ++c)
        {
            get(r, c) *= d;
        }
    }
    return *this;
}
CMatrix CMatrix::mul(CMatrix right)
{
    if (m_columns != right.m_rows){
        fprintf(stderr, "Error matrix mul sizes: [%d, %d] and [%d, %d]\n", m_rows, m_columns, right.m_rows, right.m_columns);
        throw;
    }
    CMatrix mult{m_rows, right.m_columns};

    for (int r = 0; r < mult.m_rows; ++r)
        for (int c = 0; c < mult.m_columns; ++c)
            for (int k = 0; k < m_columns; ++k)
            {
                mult.get(r, c) += get(r, k) * right.get(k, c);
            }
    return mult;
}