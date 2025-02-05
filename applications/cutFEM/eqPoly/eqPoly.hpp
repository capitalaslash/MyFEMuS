
#ifndef __femus_eqPoly_hpp__
#define __femus_eqPoly_hpp__

#include <iostream>
#include <iomanip>

#include "./LiSK/lisk.hpp"
#include <cmath>       /* exp */

using namespace std;


class EquivalentPolynomial {
  public:
    EquivalentPolynomial() {
      //_lisk = new LiSK::LiSK< complex<double> > (5);
    }
//     ~EquivalentPolynomial() {
//       delete _lisk;
//     }

    void SetCoefficients(const unsigned &dim, const unsigned &degree, const double &p, const std::vector < double > &c, const unsigned &element);
    void MatrixVectorMultiply(const std::vector<std::vector <double>> &A, const std::vector < complex < double > > &bv, std::vector < complex < double > > &xv);
    //void FindBestFit(const std::vector < double > &pts, const std::vector < double > &Npts, const unsigned &dim);
    double GetValue(std::vector <double> &x, unsigned &element);
    double IntegrationValue(const int &s, const unsigned &dim, const unsigned &degree, std::vector < double > &normal, const double &d, const unsigned &element);
    std::vector < double > BaseCases(const unsigned &dim, const unsigned &degree, const std::vector < double > &c,const double &p);
    double HyperCubeA(const unsigned & n, const int &s, std::vector<unsigned> &m,
           const std::vector <double> &a, const std::vector <double> &ma,
           const double & d, const double & md);
    double LimLi(const int &n, const double & x);
    double HyperCubeB(const unsigned & n, const int &s, std::vector<unsigned> &m,
           const std::vector <double> &a, const std::vector <double> &ma,
           const double & d, const double & md);

//     const std::vector < complex < double > > &GetCoefficients() {
//       return _coefficients;
    

//     void PrintCoefficients() {
//       for(unsigned i = 0; i < _coefficients.size(); i++) {
//         //std::cout << std::setprecision(10) << _coefficients[i].real() << " printing coeffiecints++++++++++++++++++++++++++" << std::endl;
//       }
//       std::cout << std::endl;
//     }




  private:
    std::vector <  double >  _coefficients;
    std::vector < double > _polycoeffs;
    std::vector <std::vector < complex < double > >> _basecases;
    //std::vector < double >  _bestfit;
     std::vector < complex < double > >_b_vector[3];
    LiSK::LiSK< complex<double> > *_lisk;
    unsigned _dim, _degree;
    bool _flag = true;

    static std::vector<std::vector<double>> _A13_inverse;
    static std::vector<std::vector<double>> _A22_inverse;
    static std::vector<std::vector<double>> _A23_inverse;
    static std::vector<std::vector<double>> _A24_inverse;
    static std::vector<std::vector<double>> _A22T_inverse;
    static std::vector<std::vector<double>> _A32_inverse;
    
    static std::vector<std::vector<double>> _ATA_deg6_hex;
    static std::vector<std::vector<double>> _ATA_deg8_hex;
    static std::vector<std::vector<double>> _ATA_deg3_prism;

};

#endif


/*
//TODO hexahedron = 0, tet = 1, wedge =2, quad = 3, tri = 4, line = 5, point = 6
void EquivalentPolynomial::SetCoefficients(const unsigned &dim, const unsigned &degree, const double &p, const std::vector < double > &c, const double & d, const unsigned &element) {

  _dim = dim;
  _degree = degree;


  if(dim == 1) {

    unsigned n = degree + 1;
    _b_vector.resize(n);
    _lisk = new LiSK::LiSK< complex<double> > (4);

    double a = c[0];
    double x1 = exp(d * p);
    double x2 = exp((a + d) * p);
    double x3 = exp((-a + d) * p);
    double ap = a * p;
    double a2p2 = ap * ap;
    double a3p3 = a2p2 * ap;
    double a4p4 = a3p3 * ap;

    double ln1px3 = log(1 + x3);
    double ln1px2 = log(1 + x2);

    complex <double> l2mx3 = _lisk->Li(2, -x3);
    complex <double> l2mx2 = _lisk->Li(2, -x2);
    complex <double> l3mx3 = _lisk->Li(3, -x3);
    complex <double> l3mx2 = _lisk->Li(3, -x2);



    if(degree <= 3) {

      _b_vector[0] = (-2.0 * (log(exp(ap) + x1) - ln1px2)) / (ap);

      _b_vector[1] = (2. * (ap * (ln1px3 + ln1px2) - l2mx3 + l2mx2)) / a2p2;

      _b_vector[2] = -0.6666666666666666 +
                     (2. * (a2p2 * (-ln1px3 + ln1px2) +
                            2. * ap * (l2mx3 + l2mx2) + 2. * l3mx3 - 2. * l3mx2)
                     ) / a3p3;

      _b_vector[3] = (2. * (a3p3 * (ln1px3 + ln1px2) -
                            3 * ap * (ap * (l2mx3 - l2mx2) + 2. * (l3mx3 + l3mx2)) -
                            6. * _lisk->Li(4, -x3) + 6. * _lisk->Li(4, -x2))
                     ) / a4p4;

      // TODO more cleaning
      EquivalentPolynomial::MatrixVectorMultiply(_A13_inverse, _b_vector, _coefficients);
    }
    else {
      std::cout << "Degree " << degree << " has not been implemented for dimension " << dim << std::endl;
      abort();
    }
  }
  else if(dim == 2) {


    double a = c[0];
    double b = c[1];
    unsigned n = ((degree + 1u) * (degree + 2u)) / 2u;
    _lisk = new LiSK::LiSK< complex<double> > (7);
    _b_vector.resize(n);
    std::vector <double> bvector(n);

    if(element == 3) {




      double a_squared = a * a;
      double p_squared = p * p;
      double x2 = exp((-a - b + d) * p);
      double x3 = exp((-a + b + d) * p);
      double x4 = exp((a - b + d) * p);
      double x5 = exp((a + b + d) * p);
      double b2 = b * b;
      double b3 = b * b2;
      double p4 = p * p * p * p;
      double ap = a * p;
      double ab = a * b;
      double bp = b * p;

      complex <double> l2mx2 = _lisk->Li(2, -x2);
      complex <double> l2mx3 = _lisk->Li(2, -x3);
      complex <double> l2mx4 = _lisk->Li(2, -x4);
      complex <double> l2mx5 = _lisk->Li(2, -x5);
      complex <double> l3mx2 = _lisk->Li(3, -x2);
      complex <double> l3mx4 = _lisk->Li(3, -x4);
      complex <double> l3mx3 = _lisk->Li(3, -x3);
      complex <double> l3mx5 = _lisk->Li(3, -x5);
      complex <double> l4mx2 = _lisk->Li(4, -x2);
      complex <double> l4mx4 = _lisk->Li(4, -x4);
      complex <double> l4mx3 = _lisk->Li(4, -x3);
      complex <double> l4mx5 = _lisk->Li(4, -x5);


      if(degree >= 0) {


        // 1
        _b_vector[0] = (-2. * (2 * ab * p_squared + l2mx2 -
                               l2mx4 - l2mx3 +
                               l2mx5)) / (a * b * p_squared);
        // x
        _b_vector[1] = (2. * (ap * l2mx2 + ap * l2mx4 - ap * l2mx3 -
                              ap * l2mx5 + l3mx2 - l3mx4 -
                              l3mx3 + l3mx5)) / (a_squared * b * p_squared * p);
        // y
        _b_vector[2] = (2. * (b * p * l2mx2 - bp * l2mx4 + bp * l2mx3 -
                              bp * l2mx5 + l3mx2 - l3mx4 -
                              l3mx3 + l3mx5)) / (a * b2 * p_squared * p);
        // xx
        _b_vector[3] = (-2. * (2 * a_squared * ab * p4 + 3 * a_squared * p_squared * l2mx2 -
                               3 * a_squared * p_squared * l2mx4 - 3 * a_squared * p_squared * l2mx3 +
                               3 * a_squared * p_squared * l2mx5 + 6 * ap * l3mx2 +
                               6 * ap * l3mx4 - 6 * ap * l3mx3 - 6 * ap * l3mx5 +
                               6. * l4mx2 - 6. * l4mx4 - 6. * l4mx3 +
                               6. * l4mx5)) / (3. * a_squared * ab * p4);
        // yy
        _b_vector[4] = (-2. * (2 * a * b3 * p4 + 3 * b2 * p_squared * l2mx2 -
                               3 * b2 * p_squared * l2mx4 - 3 * b2 * p_squared * l2mx3 +
                               3 * b2 * p_squared * l2mx5 + 6 * bp * l3mx2 -
                               6 * bp * l3mx4 + 6 * bp * l3mx3 - 6 * bp * l3mx5 +
                               6. * l4mx2 - 6. * l4mx4 - 6. * l4mx3 +
                               6. * l4mx5)) / (3. * a * b3 * p4);
        // xy
        _b_vector[5] = (-2. * (a * b * p_squared * l2mx2 + ab * p_squared * l2mx4 +
                               ab * p_squared * l2mx3 + ab * p_squared * l2mx5 +
                               ap * l3mx2 + bp * l3mx2 + ap * l3mx4 -
                               bp * l3mx4 - ap * l3mx3 + bp * l3mx3 -
                               ap * l3mx5 - bp * l3mx5 + l4mx2 -
                               l4mx4 - l4mx3 + l4mx5)) /
                       (a_squared * b2 * p4);


        if(degree > 2) {

          double x6 = exp(-((a + b - d) * p));
          double x7 = exp(-((-a + b + d) * p));
          double x8 = exp((b - d) * p);
          double x9 = exp((a + b - d) * p);
          double x10 = exp((-b + d) * p);
          double x11 = exp((b + d) * p);
          double x13 = exp((-a - b - d) * p);


          double bp = b * p;
          double a2p2 = ap * ap;
          double a3p3 = a2p2 * ap;
          double a4p4 = a3p3 * ap;
          double b2p2 = bp * bp;
          double b3p3 = b2p2 * bp;
          double b4p4 = b3p3 * bp;
          double a2 = a * a;
          double a4 = a2 * a2;
          double a3 = a * a2;
          double b4 = b2 * b2;
          double p5 = p4 * p;



          complex <double> l2mx6 = _lisk->Li(2, -x6);
          complex <double> l5mx4 = _lisk->Li(5, -x4);
          complex <double> l3mx6 = _lisk->Li(3, -x6);
          complex <double> l4mx6 = _lisk->Li(4, -x6);
          complex <double> l5mx6 = _lisk->Li(5, -x6);
          complex <double> l5mx3 = _lisk->Li(5, -x3);
          complex <double> l5mx5 = _lisk->Li(5, -x5);
          complex <double> l2mx9 = _lisk->Li(2, -x9);
          complex <double> l2mx7 = _lisk->Li(2, -x7);
          complex <double> l3mx9 = _lisk->Li(3, -x9);
          complex <double> l3mx7 = _lisk->Li(3, -x7);
          complex <double> l4mx9 = _lisk->Li(4, -x9);
          complex <double> l4mx7 = _lisk->Li(4, -x7);
          complex <double> l6mx6 = _lisk->Li(6, -x6);
          complex <double> l6mx4 = _lisk->Li(6, -x4);
          complex <double> l6mx3 = _lisk->Li(6, -x3);
          complex <double> l6mx5 = _lisk->Li(6, -x5);

          double ln1px9 = log(1 + x9);
          double ln1px4 = log(1 + x4);
          double ln1px7 = log(1 + x7);
          double ln1px5 = log(1 + x5);


          // xxx
          _b_vector[6] = (2. * (a3p3 * l2mx6 + a3p3 * l2mx4 -
                                a3p3 * l2mx3 - a3p3 * l2mx5 +
                                3 * a2p2 * l3mx6 - 3 * a2p2 * l3mx4 -
                                3 * a2p2 * l3mx3 + 3 * a2p2 * l3mx5 +
                                6. * ap * l4mx6 + 6. * ap * l4mx4 -
                                6. * ap * l4mx3 - 6. * ap * l4mx5 +
                                6. * l5mx6 - 6. * l5mx4 -
                                6. * l5mx3 + 6. * l5mx5)) / (a4 * b * p5);
          // yyy
          _b_vector[7] = (2. * (b3p3 * l2mx6 -
                                b3p3 * l2mx4 +
                                b3p3 * l2mx3 -
                                b3p3 * l2mx5 +
                                3 * b2p2 * l3mx6 -
                                3 * b2p2 * l3mx4 -
                                3 * b2p2 * l3mx3 +
                                3 * b2p2 * l3mx5 +
                                6. * bp * l4mx6 -
                                6. * bp * l4mx4 + 6. * bp * l4mx3 -
                                6. * bp * l4mx5 + 6. * l5mx6 -
                                6. * l5mx4 - 6. * l5mx3 +
                                6. * l5mx5)) / (a * b4 * p5);

          // xxy
          _b_vector[8] = (6. * ap * (ap * (l3mx6 -
                                           l3mx4) + 2. * (l4mx6 +
                                                          l4mx4)) +
                          bp * (a4p4 - 2 * a3p3 * ln1px9 -
                                2 * a3p3 * ln1px4 -
                                2 * a3p3 * ln1px7 +
                                2 * a3p3 * log(((1 + x9) * (1 + x3)) / x9) -
                                2 * a3p3 * ln1px5 +
                                2 * a3p3 * log((1 + x4) * (1 + x5)) -
                                6 * a2p2 * l2mx9 -
                                6 * a2p2 * l2mx4 -
                                6 * a2p2 * l2mx7 -
                                6 * a2p2 * l2mx5 +
                                12. * ap * l3mx9 +
                                12. * ap * l3mx4 +
                                12. * ap * l3mx7 +
                                12. * ap * l3mx5 +
                                12. * _lisk->Li(4, -x8) -
                                12. * l4mx9 +
                                12. * _lisk->Li(4, -x10) -
                                12. * l4mx4 +
                                12. * _lisk->Li(4, -x13) +
                                12. * _lisk->Li(4, -x11) -
                                12. * l4mx7 -
                                12. * l4mx5) -
                          6 * ap * (ap * (l3mx3 -
                                          l3mx5) + 2. * (l4mx3 +
                                                         l4mx5)) +
                          12. * (l5mx6 - l5mx4) -
                          12. * (l5mx3 - l5mx5)) /
                         (3. * a3 * b2 * p5);

          // yyx
          _b_vector[9] = (2. * (b2p2 * (ap * l2mx6 +
                                        ap * l2mx4 + l3mx6 -
                                        l3mx4) + b2p2 * (-(ap * (l2mx3 +
                                                                 l2mx5)) - l3mx3 +
                                                         l3mx5) + 2 * bp * (ap * (l3mx6 +
                                                                                  l3mx4) + l4mx6 -
                                                                            l4mx4) + 2 * bp * (ap * (l3mx3 +
                                                                                                     l3mx5) + l4mx3 -
                                                                                               l4mx5) + 2. * (ap * (l4mx6 +
                                                                                                   l4mx4) + l5mx6 -
                                                                                                   l5mx4) - 2. * (ap * (l4mx3 +
                                                                                                       l4mx5) + l5mx3 -
                                                                                                       l5mx5))) / (a2 * b3 * p5);

          if(degree > 3) {

            double a5 = a * a4;
            double p6 = p * p5;
            double b5 = b * b4;


            // xxxx
            _b_vector[10] = (-2. * (2 * a5 * b * p6 +
                                    5 * a4p4 * l2mx6 -
                                    5 * a4p4 * l2mx4 -
                                    5 * a4p4 * l2mx3 +
                                    5 * a4p4 * l2mx5 +
                                    20. * (a3p3 * l3mx6 +
                                           a3p3 * l3mx4 +
                                           3 * a2p2 * l4mx6 -
                                           3 * a2p2 * l4mx4 +
                                           6. * ap * l5mx6 +
                                           6. * ap * l5mx4 +
                                           6. * l6mx6 -
                                           6. * l6mx4) -
                                    20. * (a3p3 * l3mx3 +
                                           a3p3 * l3mx5 +
                                           3 * a2p2 * l4mx3 -
                                           3 * a2p2 * l4mx5 +
                                           6. * ap * l5mx3 +
                                           6. * ap * l5mx5 +
                                           6. * l6mx3 -
                                           6. * l6mx5))) / (5. * a5 * b * p6);

            // yyyy
            _b_vector[11] = (-2. * (2 * a * b5 * p6 +
                                    5 * b4p4 * l2mx6 -
                                    5 * b4p4 * l2mx4 -
                                    5 * b4p4 * l2mx3 +
                                    5 * b4p4 * l2mx5 +
                                    20 * b3p3 * l3mx6 -
                                    20 * b3p3 * l3mx4 +
                                    20 * b3p3 * l3mx3 -
                                    20 * b3p3 * l3mx5 +
                                    60 * b2p2 * l4mx6 -
                                    60 * b2p2 * l4mx4 -
                                    60 * b2p2 * l4mx3 +
                                    60 * b2p2 * l4mx5 +
                                    120 * bp * l5mx6 -
                                    120 * bp * l5mx4 +
                                    120 * bp * l5mx3 -
                                    120 * bp * l5mx5 +
                                    120. * l6mx6 -
                                    120. * l6mx4 -
                                    120. * l6mx3 +
                                    120. * l6mx5)) / (5. * a * b5 * p6);

            // xxyy
            _b_vector[12] = (-2. * (2. * a3 * b3 * p6 +
                                    9 * b2p2 * (a2p2 * l2mx6 -
                                                a2p2 * l2mx4 + 2. * (ap * l3mx6 +
                                                                     ap * l3mx4 + l4mx6 - l4mx4)) - 9 * b2p2 * (a2p2 * l2mx3 -
                                                                         a2p2 * l2mx5 + 2. * (ap * l3mx3 + ap * l3mx5 +
                                                                                              l4mx3 - l4mx5)) + 18. * bp * (ap * (ap * (l3mx6 -
                                                                                                  l3mx4) + 2. * (l4mx6 + l4mx4)) + 2. * (l5mx6 - l5mx4)) +
                                    18. * bp * (ap * (ap * (l3mx3 - l3mx5) + 2. * (l4mx3 +
                                                                                   l4mx5)) + 2. * (l5mx3 - l5mx5)) + 18. * (ap * (ap * (l4mx6 -
                                                                                       l4mx4) + 2. * (l5mx6 + l5mx4)) +
                                                                                       2. * (l6mx6 - l6mx4)) - 18. * (ap * (ap * (l4mx3 -
                                                                                           l4mx5) + 2. * (l5mx3 + l5mx5)) +
                                                                                           2. * l6mx3 - 2. * l6mx5))) / (9. * a3 * b3 * p6);

            // xxxy
            _b_vector[13] = (b * p * (-2 * a5 * p5 +
                                      5 * a4p4 * ln1px9 -
                                      5 * a4p4 * ln1px4 +
                                      5 * a4p4 * ln1px7 -
                                      5 * a4p4 * log(((1 + x9) * (1 + x3)) / x9) -
                                      5 * a4p4 * ln1px5 +
                                      5 * a4p4 * log((1 + x4) * (1 + x5)) +
                                      20 * a3p3 * l2mx9 -
                                      20 * a3p3 * l2mx4 +
                                      20 * a3p3 * l2mx7 -
                                      20 * a3p3 * l2mx5 -
                                      60 * a2p2 * l3mx9 +
                                      60 * a2p2 * l3mx4 -
                                      60 * a2p2 * l3mx7 +
                                      60 * a2p2 * l3mx5 +
                                      120. * ap * l4mx9 -
                                      120. * ap * l4mx4 +
                                      120. * ap * l4mx7 -
                                      120. * ap * l4mx5 +
                                      120. * _lisk->Li(5, -x8) -
                                      120. * _lisk->Li(5, -x9) - 120. * _lisk->Li(5, -x10) +
                                      120. * l5mx4 + 120. * _lisk->Li(5, -x13) -
                                      120. * _lisk->Li(5, -x11) - 120. * _lisk->Li(5, -x7) +
                                      120. * l5mx5) -
                             20. * (a3p3 * l3mx6 +
                                    a3p3 * l3mx4 +
                                    3 * a2p2 * l4mx6 -
                                    3 * a2p2 * l4mx4 +
                                    6. * ap * l5mx6 +
                                    6. * ap * l5mx4 +
                                    6. * l6mx6 -
                                    6. * l6mx4) +
                             20. * (a3p3 * l3mx3 +
                                    a3p3 * l3mx5 +
                                    3 * a2p2 * l4mx3 -
                                    3 * a2p2 * l4mx5 +
                                    6. * ap * l5mx3 +
                                    6. * ap * l5mx5 + 6. * l6mx3 -
                                    6. * l6mx5)) / (10. * a4 * b2 * p6);

            // yyyx
            _b_vector[14] = (2. * (b3p3 * (-(ap * (l2mx6 + l2mx4)) - l3mx6 + l3mx4) +
                                   b3p3 * (-(ap * (l2mx3 + l2mx5)) - l3mx3 + l3mx5) -
                                   3 * b2p2 * (ap * (l3mx6 + l3mx4) + l4mx6 - l4mx4) +
                                   3 * b2p2 * (ap * (l3mx3 + l3mx5) + l4mx3 - l4mx5) -
                                   6 * bp * (ap * (l4mx6 + l4mx4) + l5mx6 - l5mx4) -
                                   6 * bp * (ap * (l4mx3 + l4mx5) + l5mx3 - l5mx5) -
                                   6. * (ap * (l5mx6 + l5mx4) + l6mx6 - l6mx4) +
                                   6. * (ap * (l5mx3 + l5mx5) + l6mx3 - l6mx5))) /
                            (a2 * b4 * p6);

            if(degree > 4) {
              std::cout << "Degree " << degree << " has not been implemented for dimension " << dim << std::endl;
              abort();
            }
          }
        }
      }

      if(degree <= 2) EquivalentPolynomial::MatrixVectorMultiply(_A22_inverse, _b_vector, _coefficients);
      else if(degree == 3) EquivalentPolynomial::MatrixVectorMultiply(_A23_inverse, _b_vector, _coefficients);
      else if(degree == 4) EquivalentPolynomial::MatrixVectorMultiply(_A24_inverse, _b_vector, _coefficients);

    }


    else if(element == 4) {

      std::cout << " Triangle "   << endl;

      _b_vector[0] = -0.5 + (2. * ((-a + b) * _lisk->Li(2, -exp(d * p)) - b * _lisk->Li(2, -exp((a + d) * p)) + a * _lisk->Li(2, -exp((b + d) * p)))) / (a * (a - b) * b * pow(p, 2));

      _b_vector[1] = -0.16666666666666666 + (2. * (a * b * (-a + b) * p * _lisk->Li(2, -exp((a + d) * p)) +
                                                   pow(a - b, 2) * _lisk->Li(3, -exp(d * p)) + (2 * a - b) * b * _lisk->Li(3, -exp((a + d) * p))
                                                   - pow(a, 2) * _lisk->Li(3, -exp((b + d) * p)))) / (pow(a, 2) * pow(a - b, 2) * b * pow(p, 3));

      _b_vector[2] = -(pow(a, 3) * pow(b, 2) * pow(p, 3) - 2 * pow(a, 2) * pow(b, 3) * pow(p, 3) +
                       a * pow(b, 4) * pow(p, 3) - 12 * a * (a - b) * b * p * _lisk->Li(2, -exp((b + d) * p)) -
                       12 * pow(a - b, 2) * _lisk->Li(3, -exp(d * p)) + 12 * pow(b, 2) * _lisk->Li(3, -exp((a + d) * p)) +
                       12 * pow(a, 2) * _lisk->Li(3, -exp((b + d) * p)) - 24 * a * b * _lisk->Li(3, -exp((b + d) * p))) / (6. * a * pow(a - b, 2) * pow(b, 2) * pow(p, 3));

      _b_vector[3] = -0.08333333333333333 + (2. * (pow(a, 2) * pow(p, 2) * _lisk->Li(2, -exp((a + d) * p)) -
                                                   2. * (a * p * _lisk->Li(3, -exp((a + d) * p)) + _lisk->Li(4, -exp(d * p)) - _lisk->Li(4, -exp((a + d) * p))))) / (pow(a, 3) * b * pow(p, 4)) +
                     (2. * ((a - b) * p * ((-a + b) * p * _lisk->Li(2, -exp((a + d) * p)) + 2. * _lisk->Li(3, -exp((a + d) * p))) - 2. * _lisk->Li(4, -exp((a + d) * p)) + 2. * _lisk->Li(4, -exp((b + d) * p)))) /
                     (pow(a - b, 3) * b * pow(p, 4));

      _b_vector[4] = -0.08333333333333333 + (2 * a * pow(a - b, 2) * pow(b, 2) * pow(p, 2) * _lisk->Li(2, -exp((b + d) * p)) - 4 * a * (a - 2 * b) * (a - b) * b * p * _lisk->Li(3,                                 -exp((b + d) * p)) - 4 * pow(a - b, 3) * _lisk->Li(4, -exp(d * p)) - 4 * pow(b, 3) * _lisk->Li(4, -exp((a + d) * p)) +
                                             4 * a * (pow(a, 2) - 3 * a * b + 3 * pow(b, 2)) * _lisk->Li(4, -exp((b + d) * p))) / (a * pow(a - b, 3) * pow(b, 3) * pow(p, 4));

      _b_vector[5] = -(pow(a, 5) * pow(b, 2) * pow(p, 4) - 3 * pow(a, 4) * pow(b, 3) * pow(p, 4) + 3 * pow(a, 3) * pow(b, 4) * pow(p, 4) - pow(a, 2) * pow(b, 5) * pow(p, 4) +
                       48 * a * (a - b) * pow(b, 2) * p * _lisk->Li(3, -exp((a + d) * p)) + 48 * pow(a, 2) * (a - b) * b * p * _lisk->Li(3, -exp((b + d) * p)) + 48 * pow(a, 3) * _lisk->Li(4, -exp(d * p)) - 144 * pow(a, 2) * b * _lisk->Li(4, -exp(d * p)) + 144 * a * pow(b, 2) * _lisk->Li(4, -exp(d * p)) - 48 * pow(b, 3) * _lisk->Li(4, -exp(d * p)) - 144 * a * pow(b, 2) * _lisk->Li(4, -exp((a + d) * p)) + 48 * pow(b, 3) * _lisk->Li(4, -exp((a + d) * p)) -
                       48 * pow(a, 3) * _lisk->Li(4, -exp((b + d) * p)) +
                       144 * pow(a, 2) * b * _lisk->Li(4, -exp((b + d) * p))) / (24. * pow(a, 2) * pow(a - b, 3) * pow(b, 2) * pow(p, 4));



      if(degree <= 2) EquivalentPolynomial::MatrixVectorMultiply(_A22T_inverse, _b_vector, _coefficients);
      //else if(degree == 3) EquivalentPolynomial::MatrixVectorMultiply(_A23_inverse, _b_vector, _coefficients);
      //else if(degree == 4) EquivalentPolynomial::MatrixVectorMultiply(_A24_inverse, _b_vector, _coefficients);

    }

  }



  else if(dim == 3) {
    {

      double a = c[0];
      double b = c[1];
      double cz = c[2];
      unsigned n = ((degree + 1u) * (degree + 2u) * (degree + 3u)) / 6u;
      _lisk = new LiSK::LiSK< complex<double> > (7);
      _b_vector.resize(n);
      std::vector <double> bvector(n);


      if(element == 3) {

        _b_vector[0] = (-2. * (4 * a * b * cz * pow(p, 3) - _lisk->Li(3, -exp((-a - b - cz + d) * p)) + _lisk->Li(3, -exp((a - b - cz + d) * p)) + _lisk->Li(3, -exp((-a + b - cz + d) * p)) - _lisk->Li(3, -exp((a + b - cz + d) * p)) +
                               _lisk->Li(3, -exp((-a - b + cz + d) * p)) - _lisk->Li(3, -exp((a - b + cz + d) * p)) - _lisk->Li(3, -exp((-a + b + cz + d) * p)) + _lisk->Li(3, -exp((a + b + cz + d) * p)))) / (a * b * cz * pow(p, 3));

        _b_vector[1] = (-2. * (a * p * _lisk->Li(3, -exp((-a - b - cz + d) * p)) + a * p * _lisk->Li(3, -exp((a - b - cz + d) * p)) - a * p * _lisk->Li(3, -exp((-a + b - cz + d) * p)) - a * p * _lisk->Li(3, -exp((a + b - cz + d) * p)) -
                               a * p * _lisk->Li(3, -exp((-a - b + cz + d) * p)) - a * p * _lisk->Li(3, -exp((a - b + cz + d) * p)) + a * p * _lisk->Li(3, -exp((-a + b + cz + d) * p)) + a * p * _lisk->Li(3, -exp((a + b + cz + d) * p)) +
                               _lisk->Li(4, -exp((-a - b - cz + d) * p)) - _lisk->Li(4, -exp((a - b - cz + d) * p)) - _lisk->Li(4, -exp((-a + b - cz + d) * p)) + _lisk->Li(4, -exp((a + b - cz + d) * p)) -
                               _lisk->Li(4, -exp((-a - b + cz + d) * p)) + _lisk->Li(4, -exp((a - b + cz + d) * p)) + _lisk->Li(4, -exp((-a + b + cz + d) * p)) - _lisk->Li(4, -exp((a + b + cz + d) * p)))) /
                       (pow(a, 2) * b * cz * pow(p, 4));

        _b_vector[2] = (-2. * (b * p * _lisk->Li(3, -exp((-a - b - cz + d) * p)) - b * p * _lisk->Li(3, -exp((a - b - cz + d) * p)) + b * p * _lisk->Li(3, -exp((-a + b - cz + d) * p)) -
                               b * p * _lisk->Li(3, -exp((a + b - cz + d) * p)) - b * p * _lisk->Li(3, -exp((-a - b + cz + d) * p)) + b * p * _lisk->Li(3, -exp((a - b + cz + d) * p)) - b * p * _lisk->Li(3, -exp((-a + b + cz + d) * p)) +
                               b * p * _lisk->Li(3, -exp((a + b + cz + d) * p)) + _lisk->Li(4, -exp((-a - b - cz + d) * p)) - _lisk->Li(4, -exp((a - b - cz + d) * p)) - _lisk->Li(4, -exp((-a + b - cz + d) * p)) +
                               _lisk->Li(4, -exp((a + b - cz + d) * p)) - _lisk->Li(4, -exp((-a - b + cz + d) * p)) + _lisk->Li(4, -exp((a - b + cz + d) * p)) + _lisk->Li(4, -exp((-a + b + cz + d) * p)) -
                               _lisk->Li(4, -exp((a + b + cz + d) * p)))) / (a * pow(b, 2) * cz * pow(p, 4));

        _b_vector[3] = (-2. * (cz * p * _lisk->Li(3, -exp((-a - b - cz + d) * p)) - cz * p * _lisk->Li(3, -exp((a - b - cz + d) * p)) - cz * p * _lisk->Li(3, -exp((-a + b - cz + d) * p)) + cz * p * _lisk->Li(3, -exp((a + b - cz + d) * p)) +
                               cz * p * _lisk->Li(3, -exp((-a - b + cz + d) * p)) - cz * p * _lisk->Li(3, -exp((a - b + cz + d) * p)) - cz * p * _lisk->Li(3, -exp((-a + b + cz + d) * p)) + cz * p * _lisk->Li(3, -exp((a + b + cz + d) * p)) +
                               _lisk->Li(4, -exp((-a - b - cz + d) * p)) - _lisk->Li(4, -exp((a - b - cz + d) * p)) - _lisk->Li(4, -exp((-a + b - cz + d) * p)) + _lisk->Li(4, -exp((a + b - cz + d) * p)) -
                               _lisk->Li(4, -exp((-a - b + cz + d) * p)) + _lisk->Li(4, -exp((a - b + cz + d) * p)) + _lisk->Li(4, -exp((-a + b + cz + d) * p)) - _lisk->Li(4, -exp((a + b + cz + d) * p)))) /
                       (a * b * pow(cz, 2) * pow(p, 4));

        _b_vector[4] = (2. * (a * b * pow(p, 2) * _lisk->Li(3, -exp((-a - b - cz + d) * p)) + a * b * pow(p, 2) * _lisk->Li(3, -exp((a - b - cz + d) * p)) +
                              a * b * pow(p, 2) * _lisk->Li(3, -exp((-a + b - cz + d) * p)) + a * b * pow(p, 2) * _lisk->Li(3, -exp((a + b - cz + d) * p)) - a * b * pow(p, 2) * _lisk->Li(3, -exp((-a - b + cz + d) * p)) -
                              a * b * pow(p, 2) * _lisk->Li(3, -exp((a - b + cz + d) * p)) - a * b * pow(p, 2) * _lisk->Li(3, -exp((-a + b + cz + d) * p)) - a * b * pow(p, 2) * _lisk->Li(3, -exp((a + b + cz + d) * p)) +
                              a * p * _lisk->Li(4, -exp((-a - b - cz + d) * p)) + b * p * _lisk->Li(4, -exp((-a - b - cz + d) * p)) + a * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) - b * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) -
                              a * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) + b * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) - a * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) - b * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) -
                              a * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) - b * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) - a * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) + b * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) +
                              a * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) - b * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) + a * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) + b * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) +
                              _lisk->Li(5, -exp((-a - b - cz + d) * p)) - _lisk->Li(5, -exp((a - b - cz + d) * p)) - _lisk->Li(5, -exp((-a + b - cz + d) * p)) + _lisk->Li(5, -exp((a + b - cz + d) * p)) -
                              _lisk->Li(5, -exp((-a - b + cz + d) * p)) + _lisk->Li(5, -exp((a - b + cz + d) * p)) + _lisk->Li(5, -exp((-a + b + cz + d) * p)) - _lisk->Li(5, -exp((a + b + cz + d) * p)))) /
                       (pow(a, 2) * pow(b, 2) * cz * pow(p, 5));

        _b_vector[5] = (2. * (a * cz * pow(p, 2) * _lisk->Li(3, -exp((-a - b - cz + d) * p)) + a * cz * pow(p, 2) * _lisk->Li(3, -exp((a - b - cz + d) * p)) -
                              a * cz * pow(p, 2) * _lisk->Li(3, -exp((-a + b - cz + d) * p)) - a * cz * pow(p, 2) * _lisk->Li(3, -exp((a + b - cz + d) * p)) + a * cz * pow(p, 2) * _lisk->Li(3, -exp((-a - b + cz + d) * p)) +
                              a * cz * pow(p, 2) * _lisk->Li(3, -exp((a - b + cz + d) * p)) - a * cz * pow(p, 2) * _lisk->Li(3, -exp((-a + b + cz + d) * p)) - a * cz * pow(p, 2) * _lisk->Li(3, -exp((a + b + cz + d) * p)) +
                              a * p * _lisk->Li(4, -exp((-a - b - cz + d) * p)) + cz * p * _lisk->Li(4, -exp((-a - b - cz + d) * p)) + a * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) - cz * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) -
                              a * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) - cz * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) - a * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) + cz * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) -
                              a * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) + cz * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) - a * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) - cz * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) +
                              a * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) - cz * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) + a * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) + cz * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) +
                              _lisk->Li(5, -exp((-a - b - cz + d) * p)) - _lisk->Li(5, -exp((a - b - cz + d) * p)) - _lisk->Li(5, -exp((-a + b - cz + d) * p)) + _lisk->Li(5, -exp((a + b - cz + d) * p)) -
                              _lisk->Li(5, -exp((-a - b + cz + d) * p)) + _lisk->Li(5, -exp((a - b + cz + d) * p)) + _lisk->Li(5, -exp((-a + b + cz + d) * p)) - _lisk->Li(5, -exp((a + b + cz + d) * p)))) /
                       (pow(a, 2) * b * pow(cz, 2) * pow(p, 5));

        _b_vector[6] = (2. * (b * cz * pow(p, 2) * _lisk->Li(3, -exp((-a - b - cz + d) * p)) - b * cz * pow(p, 2) * _lisk->Li(3, -exp((a - b - cz + d) * p)) +
                              b * cz * pow(p, 2) * _lisk->Li(3, -exp((-a + b - cz + d) * p)) - b * cz * pow(p, 2) * _lisk->Li(3, -exp((a + b - cz + d) * p)) + b * cz * pow(p, 2) * _lisk->Li(3, -exp((-a - b + cz + d) * p)) -
                              b * cz * pow(p, 2) * _lisk->Li(3, -exp((a - b + cz + d) * p)) + b * cz * pow(p, 2) * _lisk->Li(3, -exp((-a + b + cz + d) * p)) - b * cz * pow(p, 2) * _lisk->Li(3, -exp((a + b + cz + d) * p)) +
                              b * p * _lisk->Li(4, -exp((-a - b - cz + d) * p)) + cz * p * _lisk->Li(4, -exp((-a - b - cz + d) * p)) - b * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) - cz * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) +
                              b * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) - cz * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) - b * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) + cz * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) -
                              b * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) + cz * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) + b * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) - cz * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) -
                              b * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) - cz * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) + b * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) + cz * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) +
                              _lisk->Li(5, -exp((-a - b - cz + d) * p)) - _lisk->Li(5, -exp((a - b - cz + d) * p)) - _lisk->Li(5, -exp((-a + b - cz + d) * p)) + _lisk->Li(5, -exp((a + b - cz + d) * p)) -
                              _lisk->Li(5, -exp((-a - b + cz + d) * p)) + _lisk->Li(5, -exp((a - b + cz + d) * p)) + _lisk->Li(5, -exp((-a + b + cz + d) * p)) - _lisk->Li(5, -exp((a + b + cz + d) * p)))) /
                       (a * pow(b, 2) * pow(cz, 2) * pow(p, 5));

        _b_vector[7] = (-2. * (4 * pow(a, 3) * b * cz * pow(p, 5) - 3 * pow(a, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a - b - cz + d) * p)) +
                               3 * pow(a, 2) * pow(p, 2) * _lisk->Li(3, -exp((a - b - cz + d) * p)) + 3 * pow(a, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a + b - cz + d) * p)) -
                               3 * pow(a, 2) * pow(p, 2) * _lisk->Li(3, -exp((a + b - cz + d) * p)) + 3 * pow(a, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a - b + cz + d) * p)) -
                               3 * pow(a, 2) * pow(p, 2) * _lisk->Li(3, -exp((a - b + cz + d) * p)) - 3 * pow(a, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a + b + cz + d) * p)) +
                               3 * pow(a, 2) * pow(p, 2) * _lisk->Li(3, -exp((a + b + cz + d) * p)) - 6 * a * p * _lisk->Li(4, -exp((-a - b - cz + d) * p)) - 6 * a * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) +
                               6 * a * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) + 6 * a * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) + 6 * a * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) +
                               6 * a * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) - 6 * a * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) - 6 * a * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) - 6. * _lisk->Li(5, -exp((-a - b - cz + d) * p)) +
                               6. * _lisk->Li(5, -exp((a - b - cz + d) * p)) + 6. * _lisk->Li(5, -exp((-a + b - cz + d) * p)) - 6. * _lisk->Li(5, -exp((a + b - cz + d) * p)) + 6. * _lisk->Li(5, -exp((-a - b + cz + d) * p)) -
                               6. * _lisk->Li(5, -exp((a - b + cz + d) * p)) - 6. * _lisk->Li(5, -exp((-a + b + cz + d) * p)) + 6. * _lisk->Li(5, -exp((a + b + cz + d) * p)))) / (3. * pow(a, 3) * b * cz * pow(p, 5));

        _b_vector[8] = (-2. * (4 * a * pow(b, 3) * cz * pow(p, 5) - 3 * pow(b, 2) * pow(p, 2) * _lisk->Li(3, -exp(-((a + b + cz - d) * p))) + 3 * pow(b, 2) * pow(p, 2) * _lisk->Li(3, -exp((a - b - cz + d) * p)) +
                               3 * pow(b, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a + b - cz + d) * p)) - 3 * pow(b, 2) * pow(p, 2) * _lisk->Li(3, -exp((a + b - cz + d) * p)) +
                               3 * pow(b, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a - b + cz + d) * p)) - 3 * pow(b, 2) * pow(p, 2) * _lisk->Li(3, -exp((a - b + cz + d) * p)) -
                               3 * pow(b, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a + b + cz + d) * p)) + 3 * pow(b, 2) * pow(p, 2) * _lisk->Li(3, -exp((a + b + cz + d) * p)) - 6. * b * p * _lisk->Li(4, -exp(-((a + b + cz - d) * p))) +
                               6. * b * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) - 6. * b * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) + 6. * b * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) +
                               6. * b * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) - 6. * b * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) + 6. * b * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) -
                               6. * b * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) - 6. * _lisk->Li(5, -exp(-((a + b + cz - d) * p))) + 6. * _lisk->Li(5, -exp((a - b - cz + d) * p)) + 6. * _lisk->Li(5, -exp((-a + b - cz + d) * p)) -
                               6. * _lisk->Li(5, -exp((a + b - cz + d) * p)) + 6. * _lisk->Li(5, -exp((-a - b + cz + d) * p)) - 6. * _lisk->Li(5, -exp((a - b + cz + d) * p)) - 6. * _lisk->Li(5, -exp((-a + b + cz + d) * p)) +
                               6. * _lisk->Li(5, -exp((a + b + cz + d) * p)))) / (3. * a * pow(b, 3) * cz * pow(p, 5));

        _b_vector[9] = (-2. * (4 * a * b * pow(cz, 3) * pow(p, 5) - 3 * pow(cz, 2) * pow(p, 2) * _lisk->Li(3, -exp(-((a + b + cz - d) * p))) + 3 * pow(cz, 2) * pow(p, 2) * _lisk->Li(3, -exp((a - b - cz + d) * p)) +
                               3 * pow(cz, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a + b - cz + d) * p)) - 3 * pow(cz, 2) * pow(p, 2) * _lisk->Li(3, -exp((a + b - cz + d) * p)) +
                               3 * pow(cz, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a - b + cz + d) * p)) - 3 * pow(cz, 2) * pow(p, 2) * _lisk->Li(3, -exp((a - b + cz + d) * p)) -
                               3 * pow(cz, 2) * pow(p, 2) * _lisk->Li(3, -exp((-a + b + cz + d) * p)) + 3 * pow(cz, 2) * pow(p, 2) * _lisk->Li(3, -exp((a + b + cz + d) * p)) - 6. * cz * p * _lisk->Li(4, -exp(-((a + b + cz - d) * p))) +
                               6. * cz * p * _lisk->Li(4, -exp((a - b - cz + d) * p)) + 6. * cz * p * _lisk->Li(4, -exp((-a + b - cz + d) * p)) - 6. * cz * p * _lisk->Li(4, -exp((a + b - cz + d) * p)) -
                               6. * cz * p * _lisk->Li(4, -exp((-a - b + cz + d) * p)) + 6. * cz * p * _lisk->Li(4, -exp((a - b + cz + d) * p)) + 6. * cz * p * _lisk->Li(4, -exp((-a + b + cz + d) * p)) -
                               6. * cz * p * _lisk->Li(4, -exp((a + b + cz + d) * p)) - 6. * _lisk->Li(5, -exp(-((a + b + cz - d) * p))) + 6. * _lisk->Li(5, -exp((a - b - cz + d) * p)) + 6. * _lisk->Li(5, -exp((-a + b - cz + d) * p)) -
                               6. * _lisk->Li(5, -exp((a + b - cz + d) * p)) + 6. * _lisk->Li(5, -exp((-a - b + cz + d) * p)) - 6. * _lisk->Li(5, -exp((a - b + cz + d) * p)) - 6. * _lisk->Li(5, -exp((-a + b + cz + d) * p)) +
                               6. * _lisk->Li(5, -exp((a + b + cz + d) * p)))) / (3. * a * b * pow(cz, 3) * pow(p, 5));


        if(degree <= 2) EquivalentPolynomial::MatrixVectorMultiply(_A32_inverse, _b_vector, _coefficients);
        //else if(degree == 3) EquivalentPolynomial::MatrixVectorMultiply(_A23_inverse, _b_vector, _coefficients);
        //else if(degree == 4) EquivalentPolynomial::MatrixVectorMultiply(_A24_inverse, _b_vector, _coefficients);


      }


    }
  }
  else {
    std::cout << "Wrong Dimension " << dim << std::endl;
    abort();
  }
}

void EquivalentPolynomial::MatrixVectorMultiply(const std::vector<std::vector <double>> &A, const std::vector < complex < double > > &bv, std::vector < complex < double > > &xv) {

  xv.resize(bv.size());
  for(unsigned i = 0; i < bv.size(); i++) {
    xv[i] = 0;
    for(unsigned j = 0; j < bv.size(); j++) {
      xv[i] += A[i][j] * bv[j].real();
    }
    //std::cout << "  MatrixVectorMultiply " << bv[i] <<  std::endl;
  }

}

//This function takes a vector of points as inputs and calculates the best fit plane for those points

void EquivalentPolynomial::FindBestFit(const std::vector < double > &pts, const std::vector < double > &Npts, const unsigned &dim) {


  unsigned cnt = 0;
  double normaldotcoefficients = 0.;
  double d = 0;
  unsigned numberofpoints = pts.size() / dim;
  MatrixXd m(numberofpoints, dim);
  _bestfit.resize(dim + 1);
  std::vector < double > N(dim, 0.);
  std::vector < double > centroid(dim, 0.);

//Calculate average Normal and centroid from points
  for(unsigned i = 0; i < numberofpoints; i++) {

    for(unsigned j = 0; j < dim; j++, cnt++) {
      centroid[j] += pts[cnt];
      N[j] += Npts[cnt];
    }

  }
  for(unsigned j = 0; j < dim; j++) {
    N[j] /= numberofpoints;
    centroid[j] /= numberofpoints;
  }

  cnt = 0;

  //Fill matrix to be passed to JacobiSVD
  for(unsigned i = 0; i < numberofpoints; i++) {

    for(unsigned j = 0; j < dim; j++, cnt++) {
      m(i, j) = pts[cnt] - centroid[j];
    }

  }

  JacobiSVD<MatrixXd> svd(m, ComputeThinU | ComputeThinV);
  MatrixXd v = svd.matrixV();


//If dim = 2 and line of best fit is desired, use singualr vector associated with max singular vector

  if(dim <= 2) {


    for(unsigned i = 0; i < dim; i++) {

      _bestfit[i] = v(i, 0);
      normaldotcoefficients += _bestfit[i] * N[i];
    }

  }

  //If dim = 3 and plane of best fit is desired, use singualr vector associated with min singular vector
  if(dim == 3) {

    for(unsigned i = 0; i < dim; i++) {

      _bestfit[i] = v(i, dim - 1);
      normaldotcoefficients += _bestfit[i] * N[i];
    }

  }
  
//   for(unsigned i = 0; i < dim; i++) {
// 
//     std::cout << " coefficent before dot product "  << _bestfit[i] << endl;
//   }

  //Rotate normal by pi if Normal dot coefficents is less than zero
  if(normaldotcoefficients < 0) {

    for(unsigned i = 0; i < dim; i++) {
      _bestfit[i] *= -1.;

    }

  }

//   double numerator = 0;
//   double denominator = 0;
//
//   for(unsigned i = 0; i < numberofpoints - 1; i++) {
//
//     numerator += (pts[2*i] - centroid[0]) * (pts[2*i + 1] - centroid[1]);
//     denominator += (pts[2*i + 1] - centroid[1]) * (pts[2*i + 1] - centroid[1]) - ((pts[2*i] - centroid[0]) * (pts[2*i] - centroid[0]));
//
//   }
//
//   if(denominator != 0.) {
//
//     _bestfit[0] = cos(0.5 * atan(2*numerator / denominator));
//     _bestfit[1] = sin(0.5 * atan(2*numerator / denominator));
//
//   }
//
//Calculate constant d in ax+by+d=0 or ax+by+cz+d=0
  for(unsigned i = 0; i < dim; i++) {

    d -= _bestfit[i] * centroid[i];
  }

  _bestfit[dim] = d;

//   for(unsigned i = 0; i < dim + 1; i++) {
// 
//     std::cout << " coefficent "  << _bestfit[i] << endl;
//   }




  //std::cout << _bestfit[0] * _bestfit[0] + _bestfit[2] * _bestfit[2] + _bestfit[1] * _bestfit[1]  << "   = norm squared" << endl;
  //std::cout << v << "   v matrix" << endl;
  //std::cout << _bestfit[0] * 1. + _bestfit[1] * 2.  + _bestfit[2] << "  check equation" << std::endl;



}


double EquivalentPolynomial::GetValue(std::vector <double> &x, unsigned &element) {

      unsigned numvalues = x.size();
      unsigned numcoefficients = _coefficients.size();
      //std::vector < double > values(numvalues);
      //std::fill(values.begin(), values.end(), 0.);
      double value = 0.;

      if(_dim == 1) {

        for(unsigned k = 0; k < numvalues; k++) {

          for(unsigned i = 0; i < numcoefficients; i++) {

            value += _coefficients[i].real() * pow(x[k], i);

          }

        }

      }

      if(_dim == 2) {

          if(element == 3){
            if(_degree == 2) {

            value = _coefficients[0].real() + _coefficients[1].real() * x[0] + _coefficients[2].real() * x[1] +
                    _coefficients[3].real() * x[0] * x[0] + _coefficients[4].real() * x[1] * x[1] + _coefficients[5].real() * x[0] * x[1];

            }

            if(_degree == 3) {

            value = _coefficients[0].real() + _coefficients[1].real() * x[0] + _coefficients[2].real() * x[1] +
                    _coefficients[3].real() * x[0] * x[0] + _coefficients[4].real() * x[1] * x[1] + _coefficients[5].real() * x[0] * x[1]  +
                    _coefficients[6].real() * x[0] * x[0] * x[0] + _coefficients[7].real() * x[1] * x[1] * x[1] +
                    _coefficients[8].real() * x[0] * x[0] * x[1] + _coefficients[9].real() * x[1] * x[1] * x[0];

            }

            if(_degree == 4) {

            value = _coefficients[0].real() + _coefficients[1].real() * x[0] + _coefficients[2].real() * x[1] +
                    _coefficients[3].real() * x[0] * x[0] + _coefficients[4].real() * x[1] * x[1] + _coefficients[5].real() * x[0] * x[1] +
                    _coefficients[6].real() * x[0] * x[0] * x[0] + _coefficients[7].real() * x[1] * x[1] * x[1] + _coefficients[8].real() * x[0] * x[0] * x[1] + _coefficients[9].real() * x[1] * x[1] * x[0]  + _coefficients[10].real() * x[0] * x[0] * x[0] * x[0] +
                    _coefficients[11].real() * x[1] * x[1] * x[1] * x[1] + _coefficients[12].real() * x[0] * x[0] * x[1] * x[1] +
                    _coefficients[13].real() * x[0] * x[0] * x[0] * x[1] + _coefficients[14].real() * x[1] * x[1] * x[1] * x[0];

            }

            if(_degree == 5) {

            value = _coefficients[0].real() + _coefficients[1].real() * x[0] + _coefficients[2].real() * x[1] +
                    _coefficients[3].real() * x[0] * x[0] + _coefficients[4].real() * x[1] * x[1] + _coefficients[5].real() * x[0] * x[0] * x[0] +
                    _coefficients[6].real() * x[1] * x[1] * x[1] + _coefficients[7].real() * x[0] * x[0] * x[0] * x[0] + _coefficients[8].real() * x[1] * x[1] * x[1] * x[1] +
                    _coefficients[9].real() * pow(x[0], 5) + _coefficients[10].real() * pow(x[1], 5) +
                    _coefficients[11].real() * x[0] * x[0] * x[1] + _coefficients[12].real() * x[0] * x[0] * x[0] * x[1] +
                    _coefficients[13].real() * x[0] * x[0] * x[0] * x[0] * x[1] + _coefficients[14].real() * x[1] * x[1] * x[0] +
                    _coefficients[15].real() * x[1] * x[1] * x[1] * x[0] + _coefficients[16].real() * x[1] * x[1] * x[1] * x[1] * x[0] +
                    _coefficients[17].real() * x[0] * x[0] * x[0] * x[1] * x[1] + _coefficients[18].real() * x[1] * x[1] * x[1] * x[0] * x[0] +
                    _coefficients[19].real() * x[0] * x[1] + _coefficients[20].real() * x[0] * x[0] * x[1] * x[1];

            }

          }
          
          if(element == 4){
           
              value = _coefficients[0].real() + _coefficients[1].real() * x[0] + _coefficients[2].real() * x[1] +
                    _coefficients[3].real() * x[0] * x[0] + _coefficients[4].real() * x[1] * x[1] + _coefficients[5].real() * x[0] * x[1];
              
              
              
          }
      }
      
      if(_dim == 3){
          
          if(element == 0){
          
          value = _coefficients[0].real() + _coefficients[1].real() * x[0] + _coefficients[2].real() * x[1] +
                    _coefficients[3].real() * x[2] + _coefficients[4].real() * x[0] * x[1] + _coefficients[5].real() * x[0] * x[2] +
                     _coefficients[6].real() * x[1] * x[2] + _coefficients[7].real() * x[0] * x[0] +  _coefficients[8].real() * x[1] * x[1] + 
                      _coefficients[9].real() * x[2] * x[2];
          }
      }

      return value;

}

std::vector<std::vector<double>> EquivalentPolynomial::_A13_inverse = {{9. / 8., 0, -(15. / 8), 0}, {0, 75. / 8, 0, -(105. / 8)}, {-(15. / 8), 0, 45. / 8, 0}, {0, -(105. / 8), 0, 175. / 8}};

std::vector<std::vector<double>> EquivalentPolynomial::_A22_inverse = {{7. / 8, 0, 0, -(15. / 16), -(15. / 16), 0}, {0, 3. / 4, 0, 0, 0, 0}, {0, 0, 3. / 4, 0, 0, 0},
  {-(15. / 16), 0, 0, 45. / 16, 0, 0}, {-(15. / 16), 0, 0, 0, 45. / 16, 0}, {0, 0, 0, 0, 0, 9. / 4}
};

std::vector<std::vector<double>> EquivalentPolynomial::_A23_inverse = {{7. / 8, 0, 0, -(15. / 16), -(15. / 16), 0, 0, 0, 0, 0}, {
    0, 45. / 8, 0, 0, 0,
    0, -(105. / 16), 0, 0, -(45. / 16)
  }, {
    0, 0, 45. / 8, 0, 0, 0,
    0, -(105. / 16), -(45. / 16), 0
  }, {
    -(15. / 16), 0, 0, 45. / 16, 0, 0, 0, 0, 0,
      0
    }, {-(15. / 16), 0, 0, 0, 45. / 16, 0, 0, 0, 0, 0}, {
    0, 0, 0, 0, 0, 9. / 4,
    0, 0, 0, 0
  }, {0, -(105. / 16), 0, 0, 0, 0, 175. / 16, 0, 0, 0}, {
    0,
    0, -(105. / 16), 0, 0, 0, 0, 175. / 16, 0, 0
  }, {
    0, 0, -(45. / 16), 0, 0, 0,
    0, 0, 135. / 16, 0
  }, {0, -(45. / 16), 0, 0, 0, 0, 0, 0, 0, 135. / 16}
};

std::vector<std::vector<double>> EquivalentPolynomial::_A24_inverse = {{
    243. / 128, 0, 0, -(675. / 128), -(675. / 128), 0, 0, 0, 0, 0, 945. / 256, 945. /
    256, 225. / 64, 0, 0
  }, {
    0, 45. / 8, 0, 0, 0, 0, -(105. / 16), 0, 0, -(45. / 16),
    0, 0, 0, 0, 0
  }, {
    0, 0, 45. / 8, 0, 0, 0, 0, -(105. / 16), -(45. / 16), 0, 0,
    0, 0, 0, 0
  }, {
    -(675. / 128), 0, 0, 1215. / 32, 225. / 64, 0, 0, 0, 0,
      0, -(4725. / 128), 0, -(675. / 64), 0, 0
    }, {
    -(675. / 128), 0, 0, 225. / 64,
      1215. / 32, 0, 0, 0, 0, 0, 0, -(4725. / 128), -(675. / 64), 0, 0
    }, {
    0, 0, 0,
    0, 0, 207. / 8, 0, 0, 0, 0, 0, 0,
    0, -(315. / 16), -(315. / 16)
  }, {
    0, -(105. / 16), 0, 0, 0, 0, 175. / 16, 0, 0,
    0, 0, 0, 0, 0, 0
  }, {
    0, 0, -(105. / 16), 0, 0, 0, 0, 175. / 16, 0, 0, 0, 0,
    0, 0, 0
  }, {
    0, 0, -(45. / 16), 0, 0, 0, 0, 0, 135. / 16, 0, 0, 0, 0, 0,
    0
  }, {0, -(45. / 16), 0, 0, 0, 0, 0, 0, 0, 135. / 16, 0, 0, 0, 0, 0}, {
    945. /
    256, 0, 0, -(4725. / 128), 0, 0, 0, 0, 0, 0, 11025. / 256, 0, 0, 0,
    0
  }, {
    945. / 256, 0, 0, 0, -(4725. / 128), 0, 0, 0, 0, 0, 0, 11025. / 256, 0,
    0, 0
  }, {
    225. / 64, 0, 0, -(675. / 64), -(675. / 64), 0, 0, 0, 0, 0, 0, 0,
    2025. / 64, 0, 0
  }, {
    0, 0, 0, 0, 0, -(315. / 16), 0, 0, 0, 0, 0, 0, 0, 525. /
    16, 0
  }, {0, 0, 0, 0, 0, -(315. / 16), 0, 0, 0, 0, 0, 0, 0, 0, 525. / 16}
};

std::vector<std::vector<double>> EquivalentPolynomial::_A22T_inverse = {
  {72, -240, -240, 180, 180, 360},
  {-240, 1200, 600, -1080, -360, -1440},
  {-240, 600, 1200, -360, -1080, -1440},
  {180, -1080, -360, 1080, 180, 1080}, 
  {180, -360, -1080, 180, 1080, 1080}, 
  {360, -1440, -1440, 1080, 1080, 2880
  }
};


std::vector<std::vector<double>> EquivalentPolynomial::_A32_inverse = {{19. / 32, 0, 0, 0, 0, 0, 0, -(15. / 32), -(15. / 32), -(15. / 32)}, {
    0, 3. / 8, 0,
    0, 0, 0, 0, 0, 0, 0
  }, {0, 0, 3. / 8, 0, 0, 0, 0, 0, 0, 0}, {
    0, 0, 0, 3. /
    8, 0, 0, 0, 0, 0, 0
  }, {0, 0, 0, 0, 9. / 8, 0, 0, 0, 0, 0}, {
    0, 0, 0, 0,
    0, 9. / 8, 0, 0, 0, 0
  }, {0, 0, 0, 0, 0, 0, 9. / 8, 0, 0, 0}, {
    -(15. / 32),
      0, 0, 0, 0, 0, 0, 45. / 32, 0, 0
    }, {
    -(15. / 32), 0, 0, 0, 0, 0, 0, 0, 45. /
      32, 0
    }, {-(15. / 32), 0, 0, 0, 0, 0, 0, 0, 0, 45. / 32}
};*/
