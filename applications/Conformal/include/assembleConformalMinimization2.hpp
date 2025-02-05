
// Building the Conformal Minimization system.
void AssembleConformalMinimization(MultiLevelProblem& ml_prob) {
  //  ml_prob is the global object from/to where get/set all the data
  //  level is the level of the PDE system to be assembled

  //  Extract pointers to the several objects that we are going to use.
  NonLinearImplicitSystem* mlPdeSys   = &ml_prob.get_system< NonLinearImplicitSystem> ("conformal");   // pointer to the linear implicit system named "Poisson"

  const unsigned level = mlPdeSys->GetLevelToAssemble();

  // Pointers to the mesh (level) object and elem object in mesh (level).
  Mesh *msh = ml_prob._ml_msh->GetLevel(level);
  elem *el = msh->el;

  // Pointers to the multilevel solution, solution (level) and equation (level).
  MultiLevelSolution *mlSol = ml_prob._ml_sol;

  if(counter > 0 && !stopIterate) {
    UpdateMu(*mlSol);
  }

  Solution *sol = ml_prob._ml_sol->GetSolutionLevel(level);
  LinearEquationSolver *pdeSys = mlPdeSys->_LinSolver[level];

  // Pointers to global stiffness matrix and residual vector in pdeSys (level).
  SparseMatrix *KK = pdeSys->_KK;
  NumericVector *RES = pdeSys->_RES;

  // Convenience variables to keep track of the dimension.
  const unsigned  dim = 2;
  const unsigned  DIM = 3;

  // Get the process_id (for parallel computation).
  unsigned iproc = msh->processor_id();

  // Setting the reference elements to be equilateral triangles.
  std::vector < std::vector < double > > xT(2);
  xT[0].resize(3);
  xT[0][0] = -0.5;
  xT[0][1] = 0.5;
  xT[0][2] = 0.;

  xT[1].resize(3);
  xT[1][0] = 0.;
  xT[1][1] = 0.;
  xT[1][2] = sqrt(3.) / 2.;

  std::vector<double> phi_uv0;
  std::vector<double> phi_uv1;

  std::vector< double > stdVectorPhi;
  std::vector< double > stdVectorPhi_uv;

//   unsigned solENVNIndex = mlSol->GetIndex("ENVN");
//   unsigned solENVNType = mlSol->GetSolutionType(solENVNIndex);

  // Extract positions of Dx in ml_sol object.
//   unsigned solDxIndex[DIM];
//   solDxIndex[0] = mlSol->GetIndex("Dx1");
//   solDxIndex[1] = mlSol->GetIndex("Dx2");
//   solDxIndex[2] = mlSol->GetIndex("Dx3");

  // Extract finite element type for the solution.


  // Local solution vectors for X, Dx, Xhat, XC.

  std::vector < double > xhat[DIM];
  std::vector < double > solDx[DIM];
  std::vector < double > solDxOld[DIM];

  // Get the finite element type for "x", it is always 2 (LAGRANGE QUADRATIC).
  unsigned xType = 2;

  // Get the poitions of Y in the ml_sol object.
  unsigned solDxIndex[DIM];
  solDxIndex[0] = mlSol->GetIndex("Dx1");
  solDxIndex[1] = mlSol->GetIndex("Dx2");
  solDxIndex[2] = mlSol->GetIndex("Dx3");

  unsigned solType;
  solType = mlSol->GetSolutionType(solDxIndex[0]);

  // Get the positions of Y in the pdeSys object.
  unsigned solDxPdeIndex[DIM];
  solDxPdeIndex[0] = mlPdeSys->GetSolPdeIndex("Dx1");
  solDxPdeIndex[1] = mlPdeSys->GetSolPdeIndex("Dx2");
  solDxPdeIndex[2] = mlPdeSys->GetSolPdeIndex("Dx3");

  // Local solution vectors for Nx and NDx.


  // Get the position of "Lambda1" in the ml_sol object.
  unsigned solLIndex;
  solLIndex = mlSol->GetIndex("Lambda1");

  // Get the finite element type for "Lambda1".
  unsigned solLType;
  solLType = mlSol->GetSolutionType(solLIndex);

  // Get the position of "Lambda1" in the pdeSys object.
  unsigned solLPdeIndex;
  solLPdeIndex = mlPdeSys->GetSolPdeIndex("Lambda1");

  // Local Lambda1 solution.
  std::vector < double > solL;

  std::vector < unsigned > solMuIndex(dim);
  solMuIndex[0] = mlSol->GetIndex("mu1");
  solMuIndex[1] = mlSol->GetIndex("mu2");
  unsigned solTypeMu = mlSol->GetSolutionType(solMuIndex[0]);

  std::vector < std::vector < double > > solMu(dim);



  // Local-to-global pdeSys dofs.
  std::vector < int > SYSDOF;
  // Local residual vectors.
  vector< double > Res;
  // Local Jacobian matrix (ordered by column).
  vector < double > Jac;

  KK->zero();  // Zero all the entries of the Global Matrix
  RES->zero(); // Zero all the entries of the Global Residual

  // ELEMENT LOOP: each process loops only on the elements that it owns.
  for(int iel = msh->_elementOffset[iproc]; iel < msh->_elementOffset[iproc + 1]; iel++) {

    // Numer of solution element dofs.
    short unsigned ielGeom = msh->GetElementType(iel);
    unsigned nxDofs  = msh->GetElementDofNumber(iel, solType);
    unsigned nLDofs  = msh->GetElementDofNumber(iel, solLType);

    // Resize local arrays.
    for(unsigned K = 0; K < DIM; K++) {
      xhat[K].resize(nxDofs);
      solDxOld[K].resize(nxDofs);
      solDx[K].resize(nxDofs);
      solL.resize(nLDofs);
    }

    // Resize local arrays
    unsigned sizeAll = DIM * nxDofs + nLDofs;

    SYSDOF.resize(sizeAll);
    Res.assign(sizeAll, 0.);
    Jac.assign(sizeAll * sizeAll, 0.);

    // local storage of global mapping and solution
    for(unsigned i = 0; i < nxDofs; i++) {

      // Global-to-local mapping between X solution node and solution dof.
      unsigned iDDof = msh->GetSolutionDof(i, iel, solType);
      unsigned iXDof  = msh->GetSolutionDof(i, iel, xType);

      for(unsigned K = 0; K < DIM; K++) {
        xhat[K][i] = (*msh->_topology->_Sol[K])(iXDof);

        solDxOld[K][i] = (*sol->_SolOld[solDxIndex[K]])(iDDof);
        solDx[K][i] = (*sol->_Sol[solDxIndex[K]])(iDDof);

        // Global-to-global mapping between NDx solution node and pdeSys dof.
        SYSDOF[ K * nxDofs + i] =
          pdeSys->GetSystemDof(solDxIndex[K], solDxPdeIndex[K], i, iel);
      }
    }

    // Local storage of global mapping and solution.
    for(unsigned i = 0; i < nLDofs; i++) {

      // Global-to-local mapping between Lambda solution node and solution dof.
      unsigned iLDof = msh->GetSolutionDof(i, iel, solLType);
      solL[i] = (*sol->_Sol[solLIndex])(iLDof);

      // Global-to-global mapping between Lambda solution node and pdeSys dof.
      SYSDOF[DIM * nxDofs + i] =
        pdeSys->GetSystemDof(solLIndex, solLPdeIndex, i, iel);
    }


    unsigned nDofsMu  = msh->GetElementDofNumber(iel, solTypeMu);
    for(unsigned k = 0; k < dim; k++) {
      solMu[k].resize(nDofsMu);
    }

    for(unsigned i = 0; i < nDofsMu; i++) {
      unsigned iDof = msh->GetSolutionDof(i, iel, solTypeMu);
      for(unsigned k = 0; k < dim; k++) {
        solMu[k][i] = (*sol->_Sol[solMuIndex[k]])(iDof);
      }
    }

    if(ielGeom == TRI) {

//       xT[0][1] = 0.5;
//       std::vector < unsigned > ENVN(3);
//       std::vector < double > angle(3);
//
//       for(unsigned j = 0; j < 3; j++) {
//         unsigned jnode  = msh->GetSolutionDof(j, iel, solENVNType);
//         ENVN[j] = (*sol->_Sol[solENVNIndex])(jnode);
//         angle[j] = 2 * M_PI / ENVN[j];
//       }
//
//
//       if(conformalTriangleType == 1) {  //this works with moo two levels
//         ChangeTriangleConfiguration1(ENVN, angle);
//       }
//       else if(conformalTriangleType == 2) {  //this works with mao
//         ChangeTriangleConfiguration2(ENVN, angle);
//       }
//       else { //no change
//         angle.assign(3, M_PI / 3.);
//       }
//
//       double l = xT[0][1] - xT[0][0];
//       double d = l * sin(angle[0]) * sin(angle[1]) / sin(angle[0] + angle[1]);
//       double scale = sqrt((sqrt(3.) / 2.) / (l * d));
//       l = l * scale;
//       d = d * scale;
//       xT[0][1] = xT[0][0] + l;
//       xT[0][2] = xT[0][0] + d / tan(angle[0]);
//       xT[1][2] = d;

      //std::cout << l << " " << d<<" "<< angle[0] << " " << angle[1] <<" "<< angle[2] << " " << l * d <<" "<< xT[0][2]<< " " << xT[1][2]<<  std::endl;
    }


    // *** Gauss point loop ***
    for(unsigned ig = 0; ig < msh->_finiteElement[ielGeom][solType]->GetGaussPointNumber(); ig++) {

      const double *phix;  // local test function
      const double *phiL;  // local test function
      const double *phix_uv[dim]; // local test function first order partial derivatives

      double weight; // gauss point weight

      // Get Gauss point weight, test function, and first order derivatives.
      if(ielGeom == QUAD) {
        phix = msh->_finiteElement[ielGeom][solType]->GetPhi(ig);
        phiL = msh->_finiteElement[ielGeom][solLType]->GetPhi(ig);

        phix_uv[0] = msh->_finiteElement[ielGeom][solType]->GetDPhiDXi(ig);
        phix_uv[1] = msh->_finiteElement[ielGeom][solType]->GetDPhiDEta(ig);

        weight = msh->_finiteElement[ielGeom][solType]->GetGaussWeight(ig);
      }

      // Special adjustments for triangles.
      else {
        msh->_finiteElement[ielGeom][solType]->Jacobian(xT, ig, weight, stdVectorPhi, stdVectorPhi_uv);

        phix = &stdVectorPhi[0];
        phiL = msh->_finiteElement[ielGeom][solLType]->GetPhi(ig);

        phi_uv0.resize(nxDofs);
        phi_uv1.resize(nxDofs);


        for(unsigned i = 0; i < nxDofs; i++) {
          phi_uv0[i] = stdVectorPhi_uv[i * dim];
          phi_uv1[i] = stdVectorPhi_uv[i * dim + 1];
        }

        phix_uv[0] = &phi_uv0[0];
        phix_uv[1] = &phi_uv1[0];

      }

      // Initialize and compute values of x, Dx, NDx, x_uv at the Gauss points.
      double solDxOldg[3] = {0., 0., 0.};
      double solDxg[3] = {0., 0., 0.};

      double xhat_uv[3][2] = {{0., 0.}, {0., 0.}, {0., 0.}};
      double solMx_uv[3][2] = {{0., 0.}, {0., 0.}, {0., 0.}};
      double solNx_uv[3][2] = {{0., 0.}, {0., 0.}, {0., 0.}};

      for(unsigned K = 0; K < DIM; K++) {
        for(unsigned i = 0; i < nxDofs; i++) {
          solDxOldg[K] += phix[i] * solDxOld[K][i];
          solDxg[K] += phix[i] * solDx[K][i];
        }
        for(int j = 0; j < dim; j++) {
          for(unsigned i = 0; i < nxDofs; i++) {
            xhat_uv[K][j]    += phix_uv[j][i] * (xhat[K][i] + solDxOld[K][i]);
            solMx_uv[K][j]   += phix_uv[j][i] * (xhat[K][i] + 0.5 * ((1. + !O2conformal) * solDxOld[K][i] + O2conformal * solDx[K][i]));
            solNx_uv[K][j]   += phix_uv[j][i] * (xhat[K][i] + solDx[K][i]);
          }
        }
      }

      ///////// ADDED THIS /////////
      double solLg = 0.;
      for(unsigned i = 0; i < nLDofs; i++) {
        solLg += phiL[i] * solL[i];
      }

      // Compute the metric, metric determinant, and area element.
      double g[dim][dim] = {{0., 0.}, {0., 0.}};
      for(unsigned i = 0; i < dim; i++) {
        for(unsigned j = 0; j < dim; j++) {
          for(unsigned K = 0; K < DIM; K++) {
            g[i][j] += xhat_uv[K][i] * xhat_uv[K][j];
          }
        }
      }
      double detg = g[0][0] * g[1][1] - g[0][1] * g[1][0];
      double Area = weight * sqrt(detg);
      double Area2 = weight; // Trick to give equal weight to each element.

      // Compute the metric inverse.
      double gi[dim][dim];
      gi[0][0] =  g[1][1] / detg;
      gi[0][1] = -g[0][1] / detg;
      gi[1][0] = -g[1][0] / detg;
      gi[1][1] =  g[0][0] / detg;

      // Compute components of the unit normal N.
      double normal[3];
      normal[0] = (xhat_uv[1][0] * xhat_uv[2][1] - xhat_uv[2][0] * xhat_uv[1][1]) / sqrt(detg);
      normal[1] = (xhat_uv[2][0] * xhat_uv[0][1] - xhat_uv[0][0] * xhat_uv[2][1]) / sqrt(detg);
      normal[2] = (xhat_uv[0][0] * xhat_uv[1][1] - xhat_uv[1][0] * xhat_uv[0][1]) / sqrt(detg);

      double normalMSqrtDetg[DIM];
      normalMSqrtDetg[0] = (solMx_uv[1][0] * solMx_uv[2][1] - solMx_uv[2][0] * solMx_uv[1][1]);
      normalMSqrtDetg[1] = (solMx_uv[2][0] * solMx_uv[0][1] - solMx_uv[0][0] * solMx_uv[2][1]);
      normalMSqrtDetg[2] = (solMx_uv[0][0] * solMx_uv[1][1] - solMx_uv[1][0] * solMx_uv[0][1]);


      double normalN[DIM];
      normalN[0] = (solNx_uv[1][0] * solNx_uv[2][1] - solNx_uv[2][0] * solNx_uv[1][1]);
      normalN[1] = (solNx_uv[2][0] * solNx_uv[0][1] - solNx_uv[0][0] * solNx_uv[2][1]);
      normalN[2] = (solNx_uv[0][0] * solNx_uv[1][1] - solNx_uv[1][0] * solNx_uv[0][1]);

      double normN = sqrt(normalN[0]*normalN[0] + normalN[1]*normalN[1] +normalN[2]*normalN[2]);
      normalN[0] /= normN;
      normalN[1] /= normN;
      normalN[2] /= normN;

      double mu[2] = {0., 0.};
      const double *phiMu = msh->_finiteElement[ielGeom][solTypeMu]->GetPhi(ig);  // local test function
      for(unsigned i = 0; i < nDofsMu; i++) {
        for(unsigned k = 0; k < 2; k++) {
          mu[k] += phiMu[i] * solMu[k][i];
        }
      }

      boost::math::quaternion <double> N(0, normal[0], normal[1], normal[2]);
      boost::math::quaternion <double> e1(0., 1., 0., 0.);
      boost::math::quaternion <double> e2(0., 0., 1., 0.);
      boost::math::quaternion <double> e3(0., 0., 0., 1.);

      boost::math::quaternion <double> MU(mu[0], mu[1] * normal[0], mu[1] * normal[1], mu[1] * normal[2]);
      boost::math::quaternion <double> cMU(mu[0], -mu[1] * normal[0], -mu[1] * normal[1], -mu[1] * normal[2]);

      boost::math::quaternion <double> a[6];
      a[0] = e1 - MU * e1;
      a[2] = e2 - MU * e2;
      a[4] = e3 - MU * e3;

      a[1] = N * e1 + MU * N * e1;
      a[3] = N * e2 + MU * N * e2;
      a[5] = N * e3 + MU * N * e3;

      boost::math::quaternion <double> b[6];
      b[0] = -N * e1 + MU * N * e1;
      b[2] = -N * e2 + MU * N * e2;
      b[4] = -N * e3 + MU * N * e3;

      b[1] = e1 + MU * e1;
      b[3] = e2 + MU * e2;
      b[5] = e3 + MU * e3;


      double D[6][6];

      for(unsigned i = 0; i < 6; i++) {
        for(unsigned j = 0; j < 6; j++) {
//           D[i][j] = gi[0][0] * (a[i] * conj(a[j])).real() + gi[0][1] * (a[i] * conj(b[j])).real() +
//                     gi[1][0] * (b[i] * conj(a[j])).real() + gi[1][1] * (b[i] * conj(b[j])).real();

//           D[i][j] = gi[0][0] * (a[i] * ca[j]).real() + gi[0][1] * (a[i] * cb[j]).real() +
//                     gi[1][0] * (b[i] * ca[j]).real() + gi[1][1] * (b[i] * cb[j]).real();


         D[i][j] = gi[0][0] * (a[i] % a[j]) + gi[0][1] * (a[i] % b[j]) +
                   gi[1][0] * (b[i] % a[j]) + gi[1][1] * (b[i] % b[j]);
        }
      }

      for(unsigned I = 0; I < DIM; I++) {
        for(unsigned i = 0; i < nxDofs; i++) {
          unsigned irow = I * nxDofs + i;
          unsigned istart = irow * sizeAll;
          for(unsigned J = 0; J < DIM; J++) {
            for(unsigned j = 0; j < nxDofs; j++) {
              double term = 0.;
              for(unsigned k = 0; k < dim; k++) {
                for(unsigned l = 0; l < dim; l++) {
                  term += phix_uv[k][i] * D[I * dim + k][J * dim + l] * phix_uv[l][j];
                }
              }
              if(quaternion) {
                Jac[istart + J * nxDofs + j] += term * Area;
                Res[I * nxDofs + i] -= term * Area * (xhat[J][j] + solDx[J][j]);
              }
            }
          }
        }
      }















      // Compute new X minus old X dot N, for "reparametrization".
      double DnXmDxdotNSqrtDetg = 0.;
      for(unsigned K = 0; K < DIM; K++) {
        DnXmDxdotNSqrtDetg += (solDxOldg[K] - solDxg[K]) * normalMSqrtDetg[K];
      }

      // Implement the Conformal Minimization equations.
      for(unsigned K = 0; K < DIM; K++) {
        for(unsigned i = 0; i < nxDofs; i++) {

          //Residual (Conformal Minimization + Lagrange Multiplier)
          double term = solLg * phix[i] * normalMSqrtDetg[K] * Area2;
          Res[K * nxDofs + i] -= term;

          unsigned irow = K * nxDofs + i;
          unsigned istart = irow * sizeAll;
          // Jacobian (part1 Lagrange Multiplier: delta NDX * normal)
          for(unsigned j = 0; j < nLDofs; j++) {
            Jac[istart + DIM * nxDofs + j] += phiL[j] * phix[i] * normalMSqrtDetg[K] * Area2;
          }
        }
      }



//       double mu[2] = {0., 0.};
//       const double *phiMu = msh->_finiteElement[ielGeom][solTypeMu]->GetPhi(ig);  // local test function
//       for(unsigned i = 0; i < nDofsMu; i++) {
//         for(unsigned k = 0; k < 2; k++) {
//           mu[k] += phiMu[i] * solMu[k][i];
//         }
//       }

      double Jac0[DIM][DIM * dim];

      Jac0[0][0] =  1. - mu[0];
      Jac0[0][1] = -mu[1];
      Jac0[0][2] =  normal[2] * mu[1];
      Jac0[0][3] = -normal[2] * (1. + mu[0]);
      Jac0[0][4] = -normal[1] * mu[1];
      Jac0[0][5] =  normal[1] * (1. + mu[0]);

      Jac0[1][0] = -Jac0[0][2];
      Jac0[1][1] = -Jac0[0][3];
      Jac0[1][2] =  Jac0[0][0];
      Jac0[1][3] =  Jac0[0][1];
      Jac0[1][4] =  normal[0] * mu[1];
      Jac0[1][5] = -normal[0] * (1. + mu[0]);

      Jac0[2][0] = -Jac0[0][4];
      Jac0[2][1] = -Jac0[0][5];
      Jac0[2][2] = -Jac0[1][4];
      Jac0[2][3] = -Jac0[1][5];
      Jac0[2][4] =  Jac0[0][0];
      Jac0[2][5] =  Jac0[0][1];

      double Jac1[DIM][DIM * dim];

      Jac1[0][0] = -mu[1];
      Jac1[0][1] =  1. + mu[0];
      Jac1[0][2] =  normal[2] * (1. - mu[0]);
      Jac1[0][3] = -normal[2] * mu[1];
      Jac1[0][4] = -normal[1] * (1. - mu[0]);
      Jac1[0][5] =  normal[1] * mu[1];

      Jac1[1][0] = -Jac1[0][2];
      Jac1[1][1] = -Jac1[0][3];
      Jac1[1][2] =  Jac1[0][0];
      Jac1[1][3] =  Jac1[0][1];
      Jac1[1][4] =  normal[0] * (1. - mu[0]);
      Jac1[1][5] = -normal[0] * mu[1];

      Jac1[2][0] = -Jac1[0][4];
      Jac1[2][1] = -Jac1[0][5];
      Jac1[2][2] = -Jac1[1][4];
      Jac1[2][3] = -Jac1[1][5];
      Jac1[2][4] =  Jac1[0][0];
      Jac1[2][5] =  Jac1[0][1];

      double AIJ[DIM * dim][DIM * dim];
      for(unsigned I = 0; I < DIM * dim; I++) {
        for(unsigned J = 0; J < DIM * dim; J++) {
          AIJ[I][J] = 0.;
          for(unsigned K = 0; K < DIM; K++) {

            AIJ[I][J] += (gi[0][0] * Jac0[K][I] * Jac0[K][J] +
                          gi[1][0] * Jac1[K][I] * Jac0[K][J] +
                          gi[0][1] * Jac0[K][I] * Jac1[K][J] +
                          gi[1][1] * Jac1[K][I] * Jac1[K][J]);

          }
        }
      }

      for(unsigned j = 0; j < nxDofs; j++) {
        double DQ[DIM][DIM][dim];

        DQ[0][0][0] = AIJ[0][0] * phix_uv[0][j] + AIJ[0][1] * phix_uv[1][j];
        DQ[0][0][1] = AIJ[1][0] * phix_uv[0][j] + AIJ[1][1] * phix_uv[1][j];

        DQ[0][1][0] = AIJ[0][2] * phix_uv[0][j] + AIJ[0][3] * phix_uv[1][j];
        DQ[0][1][1] = AIJ[1][2] * phix_uv[0][j] + AIJ[1][3] * phix_uv[1][j];

        DQ[0][2][0] = AIJ[0][4] * phix_uv[0][j] + AIJ[0][5] * phix_uv[1][j];
        DQ[0][2][1] = AIJ[1][4] * phix_uv[0][j] + AIJ[1][5] * phix_uv[1][j];


        DQ[1][0][0] = AIJ[2][0] * phix_uv[0][j] + AIJ[2][1] * phix_uv[1][j];
        DQ[1][0][1] = AIJ[3][0] * phix_uv[0][j] + AIJ[3][1] * phix_uv[1][j];

        DQ[1][1][0] = AIJ[2][2] * phix_uv[0][j] + AIJ[2][3] * phix_uv[1][j];
        DQ[1][1][1] = AIJ[3][2] * phix_uv[0][j] + AIJ[3][3] * phix_uv[1][j];

        DQ[1][2][0] = AIJ[2][4] * phix_uv[0][j] + AIJ[2][5] * phix_uv[1][j];
        DQ[1][2][1] = AIJ[3][4] * phix_uv[0][j] + AIJ[3][5] * phix_uv[1][j];


        DQ[2][0][0] = AIJ[4][0] * phix_uv[0][j] + AIJ[4][1] * phix_uv[1][j];
        DQ[2][0][1] = AIJ[5][0] * phix_uv[0][j] + AIJ[5][1] * phix_uv[1][j];

        DQ[2][1][0] = AIJ[4][2] * phix_uv[0][j] + AIJ[4][3] * phix_uv[1][j];
        DQ[2][1][1] = AIJ[5][2] * phix_uv[0][j] + AIJ[5][3] * phix_uv[1][j];

        DQ[2][2][0] = AIJ[4][4] * phix_uv[0][j] + AIJ[4][5] * phix_uv[1][j];
        DQ[2][2][1] = AIJ[5][4] * phix_uv[0][j] + AIJ[5][5] * phix_uv[1][j];

        double DnormalMSqrtDetg[DIM][DIM];

        DnormalMSqrtDetg[0][0] =  0.;
        DnormalMSqrtDetg[0][1] =  0.5 * (solMx_uv[2][1] * phix_uv[0][j] - solMx_uv[2][0] * phix_uv[1][j]);
        DnormalMSqrtDetg[0][2] = -0.5 * (solMx_uv[1][1] * phix_uv[0][j] - solMx_uv[1][0] * phix_uv[1][j]);

        DnormalMSqrtDetg[1][0] = -0.5 * (solMx_uv[2][1] * phix_uv[0][j] - solMx_uv[2][0] * phix_uv[1][j]);
        DnormalMSqrtDetg[1][1] =  0.;
        DnormalMSqrtDetg[1][2] =  0.5 * (solMx_uv[0][1] * phix_uv[0][j] - solMx_uv[0][0] * phix_uv[1][j]);

        DnormalMSqrtDetg[2][0] =  0.5 * (solMx_uv[1][1] * phix_uv[0][j] - solMx_uv[1][0] * phix_uv[1][j]);
        DnormalMSqrtDetg[2][1] = -0.5 * (solMx_uv[0][1] * phix_uv[0][j] - solMx_uv[0][0] * phix_uv[1][j]);
        DnormalMSqrtDetg[2][2] =  0.;

        // Jacobian (Conformal Minimization + part2 Lagrange Multiplier: NDX * delta normal)
        for(unsigned i = 0; i < nxDofs; i++) {
          for(unsigned K = 0; K < DIM; K++) {
            unsigned irow = K * nxDofs + i;
            unsigned istart = irow * sizeAll;
            for(unsigned J = 0; J < DIM; J++) {
              double term = 0.;
              for(unsigned k = 0; k < dim; k++) {
                term += DQ[K][J][k] * phix_uv[k][i];
              }
              if(!quaternion){
                Jac[istart + J * nxDofs + j] += term * Area + (1 - noLM) * solLg * phix[i] * DnormalMSqrtDetg[K][J] * Area2;
                Res[K * nxDofs + i] -= term * Area * (xhat[J][j] + solDx[J][j]);
              }
            }
          }
        }

        // Jacobian (constraint: -\delta NDx * normal + (DX - NDx) * delta normal)
        for(unsigned i = 0; i < nLDofs; i++) {
          unsigned irow = DIM * nxDofs + i;
          unsigned istart = irow * sizeAll;
          for(unsigned K = 0; K < DIM; K++) {
            double term = 0.;
            for(unsigned J = 0; J < DIM; J++) {
              term += (solDxOldg[J] - solDxg[J]) * DnormalMSqrtDetg[J][K];
            }
            Jac[istart + K * nxDofs + j] += (1 - noLM) * phiL[i] * (term - phix[j] * normalMSqrtDetg[K]) * Area2;
          }
        }
      }

      // Residual (constraint + eps) and Jacobian (eps)
      for(unsigned i = 0; i < nLDofs; i++) {
        unsigned irow = DIM * nxDofs + i;
        double term = phiL[i] * (DnXmDxdotNSqrtDetg * Area2 + eps * solLg * Area);
        Res[irow] -= (1 - noLM) * term;
        unsigned istart = irow * sizeAll;
        for(unsigned j = 0; j < nLDofs; j++) {
          Jac[istart + DIM * nxDofs + j] += eps * phiL[i] * phiL[j] *  Area;
        }
      }

    } // end GAUSS POINT LOOP

    //------------------------------------------------------------------------
    // Add the local Matrix/Vector into the global Matrix/Vector
    //copy the value of the adept::adoube aRes in double Res and store

    RES->add_vector_blocked(Res, SYSDOF);
    KK->add_matrix_blocked(Jac, SYSDOF, SYSDOF);


  } //end ELEMENT LOOP for each process.

  RES->close();
  KK->close();

  counter++;

//     //VecView ( (static_cast<PetscVector*> (RES))->vec(),  PETSC_VIEWER_STDOUT_SELF);
//     MatView ( (static_cast<PetscMatrix*> (KK))->mat(), PETSC_VIEWER_STDOUT_SELF);
//
//     PetscViewer    viewer;
//     PetscViewerDrawOpen (PETSC_COMM_WORLD, NULL, NULL, 0, 0, 900, 900, &viewer);
//     PetscObjectSetName ( (PetscObject) viewer, "PWilmore matrix");
//     PetscViewerPushFormat (viewer, PETSC_VIEWER_DRAW_LG);
//     MatView ( (static_cast<PetscMatrix*> (KK))->mat(), viewer);
//     double a;
//     std::cin >> a;

} // end AssembleO2ConformalMinimization.
