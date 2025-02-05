CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(femus)

#############################################################################################
### Source files
#############################################################################################

SET(femus_src
algebra/DenseMatrixBase.cpp
algebra/DenseMatrix.cpp
algebra/DenseSubmatrix.cpp
algebra/DenseVectorBase.cpp
algebra/FieldSplitTree.cpp
algebra/Graph.cpp
algebra/LinearEquation.cpp
algebra/LinearEquationSolver.cpp
algebra/NumericVector.cpp
algebra/LinearEquationSolverPetsc.cpp
algebra/LinearEquationSolverPetscAsm.cpp
algebra/LinearEquationSolverPetscFieldSplit.cpp
algebra/PetscMatrix.cpp
algebra/PetscPreconditioner.cpp
algebra/PetscVector.cpp
algebra/Preconditioner.cpp
algebra/SparseMatrix.cpp
algebra/FunctionBase.cpp
algebra/ParsedFunction.cpp
algebra/SlepcSVD.cpp
equations/assemble/Quantity.cpp
equations/assemble/DofMap.cpp
equations/assemble/Assemble_jacobian.cpp
equations/assemble/Assemble_unknown.cpp
equations/assemble/BoundaryConditions.cpp
equations/assemble/CurrentElem.cpp
equations/assemble/CurrentQuantity.cpp
equations/assemble/SystemTwo.cpp
equations/ExplicitSystem.cpp
equations/ImplicitSystem.cpp
equations/LinearImplicitSystem.cpp
equations/MonolithicFSINonLinearImplicitSystem.cpp
equations/NonLinearImplicitSystem.cpp
equations/NonLinearImplicitSystemWithPrimalDualActiveSetMethod.cpp
equations/MultiLevelProblem.cpp
equations/System.cpp
equations/TimeLoop.cpp
equations/TransientSystem.cpp
fe/ElemType.cpp
fe/ElemType1.cpp
fe/ElemType_template.cpp
fe/ElemType_template_base.cpp
fe/Hexaedron.cpp
fe/Line.cpp
fe/Quadrilateral.cpp
fe/Tetrahedral.cpp
fe/Triangle.cpp
fe/Wedge.cpp
ism/Marker.cpp
ism/PolynomialBases.cpp
ism/Line.cpp
mesh/gencase/Box.cpp
mesh/gencase/Domain.cpp
mesh/gencase/ElemSto.cpp
mesh/gencase/MultiLevelMeshTwo.cpp
mesh/gencase/GenCase.cpp
mesh/geom_elements/GeomElemBase.cpp
mesh/geom_elements/GeomElemHex8.cpp
mesh/geom_elements/GeomElemHex27.cpp
mesh/geom_elements/GeomElemQuad4.cpp
mesh/geom_elements/GeomElemQuad9.cpp
mesh/geom_elements/GeomElemTet4.cpp
mesh/geom_elements/GeomElemTet10.cpp
mesh/geom_elements/GeomElemTri3.cpp
mesh/geom_elements/GeomElemTri6.cpp
mesh/geom_elements/GeomElemTri7.cpp
mesh/geom_elements/GeomElemEdge2.cpp
mesh/geom_elements/GeomElemEdge3.cpp
mesh/Elem.cpp
mesh/Mesh.cpp
mesh/MultiLevelMesh.cpp
mesh/obj_io.cpp
mesh/input/MeshGeneration.cpp
mesh/input/GambitIO.cpp
mesh/input/MED_IO.cpp
mesh/refinement/MeshRefinement.cpp
mesh/partitioning/MeshPartitioning.cpp
mesh/partitioning/MeshMetisPartitioning.cpp
mesh/partitioning/MeshASMPartitioning.cpp
parallel/MyMatrix.cpp
parallel/MyVector.cpp
physics/Fluid.cpp
physics/Material.cpp
physics/Parameter.cpp
physics/Solid.cpp
quadrature/GaussPoints.cpp
quadrature/PointGaussPoints.cpp
quadrature/HexGaussLegendrePoints.cpp
quadrature/WedgeGaussLegendrePoints.cpp
quadrature/TetGaussLegendrePoints.cpp
quadrature/QuadGaussLegendrePoints.cpp
quadrature/TriGaussLegendrePoints.cpp
quadrature/LineGaussLegendrePoints.cpp
quadrature/HexGaussLobattoPoints.cpp
quadrature/WedgeGaussLobattoPoints.cpp
quadrature/TetGaussLobattoPoints.cpp
quadrature/QuadGaussLobattoPoints.cpp
quadrature/TriGaussLobattoPoints.cpp
quadrature/LineGaussLobattoPoints.cpp
solution/MultiLevelSolution.cpp
solution/Solution.cpp
solution/output/Writer.cpp
solution/output/VTKWriter.cpp
solution/output/GMVWriter.cpp
solution/output/XDMFWriter.cpp
utils/FemusInit.cpp
utils/Files.cpp
utils/input_parser/InputParser.cpp
utils/input_parser/JsonInputParser.cpp
uq/uq.cpp
utils/eigen/MyEigenFunctions.cpp
uq/sparseGrid.cpp
)

IF (NOT LIBRARY_OUTPUT_PATH)
  SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib64 CACHE PATH
       "Single output directory for building all libraries.")
  MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH)
ENDIF (NOT LIBRARY_OUTPUT_PATH)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${femus_src})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${HDF5_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Eigen3::Eigen
    ${HDF5_LIBRARIES}
)

# This is a cheap hack to fix linker errors during compilation
IF(APPLE)
  target_link_libraries(${PROJECT_NAME} ${HDF5_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} ${PETSC_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} ${B64_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} ${METIS_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} ${ADEPT_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} ${SLEPC_LIB})
  target_link_libraries(${PROJECT_NAME} ${MPI_LIBRARY})
  target_include_directories(${PROJECT_NAME} PUBLIC ${SLEPC_INC})
  target_include_directories(${PROJECT_NAME} PUBLIC ${SLEPC_DIR})
  target_include_directories(${PROJECT_NAME} PUBLIC ${HDF5_INCLUDE_DIRS})
ENDIF(APPLE)
