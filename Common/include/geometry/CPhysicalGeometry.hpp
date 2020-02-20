/*!
 * \file CPhysicalGeometry.hpp
 * \brief Headers of the physical geometry class used to read meshes from file.
 * \author F. Palacios, T. Economon
 * \version 7.0.1 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2019, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "CGeometry.hpp"
#include "meshreader/CMeshReaderFVM.hpp"
#include "../toolboxes/C2DContainer.hpp"

/*!
 * \class CPhysicalGeometry
 * \brief Class for reading a defining the primal grid which is read from the grid file in .su2 or .cgns format.
 * \author F. Palacios, T. Economon, J. Alonso
 */
class CPhysicalGeometry final : public CGeometry {

  map<unsigned long, unsigned long> Global_to_Local_Point;  /*!< \brief Global-local indexation for the points. */
  long *Local_to_Global_Point;                              /*!< \brief Local-global indexation for the points. */
  unsigned short *Local_to_Global_Marker;                   /*!< \brief Local to Global marker. */
  unsigned short *Global_to_Local_Marker;                   /*!< \brief Global to Local marker. */
  unsigned long *adj_counter;                               /*!< \brief Adjacency counter. */
  unsigned long **adjacent_elem;                            /*!< \brief Adjacency element list. */
  su2activematrix Sensitivity;                              /*!< \brief Matrix holding the sensitivities at each point. */

  vector<vector<unsigned long> > Neighbors;
  map<unsigned long, unsigned long> Color_List;
  vector<string> Marker_Tags;
  unsigned long nLocal_Point,
  nLocal_PointDomain,
  nLocal_PointGhost,
  nLocal_PointPeriodic,
  nLocal_Elem,
  nLocal_Bound_Elem,
  nGlobal_Elem,
  nGlobal_Bound_Elem,
  nLocal_Line,
  nLocal_BoundTria,
  nLocal_BoundQuad,
  nLinear_Line,
  nLinear_BoundTria,
  nLinear_BoundQuad,
  nLocal_Tria,
  nLocal_Quad,
  nLocal_Tetr,
  nLocal_Hexa,
  nLocal_Pris,
  nLocal_Pyra;
  unsigned long nMarker_Global;
  su2double *Local_Coords;
  unsigned long *Local_Points;
  unsigned long *Local_Colors;
  unsigned long *Conn_Line;
  unsigned long *Conn_BoundTria;
  unsigned long *Conn_BoundQuad;
  unsigned long *Conn_Line_Linear;
  unsigned long *Conn_BoundTria_Linear;
  unsigned long *Conn_BoundQuad_Linear;
  unsigned long *Conn_Tria;
  unsigned long *Conn_Quad;
  unsigned long *Conn_Tetr;
  unsigned long *Conn_Hexa;
  unsigned long *Conn_Pris;
  unsigned long *Conn_Pyra;
  unsigned long *ID_Line;
  unsigned long *ID_BoundTria;
  unsigned long *ID_BoundQuad;
  unsigned long *ID_Line_Linear;
  unsigned long *ID_BoundTria_Linear;
  unsigned long *ID_BoundQuad_Linear;
  unsigned long *ID_Tria;
  unsigned long *ID_Quad;
  unsigned long *ID_Tetr;
  unsigned long *ID_Hexa;
  unsigned long *ID_Pris;
  unsigned long *ID_Pyra;
  unsigned long *Elem_ID_Line;
  unsigned long *Elem_ID_BoundTria;
  unsigned long *Elem_ID_BoundQuad;
  unsigned long *Elem_ID_Line_Linear;
  unsigned long *Elem_ID_BoundTria_Linear;
  unsigned long *Elem_ID_BoundQuad_Linear;

public:
  /*--- This is to suppress Woverloaded-virtual, omitting it has no negative impact. ---*/
  using CGeometry::SetVertex;
  using CGeometry::SetMeshFile;
  using CGeometry::SetControlVolume;
  using CGeometry::SetBoundControlVolume;
  using CGeometry::SetPoint_Connectivity;

  /*!
   * \brief Constructor of the class.
   */
  CPhysicalGeometry(void);

  /*!
   * \overload
   * \brief Reads the geometry of the grid and adjust the boundary
   *        conditions with the configuration file.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_mesh_filename - Name of the file with the grid information.
   * \param[in] val_format - Format of the file with the grid information.
   * \param[in] val_iZone - Domain to be read from the grid file.
   * \param[in] val_nZone - Total number of domains in the grid file.
   */
  CPhysicalGeometry(CConfig *config, unsigned short val_iZone, unsigned short val_nZone);

  /*!
   * \overload
   * \brief Accepts a geometry container holding a linearly partitioned grid
   *        with coloring performed by ParMETIS, and this routine distributes
   *        the points and cells to all partitions based on the coloring.
   * \param[in] geometry - Definition of the geometry container holding the initial linear partitions of the grid + coloring.
   * \param[in] config - Definition of the particular problem.
   */
  CPhysicalGeometry(CGeometry *geometry, CConfig *config);

  /*!
   * \overload
   * \brief Accepts a geometry container holding a linearly partitioned grid
   *        with coloring performed by ParMETIS, and this routine distributes
   *        the points and cells to all partitions based on the coloring.
   * \param[in] geometry - Definition of the geometry container holding the initial linear partitions of the grid + coloring.
   * \param[in] config - Definition of the particular problem.
   */
  CPhysicalGeometry(CGeometry *geometry, CConfig *config, bool val_flag);

  /*!
   * \overload
   * \brief Loads the adjacency data from AMG. 
   * \param[in] PoiAdap - Vector of coordinates.
   * \param[in] EdgAdap - Vector of edg adjacency.
   * \param[in] TriAdap - Vector of tri adjacency.
   * \param[in] TetAdap - Vector of tet adjacency.
   * \param[in] val_nDim - Number of dimensions.
   * \param[in] val_iZone - Domain to be loaded.
   * \param[in] val_nZone - Total number of domains.
   */
  CPhysicalGeometry(vector<vector<passivedouble> > const &PoiAdap, vector<vector<unsigned long> > const &EdgAdap, 
                    vector<vector<unsigned long> > const &TriAdap, vector<vector<unsigned long> > const &TetAdap,
                    CConfig *config, unsigned short val_nDim, unsigned short val_nZone);

  /*!
   * \brief Destructor of the class.
   */
  ~CPhysicalGeometry(void);

  /*!
   * \brief Distributes the coloring from ParMETIS so that each rank has complete information about the local grid points.
   * \param[in] geometry - Definition of the geometry container holding the initial linear partitions of the grid + coloring.
   * \param[in] config - Definition of the particular problem.
   */
  void DistributeColoring(CConfig *config, CGeometry *geometry);

  /*!
   * \brief Distribute the grid points, including ghost points, across all ranks based on a ParMETIS coloring.
   * \param[in] config - Definition of the particular problem.
   * \param[in] geometry - Geometrical definition of the problem.
   */
  void DistributePoints(CConfig *config, CGeometry *geometry);

  /*!
   * \brief Distribute the connectivity for a single volume element type across all ranks based on a ParMETIS coloring.
   * \param[in] config - Definition of the particular problem.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] Elem_Type - VTK index of the element type being distributed.
   */
  void DistributeVolumeConnectivity(CConfig *config, CGeometry *geometry, unsigned short Elem_Type);

  /*!
   * \brief Distribute the connectivity for a single surface element type in all markers across all ranks based on a ParMETIS coloring.
   * \param[in] config - Definition of the particular problem.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] Elem_Type - VTK index of the element type being distributed.
   */
  void DistributeSurfaceConnectivity(CConfig *config, CGeometry *geometry, unsigned short Elem_Type);

  /*!
   * \brief Broadcast the marker tags for all boundaries from the master rank to all other ranks.
   * \param[in] config - Definition of the particular problem.
   * \param[in] geometry - Geometrical definition of the problem.
   */
  void DistributeMarkerTags(CConfig *config, CGeometry *geometry);

  /*!
   * \brief Partition the marker connectivity held on the master rank according to a linear partitioning.
   * \param[in] config - Definition of the particular problem.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] Elem_Type - VTK index of the element type being distributed.
   */
  void PartitionSurfaceConnectivity(CConfig *config, CGeometry *geometry, unsigned short Elem_Type);

  /*!
   * \brief Load the local grid points after partitioning (owned and ghost) into the geometry class objects.
   * \param[in] config - Definition of the particular problem.
   * \param[in] geometry - Geometrical definition of the problem.
   */
  void LoadPoints(CConfig *config, CGeometry *geometry);

  /*!
   * \brief Load the local volume elements after partitioning (owned and ghost) into the geometry class objects.
   * \param[in] config - Definition of the particular problem.
   * \param[in] geometry - Geometrical definition of the problem.
   */
  void LoadVolumeElements(CConfig *config, CGeometry *geometry);

  /*!
   * \brief Load the local surface elements after partitioning (owned and ghost) into the geometry class objects.
   * \param[in] config - Definition of the particular problem.
   * \param[in] geometry - Geometrical definition of the problem.
   */
  void LoadSurfaceElements(CConfig *config, CGeometry *geometry);

  /*!
   * \brief Routine to launch non-blocking sends and recvs amongst all processors.
   * \param[in] bufSend - Buffer of data to be sent.
   * \param[in] nElemSend - Array containing the number of elements to send to other processors in cumulative storage format.
   * \param[in] sendReq - Array of MPI send requests.
   * \param[in] bufRecv - Buffer of data to be received.
   * \param[in] nElemSend - Array containing the number of elements to receive from other processors in cumulative storage format.
   * \param[in] sendReq - Array of MPI recv requests.
   * \param[in] countPerElem - Pieces of data per element communicated.
   */
  void InitiateCommsAll(void *bufSend,
                        int *nElemSend,
                        SU2_MPI::Request *sendReq,
                        void *bufRecv,
                        int *nElemRecv,
                        SU2_MPI::Request *recvReq,
                        unsigned short countPerElem,
                        unsigned short commType);

  /*!
   * \brief Routine to complete the set of non-blocking communications launched with InitiateComms() with MPI_Waitany().
   * \param[in] nSends - Number of sends to be completed.
   * \param[in] sendReq - Array of MPI send requests.
   * \param[in] nRecvs - Number of receives to be completed.
   * \param[in] sendReq - Array of MPI recv requests.
   */
  void CompleteCommsAll(int nSends,
                        SU2_MPI::Request *sendReq,
                        int nRecvs,
                        SU2_MPI::Request *recvReq);

  /*!
   * \brief Routine to compute the initial linear partitioning offset counts and store in persistent data structures.
   * \param[in] val_npoint_global - total number of grid points in the mesh.
   */
  void PrepareOffsets(unsigned long val_npoint_global);

  /*!
   * \brief Get the processor that owns the global numbering index based on the linear partitioning.
   * \param[in] val_global_index - Global index for a point.
   * \return Rank of the owner processor for the current point based on linear partitioning.
   */
  unsigned long GetLinearPartition(unsigned long val_global_index);

  /*!
   * \brief Routine to sort the adjacency for ParMETIS for graph partitioning in parallel.
   * \param[in] config - Definition of the particular problem.
   */
  void SortAdjacency(CConfig *config);

  /*!
   * \brief Set the send receive boundaries of the grid.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_domain - Number of domains for parallelization purposes.
   */
  void SetSendReceive(CConfig *config) override;

  /*!
   * \brief Set the send receive boundaries of the grid.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_domain - Number of domains for parallelization purposes.
   */
  void SetBoundaries(CConfig *config) override;

  /*!
   * \brief Set the local index that correspond with the global numbering index.
   */
  void SetGlobal_to_Local_Point() override;

  /*!
   * \brief Get the local index that correspond with the global numbering index.
   * \param[in] val_ipoint - Global point.
   * \return Local index that correspond with the global index, -1 if not found on the current rank (process).
   */
  inline long GetGlobal_to_Local_Point(unsigned long val_ipoint) const override {
    auto it = Global_to_Local_Point.find(val_ipoint);
    if (it != Global_to_Local_Point.cend())
      return it->second;
    return -1;
  }

  /*!
   * \brief Get the local marker that correspond with the global marker.
   * \param[in] val_ipoint - Global marker.
   * \return Local marker that correspond with the global index.
   */
  inline unsigned short GetGlobal_to_Local_Marker(unsigned short val_imarker) const override {
    return Global_to_Local_Marker[val_imarker];
  }

  /*!
   * \brief Reads the geometry of the grid and adjust the boundary
   *        conditions with the configuration file in parallel (for parmetis).
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_mesh_filename - Name of the file with the grid information.
   * \param[in] val_format - Format of the file with the grid information.
   * \param[in] val_iZone - Domain to be read from the grid file.
   * \param[in] val_nZone - Total number of domains in the grid file.
   */
  void Read_Mesh_FVM(CConfig *config, string val_mesh_filename, unsigned short val_iZone, unsigned short val_nZone);

  /*!
   * \brief Reads for the FEM solver the geometry of the grid and adjust the boundary
   *        conditions with the configuration file in parallel (for parmetis).
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_mesh_filename - Name of the file with the grid information.
   * \param[in] val_iZone - Domain to be read from the grid file.
   * \param[in] val_nZone - Total number of domains in the grid file.
   */
  void Read_SU2_Format_Parallel_FEM(CConfig *config, string val_mesh_filename, unsigned short val_iZone, unsigned short val_nZone);

  /*!
   * \brief Reads for the FEM solver the geometry of the grid and adjust the boundary
   *        conditions with the configuration file in parallel (for parmetis).
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_mesh_filename - Name of the file with the grid information.
   * \param[in] val_iZone - Domain to be read from the grid file.
   * \param[in] val_nZone - Total number of domains in the grid file.
   */
  void Read_CGNS_Format_Parallel_FEM(CConfig *config, string val_mesh_filename, unsigned short val_iZone, unsigned short val_nZone);

  /*!
   * \brief Routine to load the CGNS grid points from a single zone into the proper SU2 data structures.
   * \param[in] config - definition of the particular problem.
   * \param[in] mesh   - mesh reader object containing the current zone data.
   */
  void LoadLinearlyPartitionedPoints(CConfig *config, CMeshReaderFVM *mesh);

  /*!
   * \brief Loads the interior volume elements from the mesh reader object into the primal element data structures.
   * \param[in] config - definition of the particular problem.
   * \param[in] mesh   - mesh reader object containing the current zone data.
   */
  void LoadLinearlyPartitionedVolumeElements(CConfig *config, CMeshReaderFVM *mesh);

  /*!
   * \brief Loads the boundary elements (markers) from the mesh reader object into the primal element data structures.
   * \param[in] config - definition of the particular problem.
   * \param[in] mesh   - mesh reader object containing the current zone data.
   */
  void LoadUnpartitionedSurfaceElements(CConfig *config, CMeshReaderFVM *mesh);

  /*!
   * \brief Prepares the grid point adjacency based on a linearly partitioned mesh object needed by ParMETIS for graph partitioning in parallel.
   * \param[in] config - Definition of the particular problem.
   */
  void PrepareAdjacency(CConfig *config);

  /*! 
   * \brief Loads the geometry of the adapted grid from the python interface and
   *        adjust the boundary conditions with the configuration file in
   *        parallel (for parmetis).
   * \param[in] PoiAdap - Vector of coordinates.
   * \param[in] EdgAdap - Vector of edg adjacency.
   * \param[in] TriAdap - Vector of tri adjacency.
   * \param[in] TetAdap - Vector of tet adjacency.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_iZone - Domain to be read from the grid file.
   */
  void LoadAdaptedMeshParallel_FVM(vector<vector<passivedouble> > const &PoiAdap, vector<vector<unsigned long> > const &EdgAdap, 
                                      vector<vector<unsigned long> > const &TriAdap, vector<vector<unsigned long> > const &TetAdap,
                                      CConfig *config);

  /*!
   * \brief Find repeated nodes between two elements to identify the common face.
   * \param[in] first_elem - Identification of the first element.
   * \param[in] second_elem - Identification of the second element.
   * \param[in] face_first_elem - Index of the common face for the first element.
   * \param[in] face_second_elem - Index of the common face for the second element.
   * \return It provides 0 or 1 depending if there is a common face or not.
   */
  bool FindFace(unsigned long first_elem, unsigned long second_elem, unsigned short &face_first_elem,
                unsigned short &face_second_elem) override;

  /*!
   * \brief Computes the distance to the nearest no-slip wall for each grid node.
   * \param[in] config - Definition of the particular problem.
   */
  void ComputeWall_Distance(CConfig *config) override;

  /*!
   * \brief Compute surface area (positive z-direction) for force coefficient non-dimensionalization.
   * \param[in] config - Definition of the particular problem.
   */
  void SetPositive_ZArea(CConfig *config) override;

  /*!
   * \brief Set points which surround a point.
   */
  void SetPoint_Connectivity(void) override;

  /*!
   * \brief Set a renumbering using a Reverse Cuthill-McKee Algorithm
   * \param[in] config - Definition of the particular problem.
   */
  void SetRCM_Ordering(CConfig *config) override;

  /*!
   * \brief Set elements which surround an element.
   */
  void SetElement_Connectivity(void) override;

  /*!
   * \brief Set the volume element associated to each boundary element.
   */
  void SetBoundVolume(void) override;

  /*!
   * \brief Set boundary vertex.
   * \param[in] config - Definition of the particular problem.
   */
  void SetVertex(CConfig *config) override;

  /*!
   * \brief Set number of span wise level for turbomachinery computation.
   * \param[in] config - Definition of the particular problem.
   */
  void ComputeNSpan(CConfig *config, unsigned short val_iZone, unsigned short marker_flag, bool allocate) override;

  /*!
   * \brief Set turbo boundary vertex.
   * \param[in] config - Definition of the particular problem.
   */
  void SetTurboVertex(CConfig *config,unsigned short val_iZone, unsigned short marker_flag, bool allocate) override;

  /*!
 * \brief update turbo boundary vertex.
 * \param[in] config - Definition of the particular problem.
 */
  void UpdateTurboVertex(CConfig *config,unsigned short val_iZone, unsigned short marker_flag) override;

  /*!
   * \brief Set turbo boundary vertex.
   * \param[in] config - Definition of the particular problem.
   */
  void SetAvgTurboValue(CConfig *config, unsigned short val_iZone, unsigned short marker_flag, bool allocate) override;

  /*!
   * \brief Set turbo boundary vertex.
   * \param[in] config - Definition of the particular problem.
   */
  void GatherInOutAverageValues(CConfig *config, bool allocate) override;

  /*!
   * \brief Set the center of gravity of the face, elements and edges.
   */
  void SetCoord_CG(void) override;

  /*!
   * \brief Set the edge structure of the control volume.
   * \param[in] config - Definition of the particular problem.
   * \param[in] action - Allocate or not the new elements.
   */
  void SetControlVolume(CConfig *config, unsigned short action) override;

  /*!
   * \brief Visualize the structure of the control volume(s).
   * \param[in] config - Definition of the particular problem.
   * \param[in] action - Allocate or not the new elements.
   */
  void VisualizeControlVolume(CConfig *config, unsigned short action) override;

  /*!
   * \brief Mach the near field boundary condition.
   * \param[in] config - Definition of the particular problem.
   */
  void MatchNearField(CConfig *config) override;

  /*!
   * \brief Mach the near field boundary condition.
   * \param[in] config - Definition of the particular problem.
   */
  void MatchActuator_Disk(CConfig *config) override;

  /*!
   * \brief Mach the periodic boundary conditions.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_periodic - Index of the first periodic face in a pair.
   */
  void MatchPeriodic(CConfig *config, unsigned short val_periodic) override;

  /*!
   * \brief Set boundary vertex structure of the control volume.
   * \param[in] config - Definition of the particular problem.
   * \param[in] action - Allocate or not the new elements.
   */
  void SetBoundControlVolume(CConfig *config, unsigned short action) override;

  /*!
   * \brief Set the maximum cell-center to cell-center distance for CVs.
   * \param[in] config - Definition of the particular problem.
   */
  void SetMaxLength(CConfig* config) override;

  /*!
   * \brief Set the Tecplot file.
   * \param[in] config_filename - Name of the file where the Tecplot
   *            information is going to be stored.
   * \param[in] new_file - Create a new file.
   */
  void SetTecPlot(char config_filename[MAX_STRING_SIZE], bool new_file) override;

  /*!
   * \brief Set the output file for boundaries in Tecplot
   * \param[in] config - Definition of the particular problem.
   * \param[in] mesh_filename - Name of the file where the Tecplot
   *            information is going to be stored.
   * \param[in] new_file - Create a new file.
   */
  void SetBoundTecPlot(char mesh_filename[MAX_STRING_SIZE], bool new_file, CConfig *config) override;

  /*!
   * \brief Check the volume element orientation.
   * \param[in] config - Definition of the particular problem.
   */
  void Check_IntElem_Orientation(CConfig *config) override;

  /*!
   * \brief Check the volume element orientation.
   * \param[in] config - Definition of the particular problem.
   */
  void Check_BoundElem_Orientation(CConfig *config) override;

  /*!
   * \brief Set the domains for grid grid partitioning using ParMETIS.
   * \param[in] config - Definition of the particular problem.
   */
  void SetColorGrid_Parallel(CConfig *config) override;

  /*!
   * \brief Set the domains for FEM grid partitioning using ParMETIS.
   * \param[in] config - Definition of the particular problem.
   */
  void SetColorFEMGrid_Parallel(CConfig *config) override;

  /*!
   * \brief Compute the weights of the FEM graph for ParMETIS.
   * \param[in]  config                       - Definition of the particular problem.
   * \param[in]  localFaces                   - Vector, which contains the element faces of this rank.
   * \param[in]  adjacency                    - Neighbors of the element.
   * \param[in]  mapExternalElemIDToTimeLevel - Map from the external element ID's to their time level
                                                and number of DOFs.
   * \param[out] vwgt                         - Weights of the vertices of the graph, i.e. the elements.
   * \param[out] adjwgt                       - Weights of the edges of the graph.
   */
  void ComputeFEMGraphWeights(
      CConfig                                    *config,
      const vector<CFaceOfElement>               &localFaces,
      const vector<vector<unsigned long> >       &adjacency,
      const map<unsigned long, CUnsignedShort2T> &mapExternalElemIDToTimeLevel,
      vector<su2double>                          &vwgt,
      vector<vector<su2double> >                 &adjwgt);

  /*!
   * \brief Determine the donor elements for the boundary elements on viscous
            wall boundaries when wall functions are used.
   * \param[in]  config - Definition of the particular problem.
   */
  void DetermineDonorElementsWallFunctions(CConfig *config);

  /*!
   * \brief Determine whether or not the Jacobians of the elements and faces
            are constant and a length scale of the elements.
   * \param[in]  config - Definition of the particular problem.
   */
  void DetermineFEMConstantJacobiansAndLenScale(CConfig *config);

  /*!
   * \brief Determine the neighboring information for periodic faces of a FEM grid.
   * \param[in]     config      - Definition of the particular problem.
   * \param[in,out] localFaces  - Vector, which contains the element faces of this rank.
   */
  void DeterminePeriodicFacesFEMGrid(CConfig                *config,
                                     vector<CFaceOfElement> &localFaces);

  /*!
   * \brief Determine the time level of the elements when time accurate local time stepping is employed.
   * \param[in]  config                       - Definition of the particular problem.
   * \param[in]  localFaces                   - Vector, which contains the element faces of this rank.
   * \param[out] mapExternalElemIDToTimeLevel - Map from the external element ID's to their time level and number of DOFs.
   */
  void DetermineTimeLevelElements(CConfig                              *config,
                                  const vector<CFaceOfElement>         &localFaces,
                                  map<unsigned long, CUnsignedShort2T> &mapExternalElemIDToTimeLevel);

  /*!
   * \brief Do an implicit smoothing of the grid coordinates.
   * \param[in] val_nSmooth - Number of smoothing iterations.
   * \param[in] val_smooth_coeff - Relaxation factor.
   * \param[in] config - Definition of the particular problem.
   */
  void SetCoord_Smoothing(unsigned short val_nSmooth, su2double val_smooth_coeff, CConfig *config) override;

  /*!
   * \brief Write the .su2 file.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_mesh_out_filename - Name of the output file.
   */
  void SetMeshFile(CConfig *config, string val_mesh_out_filename) override;

  /*!
   * \brief Compute 3 grid quality metrics: orthogonality angle, dual cell aspect ratio, and dual cell volume ratio.
   * \param[in] config - Definition of the particular problem.
   */
  void ComputeMeshQualityStatistics(CConfig *config) override;

  /*!
   * \brief Find and store the closest neighbor to a vertex.
   * \param[in] config - Definition of the particular problem.
   */
  void FindNormal_Neighbor(CConfig *config) override;

  /*!
   * \brief Read the sensitivity from an input file.
   * \param[in] config - Definition of the particular problem.
   */
  void SetBoundSensitivity(CConfig *config) override;

  /*!
   * \brief Compute the maximum thickness of an airfoil.
   * \return Maximum thickness at a particular seccion.
   */
  su2double Compute_MaxThickness(su2double *Plane_P0, su2double *Plane_Normal, CConfig *config, vector<su2double> &Xcoord_Airfoil,
                                 vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the twist of an airfoil.
   * \return Twist at a particular seccion.
   */
  su2double Compute_Twist(su2double *Plane_P0, su2double *Plane_Normal, vector<su2double> &Xcoord_Airfoil,
                          vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the leading/trailing edge location of an airfoil.
   */
  void Compute_Wing_LeadingTrailing(su2double *LeadingEdge, su2double *TrailingEdge, su2double *Plane_P0,
                                    su2double *Plane_Normal, vector<su2double> &Xcoord_Airfoil,
                                    vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
  * \brief Compute the leading/trailing edge location of a fuselage.
  */
  void Compute_Fuselage_LeadingTrailing(su2double *LeadingEdge, su2double *TrailingEdge, su2double *Plane_P0,
                                        su2double *Plane_Normal, vector<su2double> &Xcoord_Airfoil,
                                        vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the chord of an airfoil.
   * \return Chord of an airfoil.
   */
  su2double Compute_Chord(su2double *Plane_P0, su2double *Plane_Normal, vector<su2double> &Xcoord_Airfoil,
                          vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the chord of an airfoil.
   * \return Chord of an airfoil.
   */
  su2double Compute_Width(su2double *Plane_P0, su2double *Plane_Normal, vector<su2double> &Xcoord_Airfoil,
                          vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the chord of an airfoil.
   * \return Chord of an airfoil.
   */
  su2double Compute_WaterLineWidth(su2double *Plane_P0, su2double *Plane_Normal, CConfig *config, vector<su2double> &Xcoord_Airfoil,
                                   vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the chord of an airfoil.
   * \return Chord of an airfoil.
   */
  su2double Compute_Height(su2double *Plane_P0, su2double *Plane_Normal, vector<su2double> &Xcoord_Airfoil,
                           vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the chord of an airfoil.
   * \return Chord of an airfoil.
   */
  su2double Compute_LERadius(su2double *Plane_P0, su2double *Plane_Normal, vector<su2double> &Xcoord_Airfoil,
                             vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the thickness of an airfoil.
   */
  su2double Compute_Thickness(su2double *Plane_P0, su2double *Plane_Normal, su2double Location, CConfig *config, vector<su2double> &Xcoord_Airfoil,
                              vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil, su2double &ZLoc) override;

  /*!
   * \brief Compute the area of an airfoil.
   * \return Area of an airfoil.
   */
  su2double Compute_Area(su2double *Plane_P0, su2double *Plane_Normal, CConfig *config, vector<su2double> &Xcoord_Airfoil,
                         vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the length of an airfoil.
   * \return Area of an airfoil.
   */
  su2double Compute_Length(su2double *Plane_P0, su2double *Plane_Normal, CConfig *config, vector<su2double> &Xcoord_Airfoil,
                           vector<su2double> &Ycoord_Airfoil, vector<su2double> &Zcoord_Airfoil) override;

  /*!
   * \brief Compute the dihedral of a wing.
   * \return Dihedral at a particular seccion.
   */
  su2double Compute_Dihedral(su2double *LeadingEdge_im1, su2double *TrailingEdge_im1,
                             su2double *LeadingEdge_i, su2double *TrailingEdge_i) override;

  /*!
   * \brief Compute the curvature of a wing.
   */
  su2double Compute_Curvature(su2double *LeadingEdge_im1, su2double *TrailingEdge_im1,
                              su2double *LeadingEdge_i, su2double *TrailingEdge_i,
                              su2double *LeadingEdge_ip1, su2double *TrailingEdge_ip1) override;

  /*!
   * \brief Evaluate geometrical parameters of a wing.
   */
  void Compute_Wing(CConfig *config, bool original_surface,
                    su2double &Wing_Volume, su2double &Wing_MinMaxThickness, su2double &Wing_MaxMaxThickness,
                    su2double &Wing_MinChord, su2double &Wing_MaxChord,
                    su2double &Wing_MinLERadius, su2double &Wing_MaxLERadius,
                    su2double &Wing_MinToC, su2double &Wing_MaxToC,
                    su2double &Wing_ObjFun_MinToC, su2double &Wing_MaxTwist,
                    su2double &Wing_MaxCurvature, su2double &Wing_MaxDihedral) override;

  /*!
   * \brief Evaluate geometrical parameters of a wing.
   */
  void Compute_Fuselage(CConfig *config, bool original_surface,
                        su2double &Fuselage_Volume, su2double &Fuselage_WettedArea,
                        su2double &Fuselage_MinWidth, su2double &Fuselage_MaxWidth,
                        su2double &Fuselage_MinWaterLineWidth, su2double &Fuselage_MaxWaterLineWidth,
                        su2double &Fuselage_MinHeight, su2double &Fuselage_MaxHeight,
                        su2double &Fuselage_MaxCurvature) override;

  /*!
   * \brief Evaluate geometrical parameters of a wing.
   */
  void Compute_Nacelle(CConfig *config, bool original_surface,
                       su2double &Nacelle_Volume, su2double &Nacelle_MinMaxThickness, su2double &Nacelle_MaxMaxThickness,
                       su2double &Nacelle_MinChord, su2double &Nacelle_MaxChord,
                       su2double &Nacelle_MinLERadius, su2double &Nacelle_MaxLERadius,
                       su2double &Nacelle_MinToC, su2double &Nacelle_MaxToC,
                       su2double &Nacelle_ObjFun_MinToC, su2double &Nacelle_MaxTwist) override;

  /*!
   * \brief Read the sensitivity from adjoint solution file and store it.
   * \param[in] config - Definition of the particular problem.
   */
  void SetSensitivity(CConfig *config) override;

  /*!
   * \brief Set Senitivity to Zero.
   */
  void InitializeSensitivity(void) override;

  /*!
   * \brief Read the sensitivity from unordered ASCII adjoint solution file and store it.
   * \param[in] config - Definition of the particular problem.
   */
  void ReadUnorderedSensitivity(CConfig *config) override;

  /*!
   * \brief Get the Sensitivity at a specific point.
   * \param[in] iPoint - The point where to get the sensitivity.
   * \param[in] iDim - The component of the dim. vector.
   * \return The sensitivity at point iPoint and dim. iDim.
   */
  inline su2double GetSensitivity(unsigned long iPoint, unsigned short iDim) const override { return Sensitivity(iPoint,iDim); }

  /*!
   * \brief Set the Sensitivity at a specific point.
   * \param[in] iPoint - The point where to get the sensitivity.
   * \param[in] iDim - The component of the dim. vector.
   * \param[in] val - Value of the sensitivity.
   */
  inline void SetSensitivity(unsigned long iPoint, unsigned short iDim, su2double val) override { Sensitivity(iPoint,iDim) = val; }

  /*!
   * \brief Check the mesh for periodicity and deactivate multigrid if periodicity is found.
   * \param[in] config - Definition of the particular problem.
   */
  void Check_Periodicity(CConfig *config) override;

};
