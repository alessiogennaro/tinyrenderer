
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "geometry.h"


template <> template <> Vec3<int>::Vec3<>(const Vec3<float> &v) : x(int(v.x + .5)), y(int(v.y + .5)), z(int(v.z + .5)) {
}

template <> template <> Vec3<float>::Vec3<>(const Vec3<int> &v) : x(v.x), y(v.y), z(v.z) {
}


/* ---- MATRICES ---- */

Matrix::Matrix(int r, int c): rows_(r), cols_(c),
    m_(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f)))
{}

inline int Matrix::nrows() const { return rows_; }
inline int Matrix::ncols() const { return cols_; }

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i = 0; i < dimensions; i++) {
        E[i][i] = 1;
    }
    return E;
}

std::vector<float>& Matrix::operator[](const int i) {
    assert (i >= 0 && i < nrows());
    return m_[i];
}

const std::vector<float>& Matrix::operator[](const int i) const {
    assert (i >= 0 && i < nrows());
    return m_[i];
}

Matrix Matrix::operator*(const Matrix& a) {
    assert (ncols() == a.nrows());
    Matrix res(nrows(), a.ncols());

    for (int i = 0; i < nrows(); i++) {
        for (int j = 0; j < a.ncols(); j++) {
            res.m_[i][j] = 0.f;

            for (int k = 0; k < ncols(); k++) {
                res.m_[i][j] = res.m_[i][j] +
                    (m_[i][k] * a.m_[k][j]);
            }
        }
    }

    return res;
}

Matrix Matrix::transpose() {
    Matrix res(ncols(), nrows());
    for (int i = 0; i < nrows(); i++) {
        for (int j = 0; j < ncols(); j++) {
            res[j][i] = m_[i][j];
        }
    }
    return res;
}

Matrix Matrix::inverse() {
    assert (nrows() == ncols());
    
    Matrix res(nrows(), ncols() * 2);

    // copy the values
    for (int i = 0; i < nrows(); i++) {
        for (int j = 0; j < ncols(); j++) {
            res[i][j] = m_[i][j];
        }   
    }

    // the right side of res holds the ident matrix
    for (int i = 0; i < nrows(); i++) {
        res[i][i + ncols()] = 1.;
    }

    // first pass
    for (int i = 0; i < nrows() - 1; i++) {
        // normalize the first row
        for (int j = res.ncols() - 1; j >= 0; j--) {
            res[i][j] = res[i][j] / res[i][i];
        }
        for (int k = i + 1; k < nrows(); k++) {
            float coeff = res[k][i];

            for (int j = 0; j < res.ncols(); j++) {
                res[k][j] = res[k][j] - (res[i][j] * coeff);
            }
        }
    }

    // normalize last row
    for (int j = res.ncols() - 1; j >= nrows() - 1; j--) {
        res[nrows() - 1][j] = res[nrows() - 1][j] / res[nrows() - 1][nrows() - 1];
    }

    // second pass
    for (int i = nrows() - 1; i > 0; i--) {
        for (int k = i - 1; k >= 0; k--) {
            float coeff = res[k][i];
            for (int j = 0; j < res.ncols(); j++) {
                res[k][j] = res[k][j] - (res[i][j] * coeff);
            }
        }
    }

    // cut the identity matrix
    Matrix trunc(nrows(), ncols());

    for (int i = 0; i < nrows(); i++) {
        for (int j = 0; j < ncols(); j++) {
            trunc[i][j] = res[i][j + ncols()];
        }
    }

    return trunc;   
}

std::ostream& operator<< (std::ostream& s, Matrix& m) {
    for (int i = 0; i < m.nrows(); i++) {
        for (int j = 0; j < m.ncols(); j++) {
            s << m[i][j];
            if (j < m.ncols() - 1) s << "\t";
        }
        s << "\n";
    }
    return s;
}
