
#include <iostream>
#include <iomanip>

#include <boost/math/special_functions/factorials.hpp>
#include <boost/math/special_functions/beta.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/sum_kahan.hpp>

#include "LineOld.hpp"
//#include "TestSquareS.hpp"


using namespace boost;
using namespace accumulators;

using boost::math::factorial;
using boost::math::beta;


#include <boost/multiprecision/cpp_bin_float.hpp>
namespace boost {
  namespace multiprecision {
    typedef number < backends::cpp_bin_float < 24, backends::digit_base_2, void, boost::int16_t, -126, 127 >, et_off >         cpp_bin_float_single;
    typedef number < backends::cpp_bin_float < 53, backends::digit_base_2, void, boost::int16_t, -1022, 1023 >, et_off >       cpp_bin_float_double;
    typedef number < backends::cpp_bin_float < 64, backends::digit_base_2, void, boost::int16_t, -16382, 16383 >, et_off >     cpp_bin_float_double_extended;
    typedef number < backends::cpp_bin_float < 113, backends::digit_base_2, void, boost::int16_t, -16382, 16383 >, et_off >    cpp_bin_float_quad;
    typedef number < backends::cpp_bin_float < 237, backends::digit_base_2, void, boost::int32_t, -262142, 262143 >, et_off >  cpp_bin_float_oct;
  }
} // namespaces

using boost::multiprecision::cpp_bin_float_oct;
using boost::multiprecision::cpp_bin_float_quad;
std::vector<bool> statements(18, false);





template <class TypeA>
TypeA F0(const int &s, const std::vector<unsigned> &m, const std::vector <TypeA> &a, const TypeA &d, const TypeA &x2) {



  TypeA sum(0);
  for(unsigned i = 0; i <= s; i++) {
    TypeA c1 = pow(a[1], i) / ((m[1] + i + 1) * factorial<TypeA>(i));
    for(unsigned k = 0; k <= s - i; k++) {
      sum += c1 * pow(a[0], k) * pow(d, s - i - k) * (pow(x2, m[0] + k + 1)) /
             (factorial<TypeA>(s - i - k) *  factorial<TypeA>(k) * (m[0] + k + 1));
    }
  }

  if(d == -2) {
    //std::cout << "F0 = " << sum << std::endl;
  }

  return  sum;
}

template <class TypeA>
TypeA F1(const int &s, const std::vector<unsigned> &m, const std::vector <TypeA> &a, const TypeA &d, const TypeA &onemx1) {

  //std::cout << "F" << std::endl;

  TypeA sum(0);
  for(unsigned i = 0; i <= s; i++) {

    TypeA c1 = pow(a[1], i) / ((m[1] + i + 1) * factorial<TypeA>(i));
    for(unsigned j = 0; j <= s - i; j++) {
      unsigned M = m[0] + j + 1;
      TypeA sumk(0);

      for(unsigned k = 1; k <= M; k++) {
        sumk -= pow(-onemx1, k) / (factorial<TypeA>(k) * factorial<TypeA>(M - k));
      }

      sum += (sumk * c1 * pow(a[0], j) * pow(d, s - i - j) * factorial<TypeA>(M)) / (factorial<TypeA>(s - i - j) *  factorial<TypeA>(j) * (m[0] + j + 1));
      //std::cout << "sumk = " << sumk << std::endl;
    }
  }

  if(d == -2) {
    //std::cout << "F1 = " << sum << std::endl;

  }

  return sum;

}


template <class TypeA>
TypeA F(const int &s, const std::vector<unsigned> &m, const std::vector <TypeA> &a, const TypeA &d, const TypeA &x1, const TypeA &x2) {


  TypeA sum(0);
  for(unsigned i = 0; i <= s; i++) {
    TypeA c1 = pow(a[1], i) / ((m[1] + i + 1) * factorial<TypeA>(i));
    for(unsigned k = 0; k <= s - i; k++) {
      sum += c1 * pow(a[0], k) * pow(d, s - i - k) * (pow(x2, m[0] + k + 1) - pow(x1, m[0] + k + 1)) / //TODO build F0 and F1
             (factorial<TypeA>(s - i - k) *  factorial<TypeA>(k) * (m[0] + k + 1));
      //std::cout << "stuff = " << pow(x2, m[0] + k + 1) - pow(x1, m[0] + k + 1) << std::endl;
    }
  }
  if(d == -2) {
    //std::cout << "F = " << sum << std::endl;
  }

  return  sum;
}

template <class TypeA>
TypeA G0(const int &s, const std::vector<unsigned> &m, const std::vector <TypeA> &a, const TypeA &d, const TypeA &x2) {

  //std::cout << "G0" << std::endl;
  TypeA sum = 0;
  TypeA prod = 1;
  for(unsigned j = 0; j <= s + m[1] + 1; j++) {
    sum += pow(x2, m[0] + j + 1) * (pow(a[0], j) * pow(d, s + m[1] + 1 - j)) / (factorial<TypeA>(s + m[1] + 1 - j) * factorial<TypeA>(j) * (m[0] + j + 1));
  }

  prod = ((m[1] % 2 == 0) ? -1 : 1) * (factorial<TypeA>(m[1]) / pow(a[1], m[1] + 1));

  if(d == -2) {
    //std::cout << "G0_sum = " << sum << "  G0_prod = " << prod << std::endl;
  }

  return sum * prod;
}

template <class TypeA>
TypeA G1(const int &s, const std::vector<unsigned> &m, const std::vector <TypeA> &a, const TypeA &d, const TypeA &onemx1) {

  //std::cout << "G1" << std::endl;

  TypeA sum = 0;
  TypeA prod = 1;
  for(unsigned j = 0; j <= s + m[1] + 1; j++) {
    unsigned M = m[0] + j + 1;
    TypeA sumk(0);
    for(unsigned k = 1; k <= M; k++) {
      sumk -= pow(-onemx1, k) / (factorial<TypeA>(k) * factorial<TypeA>(M - k)) ;
    }
    sum += sumk * factorial<TypeA>(m[0] + j + 1) * (pow(a[0], j) * pow(d, s + m[1] + 1 - j)) / (factorial<TypeA>(s + m[1] + 1 - j) * factorial<TypeA>(j) * (m[0] + j + 1));
  }

  prod = ((m[1] % 2 == 0) ? -1 : 1) * (factorial<TypeA>(m[1]) / pow(a[1], m[1] + 1));

  if(d == -2) {
    //std::cout << "G1_sum = " << sum << "  G1_prod = " << prod << std::endl;
  }

  return sum * prod;
}

template <class TypeA>
TypeA G(const int &s, const std::vector<unsigned> &m, const std::vector <TypeA> &a, const TypeA &d, const TypeA &x1, const TypeA &x2) {

  //std::cout << "G" << std::endl;

  TypeA sum = 0;
  TypeA prod = 1;
  for(unsigned j = 0; j <= s + m[1] + 1; j++) {
    sum += (pow(x2, m[0] + j + 1) - pow(x1, m[0] + j + 1)) * (pow(a[0], j) * pow(d, s + m[1] + 1 - j)) / (factorial<TypeA>(s + m[1] + 1 - j) * factorial<TypeA>(j) * (m[0] + j + 1));
  }

  prod = ((m[1] % 2 == 0) ? -1 : 1) * (factorial<TypeA>(m[1]) / pow(a[1], m[1] + 1));

  if(d == -2) {
    //std::cout << "G_sum = " << sum << "  G_prod = " << prod << std::endl;
  }

  return sum * prod;
}

template <class TypeIO, class TypeA>
TypeIO SquareA(const int &s, const std::vector<unsigned> &m_input, const std::vector <TypeIO> &a_input, const TypeIO &d_input) {

  //std::vector<bool> statements(18, false);

  std::vector<unsigned> m = m_input;
  std::vector<TypeA> a(a_input.begin(), a_input.end());
  TypeA d(d_input);

  TypeA SQI(1);

  for(int i = a.size() - 1; i >= 0; i--) {
    if(a[i] == 0) {
      SQI /= TypeA(m[i] + 1);
      a.erase(a.begin() + i);
      m.erase(m.begin() + i);
    }
  }

  if(a.size() == 0) {
    return static_cast <TypeIO>(-SQI * LimLi(s, d));
  }
  else if(a.size() == 1) {
    return static_cast <TypeIO>(SQI * LSI(s, m[0], a[0], d)) ;
  }
  else if(a.size() == 2) {

    if(fabs(a[1]) < fabs(a[0])) {
      std::swap(a[1], a[0]);
      std::swap(m[1], m[0]);
      //std::cout << "Swap has occurred, a[0] = " << a[0] << std::endl;
    }
    //TODO just for testing
//     d = d - a[0] - a[1];
//     a[0] = 2 * a[0];
//     a[1] = 2 * a[1];

    TypeA xf = (-a[1] - d) / a[0];
    TypeA xg = - d / a[0];

    //std::cout << xf << " " << xg << std::endl;




    SQI = 0;

    if(a[0] > 0) {
      if(xf < 0) {
        if(xg < 0) {
          SQI = F0<TypeA>(s, m, a, d, 1);
          statements[0] = true;
        }
        else if(xg < 1) {
          SQI =  F0<TypeA>(s, m, a, d, 1) - G0<TypeA>(s, m, a, d, xg) ;
          statements[1] = true;
        }
        else {
          SQI =  F0<TypeA>(s, m, a, d, 1) - G0<TypeA>(s, m, a, d, 1)  ;
          statements[2] = true;
        }
      }
      else if(xf < 1) {
        if(xg < 0) {
          SQI = G0<TypeA>(s, m, a, d, xf) +  F1<TypeA>(s, m, a, d, 1 - xf);
          statements[3] = true;
        }
        else if(xg < xf) {
          SQI = G<TypeA>(s, m, a, d, xg, xf) +  F1<TypeA>(s, m, a, d, 1 - xf);
          statements[4] = true;
        }
        else if(xg < 1) {
          SQI = -G<TypeA>(s, m, a, d, xf, xg) +  F1<TypeA>(s, m, a, d, 1 - xf);
          statements[5] = true;
        }
        else {


          //SQI =  F<TypeA>(s, m, a, d, xf, 1) - G<TypeA>(s, m, a, d, xf, 1)  ; //xg=1,xf=0,d=-1,a[0]=a[1]=1
//          SQI =  F1<TypeA>(s, m, a, d, 1 - xf) - G1<TypeA>(s, m, a, d, 1 - xf);   //xg=1,xf=0,d=-1,a[0]=a[1]=1

          SQI =  F<TypeA>(s, m, a, d, xf, 1) - G<TypeA>(s, m, a, d, xf, 1)  ; //xg=1,xf=0,d=-1,a[0]=a[1]=1
          //SQI =  F1<TypeA>(s, m, a, d, 1 - xf) - G1<TypeA>(s, m, a, d, 1 - xf);   //xg=1,xf=0,d=-1,a[0]=a[1]=1

          //SQI =  F<TypeA>(s, m, a, d, xf, 1) - G<TypeA>(s, m, a, d, xf, 1); //xg=1,xf=0,d=-1,a[0]=a[1]=1

          //std::cout << "SQI11 = " << SQI << std::endl;

          //std::cout << "SQI = " << SQI << std::endl;
          std::cout << "statement 6**************** " << std::endl;
          statements[6] = true;
        }
      }
      else {
        if(xg < 0) {
          SQI = G0<TypeA>(s, m, a, d, 1);
          statements[7] = true;
        }
        else if(xg < 1) {
          SQI = G1<TypeA>(s, m, a, d, 1 - xg);
          //std::cout << "statement 8++++++++++++++++++++++ " << std::endl;
          statements[8] = true;
        }

      }
    }
    else {
      if(xf > 1) {
        if(xg > 1) {
          SQI =  F0<TypeA>(s, m, a, d, 1);
          statements[9] = true;
        }
        else if(xg > 0) {
          SQI =  F0<TypeA>(s, m, a, d, 1) - G1<TypeA>(s, m, a, d, 1 - xg);
          statements[10] = true;
        }
        else {
          SQI =  F0<TypeA>(s, m, a, d, 1) - G0<TypeA>(s, m, a, d, 1); //xf=1,xg=0,a[0]=-1,a[1]=1,d=0
          statements[11] = true;
        }
      }
      else if(xf > 0) {
        if(xg > 1) {
          SQI =  F0<TypeA>(s, m, a, d, xf) + G1<TypeA>(s, m, a, d, 1 - xf);
          statements[12] = true;
        }
        else if(xg > xf) {
          SQI =  F0<TypeA>(s, m, a, d, xf) + G<TypeA>(s, m, a, d, xf, xg);
          statements[13] = true;
        }
        else if(xg > 0) {
          SQI =  F0<TypeA>(s, m, a, d, xf) - G<TypeA>(s, m, a, d, xg, xf);
          statements[14] = true;
        }
        else {
          SQI =  F0<TypeA>(s, m, a, d, xf) - G0<TypeA>(s, m, a, d, xf)  ;
          statements[15] = true;
        }
      }
      else {
        if(xg > 1) {
          SQI = G0<TypeA>(s, m, a, d, 1);
          statements[16] = true;
        }
        else if(xg > 0) {
          SQI =  G0<TypeA>(s, m, a, d, xg);
          //SQI =  G<TypeA>(s, m, a, d, 0, xg);
          //std::cout << "statement 17 ++++++++++++++++++++++ " << std::endl;

          statements[17] = true;
        }
      }
    }
//   for(unsigned i = 0; i < statements.size(); i++) {
//     std::cout << "conditional statement " << i << " is " << statements[i] << std::endl;
//   }

    return static_cast<TypeIO>(SQI);
  }
}


template <class TypeIO, class TypeA>
TypeIO SQUmap(const int &s, const std::vector<unsigned> &m, const std::vector <TypeIO> &a, const TypeIO &d) {
  std::vector<TypeA> a2(2);
  TypeIO d1 = d;
  d1 = d1 - a[0] - a[1];
  a2[0] = static_cast<TypeA>(2 * a[0]);
  a2[1] = static_cast<TypeA>(2 * a[1]);

  TypeA d2 = static_cast<TypeA>(d1);
  //return 4 * static_cast<TypeIO>(this->SquareA(s, m, _a2, d2));
  return 4 * SquareA(s, m, a2, d2);
}





template <class Float1>
void TestQuad(const Float1 &eps) {

  typedef typename boost::math::tools::promote_args<Float1>::type myType;
  typedef boost::multiprecision::cpp_bin_float_oct myTypeB;

  std::cout << "testing the Quadrilateral \n";
  std::vector<unsigned> m = {13, 5};
  std::vector<myType> a = {0.0000000001, 0.0000000001};
  std::vector<myTypeB> af = {0., 0.};

  myType d = 0.5;
  myTypeB df;

  std::cout.precision(14);


  for(unsigned i = 0; i < a.size(); i++) {
    af[0] = a[0];
    af[1] = a[1];
    //af[2] = a[2];
  }
  df = d;

  unsigned s = 0;

  myType eps1 = 1.0e-6;
  std::vector<std::vector<myType>> epsCut{{0, 0, 0},
    {eps1, 0, 0}, {-eps1, 0, 0}, {0, eps1, 0}, {0, -eps1, 0}, {0, 0, eps1}, {0, 0, -eps1},
    {eps1, eps1, 0}, {eps1, -eps1, 0}, {-eps1, eps1, 0}, {-eps1, -eps1, 0},
    {eps1, eps1, 0}, {eps1, -eps1, eps1}, {-eps1, eps1, 0}, {-eps1, -eps1, eps1},
    {eps1, eps1, 0}, {eps1, -eps1, -eps1}, {-eps1, eps1, 0}, {-eps1, -eps1, -eps1}
  };

  std::vector<std::vector<myType>> smallCut{{0, 0, 0}, {0, 0, 1}, {0, 0, -1},
    {-1, -1, -2}, {-1, 0, -1}, {-1, 1, -2}, {0, 1, -1},
    {1, 1, -2}, {1, 0, -1}, {-1, -1, -2}, {0, -1, -1}};

  for(unsigned i = 0; i < smallCut.size(); i++) {
    for(unsigned j = 0; j < epsCut.size(); j++) {

      a[0] = smallCut[i][0] + epsCut[j][0];
      a[1] = smallCut[i][1] + 1.0001 * epsCut[j][1];
      d = smallCut[i][2] + epsCut[j][2];

      myType I1 = SquareA<myType, myType>(s,  m, a, d);
      myType I2 = SquareA<myType, myTypeB>(s,  m, a, d);
      if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
        std::cout << "passed ";
        std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
      }
      else {
        std::cout << "Warning failed ";
        std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
      }
    }
    std::cout << std::endl;
  }






  for(unsigned j = 0; j < 2; j++) {
    myType eps1 = 1.0e-8;
    myType c1 = (j == 0) ? eps1 : 1.;
    myType c2 = (j == 0) ? 1. : eps1;

    std::cout << "Epsilon cases\n";

    std::cout << "\na = 0, b = +-eps\n";

    a = {0.0, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    myType I1 = SquareA<double, double>(s,  m, a, d);
    myTypeB I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { 0.0, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { 0.0, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { 0.0, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }


    //////////////////////////////////////////////
    std::cout << "\na = +-eps, b = 0\n";

    a = {-c1, 0.0};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = {-c1, 0.0};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = {c1, 0.0};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = {c1, 0.0};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }


    //////////////////////////////////////////////
    std::cout << "\na = -+eps, b = +-eps\n";

    a = { c1, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { c1, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = {-c1, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }


    a = {-c1, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    //////////////////////////////////////////////
    std::cout << "\na = +-eps, b = +-eps\n";

    a = { c1, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { c1, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = {-c1, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = {-c1, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    //////////////////////////////////////////////
    std::cout << "\na = +-1, b = +-eps\n";

    a = { 1.0, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { 1.0, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { 1.0, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }


    a = { 1.0, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }


    a = { -1.0, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { -1.0, -c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { -1.0, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = -c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }

    a = { -1.0, c1};
    af[0] = a[0];
    af[1] = a[1];
    d = c2;
    df = d;

    I1 = SquareA<double, double>(s,  m, a, d);
    I2 = SquareA<double, myTypeB>(s,  m, a, d);
    if((I2 != 0. &&  fabs((I1 - I2) / I2) < eps) || I1 == 0.) {
      std::cout << "passed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
    }
    else {
      std::cout << "Warning failed ";
      std::cout << "s = " << s << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
    }
  }


  return;

//   std::cout << HyperCubeBOld(6, a.size() - 1, m, a, d) << std::endl;
//   std::cout << HyperCubeBOld(6, af.size() - 1, m, af, df) << std::endl;
//   std::cout << SquareA(6, m, a, d) << std::endl;
//   std::cout << SquareA(6, m, af, df) << std::endl;
//
//
//   for(unsigned i = 0; i < 1000; i++) {
//     a[0] = (0.5 * RAND_MAX - static_cast <myType>(rand())) / RAND_MAX;
//     a[1] = (0.5 * RAND_MAX - static_cast <myType>(rand())) / RAND_MAX;
//
//     myType det = sqrt(a[0] * a[0] + a[1] * a[1]);
//     a[0] /= 1.e12 * det;
//     a[1] /= 1. * det;
//
//     d = 5 * (0.5 * RAND_MAX - static_cast <myType>(rand())) / RAND_MAX;
//
//     af[0] = a[0];
//     af[1] = a[1];
//     df = d;
//
//     if(fabs(SquareA(-1, m, a, d) - HyperCubeBOld(-1, 1, m, af, df)) > eps) {
//       std::cout << "surface test failed" << std::endl;
//       std::cout << a[0] << " " << a[1] << " " << d << "\n";
//       std::cout << SquareA(-1, m, a, d) << " " << HyperCubeBOld(-1, 1, m, af, df) << std::endl;
//     }
//
//     if(fabs(SquareA(0, m, a, d) - HyperCubeBOld(0, 1, m, af, df)) > eps) {
//       std::cout << "volume test failed" << std::endl;
//       std::cout << a[0] << " " << a[1] << " " << d << "\n";
//       std::cout << SquareA(0, m, a, d) << " " << HyperCubeBOld(0, 1, m, af, df) << std::endl;
//     }
//
//     if(fabs(SquareA(5, m, a, d) - HyperCubeBOld(5, 1, m, af, df)) > eps) {
//       std::cout << "s test failed" << std::endl;
//       std::cout << a[0] << " " << a[1] << " " << d << "\n";
//       std::cout << SquareA(5, m, a, d) << " " << HyperCubeBOld(5, 1, m, af, df) <<  " " << SquareA(5, m, af, df) << std::endl;
//     }
//   }
//
//   for(unsigned i = 0; i < 1000; i++) {
//     a[0] = 0.;
//     a[1] = (0.5 * RAND_MAX - static_cast <myType>(rand())) / RAND_MAX;
//
//     myType det = sqrt(a[0] * a[0] + a[1] * a[1]);
//     a[0] /= det;
//     a[1] /= det;
//
//     d = 5. * (0.5 * RAND_MAX - static_cast <myType>(rand())) / RAND_MAX;
//
//     af[0] = a[0];
//     af[1] = a[1];
//     df = d;
//
//     if(fabs(SquareA(-1, m, a, d) - HyperCubeBOld(-1, 1, m, af, df)) > eps) {
//       std::cout << "surface test failed" << std::endl;
//       std::cout << a[0] << " " << a[1] << " " << d << "\n";
//       std::cout << SquareA(-1, m, a, d) << " " << HyperCubeBOld(-1, 1, m, af, df) << std::endl;
//     }
//
//     if(fabs(SquareA(0, m, a, d) - HyperCubeBOld(0, 1, m, af, df)) > eps) {
//       std::cout << "volume test failed" << std::endl;
//       std::cout << a[0] << " " << a[1] << " " << d << "\n";
//       std::cout << SquareA(0, m, a, d) << " " << HyperCubeBOld(0, 1, m, af, df) << std::endl;
//     }
//   }
//
//   for(unsigned i = 0; i < 1000; i++) {
//     a[0] = (0.5 * RAND_MAX - static_cast <myType>(rand())) / RAND_MAX;
//     a[1] = 0.;
//
//     myType det = sqrt(a[0] * a[0] + a[1] * a[1]);
//     a[0] /= det;
//     a[1] /= det;
//
//     d = 5. * (0.5 * RAND_MAX - static_cast <myType>(rand())) / RAND_MAX;
//
//     af[0] = a[0];
//     af[1] = a[1];
//     df = d;
//
//     if(fabs(SquareA(-1, m, a, d) - HyperCubeBOld(-1, 1, m, af, df)) > eps) {
//       std::cout << "surface test failed" << std::endl;
//       std::cout << a[0] << " " << a[1] << " " << d << "\n";
//       std::cout << SquareA(-1, m, a, d) << " " << HyperCubeBOld(-1, 1, m, af, df) << std::endl;
//     }
//
//     if(fabs(SquareA(0, m, a, d) - HyperCubeBOld(0, 1, m, af, df)) > eps) {
//       std::cout << "volume test failed" << std::endl;
//       std::cout << a[0] << " " << a[1] << " " << d << "\n";
//       std::cout << SquareA(0, m, a, d) << " " << HyperCubeBOld(0, 1, m, af, df) << std::endl;
//     }
//   }
// }

}



int main() {

  std::vector<double> a = {1, -1};
  std::vector<unsigned> m = {0, 0};
  double d = 0.;
  unsigned q = 6;
  double dt = 0.;
  double delta = 0.;
  double temp1 = 0.;
  
  

//   for(unsigned k = 0; k < 401; k++) {
//
//     a[1] = 1;
//     a[0] = -1 / (1. - (2 * delta));
//     d = delta / (1 - (2 * delta));
//
//     if(abs(delta - 0.5) > 0.00000000001) {
//       dt = SquareA<double, double>(0, m, a, d);
//     }
//
//     if(!(abs(dt - 0.5) < 0.000000000000001)) {
//       std::cout << "failedmmm!! " << dt << std::endl;
//     }
//
//     a[1] *= -1;
//     a[0] *= -1;
//     d *= -1;
//     if(abs(delta - 0.5) > 0.00000000001) {
//       dt = SquareA<double, double>(0, m, a, d);
//     }
//     if(!(abs(dt - 0.5) < 0.000000000000001)) {
//       std::cout << "failedmmm!! " << dt << std::endl;
//     }
//
//     a[0] = 1;
//     a[1] = -1 / (1. - (2 * delta));
//     d = delta / (1 - (2 * delta));
//
//     if(abs(delta - 0.5) > 0.00000000001) {
//       dt = SquareA<double, double>(0, m, a, d);
//     }
//
//     if(!(abs(dt - 0.5) < 0.000000000000001)) {
//       std::cout << "failedmmm!! " << dt << std::endl;
//     }
//
//     a[1] *= -1;
//     a[0] *= -1;
//     d *= -1;
//     if(abs(delta - 0.5) > 0.00000000001) {
//       dt = SquareA<double, double>(0, m, a, d);
//     }
//     if(!(abs(dt - 0.5) < 0.000000000000001)) {
//       std::cout << "failedmmm!! " << dt << std::endl;
//     }
//     delta += 0.005;
//   }
//
//
//   a[0] = 1;
//   a[1] = 1;
//   d = 0;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//   a[0] = -1;
//   a[1] = -1;
//   d = 0;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//
//   a[0] = -1;
//   a[1] = -1;
//   d = 2;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//   a[0] = 1;
//   a[1] = 1;
//   d = -2;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//
//   a[0] = -1;
//   a[1] = 1;
//   d = 1;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//   a[0] = 1;
//   a[1] = -1;
//   d = -1;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//   a[0] = 1;
//   a[1] = -1;
//   d = 1;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//   a[0] = -1;
//   a[1] = 1;
//   d = -1;
//   std::cout << SquareA<double, double>(0, m, a, d) << "\n";
//
//
//   d = -1;
//   a[0] = a[1] = 1;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//   d = 1;
//   a[0] = a[1] = -1;
//   std::cout << SquareA<double, double>(0, m, a, d) << "\n";
// //   d = -1;
// //   a[0] = -1;
// //   a[1] = 1;
// //   std::cout << SquareA<double, double>(0, m, a, d) << " ";
// //   d = 1;
// //   a[0] = 1;
// //   a[1] = -1;
// //   std::cout << SquareA<double, double>(0, m, a, d) << "\n";
//
//   d = 1;
//   a[0] = 1;
//   a[1] = 1;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//   d = 2;
//   a[0] = -1;
//   a[1] = 1;
//   std::cout << SquareA<double, double>(0, m, a, d) << " ";
//
//
//   std::cout << std::endl;
//   for(unsigned j = 0; j < statements.size(); j++) {
//
//     if(!statements[j]) {
//       std::cout << "Case " << j << " has not been tested yet!" << std::endl;
//     }
//
//   }


//   for(unsigned k = 0; k < q; k++) {
//
//     a[0] = -1;
//     a[1] = -1;
//     d=-1;
//
//     for(unsigned j = 0; j < q; j++) {
//       double temp  = 1 / (double)((j + 1) * (k + j + 2));
//       double diff = temp - SquareA <double, double> (0, {k, j}, a, d);//TODO Fix ordering...?
//
//       //std::cout << "new = " << SquareA<double, double>(0, {k, j}, a, d) << std::endl;
//       //std::cout << "analytical = " << temp << std::endl;
//       std::cout << "difference for m = " << k << " and n = " << j << "difference = " << diff << std::endl;
//     }
//   }
//
//   for(unsigned j = 0; j < statements.size(); j++) {
//
//     if(!statements[j]) {
//       std::cout << "Case " << j << " has not been tested yet!" << std::endl;
//     }
//
//   }

//   a[0] = -1;
//   a[1] = -1;
//
//   m[0] = 3;
//   m[1] = 5;
//   d = 1.1;

//   double I1 = pow(d, 2 + m[0] + m[1]) * factorial<double> (m[0]) * factorial<double>(m[1]) / factorial<double> (2 + m[0] + m[1]);
//
//   std::cout << I1 << " " << SquareA<double, double>(0, m, a, d) << std::endl;
//
//   for(unsigned j = 0; j < statements.size(); j++) {
//
//     if(!statements[j]) {
//       std::cout << "Case " << j << " has not been tested yet!" << std::endl;
//     }
//
//   }

  double eps1 = 1.0e-5;

  std::vector<std::vector<double>> epsCut{{0, 0, 0},
    {eps1, 0, 0}, {-eps1, 0, 0}, {0, eps1, 0}, {0, -eps1, 0}, {0, 0, eps1}, {0, 0, -eps1},
    {eps1, eps1, 0}, {eps1, -eps1, 0}, {-eps1, eps1, 0}, {-eps1, -eps1, 0},
    {eps1, eps1, 0}, {eps1, -eps1, eps1}, {-eps1, eps1, 0}, {-eps1, -eps1, eps1},
    {eps1, eps1, 0}, {eps1, -eps1, -eps1}, {-eps1, eps1, 0}, {-eps1, -eps1, -eps1}
  };

  std::vector<std::vector<double>> smallCut{{0, 0, 0}, {0, 0, 1}, {0, 0, -1},
    {-1, 1, -1}, {-1, 0, -1}, {-1, 1, 1}, {0, 1, -1},
    {1, 1, 0}, {1, 0, -1}, {0, 1, 0}, {0, -1, -1}};


//   m[0] = 0;
//   m[1] = 0;
//
// // small cut in lower left corner **************************************************************************
//   eps1 = 0.1;
//   a[0] = -1.;
//   a[1] = -1.;
//
//   d = eps1;
//   double an = (d * d) / 2.;

//   std::cout << "small cut in lower left corner *************************************************************" << std::endl;
//   for(unsigned i = 0; i < 10; i++) {
//
//     std::cout << "Double with eps = " << d << ": " << SquareA<double, double>(0, m, a, d) << std::endl;
//     std::cout << "Oct with eps = " << d << ": " << SquareA<double, cpp_bin_float_oct>(0, m, a, d) << std::endl;
//     //std::cout << "analytical = " << an << std::endl;
//
//
//     d /= 10.;
//     an = (d * d) / 2.;
//   }
//
//   // small cut in upper left corner **************************************************************************
//   eps1 = 0.1;
//   a[0] = -1.;
//   a[1] = 1.;
//
//   d = -1. + eps1;
//   std::cout << "small cut in upper left corner *************************************************************" << std::endl;
//
//   for(unsigned i = 0; i < 10; i++) {
//
//     std::cout << "Double with eps = " << eps1 << ": " << SquareA<double, double>(0, m, a, d) << std::endl;
//     std::cout << "Oct with eps = " << eps1 << ": " << SquareA<double, cpp_bin_float_oct>(0, m, a, d) << std::endl;
//
//     eps1 /= 10.;
//     d = -1 + eps1;
//   }
//
//   // small cut in upper right corner **************************************************************************
//   eps1 = 0.1;
//   a[0] = -1.;
//   a[1] = -1.;
//
//   d = 2. - eps1;
//   std::cout << "small cut in upper right corner *************************************************************" << std::endl;
//
//   for(unsigned i = 0; i < 10; i++) {
//     std::cout.precision(50);
//     std::cout << "Double with eps = " << eps1 << ": " << SquareA<double, double>(0, m, a, d) << std::endl;
//     std::cout << "Oct with eps = " << eps1 << ": " << SquareA<double, cpp_bin_float_oct>(0, m, a, d) << std::endl;
//
//     eps1 /= 10.;
//     d = 2. - eps1;
//   }
//
//
//   // small cut in lower right corner **************************************************************************
//   eps1 = 0.1;
//   a[0] = 1.;
//   a[1] = -1.;
//
//   d = -1. + eps1;
//   std::cout << "small cut in lower right corner *************************************************************" << std::endl;
//
//   std::vector<cpp_bin_float_oct> af(2);
//   af[0] = static_cast<cpp_bin_float_oct>(a[0]);
//   af[1] = static_cast<cpp_bin_float_oct>(a[1]);
//
//
//   for(unsigned i = 0; i < 10; i++) {
//
//     cpp_bin_float_oct df = static_cast<cpp_bin_float_oct>(d);
//
//     std::cout.precision(40);
//     std::cout << a[0] << " " << a[1] << std::endl;
//     std::cout << af[0] << " " << af[1] << std::endl;
//     std::cout << d << "\n" << df << std::endl;
//
//     std::cout << "Double with eps = " << eps1 << ": " << SquareA<double, double>(0, m, a, d) << std::endl;
//     std::cout << "Oct with eps    = " << eps1 << ": " << SquareA<cpp_bin_float_oct, cpp_bin_float_oct>(0, m, af, df) << std::endl;
//
//     eps1 = pow(0.1, i + 2);
//     d = -1 + eps1;
//   }
//
//   for(unsigned j = 0; j < statements.size(); j++) {
//
//     if(!statements[j]) {
//       std::cout << "Case " << j << " has not been tested yet!" << std::endl;
//     }
//
//   }


//   eps1 = 0.1;
//   a[0] = -1.;
//   a[1] = -eps1;
//
//   d = eps1;
//   std::cout << "small cut with line approaching vertical x = 1 ******************************************" << std::endl;
//
//   for(unsigned i = 0; i < 10; i++) {
//
//     std::cout << "Double with eps = " << eps1 << ": " << SquareA<double, double>(0, m, a, d) << std::endl;
//     std::cout << "Oct with eps = " << eps1 << ": " << SquareA<double, cpp_bin_float_oct>(0, m, a, d) << std::endl;
//
//     eps1 /= 10.;
//     a[1] = -eps1;
//     d = eps1;
//   }


//   eps1 = 0.1;
//   a[0] = -eps1;
//   a[1] = -1.;
//
//   d = eps1;
//   std::cout << "small cut with line approaching horizontal y = 0 ******************************************" << std::endl;
//
//   for(unsigned i = 0; i < 10; i++) {
//
//     std::cout << "Double with eps = " << eps1 << ": " << SquareA<double, double>(0, m, a, d) << std::endl;
//     std::cout << "Oct with eps = " << eps1 << ": " << SquareA<double, cpp_bin_float_oct>(0, m, a, d) << std::endl;
//
//     eps1 /= 10.;
//     a[0] = -eps1;
//     a[1] = -1.;
//     d = eps1;
//   }
//
//   for(unsigned j = 0; j < statements.size(); j++) {
//
//     if(!statements[j]) {
//       std::cout << "Case " << j << " has not been tested yet!" << std::endl;
//     }
//
//   }

//   a[0] = 1;
//   a[1] = 1;
//
//   m[0] = 0;
//   m[1] = 0;
//   d = -1.;

//   std::cout << "d =  -" << -1 << " m = " << m[0] << " n =  " << m[1] << " value = " << SquareA<double, double>(0, m, a, d) << std::endl;
//
//   for(unsigned j = 0; j < statements.size(); j++) {
//
//     if(!statements[j]) {
//       std::cout << "Case " << j << " has not been tested yet!" << std::endl;
//     }
//
//   }

  typedef boost::multiprecision::cpp_bin_float_oct myTypeB;

  a[0] = -1.0e-6;
  a[1] = -1.0e-6;

  m[0] = 0;
  m[1] = 0;
  d = 1.0e-6;
  double I1;
  myTypeB I2;
  std::cout << "d =  -" << d << " m = " << m[0] << " n =  " << m[1] << " value = " << SquareA<double, double>(0, m, a, d) << std::endl;

//   for(int i = 0; i < 28; i++) {
//     m[0] = i + 1;
//     for(int k = 0; k < 12; k++) {
// 
//       m[1] = k + 1;
// //       std::cout << "d =  " << d << " m = " << m[0] << " n =  " << m[1] << " diff = " << abs(SquareA<double, double>(0, m, a, d) - SquareA<double, myTypeB>(0, m, a, d)) << std::endl;
// 
//       I1 = SquareA<double, double>(0,  m, a, d);
//       I2 = SquareA<double, myTypeB>(0,  m, a, d);
//       if((I2 != 0. &&  fabs((I1 - I2) / I2) < 5.0e-12) || I1 == 0.) {
//         std::cout << "passed ";
//         std::cout << "s = " << 0 << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I = " << I1 << std::endl;
//       }
//       else {
//         std::cout << "Warning failed ";
//         std::cout << "s = " << 0 << " a = " << a[0] << " b = " << a[1] << " d = " << d << " I1 =" << I1 << " I2 = " << I2 << std::endl;
//         std::cout << " m = " << m[0] << " n =  " << m[1] << std::endl;
//       }
// 
//     }
//   }




  //I1 = pow(eps, -1 - m[1]) * beta<double> (m[1]+1,  m[0]+2, eps) / (1 + m[0]);
//   std::cout << SquareA<double, double>(0, m, a, d) << std::endl;
//   std::cout << SquareA<double, cpp_bin_float_oct>(0, m, a, d) << std::endl;
//   std::cout << SquareA<double, double>(0, m, a, d) << std::endl;

  eps1 = 5.0e-12;
  TestQuad(eps1);

  for(unsigned j = 0; j < statements.size(); j++) {

    if(!statements[j]) {
      std::cout << "Case " << j << " has not been tested yet!" << std::endl;
    }

  }


  return 1;
}



