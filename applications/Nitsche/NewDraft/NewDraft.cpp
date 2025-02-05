// This application describes integration of modified Heaviside function using particles as integration points
//on a cubic domain cut by an arc, IntSum is this computed quantity. We also verify the rule can integrate exactly 
// upto a prescribed order m in 2D and 3D. This is all achieved by computing new weights w_new which are close to
// the original ones. We verify the convergence by refining the domain lmax times. 

#include "FemusInit.hpp"
#include <iostream>
#include <vector>
#include <ctime>
//#include <eigen3/Eigen/Dense>
#include <fstream>
#include <cmath>
#include "NewDraft.hpp"
// valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./executable



double a0, a1, a3, a5, a7, a9;

int main(int argc, char** args) {
  unsigned dim = 2;
  unsigned NG = 4; // Gauss points
  unsigned m = 1; // Chebshev Polynomial degree
  
//   double eps = 0.001; // width of transition region
//   //unsigned nbl = 1; // number of bands on boundary
//   //bool gradedbl = false; // nobody knows what???
//   double a = 0.;
//   double b = 1.;
//
//   unsigned lmax = 0; // number of refinements
//   std::vector<double> EQ; // quadrature errors
//   std::vector<double> EI; // integral errors
//   EQ.resize(lmax + 1);
//   EI.resize(lmax + 1);
//
//   double QuadExact = pow((pow(b, m + 1) - pow(a, m + 1)) / (m + 1), dim);
//   double IntExact2D = M_PI * 0.75 * 0.75 * 0.75 * 0.75 / 8.;
//   double IntExact3D = 0.04970097753; // int(int(int((R^2 - rho^2)*rho^2*sin(phi), rho = 0 .. R), phi = 0 .. Pi/2), theta = 0 .. Pi/2)
//   std::vector <double> IntExact = {IntExact2D, IntExact3D};
//   double QuadSum; // quadrature sum at each refinement level
//   double IntSum; // integral sum at each refinement level
//
//
//   for(unsigned l = 0; l < lmax + 1 ; l++) {
//     unsigned NT = 0;
//
//     std::vector<double> VxL(dim);
//     std::vector<double> VxR(dim);
//
//     std::clock_t c_start = std::clock();
//     double Qsum = 0.; // quadrature sum at the subrectangles of refinement l
//     double Isum = 0.; // integral sum at the subrectangles of refinement l
//     double h = (b - a) / pow(2, l);
//     std::vector<double> x1D(pow(2, l) + 1); // nodes in one direction at level l;
//     for(unsigned i = 0; i < x1D.size(); i++) {
//       x1D[i] = a + i * h;
//     }
//
//     std::vector<unsigned> I(dim);
//     std::vector<unsigned> N(dim);
//     for(unsigned k = 0; k < dim ; k++) {
//       N[k] = pow(2, l * (dim - k - 1));
//     }
//
//     std::vector < std::vector <double> > xp;
//     std::vector <double> wp;
//     std::vector< double > dist;
//     for(unsigned t = 0; t < pow(2, dim * l) ; t++) {
//       I[0] = t / N[0];
//       for(unsigned k = 1; k < dim ; k++) {
//         unsigned pk = t % N[k - 1];
//         I[k] = pk / N[k];
//       }
//
//       for(unsigned k = 0; k < dim; k++) {
//         VxL[k] = x1D[I[k]];
//         VxR[k] = x1D[I[k] + 1];
//       }
//
//        InitParticlesDisk3D(dim, NG, VxL, VxR, {0., 0., 0.}, 0.75, xp, wp, dist);
//
//
//       Eigen::VectorXd wP = Eigen::VectorXd::Map(&wp[0], wp.size());
//
//       Eigen::MatrixXd xP(xp.size(), xp[0].size());
//       for(int i = 0; i < xp.size(); ++i) {
//         xP.row(i) = Eigen::VectorXd::Map(&xp[i][0], xp[0].size());
//       }
//       NT += xP.row(0).size();
//
//       unsigned np = xP.cols();
//       Eigen::MatrixXd xI;
//       xI.resize(dim, np);
//
//       for(unsigned k = 0; k < dim; k++) {
//         for(unsigned j = 0; j < np ; j++) {
//           xI(k, j) = (2. / (VxR[k] - VxL[k])) * xP(k, j) - ((VxR[k] + VxL[k]) / (VxR[k] - VxL[k])) ;
//         }
//       }
//
//
//       Eigen::Tensor<double, 3, Eigen::RowMajor> PmX;
//       GetChebXInfo(m, dim, np, xI, PmX);
//
//       Eigen::VectorXd xg;
//       Eigen::VectorXd wg;
//       GetGaussPointsWeights(NG, xg, wg);
//       Eigen::MatrixXd Pg;
//       Cheb(m, xg, Pg);
//
//       Eigen::MatrixXd A;
//       GetMultiDimChebMatrix(dim, m, np, PmX, A);
//
//       Eigen::VectorXd F;
//       GetChebGaussF(dim, m, VxL, VxR, Pg,  wg, F);
//
//
//       Eigen::VectorXd w_new;
//       SolWeightEigen(A, F, wP, w_new);
//
//       PrintMarkers(dim, xP, dist, wP, w_new, l, t);
//
//       Testing(a, b, m, dim, xP, w_new, dist, eps, QuadSum, IntSum);
//       Qsum += QuadSum;
//       Isum += IntSum;
//     }
//     EQ[l] = fabs((Qsum - QuadExact) / QuadExact); // relative quadrarture error at level l
//     EI[l] = fabs((Isum - IntExact[dim - 2]) / IntExact[dim - 2]); // relative integral error at level l
//
//     std::cout << "Time at level" << l << " = " << 1000. * (clock() - c_start) / CLOCKS_PER_SEC << std::endl;
//     std::cout << "TotalPoints at  "<< "Level " << l << " = "<< NT << std::endl;
//     std::cout << std::endl;
//
//   }
//
//   std::cout << "Quadrature_Errors: " ;
//   PrintVec(EQ);
//   std::cout << "\nHeaviside_Errors: ";
//   PrintVec(EI);
//
//   for(unsigned j = 0; j < EQ.size() - 1 ; j++) {
//     double c2 = fabs(log(EI[0] / EI[j + 1]) / log(pow(2, j + 1)));
//     std::cout << "Refinement " << j << " Integral_Convergance_Rate: " << c2 << std::endl;
//     std::cout << "\n" << std::endl;
//   }
//
  return 0;
}

