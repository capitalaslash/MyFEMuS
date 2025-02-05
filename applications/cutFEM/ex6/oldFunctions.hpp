#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>

using boost::multiprecision::cpp_dec_float_50;
using boost::multiprecision::cpp_dec_float_100;

namespace boost {
  namespace multiprecision {
    typedef number<cpp_dec_float<200> > cpp_dec_float_200;

    typedef number < backends::cpp_bin_float < 24, backends::digit_base_2, void, boost::int16_t, -126, 127 >, et_off >         cpp_bin_float_single;
    typedef number < backends::cpp_bin_float < 53, backends::digit_base_2, void, boost::int16_t, -1022, 1023 >, et_off >       cpp_bin_float_double;
    typedef number < backends::cpp_bin_float < 64, backends::digit_base_2, void, boost::int16_t, -16382, 16383 >, et_off >     cpp_bin_float_double_extended;
    typedef number < backends::cpp_bin_float < 113, backends::digit_base_2, void, boost::int16_t, -16382, 16383 >, et_off >    cpp_bin_float_quad;
    typedef number < backends::cpp_bin_float < 237, backends::digit_base_2, void, boost::int32_t, -262142, 262143 >, et_off >  cpp_bin_float_oct;

    typedef number<cpp_dec_float<7> > cpp_dec_float_7;
    typedef number<cpp_dec_float<14> > cpp_dec_float_14;
    typedef number<cpp_dec_float<21> > cpp_dec_float_21;
    typedef number<cpp_dec_float<28> > cpp_dec_float_28;
    typedef number<cpp_dec_float<35> > cpp_dec_float_35;
    typedef number<cpp_dec_float<42> > cpp_dec_float_42;
    typedef number<cpp_dec_float<49> > cpp_dec_float_49;
    typedef number<cpp_dec_float<56> > cpp_dec_float_56;
    typedef number<cpp_dec_float<63> > cpp_dec_float_63;
    typedef number<cpp_dec_float<63> > cpp_dec_float_70;


  }
} // namespaces

//typedef double myType;
//   typedef double myType;
//typedef boost::multiprecision::cpp_bin_float_oct myType;


//typedef cpp_dec_float_50 myType;
//typedef cpp_dec_float_100 myType;
//typedef boost::multiprecision::cpp_dec_float_200 myType;
//typedef boost::multiprecision::cpp_bin_float_double myType;
//typedef boost::multiprecision::cpp_bin_float_double_extended myType; //long double
//typedef boost::multiprecision::cpp_bin_float_quad myType;
//typedef long double myTypeB;


template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
Int0to1LimLiA(const int &s, const unsigned &m, const Float1 &a, const Float2 &d) {
  //this function can be called only if s>0, (a+d) and d are both non-negative
  typedef typename boost::math::tools::promote_args<Float1, Float2>::type Type;
  Type INT(0.);
  Type x(a + d);

  for(unsigned i = 1; i <= s; i++) {
    INT -= pow(-a, s - i) / factorial<Type>(m + 1 + s - i) * LimLi(i, x) ;
  }
  INT += pow(-a, s) / factorial<Type>(m + 1 + s);
  INT *= factorial<Type>(m);
  return INT;

}

template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
Int0to1LimLiB(const int &s, const unsigned &m, const Float1 &a, const Float2 &d) {
  //this function can be called for any s, (a+d) and d, however if they are both non-negative and a->0 it suffers of significant digit cancellation
  //for s > 0, and (a+d) and d non-negative, we then call the alternative version Int0to1LimLiA
  //for s=-1 and s=0, we need to be sure that d is negative, so that cancellation won't occurs, this is done apriori.
  typedef typename boost::math::tools::promote_args<Float1, Float2>::type Type;

  Type INT = 0.;
  Type x(a + d);
  if(x >= 0.) {
    Type g =  1. / (-a);
    for(unsigned i = 1; i <= m + 1; i++) {
      INT += g * LimLi(s + i, x);
      g *= (m + 1 - i) / (-a);
    }
  }
  INT += pow(-1. / a, m) * LimLi(s + m + 1, d) / a * factorial<Type>(m);
  return INT;
}

template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
Int0to1LimLiC(const int &s, const unsigned &m, const Float1 &a, const Float2 &d) {

  if(s > 0 && d >= 0 && a + d >= 0) {
    return Int0to1LimLiA(s, m, a, d);
  }
  else {
    return Int0to1LimLiB(s, m, a, d);
  }
}


template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
Intm1to1LimLiA(const int &s, const unsigned &m, const Float1 &a, const Float2 &d) {
  //this function can be called only if s > 0, (a+d) and (-a+d) are both non-negative

  typedef typename boost::math::tools::promote_args<Float1, Float2>::type Type;
  Type INT(0.);
  Type x(a + d);
  Type y(-a + d);

  for(unsigned i = 1; i <= s; i++) {
    INT += pow(-a, s - i) / factorial<Type>(m + 1 + s - i) *
           (pow(x, i) + pow(-1, m + s - i) * pow(y, i)) / factorial<Type>(i);
  }
  INT += pow(-a, s) * (1 + pow(-1, m + s)) / factorial<Type>(m + 1 + s);
  INT *= factorial<Type>(m);
  return INT;

}

template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
Intm1to1LimLiB(const int &s, const unsigned &m, const Float1 &a, const Float2 &d) {
  //this function can be called for any s, (a+d) and (-a+d), however if they are both non-negative and a->0 it suffers of significant digit cancellation
  //for s > 0, and (a+d) and (-a+d) non-negative, we then call the alternative version Intm1to1LimLiA
  //for s=-1 and s=0, we need to be sure that d is negative, so that cancellation won't occurs, this is done apriori.
  typedef typename boost::math::tools::promote_args<Float1, Float2>::type Type;
  Type INT = 0.;
  Type x(a + d);
  if(x >= 0.) {
    Type g =  1. / (-a);
    for(unsigned i = 1; i <= m + 1; i++) {
      INT += g * LimLi(s + i, x);
      g *= (m + 1 - i) / (-a);
    }
  }
  x = (-a + d);
  if(x >= 0.) {
    Type g =  pow(-1., m) / a;
    for(unsigned i = 1; i <= m + 1; i++) {
      INT += g * LimLi(s + i, x);
      g *= (m + 1 - i) / a;
    }
  }
  return INT;
}

template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
Intm1to1LimLiC(const int &s, const unsigned &m, const Float1 &a, const Float2 &d) {

  if(s > 0 && -a + d >= 0 && a + d >= 0) {
    return Intm1to1LimLiA(s, m, a, d);
  }
  else {
    return Intm1to1LimLiB(s, m, a, d);
  }
}

template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
HyperCubeCold(const int &s, unsigned i, const std::vector<unsigned> &m, const std::vector <Float1> &a, const Float2 &d) {

  typedef typename boost::math::tools::promote_args<Float1, Float2>::type Type;

  Type aiI = 1. / a[i];

  Type dl = -a[i] + d;
  Type dr = a[i] + d;

  Type HPI = 0.;

  int sl = (m[i] % 2 == 1) ? 1. : -1.; // this is (-1)^(m-1)
  int sr = 1; // this is (-1)^(j-1) for j = 1, ..., m + 1
  Type c = aiI; // this is m!/(m+1-j)! 1/a^j for j = 1,...,m + 1
  if(i > 0) {
    for(int j = 1; j <= m[i] + 1;  c *= aiI * (m[i] + 1 - j), sr *= -1, j++) {
      HPI += c * (sl * HyperCubeCold(s + j, i - 1, m, a, dl) + sr * HyperCubeCold(s + j, i - 1, m, a, dr));
    }
  }
  else {
    for(int j = 1; j <= m[i] + 1;  c *= aiI * (m[i] + 1 - j), sr *= -1, j++) {
      HPI += -c * (sl * LimLi(s + j, dl) + sr * LimLi(s + j, dr));
    }
  }
  return HPI;
}

template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
HyperCubeAold(const int &s, std::vector<unsigned> m, std::vector <Float1> a, const Float2 &d) {

  typedef typename boost::math::tools::promote_args<Float1, Float2>::type Type;
  Type HCI = 1.;
  for(int i = a.size() - 1; i >= 0; i--) {
    if(a[i] == 0) {
      if(m[i] % 2 == 1) {
        return 0.;
      }
      else {
        HCI *= 2. / (m[i] + 1.);
        a.erase(a.begin() + i);
        m.erase(m.begin() + i);
      }
    }
  }
  return HCI * HyperCubeCold(s, a.size() - 1, m, a, d);
}

template <class Float1, class Float2>
typename boost::math::tools::promote_args<Float1, Float2>::type
Int0to1LimLiAAAA(const int &s, const unsigned &m, Float1 a, Float2 d) {
  typedef typename boost::math::tools::promote_args<Float1, Float2>::type Type;

  Type INT = 0.;
  bool unitStepFunctionComplement = false;

  if(d > 0) { //this assures that for s < 1, either limit a + d or d below is negative
    if(s == -1) { //this is the Dirac distribution, the sign of the normal can be changed and INT(delta(ax+d)) = INT(delta(-ax-d))
      a = -a;
      d = -d;
    }
    else if(s == 0) { //this is the unit step function, the sign of the normal can be changed if INT(U(ax+d)) = INT(1) - INT(U(-ax-d))
      a = -a;
      d = -d;
      unitStepFunctionComplement = true;
      INT = -1. / (m + 1);
    }
  }

  Type x(a + d);

  if(x < 0 || d < 0) { // in all these cases no-significant digit cancellation occurs
    if(x >= 0.) {
      Type g =  1. / (-a);
      for(unsigned i = 1; i <= m + 1; i++) {
        INT += g * LimLi(s + i, x);
        g *= (m + 1 - i) / (-a);
      }
    }
    else if(d >= 0.) {
      INT += pow(-1. / a, m) * LimLi(s + m + 1, d) / a * factorial<Type>(m);
    }
  }
  else { //alternative formula to avoid significant digit cancellations when s>1, and (a+d) and d are non-negative and d >> a
    for(int i = 1; i <= s; i++) {
      INT -= pow(-a, s - i) / factorial<Type>(m + 1 + s - i) * LimLi(i, x) ;
    }
    INT += pow(-a, s) / factorial<Type>(m + 1 + s);
    INT *= factorial<Type>(m);
  }
  return (unitStepFunctionComplement) ? -INT : INT;
}

// {
//   INT = 0;
//   Type m0f = factorial<Type>(m[0]);
// 
//   INT += m0f / factorial<Type>(m[0] + s) * pow(-a[0], s)
//          * TriangleA(0, std::vector<unsigned> {m[0] + s, m[1]}, a, d) ;
// 
//   std::cout << INT << " ";
// 
//   for(int i = s - 1; i >= 0; i--) {
//     INT += m0f / factorial<Type>(m[0] + 1 + i) * pow(-a[0], i) * (
//            Int0to1LimLi(s - i, m[1], a[1], a[0] + d)
// 
//            - Int0to1LimLi(s - i, m[0] + m[1] + 1 + i, a[0] + a[1], d));
//     std::cout << INT << " ";
//   }
// }

//  if(d > 0.) {
//         for(unsigned i = 1; i <= m + 1; i++) {
//           TRI += factorial<Type>(m + n + 1 - i) / factorial<Type>(m + 1 - i) * pow((a + b) / a, i);
//           std::cout << TRI << " ";
//         }
//         std::cout << "d = " << d << " ";
//         TRI *= pow(-1., m + n + 1) * LimLi(s + m + n + 2, d) / pow(a + b, m + n + 2) ;
//         std::cout << TRI << " ";
//       }
//       if( a + d > 0.){
//         Type TRI2 = 0.;  
//         for(unsigned i = 1; i <= m + 1; i++) {
//           TRI2 +=  LimLi(s + n + i + 1, a + d) / (factorial<Type>(m + 1 - i) * pow(-a, i));
//           std::cout << TRI2 << " ";
//         }
//         std::cout << "d = " << d << " ";
//         TRI += TRI2 * factorial<Type>(n) / pow(-b, n + 1) ;
//         std::cout << TRI2 << " ";  
//       }
//       if(d > 0.) std::cout << TRI << "\n";
//       TRI *= factorial<Type>(m);
//     }

//    Type TET =  pow(-a[2], s) / factorial<Type>(m[2] + s) *
//                   Tetrahedron(0, std::vector<unsigned> {m[0], m[1], m[2] + s}, a, d) ;
//       for(unsigned i = 1; i <= s; i++)  {
//         Type TETi = 0.;
//         for(unsigned j = 0; j <= m[2] + i; j++)  {
//           TETi += pow(-1., j) / (factorial<Type>(j) * factorial<Type>(m[2] + i - j)) *
//                   Triangle(s + 1 - i, std::vector<unsigned> {m[0] + m[2] + i - j, m[1] + j}, std::vector<Type> {a[0] + a[2], a[1] - a[2]}, d);
//         }
//         TET += TETi * pow(-a[2], i - 1);
//       }
//       TET *= factorial <Type> (m[2]);

