struct MyClx {
    double real, imag;
    MyClx(double x, double y):real(x),imag(y){}
    MyClx operator+(const MyClx& z) const {return MyClx(real+z.real,imag+z.imag);}
    MyClx operator-(const MyClx& z) const {return MyClx(real-z.real,imag-z.imag);}
    MyClx operator*(const MyClx& z) const {return MyClx(real*z.real-imag*z.imag,real*z.imag+imag*z.real);}
    MyClx operator/(const MyClx& z) const {
        double t=z.real*z.real+z.imag*z.imag;
        return MyClx((real*z.real+imag*z.imag)/t,(imag*z.real-real*z.imag)/t);
    }
    MyClx& operator+=(const MyClx& z){real+=z.real;imag+=z.imag;return *this;}
    MyClx& operator-=(const MyClx& z){real-=z.real;imag-=z.imag;return *this;}
    MyClx& operator*=(const MyClx& z){
        double nr=real*z.real-imag*z.imag;
        double ni=imag=real*z.imag+imag*z.real;
        real=nr; imag=ni; return *this;
    }
    MyClx& operator/=(const MyClx& z){
        double t=z.real*z.real+z.imag*z.imag;
        double nr=(real*z.real+imag*z.imag)/t;
        double ni=(imag*z.real-real*z.imag)/t;
        real=nr; imag=ni; return *this;
    }
};
