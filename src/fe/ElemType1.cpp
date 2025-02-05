/*=========================================================================

 Program: FEMUS
 Module: ElemType
 Authors: Eugenio Aulisa, Giorgio Bornia

 Copyright (c) FEMTTU
 All rights reserved.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

//----------------------------------------------------------------------------
// includes :
//----------------------------------------------------------------------------

#include "ElemType.hpp"
#include "GaussPoints.hpp"
#include "FETypeEnum.hpp"
#include "Elem.hpp"
#include "NumericVector.hpp"

using std::cout;
using std::endl;


#define  PHIFACE_ONLY_FOR_LAGRANGIAN_FAMILIES  1



namespace femus {

//BEGIN Base Class specialization for template and inheritance.

  void elem_type::GetGaussQuantities(const vector < vector < adept::adouble > >& vt, const unsigned& ig,
                                     adept::adouble &weight,
                                     const double *&phi,
                                     boost::optional < vector < adept::adouble > & > gradphi,
                                     boost::optional < vector < adept::adouble > & > nablaphi) const {
    if(_dim1) {
      static_cast<const elem_type_1D&>(*this).GetGaussQuantities_type(vt, ig, weight,  phi, gradphi, nablaphi);
    }
    else if(_dim2) {
      static_cast<const elem_type_2D&>(*this).GetGaussQuantities_type(vt, ig, weight, phi, gradphi, nablaphi);
    }
    else {
      static_cast<const elem_type_3D&>(*this).GetGaussQuantities_type(vt, ig, weight, phi, gradphi, nablaphi);
    }
  }


  void elem_type::GetGaussQuantities(const vector < vector < double > >& vt, const unsigned& ig,
                                     double& weight,
                                     const double *&phi,
                                     boost::optional < vector < double > & > gradphi,
                                     boost::optional < vector < double > & > nablaphi) const {
    if(_dim1) {
      static_cast<const elem_type_1D&>(*this).GetGaussQuantities_type(vt, ig, weight, phi, gradphi, nablaphi);
    }
    else if(_dim2) {
      static_cast<const elem_type_2D&>(*this).GetGaussQuantities_type(vt, ig, weight, phi, gradphi, nablaphi);
    }
    else {
      static_cast<const elem_type_3D&>(*this).GetGaussQuantities_type(vt, ig, weight, phi, gradphi, nablaphi);
    }

  }


  void elem_type::GetJacobianMatrix(const vector < vector < adept::adouble > >& vt, const  vector < double  >& xi,
                                    adept::adouble &det,
                                    vector < vector < adept::adouble > > & Jac,
                                    vector < vector < adept::adouble > > & JacI) const {
    if(_dim1) {
      static_cast<const elem_type_1D&>(*this).GetJacobianMatrix_type(vt, xi, det, Jac, JacI);
    }
    else if(_dim2) {
      static_cast<const elem_type_2D&>(*this).GetJacobianMatrix_type(vt, xi, det, Jac, JacI);
    }
    else {
      static_cast<const elem_type_3D&>(*this).GetJacobianMatrix_type(vt, xi, det, Jac, JacI);
    }
  }


  void elem_type::GetJacobianMatrix(const vector < vector < double > >& vt, const  vector < double  >& xi,
                                    double &det,
                                    vector < vector < double > > & Jac,
                                    vector < vector < double > > & JacI) const {
    if(_dim1) {
      static_cast<const elem_type_1D&>(*this).GetJacobianMatrix_type(vt, xi, det, Jac, JacI);
    }
    else if(_dim2) {
      static_cast<const elem_type_2D&>(*this).GetJacobianMatrix_type(vt, xi, det, Jac, JacI);
    }
    else {
      static_cast<const elem_type_3D&>(*this).GetJacobianMatrix_type(vt, xi, det, Jac, JacI);
    }
  }

  void elem_type::GetJacobianMatrix(const vector < vector < adept::adouble > >& vt, const unsigned& ig, adept::adouble& Weight,
                                    vector< vector < adept::adouble > >& Jac, vector< vector < adept::adouble > >& JacI) const {
    if(_dim1) {
      static_cast<const elem_type_1D&>(*this).GetJacobianMatrix_type(vt, ig, Weight, Jac, JacI);
    }
    else if(_dim2) {
      static_cast<const elem_type_2D&>(*this).GetJacobianMatrix_type(vt, ig, Weight, Jac, JacI);
    }
    else {
      static_cast<const elem_type_3D&>(*this).GetJacobianMatrix_type(vt, ig, Weight, Jac, JacI);
    }
  }

  void elem_type::GetJacobianMatrix(const vector < vector < double > >& vt, const unsigned& ig, double& Weight,
                                    vector< vector < double > >& Jac, vector< vector < double > >& JacI) const {
    if(_dim1) {
      static_cast<const elem_type_1D&>(*this).GetJacobianMatrix_type(vt, ig, Weight, Jac, JacI);
    }
    else if(_dim2) {
      static_cast<const elem_type_2D&>(*this).GetJacobianMatrix_type(vt, ig, Weight, Jac, JacI);
    }
    else {
      static_cast<const elem_type_3D&>(*this).GetJacobianMatrix_type(vt, ig, Weight, Jac, JacI);
    }
  }



  //END Base Class specialization for template and inheritance.


  //BEGIN Derived Class specialization for template and inheritance.
  template <class type>
  void elem_type_1D::GetGaussQuantities_type(const vector < vector < type > >& vt, const unsigned& ig,
                                             type& weight,
                                             const double *&phi,
                                             boost::optional < vector < type >& > gradphi,
                                             boost::optional < vector < type > & > nablaphi) const {

    phi = _phi[ig];

    type Jac = 0.;
    type JacI;

    const double* dxi = _dphidxi[ig];
    for(int inode = 0; inode < _nc; inode++, dxi++) {
      Jac += (*dxi) * vt[0][inode];
    }

    weight = Jac * _gauss.GetGaussWeightsPointer()[ig];

    if(gradphi) {
      gradphi->resize(_nc * 1);
      JacI = 1. / Jac;
      dxi = _dphidxi[ig];
      for(int inode = 0; inode < _nc; inode++, dxi++) {
        (*gradphi)[inode] = (*dxi) * JacI;

      }

      if(nablaphi) {
        nablaphi->resize(_nc * 1);
        const double* dxi2 = _d2phidxi2[ig];
        for(int inode = 0; inode < _nc; inode++, dxi++, dxi2++) {
          (*nablaphi)[inode] = (*dxi2) * JacI * JacI;
        }
      }
    }
  }

  template <class type>
  void elem_type_1D::GetJacobianMatrix_type(const vector < vector < type > >& vt, const vector < double > & xi,
                                            type &det,
                                            vector < vector < type > > & Jac,
                                            vector < vector < type > > & JacI) const {

    std::vector <double> dphidxi(_nc);

    for(int j = 0; j < _nc; j++) {
      dphidxi[j] = _pt_basis->eval_dphidx(_IND[j], &xi[0]);
    }

    Jac = {{0.}};
    JacI.resize(1, std::vector<type>(1));;

    const double* dxi = &dphidxi[0];
    for(int inode = 0; inode < _nc; inode++, dxi++) {
      Jac[0][0] += (*dxi) * vt[0][inode];
    }
    det = Jac[0][0];

    JacI[0][0] = 1. / det;
  }


  ///////////////////////////////////////////////////////

  template <class type>
  void elem_type_2D::GetGaussQuantities_type(const vector < vector < type > >& vt, const unsigned& ig,
                                             type & weight,
                                             const double *&phi,
                                             boost::optional < vector < type >& > gradphi,
                                             boost::optional < vector < type > & > nablaphi) const {

    phi = _phi[ig];

    type Jac[2][2] = {{0, 0}, {0, 0}};
    const double* dxi = _dphidxi[ig];
    const double* deta = _dphideta[ig];
    typename std::vector< type >::const_iterator vt0i = vt[0].begin();
    typename std::vector< type >::const_iterator vt1i = vt[1].begin();
    // CAREFUL!!! Do not use vt[0].end() to close this loop since vt[0].size() >= _nc
    for(; vt0i < vt[0].begin() + _nc; dxi++, deta++, vt0i++, vt1i++) {
      Jac[0][0] += (*dxi) * (*vt0i);
      Jac[1][0] += (*dxi) * (*vt1i);
      Jac[0][1] += (*deta) * (*vt0i);
      Jac[1][1] += (*deta) * (*vt1i);
    }

    type det = (Jac[0][0] * Jac[1][1] - Jac[0][1] * Jac[1][0]);
    weight = det * _gauss.GetGaussWeightsPointer()[ig];

    if(gradphi) {

      type detI = 1. / det;
      type JacI[2][2];
      JacI[0][0] = Jac[1][1] * detI;
      JacI[0][1] = -Jac[0][1] * detI;
      JacI[1][0] = -Jac[1][0] * detI;
      JacI[1][1] = Jac[0][0] * detI;

      gradphi->resize(_nc * 2);
      dxi = _dphidxi[ig];
      deta = _dphideta[ig];

      for(int inode = 0; inode < _nc; inode++, dxi++, deta++) {
        (*gradphi)[2 * inode + 0] = (*dxi) * JacI[0][0] + (*deta) * JacI[1][0];
        (*gradphi)[2 * inode + 1] = (*dxi) * JacI[0][1] + (*deta) * JacI[1][1];
      }

      if(nablaphi) {
        nablaphi->resize(_nc * 3);
        const double* dxi2 = _d2phidxi2[ig];
        const double* deta2 = _d2phideta2[ig];
        const double* dxideta = _d2phidxideta[ig];
        for(int inode = 0; inode < _nc; inode++, dxi2++, deta2++, dxideta++) {
          (*nablaphi)[3 * inode + 0] =
          ((*dxi2)   *  JacI[0][0] + (*dxideta) * JacI[1][0]) * JacI[0][0] +
          ((*dxideta) * JacI[0][0] + (*deta2)  *  JacI[1][0]) * JacI[1][0];
          (*nablaphi)[3 * inode + 1] =
          ((*dxi2)   *  JacI[0][1] + (*dxideta) * JacI[1][1]) * JacI[0][1] +
          ((*dxideta) * JacI[0][1] + (*deta2)  *  JacI[1][1]) * JacI[1][1];
          (*nablaphi)[3 * inode + 2] =
          ((*dxi2)   *  JacI[0][0] + (*dxideta) * JacI[1][0]) * JacI[0][1] +
          ((*dxideta) * JacI[0][0] + (*deta2)  *  JacI[1][0]) * JacI[1][1];
        }
      }
    }
  }

  template <class type>
  void elem_type_2D::GetJacobianMatrix_type(const vector < vector < type > >& vt, const  vector < double  >& xi,
                                            type &det,
                                            vector < vector < type > > & Jac,
                                            vector < vector < type > > & JacI) const {

    vector <double> dphidxi(_nc);
    vector <double> dphideta(_nc);

    for(int j = 0; j < _nc; j++) {
      dphidxi[j] = _pt_basis->eval_dphidx(_IND[j], &xi[0]);
      dphideta[j] = _pt_basis->eval_dphidy(_IND[j], &xi[0]);
    }

    Jac = {{0., 0,}, {0., 0.}};
    JacI.resize(2, std::vector<type>(2));

    const double* dxi = &dphidxi[0];
    const double* deta = &dphideta[0];

    for(int inode = 0; inode < _nc; inode++, dxi++, deta++) {
      Jac[0][0] += (*dxi) * vt[0][inode];
      Jac[1][0] += (*dxi) * vt[1][inode];
      Jac[0][1] += (*deta) * vt[0][inode];
      Jac[1][1] += (*deta) * vt[1][inode];
    }

    det = (Jac[0][0] * Jac[1][1] - Jac[0][1] * Jac[1][0]);

    JacI[0][0] = +Jac[1][1] / det;
    JacI[0][1] = -Jac[0][1] / det;
    JacI[1][0] = -Jac[1][0] / det;
    JacI[1][1] = +Jac[0][0] / det;

  }

  ///////////////////////////////////////////////////////

  template <class type>
  void elem_type_3D::GetGaussQuantities_type(const vector < vector < type > >& vt, const unsigned& ig,
                                             type& weight,
                                             const double *&phi,
                                             boost::optional < vector < type >& > gradphi,
                                             boost::optional < vector < type > & > nablaphi) const {
    phi = _phi[ig];

    type Jac[3][3] = {{0., 0., 0.}, {0., 0., 0.}, {0., 0., 0.}};
    type JacI[3][3];

    const double* dxi = _dphidxi[ig];
    const double* deta = _dphideta[ig];
    const double* dzeta = _dphidzeta[ig];

    for(int inode = 0; inode < _nc; inode++, dxi++, deta++, dzeta++) {
      Jac[0][0] += (*dxi) * vt[0][inode];
      Jac[1][0] += (*dxi) * vt[1][inode];
      Jac[2][0] += (*dxi) * vt[2][inode];
      Jac[0][1] += (*deta) * vt[0][inode];
      Jac[1][1] += (*deta) * vt[1][inode];
      Jac[2][1] += (*deta) * vt[2][inode];
      Jac[0][2] += (*dzeta) * vt[0][inode];
      Jac[1][2] += (*dzeta) * vt[1][inode];
      Jac[2][2] += (*dzeta) * vt[2][inode];
    }

    type det = (Jac[0][0] * (Jac[1][1] * Jac[2][2] - Jac[1][2] * Jac[2][1]) +
                Jac[0][1] * (Jac[1][2] * Jac[2][0] - Jac[1][0] * Jac[2][2]) +
                Jac[0][2] * (Jac[1][0] * Jac[2][1] - Jac[1][1] * Jac[2][0]));

    weight = det * _gauss.GetGaussWeightsPointer()[ig];

    if(gradphi) {
      gradphi->resize(_nc * 3);

      type detI = 1. / det;

      JacI[0][0] = (-Jac[1][2] * Jac[2][1] + Jac[1][1] * Jac[2][2]) * detI;
      JacI[0][1] = (Jac[0][2] * Jac[2][1] - Jac[0][1] * Jac[2][2]) * detI;
      JacI[0][2] = (-Jac[0][2] * Jac[1][1] + Jac[0][1] * Jac[1][2]) * detI;
      JacI[1][0] = (Jac[1][2] * Jac[2][0] - Jac[1][0] * Jac[2][2]) * detI;
      JacI[1][1] = (-Jac[0][2] * Jac[2][0] + Jac[0][0] * Jac[2][2]) * detI;
      JacI[1][2] = (Jac[0][2] * Jac[1][0] - Jac[0][0] * Jac[1][2]) * detI;
      JacI[2][0] = (-Jac[1][1] * Jac[2][0] + Jac[1][0] * Jac[2][1]) * detI;
      JacI[2][1] = (Jac[0][1] * Jac[2][0] - Jac[0][0] * Jac[2][1]) * detI;
      JacI[2][2] = (-Jac[0][1] * Jac[1][0] + Jac[0][0] * Jac[1][1]) * detI;

      {
        dxi = _dphidxi[ig];
        deta = _dphideta[ig];
        dzeta = _dphidzeta[ig];

        for(int inode = 0; inode < _nc; inode++, dxi++, deta++, dzeta++) {
          (*gradphi)[3 * inode + 0] = (*dxi) * JacI[0][0] + (*deta) * JacI[1][0] + (*dzeta) * JacI[2][0];
          (*gradphi)[3 * inode + 1] = (*dxi) * JacI[0][1] + (*deta) * JacI[1][1] + (*dzeta) * JacI[2][1];
          (*gradphi)[3 * inode + 2] = (*dxi) * JacI[0][2] + (*deta) * JacI[1][2] + (*dzeta) * JacI[2][2];
        }
      }

      if(nablaphi) {

        nablaphi->resize(_nc * 6);
        const double* dxi2 = _d2phidxi2[ig];
        const double* deta2 = _d2phideta2[ig];
        const double* dzeta2 = _d2phidzeta2[ig];
        const double* dxideta = _d2phidxideta[ig];
        const double* detadzeta = _d2phidetadzeta[ig];
        const double* dzetadxi = _d2phidzetadxi[ig];

        for(int inode = 0; inode < _nc; inode++, dxi2++, deta2++, dzeta2++, dxideta++, detadzeta++, dzetadxi++) {

          type dxdxi   = ((*dxi2)     * JacI[0][0] + (*dxideta)   * JacI[1][0] + (*dzetadxi)  * JacI[2][0]);
          type dxdeta  = ((*dxideta)  * JacI[0][0] + (*deta2)     * JacI[1][0] + (*detadzeta) * JacI[2][0]);
          type dxdzeta = ((*dzetadxi) * JacI[0][0] + (*detadzeta) * JacI[1][0] + (*dzeta2)    * JacI[2][0]);

          type dydxi   = ((*dxi2)     * JacI[0][1] + (*dxideta)   * JacI[1][1] + (*dzetadxi)  * JacI[2][1]);
          type dydeta  = ((*dxideta)  * JacI[0][1] + (*deta2)     * JacI[1][1] + (*detadzeta) * JacI[2][1]);
          type dydzeta = ((*dzetadxi) * JacI[0][1] + (*detadzeta) * JacI[1][1] + (*dzeta2)    * JacI[2][1]);

          type dzdxi   = ((*dxi2)     * JacI[0][2] + (*dxideta)   * JacI[1][2] + (*dzetadxi)  * JacI[2][2]);
          type dzdeta  = ((*dxideta)  * JacI[0][2] + (*deta2)     * JacI[1][2] + (*detadzeta) * JacI[2][2]);
          type dzdzeta = ((*dzetadxi) * JacI[0][2] + (*detadzeta) * JacI[1][2] + (*dzeta2)    * JacI[2][2]);

          (*nablaphi)[6 * inode + 0] = dxdxi * JacI[0][0] + dxdeta  * JacI[1][0] + dxdzeta * JacI[2][0];   //_xx
          (*nablaphi)[6 * inode + 1] = dydxi * JacI[0][1] + dydeta  * JacI[1][1] + dydzeta * JacI[2][1];   //_yy
          (*nablaphi)[6 * inode + 2] = dzdxi * JacI[0][2] + dzdeta  * JacI[1][2] + dzdzeta * JacI[2][2];   //_zz

          (*nablaphi)[6 * inode + 3] = dxdxi * JacI[0][1] + dxdeta  * JacI[1][1] + dxdzeta * JacI[2][1];   //_xy
          (*nablaphi)[6 * inode + 4] = dydxi * JacI[0][2] + dydeta  * JacI[1][2] + dydzeta * JacI[2][2];   //_yz
          (*nablaphi)[6 * inode + 5] = dzdxi * JacI[0][0] + dzdeta  * JacI[1][0] + dzdzeta * JacI[2][0];   //_zx


//           (*nablaphi)[6 * inode + 0] =
//           ((*dxi2)     * JacI[0][0] + (*dxideta)   * JacI[1][0] + (*dzetadxi)  * JacI[2][0]) * JacI[0][0] +
//           ((*dxideta)  * JacI[0][0] + (*deta2)     * JacI[1][0] + (*detadzeta) * JacI[2][0]) * JacI[1][0] +
//           ((*dzetadxi) * JacI[0][0] + (*detadzeta) * JacI[1][0] + (*dzeta2)    * JacI[2][0]) * JacI[2][0];
//           (*nablaphi)[6 * inode + 1] =
//           ((*dxi2)     * JacI[0][1] + (*dxideta)   * JacI[1][1] + (*dzetadxi)  * JacI[2][1]) * JacI[0][1] +
//           ((*dxideta)  * JacI[0][1] + (*deta2)     * JacI[1][1] + (*detadzeta) * JacI[2][1]) * JacI[1][1] +
//           ((*dzetadxi) * JacI[0][1] + (*detadzeta) * JacI[1][1] + (*dzeta2)    * JacI[2][1]) * JacI[2][1];
//           (*nablaphi)[6 * inode + 2] =
//           ((*dxi2)     * JacI[0][2] + (*dxideta)   * JacI[1][2] + (*dzetadxi)  * JacI[2][2]) * JacI[0][2] +
//           ((*dxideta)  * JacI[0][2] + (*deta2)     * JacI[1][2] + (*detadzeta) * JacI[2][2]) * JacI[1][2] +
//           ((*dzetadxi) * JacI[0][2] + (*detadzeta) * JacI[1][2] + (*dzeta2)    * JacI[2][2]) * JacI[2][2];
//           (*nablaphi)[6 * inode + 3] =
//           ((*dxi2)     * JacI[0][0] + (*dxideta)   * JacI[1][0] + (*dzetadxi)  * JacI[2][0]) * JacI[0][1] +
//           ((*dxideta)  * JacI[0][0] + (*deta2)     * JacI[1][0] + (*detadzeta) * JacI[2][0]) * JacI[1][1] +
//           ((*dzetadxi) * JacI[0][0] + (*detadzeta) * JacI[1][0] + (*dzeta2)    * JacI[2][0]) * JacI[2][1];
//           (*nablaphi)[6 * inode + 4] =
//           ((*dxi2)     * JacI[0][1] + (*dxideta)   * JacI[1][1] + (*dzetadxi)  * JacI[2][1]) * JacI[0][2] +
//           ((*dxideta)  * JacI[0][1] + (*deta2)     * JacI[1][1] + (*detadzeta) * JacI[2][1]) * JacI[1][2] +
//           ((*dzetadxi) * JacI[0][1] + (*detadzeta) * JacI[1][1] + (*dzeta2)    * JacI[2][1]) * JacI[2][2];
//           (*nablaphi)[6 * inode + 5] =
//           ((*dxi2)     * JacI[0][2] + (*dxideta)   * JacI[2][1] + (*dzetadxi)  * JacI[2][2]) * JacI[0][0] +
//           ((*dxideta)  * JacI[0][2] + (*deta2)     * JacI[2][1] + (*detadzeta) * JacI[2][2]) * JacI[1][0] +
//           ((*dzetadxi) * JacI[0][2] + (*detadzeta) * JacI[2][1] + (*dzeta2)    * JacI[2][2]) * JacI[2][0];

        }
      }
    }
  }
  //END Derived Class specialization for template and inheritance.


  template <class type>
  void elem_type_3D::GetJacobianMatrix_type(const vector < vector < type > >& vt, const vector < double > & xi,
                                            type &det,
                                            vector < vector < type > > & Jac, vector < vector < type > > & JacI) const {

    std::vector < double > dphidxi(_nc);
    std::vector < double > dphideta(_nc);
    std::vector < double > dphidzeta(_nc);

    for(int j = 0; j < _nc; j++) {
      dphidxi[j] = _pt_basis->eval_dphidx(_IND[j], &xi[0]);
      dphideta[j] = _pt_basis->eval_dphidy(_IND[j], &xi[0]);
      dphidzeta[j] = _pt_basis->eval_dphidz(_IND[j], &xi[0]);
    }

    Jac = {{0., 0., 0.}, {0., 0., 0.}, {0., 0., 0.}};
    JacI.resize(3, std::vector<type>(3));

    const double* dxi = &dphidxi[0];
    const double* deta = &dphideta[0];
    const double* dzeta = &dphidzeta[0];

    for(int inode = 0; inode < _nc; inode++, dxi++, deta++, dzeta++) {
      Jac[0][0] += (*dxi) * vt[0][inode];
      Jac[1][0] += (*dxi) * vt[1][inode];
      Jac[2][0] += (*dxi) * vt[2][inode];
      Jac[0][1] += (*deta) * vt[0][inode];
      Jac[1][1] += (*deta) * vt[1][inode];
      Jac[2][1] += (*deta) * vt[2][inode];
      Jac[0][2] += (*dzeta) * vt[0][inode];
      Jac[1][2] += (*dzeta) * vt[1][inode];
      Jac[2][2] += (*dzeta) * vt[2][inode];
    }

    det = (Jac[0][0] * (Jac[1][1] * Jac[2][2] - Jac[1][2] * Jac[2][1]) +
           Jac[0][1] * (Jac[1][2] * Jac[2][0] - Jac[1][0] * Jac[2][2]) +
           Jac[0][2] * (Jac[1][0] * Jac[2][1] - Jac[1][1] * Jac[2][0]));

    JacI[0][0] = (-Jac[1][2] * Jac[2][1] + Jac[1][1] * Jac[2][2]) / det;
    JacI[0][1] = (+Jac[0][2] * Jac[2][1] - Jac[0][1] * Jac[2][2]) / det;
    JacI[0][2] = (-Jac[0][2] * Jac[1][1] + Jac[0][1] * Jac[1][2]) / det;
    JacI[1][0] = (+Jac[1][2] * Jac[2][0] - Jac[1][0] * Jac[2][2]) / det;
    JacI[1][1] = (-Jac[0][2] * Jac[2][0] + Jac[0][0] * Jac[2][2]) / det;
    JacI[1][2] = (+Jac[0][2] * Jac[1][0] - Jac[0][0] * Jac[1][2]) / det;
    JacI[2][0] = (-Jac[1][1] * Jac[2][0] + Jac[1][0] * Jac[2][1]) / det;
    JacI[2][1] = (+Jac[0][1] * Jac[2][0] - Jac[0][0] * Jac[2][1]) / det;
    JacI[2][2] = (-Jac[0][1] * Jac[1][0] + Jac[0][0] * Jac[1][1]) / det;

  }

  template <class type>
  void elem_type_1D::GetJacobianMatrix_type(const vector < vector < type > >& vt, const unsigned& ig, type& Weight,
                                            vector < vector < type > >& Jac, vector < vector < type > >& JacI) const {

    Jac = {{0}};
    JacI.resize(1, std::vector<type>(1));

    const double* dxi = _dphidxi[ig];

    for(int inode = 0; inode < _nc; inode++, dxi++) {
      Jac[0][0] += (*dxi) * vt[0][inode];
    }

    JacI[0][0] = 1 / Jac[0][0];

    Weight = Jac[0][0] * _gauss.GetGaussWeightsPointer()[ig];

  }

  template <class type>
  void elem_type_2D::GetJacobianMatrix_type(const vector < vector < type > >& vt, const unsigned& ig, type& Weight,
                                            vector < vector < type > >& Jac, vector < vector < type > >& JacI) const {


    Jac = {{0, 0}, {0, 0}};
    JacI.resize(2, std::vector<type>(2));

    const double* dxi = _dphidxi[ig];
    const double* deta = _dphideta[ig];

    for(int inode = 0; inode < _nc; inode++, dxi++, deta++) {
      Jac[0][0] += (*dxi) * vt[0][inode];
      Jac[1][0] += (*dxi) * vt[1][inode];
      Jac[0][1] += (*deta) * vt[0][inode];
      Jac[1][1] += (*deta) * vt[1][inode];
    }

    type det = (Jac[0][0] * Jac[1][1] - Jac[0][1] * Jac[1][0]);

    JacI[0][0] = Jac[1][1] / det;
    JacI[0][1] = -Jac[0][1] / det;
    JacI[1][0] = -Jac[1][0] / det;
    JacI[1][1] = Jac[0][0] / det;

    Weight = det * _gauss.GetGaussWeightsPointer()[ig];

  }

  template <class type>
  void elem_type_3D::GetJacobianMatrix_type(const vector < vector < type > >& vt, const unsigned& ig, type& Weight,
                                            vector < vector < type > >& Jac, vector < vector < type > >& JacI) const {

    Jac = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    JacI.resize(3, std::vector<type>(3));



    const double* dxi = _dphidxi[ig];
    const double* deta = _dphideta[ig];
    const double* dzeta = _dphidzeta[ig];

    for(int inode = 0; inode < _nc; inode++, dxi++, deta++, dzeta++) {
      Jac[0][0] += (*dxi) * vt[0][inode];
      Jac[1][0] += (*dxi) * vt[1][inode];
      Jac[2][0] += (*dxi) * vt[2][inode];
      Jac[0][1] += (*deta) * vt[0][inode];
      Jac[1][1] += (*deta) * vt[1][inode];
      Jac[2][1] += (*deta) * vt[2][inode];
      Jac[0][2] += (*dzeta) * vt[0][inode];
      Jac[1][2] += (*dzeta) * vt[1][inode];
      Jac[2][2] += (*dzeta) * vt[2][inode];
    }

    type det = (Jac[0][0] * (Jac[1][1] * Jac[2][2] - Jac[1][2] * Jac[2][1]) +
                Jac[0][1] * (Jac[1][2] * Jac[2][0] - Jac[1][0] * Jac[2][2]) +
                Jac[0][2] * (Jac[1][0] * Jac[2][1] - Jac[1][1] * Jac[2][0]));

    JacI[0][0] = (-Jac[1][2] * Jac[2][1] + Jac[1][1] * Jac[2][2]) / det;
    JacI[0][1] = (Jac[0][2] * Jac[2][1] - Jac[0][1] * Jac[2][2]) / det;
    JacI[0][2] = (-Jac[0][2] * Jac[1][1] + Jac[0][1] * Jac[1][2]) / det;
    JacI[1][0] = (Jac[1][2] * Jac[2][0] - Jac[1][0] * Jac[2][2]) / det;
    JacI[1][1] = (-Jac[0][2] * Jac[2][0] + Jac[0][0] * Jac[2][2]) / det;
    JacI[1][2] = (Jac[0][2] * Jac[1][0] - Jac[0][0] * Jac[1][2]) / det;
    JacI[2][0] = (-Jac[1][1] * Jac[2][0] + Jac[1][0] * Jac[2][1]) / det;
    JacI[2][1] = (Jac[0][1] * Jac[2][0] - Jac[0][0] * Jac[2][1]) / det;
    JacI[2][2] = (-Jac[0][1] * Jac[1][0] + Jac[0][0] * Jac[1][1]) / det;

    Weight = det * _gauss.GetGaussWeightsPointer()[ig];

  }


} //end namespace femus













