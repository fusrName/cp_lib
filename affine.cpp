// row vector
struct Vec : array<int, 2> {
  friend Vec operator+(Vec u, Vec v) {
    return {u[0]+v[0], u[1]+v[1]};
  }
  friend Vec operator*(int c, Vec v) {
    return {c*v[0], c*v[1]};
  }
};

struct Mat : array<Vec, 2> {
  friend Vec operator*(Vec v, Mat A) {
    return v[0]*A[0] + v[1]*A[1];
  }
  friend Mat operator*(Mat A, Mat B) {
    return {A[0][0]*B[0] + A[0][1]*B[1],
            A[1][0]*B[0] + A[1][1]*B[1]};
  }
  int det() {
    return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
  }
  Mat inv() {
    // assume orthogonality
    return det() == 1 ?
        Mat{Vec{(*this)[1][1], -(*this)[0][1]},
            Vec{-(*this)[1][0], (*this)[0][0]}} :
        Mat{Vec{-(*this)[1][1], (*this)[0][1]},
            Vec{(*this)[1][0], -(*this)[0][0]}};
  }
};

struct Affine {
  // y = xA + b
  Mat A;
  Vec b;
  friend Affine operator*(Affine f, Affine g) {
    // (xAf+bf)Ag+bg = xAfAg+bfAg+bg
    return {f.A*g.A, f.b*g.A + g.b};
  }
  friend Vec operator*(Vec v, Affine f) {
    return v[0]*f.A[0] + v[1]*f.A[1] + f.b;
  }
  Affine inv() {
    // y=xA+b <=> x=(y-b)A^-1
    Affine res{A.inv()};
    res.b = (-1) * b * res.A;
    return res;
  }
};
