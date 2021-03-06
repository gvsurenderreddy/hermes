project(hermes2d)

  # Generating classes for XML mesh parsing according to your version of XSD.
  include(BuildAndInstallScripts)

  set(MESH_2D_XML_H "include/mesh/mesh_h2d_xml.h")
  set(MESH_2D_XML_CPP "mesh/mesh_h2d_xml.cpp")
  GENERATE_XSD_FILES(XSDMesh2D ${MESH_2D_XML_H} ${MESH_2D_XML_CPP} xml_schemas/mesh_h2d_xml.xsd mesh)
  set(MESH_1D_XML_H "include/mesh/mesh_h1d_xml.h")
  set(MESH_1D_XML_CPP "mesh/mesh_h1d_xml.cpp")
  GENERATE_XSD_FILES(XSDMesh1D ${MESH_1D_XML_H} ${MESH_1D_XML_CPP} xml_schemas/mesh_h1d_xml.xsd mesh)
  set(SOLUTION_XML_H "include/function/solution_h2d_xml.h")
  set(SOLUTION_XML_CPP "function/solution_h2d_xml.cpp")
  GENERATE_XSD_FILES(XSDSolution2D ${SOLUTION_XML_H} ${SOLUTION_XML_CPP} xml_schemas/solution_h2d_xml.xsd function)
  set(SPACE_XML_H "include/space/space_h2d_xml.h")
  set(SPACE_XML_CPP "space/space_h2d_xml.cpp")
  GENERATE_XSD_FILES(XSDSpace2D ${SPACE_XML_H} ${SPACE_XML_CPP} xml_schemas/space_h2d_xml.xsd space)
  set(SUBDOMAINS_XML_H "include/mesh/subdomains_h2d_xml.h")
  set(SUBDOMAINS_XML_CPP "mesh/subdomains_h2d_xml.cpp")
  GENERATE_XSD_FILES(XSDSubdomains2D ${SUBDOMAINS_XML_H} ${SUBDOMAINS_XML_CPP} xml_schemas/subdomains_h2d_xml.xsd mesh)
  
  SET(H2D_XML_SCHEMAS_DIRECTORY "\"${PROJECT_SOURCE_DIR}/xml_schemas\"")
  SET(H2D_PRECALCULATED_FORMS_DIRECTORY "\"${PROJECT_SOURCE_DIR}/precalculated_forms\"")

  configure_file(
    config.h.cmake
    include/config.h
  )

  # Determine which version (debug x release) to build.
  if (NOT H2D_DEBUG AND NOT H2D_RELEASE)
    message(FATAL_ERROR "WITH_H2D is set to YES, but no versions are specified.
                         Re-run with H2D_DEBUG and/or H2D_RELEASE set to YES.")
  endif (NOT H2D_DEBUG AND NOT H2D_RELEASE)

  if(MSVC)
    set(HERMES2D "hermes2d")              # Base name of the library.
    if(WIN64)
      set(HERMES2D "hermes2d_64")
    endif()
  else(MSVC)
    if(H2D_DEBUG)
      set(HERMES2D_DEBUG "hermes2d-debug")
      set(HERMES2D ${HERMES2D_DEBUG})
      # set(CMAKE_BUILD_TYPE Debug)
    # This does not work with multi-version configurations,
    # because it is linked to the project (i.e. we would have
    # to have separate projects for Debug and Release)
    endif(H2D_DEBUG)

    if(H2D_RELEASE)
      set(HERMES2D_RELEASE "hermes2d")
      set(HERMES2D ${HERMES2D_RELEASE})
      # set(CMAKE_BUILD_TYPE Release)
    endif(H2D_RELEASE)
  endif(MSVC)

  #
  # Optional libraries and settings for H2D.
  #
  if(H2D_WITH_GLUT)
    if(NOT APPLE)   # See hermes/OSX.cmake for APPLE-specific search procedures.
      find_package(GLUT REQUIRED)
      find_package(GLEW REQUIRED)
    endif(NOT APPLE)
    # For views synchronization, pthreads are required.
    find_package(PTHREAD REQUIRED)
    include_directories(${PTHREAD_INCLUDE_DIR})
  else(H2D_WITH_GLUT)
    add_definitions(-DNOGLUT)
  endif(H2D_WITH_GLUT)

  # Mesh format.
  if(WITH_EXODUSII)
    find_package(EXODUSII REQUIRED)
    include_directories(${EXODUSII_INCLUDE_DIR})
  endif(WITH_EXODUSII)
  
  include_directories(${XSD_INCLUDE_DIR})
  include_directories(${XERCES_INCLUDE_DIR})
  
  #
  # Header files location for the Hermes2d library.
  #
  include_directories(${PROJECT_SOURCE_DIR}/../hermes_common/include)
  include_directories(${PROJECT_SOURCE_DIR}/../hermes_common/include/algebra)
  include_directories(${PROJECT_SOURCE_DIR}/../hermes_common/include/data_structures)
  include_directories(${PROJECT_SOURCE_DIR}/../hermes_common/include/util)
  include_directories(${PROJECT_SOURCE_DIR}/../hermes_common/include/solvers)
  include_directories(${PROJECT_SOURCE_DIR}/../hermes_common/include/solvers/interfaces)
  include_directories(${PROJECT_SOURCE_DIR})
  include_directories(${PROJECT_SOURCE_DIR}/include)
  include_directories(${PROJECT_SOURCE_DIR}/include/form)
  include_directories(${PROJECT_SOURCE_DIR}/include/discrete_problem)
  include_directories(${PROJECT_SOURCE_DIR}/include/space)
  include_directories(${PROJECT_SOURCE_DIR}/include/mesh)
  include_directories(${PROJECT_SOURCE_DIR}/include/boundary_conditions)
  include_directories(${PROJECT_SOURCE_DIR}/include/shapeset)
  include_directories(${PROJECT_SOURCE_DIR}/include/function)
  include_directories(${PROJECT_SOURCE_DIR}/include/integrals)
  include_directories(${PROJECT_SOURCE_DIR}/include/linearizer)
  include_directories(${PROJECT_SOURCE_DIR}/include/adapt)
  include_directories(${PROJECT_SOURCE_DIR}/include/projections)
  include_directories(${PROJECT_SOURCE_DIR}/include/quadrature)
  include_directories(${PROJECT_SOURCE_DIR}/include/refinement_selectors)
  include_directories(${PROJECT_SOURCE_DIR}/include/views)
  include_directories(${PROJECT_SOURCE_DIR}/include/weakform)
  include_directories(${PROJECT_SOURCE_DIR}/include/weakform_library)

  #
  # Source files for the Hermes2d library.
  #
  set(SRC
    src/boundary_conditions/essential_boundary_conditions.cpp
    src/api2d.cpp
    src/graph.cpp
    src/mixins2d.cpp
    src/weakform/weakform.cpp
  
    src/neighbor_search.cpp
    src/norm_form.cpp
    src/spline.cpp
    src/forms.cpp
    src/asmlist.cpp
    src/projections/ogprojection.cpp
    src/projections/ogprojection_nox.cpp
    src/quadrature/limit_order.cpp
    src/quadrature/quad_std.cpp

    src/function/transformable.cpp
    src/function/function.cpp
    src/function/exact_solution.cpp
    src/function/solution.cpp
    src/function/filter.cpp
    src/function/mesh_function.cpp
    src/function/solution_h2d_xml.cpp
    src/function/postprocessing.cpp

    src/mesh/refmap.cpp
    src/mesh/curved.cpp
    src/mesh/mesh_reader_exodusii.cpp
    src/mesh/hash.cpp
    src/mesh/mesh_reader_h2d.cpp
    src/mesh/mesh_reader_h2d_bson.cpp
    src/mesh/mesh_reader_h2d_xml.cpp
    src/mesh/mesh_reader_h1d_xml.cpp
    src/mesh/mesh_h2d_xml.cpp
    src/mesh/mesh_h1d_xml.cpp
    src/mesh/subdomains_h2d_xml.cpp
    src/mesh/element.cpp
    src/mesh/mesh.cpp
    src/mesh/mesh_util.cpp
    src/mesh/traverse.cpp
    src/mesh/mesh_data.cpp

    src/shapeset/shapeset.cpp
    src/shapeset/shapeset_h1_jacobi.cpp
    src/shapeset/shapeset_h1_quad.cpp
    src/shapeset/shapeset_hc_gradleg.cpp
    src/shapeset/shapeset_hd_legendre.cpp
    src/shapeset/shapeset_l2_legendre.cpp
    src/shapeset/shapeset_l2_taylor.cpp
    src/shapeset/precalc.cpp

    src/space/space.cpp
    src/space/space_h1.cpp
    src/space/space_hcurl.cpp
    src/space/space_l2.cpp
    src/space/space_hdiv.cpp
    src/space/space_h2d_xml.cpp

    src/views/base_view.cpp
    src/views/mesh_view.cpp
    src/views/order_view.cpp
    src/views/scalar_view.cpp
    src/views/vector_base_view.cpp
    src/views/vector_view.cpp
    src/views/view.cpp
    src/views/view_data.cpp
    src/views/view_support.cpp
    src/views/thread_linearizer.cpp
    src/views/linearizer.cpp
    src/views/orderizer.cpp
    
    src/weakform_library/weakforms_elasticity.cpp
    src/weakform_library/weakforms_h1.cpp
    src/weakform_library/weakforms_hcurl.cpp
    src/weakform_library/weakforms_maxwell.cpp
    src/weakform_library/weakforms_neutronics.cpp
    
    src/discrete_problem/discrete_problem.cpp
    src/discrete_problem/discrete_problem_helpers.cpp    
    src/discrete_problem/discrete_problem_selective_assembler.cpp
    src/discrete_problem/discrete_problem_thread_assembler.cpp
    src/discrete_problem/discrete_problem_integration_order_calculator.cpp
    src/discrete_problem/dg/discrete_problem_dg_assembler.cpp
    src/discrete_problem/dg/multimesh_dg_neighbor_tree.cpp
    src/discrete_problem/dg/multimesh_dg_neighbor_tree_node.cpp
    
    src/solver/solver.cpp
    src/solver/linear_solver.cpp
    src/solver/nox_solver.cpp
    src/solver/newton_solver.cpp
    src/solver/picard_solver.cpp
    src/solver/runge_kutta.cpp
    
    src/adapt/adapt.cpp
    src/adapt/adapt_solver.cpp
    src/adapt/kelly_type_adapt.cpp
    src/adapt/error_calculator.cpp
    src/adapt/error_thread_calculator.cpp
    
    src/refinement_selectors/candidates.cpp
    src/refinement_selectors/element_to_refine.cpp
    src/refinement_selectors/selector.cpp
    src/refinement_selectors/order_permutator.cpp
    src/refinement_selectors/optimum_selector.cpp
    src/refinement_selectors/proj_based_selector.cpp
    src/refinement_selectors/l2_proj_based_selector.cpp
    src/refinement_selectors/h1_proj_based_selector.cpp
    src/refinement_selectors/hcurl_proj_based_selector.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Internal" FILES 
    src/neighbor_search.cpp
    src/norm_form.cpp
    src/spline.cpp
    src/forms.cpp
    src/asmlist.cpp
    src/projections/ogprojection.cpp
    src/projections/ogprojection_nox.cpp
    src/quadrature/limit_order.cpp
    src/quadrature/quad_std.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Weakform Library" FILES 
    src/weakform_library/weakforms_elasticity.cpp
    src/weakform_library/weakforms_h1.cpp
    src/weakform_library/weakforms_hcurl.cpp
    src/weakform_library/weakforms_maxwell.cpp
    src/weakform_library/weakforms_neutronics.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Mesh Functions" FILES 
    src/function/transformable.cpp
    src/function/function.cpp
    src/function/exact_solution.cpp
    src/function/solution.cpp
    src/function/filter.cpp
    src/function/mesh_function.cpp
    src/function/solution_h2d_xml.cpp
    src/function/postprocessing.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Mesh" FILES 
    src/mesh/refmap.cpp
    src/mesh/curved.cpp
    src/mesh/mesh_reader_exodusii.cpp
    src/mesh/hash.cpp
    src/mesh/mesh_reader_h2d.cpp
    src/mesh/mesh_reader_h2d_bson.cpp
    src/mesh/mesh_reader_h2d_xml.cpp
    src/mesh/mesh_reader_h1d_xml.cpp
    src/mesh/mesh_h2d_xml.cpp
    src/mesh/mesh_h1d_xml.cpp
    src/mesh/subdomains_h2d_xml.cpp
    src/mesh/element.cpp
    src/mesh/mesh.cpp
    src/mesh/mesh_util.cpp
    src/mesh/traverse.cpp
    src/mesh/mesh_data.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Views" FILES 
    src/views/base_view.cpp
    src/views/mesh_view.cpp
    src/views/order_view.cpp
    src/views/scalar_view.cpp
    src/views/vector_base_view.cpp
    src/views/vector_view.cpp
    src/views/view.cpp
    src/views/view_data.cpp
    src/views/view_support.cpp
    src/views/linearizer.cpp
    src/views/thread_linearizer.cpp
    src/views/orderizer.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Space" FILES 
    src/shapeset/shapeset.cpp
    src/shapeset/shapeset_h1_jacobi.cpp
    src/shapeset/shapeset_h1_quad.cpp
    src/shapeset/shapeset_hc_gradleg.cpp
    src/shapeset/shapeset_hd_legendre.cpp
    src/shapeset/shapeset_l2_legendre.cpp
    src/shapeset/shapeset_l2_taylor.cpp
    src/shapeset/precalc.cpp

    src/space/space.cpp
    src/space/space_h1.cpp
    src/space/space_hcurl.cpp
    src/space/space_l2.cpp
    src/space/space_hdiv.cpp
    src/space/space_h2d_xml.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Solvers" FILES
    src/solver/solver.cpp
    src/solver/linear_solver.cpp    
    src/solver/nox_solver.cpp
    src/solver/newton_solver.cpp
    src/solver/picard_solver.cpp
    src/solver/nonlinear_convergence_measurement.cpp
    src/solver/runge_kutta.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Discrete Problem" FILES 
    src/discrete_problem/discrete_problem.cpp
    src/discrete_problem/discrete_problem_helpers.cpp
    src/discrete_problem/discrete_problem_selective_assembler.cpp
    src/discrete_problem/discrete_problem_thread_assembler.cpp
    src/discrete_problem/discrete_problem_integration_order_calculator.cpp
    src/discrete_problem/dg/discrete_problem_dg_assembler.cpp
    src/discrete_problem/dg/multimesh_dg_neighbor_tree.cpp
    src/discrete_problem/dg/multimesh_dg_neighbor_tree_node.cpp
  )
  
  SOURCE_GROUP(
    "Source Files\\Adaptivity" FILES 
    src/adapt/adapt.cpp
    src/adapt/adapt_solver.cpp
    src/adapt/kelly_type_adapt.cpp
    src/adapt/error_calculator.cpp
    src/adapt/error_thread_calculator.cpp
  )

  SOURCE_GROUP(
    "Source Files\\Adaptivity\\Refinement Selectors" FILES 
    src/refinement_selectors/candidates.cpp
    src/refinement_selectors/element_to_refine.cpp
    src/refinement_selectors/selector.cpp
    src/refinement_selectors/order_permutator.cpp
    src/refinement_selectors/optimum_selector.cpp
    src/refinement_selectors/proj_based_selector.cpp
    src/refinement_selectors/l2_proj_based_selector.cpp
    src/refinement_selectors/h1_proj_based_selector.cpp
    src/refinement_selectors/hcurl_proj_based_selector.cpp
  )
  
  set(HEADERS
    include/api2d.h
    include/boundary_conditions/essential_boundary_conditions.h
    include/mixins2d.h
    include/graph.h
    include/weakform/weakform.h
    
    include/projections/ogprojection.h
    include/projections/ogprojection_nox.h
    include/global.h
    include/asmlist.h
    include/forms.h
    include/neighbor_search.h
    include/norm_form.h
    include/spline.h
    include/quadrature/limit_order.h
    include/quadrature/quad.h
    include/quadrature/quad_all.h

    include/function/transformable.h
    include/function/function.h
    include/function/exact_solution.h
    include/function/solution.h
    include/function/filter.h
    include/function/mesh_function.h
    include/function/solution_h2d_xml.h
    include/function/postprocessing.h

    include/mesh/refmap.h
    include/mesh/curved.h
    include/mesh/mesh_reader_exodusii.h
    include/mesh/hash.h
    include/mesh/mesh_reader.h
    include/mesh/mesh_reader_h2d.h
    include/mesh/mesh_reader_h2d_bson.h
    include/mesh/mesh_reader_h2d_xml.h
    include/mesh/mesh_reader_h1d_xml.h
    include/mesh/mesh_h2d_xml.h
    include/mesh/mesh_h1d_xml.h
    include/mesh/subdomains_h2d_xml.h
    include/mesh/element.h
    include/mesh/mesh.h
    include/mesh/mesh_util.h
    include/mesh/traverse.h
    include/mesh/mesh_data.h

    include/shapeset/shapeset.h
    include/shapeset/shapeset_h1_all.h
    include/shapeset/shapeset_hc_all.h
    include/shapeset/shapeset_hd_all.h
    include/shapeset/shapeset_l2_all.h
    include/shapeset/precalc.h

    include/space/space.h
    include/space/space_h1.h
    include/space/space_hcurl.h
    include/space/space_l2.h
    include/space/space_hdiv.h
    include/space/space_h2d_xml.h

    include/views/base_view.h
    include/views/mesh_view.h
    include/views/order_view.h
    include/views/scalar_view.h
    include/views/vector_base_view.h
    include/views/vector_view.h
    include/views/view.h
    include/views/view_support.h
    include/views/thread_linearizer.h
    include/views/linearizer.h
    include/views/linearizer_utils.h
    include/views/orderizer.h

    include/weakform_library/weakforms_elasticity.h
    include/weakform_library/weakforms_h1.h
    include/weakform_library/integrals_h1.h
    include/weakform_library/weakforms_hcurl.h
    include/weakform_library/weakforms_maxwell.h
    include/weakform_library/weakforms_neutronics.h
    
    include/discrete_problem/discrete_problem.h
    include/discrete_problem/discrete_problem_helpers.h
    include/discrete_problem/discrete_problem_selective_assembler.h
    include/discrete_problem/discrete_problem_thread_assembler.h
    include/discrete_problem/discrete_problem_integration_order_calculator.h
    include/discrete_problem/dg/discrete_problem_dg_assembler.h
    include/discrete_problem/dg/multimesh_dg_neighbor_tree.h
    include/discrete_problem/dg/multimesh_dg_neighbor_tree_node.h
    
    include/solver/solver.h
    include/solver/linear_solver.h
    include/solver/nox_solver.h
    include/solver/newton_solver.h
    include/solver/picard_solver.h
    include/solver/runge_kutta.h
    
    include/adapt/adapt.h
    include/adapt/adapt_solver.h
    include/adapt/kelly_type_adapt.h
    include/adapt/error_calculator.h
    include/adapt/error_thread_calculator.h
    
    include/refinement_selectors/element_to_refine.h
    include/refinement_selectors/candidates.h
    include/refinement_selectors/selector.h
    include/refinement_selectors/order_permutator.h
    include/refinement_selectors/optimum_selector.h
    include/refinement_selectors/proj_based_selector.h
    include/refinement_selectors/l2_proj_based_selector.h
    include/refinement_selectors/h1_proj_based_selector.h
    include/refinement_selectors/hcurl_proj_based_selector.h
  )
  
  SOURCE_GROUP(
    "Header Files\\Internal" FILES 
    include/projections/ogprojection.h
    include/projections/ogprojection_nox.h
    include/global.h
    include/asmlist.h
    include/forms.h
    include/neighbor_search.h
    include/norm_form.h
    include/spline.h
    include/quadrature/limit_order.h
    include/quadrature/quad.h
    include/quadrature/quad_all.h
  )
  
  SOURCE_GROUP(
    "Header Files\\Weakform Library" FILES 
    include/weakform_library/weakforms_elasticity.h
    include/weakform_library/weakforms_h1.h
    include/weakform_library/integrals_h1.h
    include/weakform_library/weakforms_hcurl.h
    include/weakform_library/weakforms_maxwell.h
    include/weakform_library/weakforms_neutronics.h
  )
  
  SOURCE_GROUP(
    "Header Files\\Mesh Functions" FILES 
    include/function/transformable.h
    include/function/function.h
    include/function/exact_solution.h
    include/function/solution.h
    include/function/filter.h
    include/function/mesh_function.h
    include/function/solution_h2d_xml.h
    include/function/postprocessing.h
  )
    
  SOURCE_GROUP(
    "Header Files\\Mesh" FILES 
    include/mesh/refmap.h
    include/mesh/curved.h
    include/mesh/mesh_reader_exodusii.h
    include/mesh/hash.h
    include/mesh/mesh_reader.h
    include/mesh/mesh_reader_h2d.h
    include/mesh/mesh_reader_h2d_bson.h
    include/mesh/mesh_reader_h2d_xml.h
    include/mesh/mesh_reader_h1d_xml.h
    include/mesh/mesh_h2d_xml.h
    include/mesh/mesh_h1d_xml.h
    include/mesh/subdomains_h2d_xml.h
    include/mesh/element.h
    include/mesh/mesh.h
    include/mesh/mesh_util.h
    include/mesh/traverse.h
    include/mesh/mesh_data.h
  )
  
  SOURCE_GROUP(
    "Header Files\\Views" FILES 
    include/views/base_view.h
    include/views/mesh_view.h
    include/views/order_view.h
    include/views/scalar_view.h
    include/views/vector_base_view.h
    include/views/vector_view.h
    include/views/view.h
    include/views/view_support.h
    include/views/thread_linearizer.h
    include/views/linearizer.h
    include/views/linearizer_utils.h
    include/views/orderizer.h
  )
  
  SOURCE_GROUP(
    "Header Files\\Space" FILES 
    include/shapeset/shapeset.h
    include/shapeset/shapeset_h1_all.h
    include/shapeset/shapeset_hc_all.h
    include/shapeset/shapeset_hd_all.h
    include/shapeset/shapeset_l2_all.h
    include/shapeset/precalc.h

    include/space/space.h
    include/space/space_h1.h
    include/space/space_hcurl.h
    include/space/space_l2.h
    include/space/space_hdiv.h
    include/space/space_h2d_xml.h
  )
  
  SOURCE_GROUP(
    "Header Files\\Solvers" FILES 
    include/solver/solver.h
    include/solver/linear_solver.h
    include/solver/nox_solver.h
    include/solver/newton_solver.h
    include/solver/picard_solver.h
    include/solver/nonlinear_convergence_measurement.h
    include/solver/runge_kutta.h
  )
  
  SOURCE_GROUP(
    "Header Files\\Discrete Problem" FILES 
    include/discrete_problem/discrete_problem.h
    include/discrete_problem/discrete_problem_helpers.h
    include/discrete_problem/discrete_problem_selective_assembler.h
    include/discrete_problem/discrete_problem_thread_assembler.h
    include/discrete_problem/discrete_problem_integration_order_calculator.h
    include/discrete_problem/dg/discrete_problem_dg_assembler.h
    include/discrete_problem/dg/multimesh_dg_neighbor_tree.h
    include/discrete_problem/dg/multimesh_dg_neighbor_tree_node.h
  )
  
  SOURCE_GROUP(
    "Header Files\\Adaptivity" FILES 
    include/adapt/adapt.h
    include/adapt/adapt_solver.h
    include/adapt/kelly_type_adapt.h
    include/adapt/error_calculator.h
    include/adapt/error_thread_calculator.h
    )
    
  SOURCE_GROUP(
    "Header Files\\Adaptivity\\Refinement Selectors" FILES
    include/refinement_selectors/candidates.h
    include/refinement_selectors/element_to_refine.h
    include/refinement_selectors/selector.h
    include/refinement_selectors/order_permutator.h
    include/refinement_selectors/optimum_selector.h
    include/refinement_selectors/proj_based_selector.h
    include/refinement_selectors/l2_proj_based_selector.h
    include/refinement_selectors/h1_proj_based_selector.h
    include/refinement_selectors/hcurl_proj_based_selector.h
  )
    
  #
  # Build and install the library.
  #
  
  # Add a target for a specified version of the Hermes library,
  # and set its build properties (compile and link flags, installation directories). Due to
  # different optional libraries for H1D, H2D and H3D, this macro is dimension-specific.
  macro(BUILD_2D_LIB HERMES_LIB HERMES_COMMON_LIB BUILD_FLAGS)
    if(${HERMES_STATIC_LIBS})
      add_library(${HERMES_LIB} ${SRC} ${HEADERS})
    else()
      add_library(${HERMES_LIB} SHARED ${SRC} ${HEADERS})
    endif()

    if(NOT MSVC)
      set(_FLAGS ${BUILD_FLAGS})
      set_target_properties(${HERMES_LIB} PROPERTIES COMPILE_FLAGS ${_FLAGS})
    endif()

    target_link_libraries(  ${HERMES_LIB}
      ${HERMES_COMMON_LIB}
      ${GLUT_LIBRARY} ${GL_LIBRARY} ${GLEW_LIBRARY} ${PTHREAD_LIBRARY}
      ${ANTTWEAKBAR_LIBRARY}
      ${XSD_LIBRARY}
      ${XERCES_LIBRARY}
      ${LIBIBERTY_LIBRARY}
      ${PJLIB_LIBRARY}
      ${LAPACK_LIBRARY}
      ${CLAPACK_LIBRARY} ${BLAS_LIBRARY}
    )
    
    if(MSVC)
      if(WITH_PJLIB)
        TARGET_LINK_LIBRARIES(${HERMES_LIB} Ws2_32 wsock32)
      endif(WITH_PJLIB)
    endif(MSVC)
  endmacro(BUILD_2D_LIB)

  # Build and install:

  if(MSVC)
    BUILD_2D_LIB(${HERMES2D} ${HERMES_COMMON_LIB} "")
    ADD_MSVC_BUILD_FLAGS(${HERMES2D})
    INSTALL_LIB(${HERMES2D})
  else(MSVC)
    if(H2D_DEBUG)
      BUILD_2D_LIB(${HERMES2D_DEBUG} ${HERMES_COMMON_LIB_DEBUG} ${HERMES_DEBUG_FLAGS})
      INSTALL_LIB(${HERMES2D_DEBUG})
    endif(H2D_DEBUG)
    if(H2D_RELEASE)
      BUILD_2D_LIB(${HERMES2D_RELEASE} ${HERMES_COMMON_LIB_RELEASE} ${HERMES_RELEASE_FLAGS})
      INSTALL_LIB(${HERMES2D_RELEASE})
    endif(H2D_RELEASE)
  endif(MSVC)

  # Install header files.

  file(GLOB INC            "include/*.h")
  file(GLOB INC_SOLVER          "include/solver/*.h")
  file(GLOB INC_DP          "include/discrete_problem/*.h")
  file(GLOB INC_DP_DG       "include/discrete_problem/dg/*.h")
  file(GLOB INC_FORM       "include/form/*.h")
  file(GLOB INC_SPACE      "include/space/*.h")
  file(GLOB INC_MESH       "include/mesh/*.h")
  file(GLOB INC_XSD        "xml_schemas/*.xsd")
  file(GLOB INC_BC         "include/boundary_conditions/*.h")
  file(GLOB INC_SHAPESET   "include/shapeset/*.h")
  file(GLOB INC_FUNCTION   "include/function/*.h")
  file(GLOB INC_LINEARIZER "include/linearizer/*.h")
  file(GLOB INC_ADAPT      "include/adapt/*.h")
  file(GLOB INC_PROJECTIONS "include/projections/*.h")
  file(GLOB INC_PRECALC     "include/precalculated_forms/*.h")
  file(GLOB INC_QUAD       "include/quadrature/*.h")
  file(GLOB INC_REF_SEL    "include/refinement_selectors/*.h")
  file(GLOB INC_VIEWS      "include/views/*.h")
  file(GLOB INC_WEAKFORM   "include/weakform/*.h")
  file(GLOB INC_WF_LIBRARY "include/weakform_library/*.h")
  
  install(FILES ${INC}              DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d)
  install(FILES ${INC_SOLVER}       DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/solver)
  install(FILES ${INC_DP}           DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/discrete_problem)
  install(FILES ${INC_DP_DG}        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/discrete_problem/dg)
  install(FILES ${INC_SPACE}        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/space)
  install(FILES "include/space/space_h2d_xml.h"        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/space)
  install(FILES ${INC_MESH}         DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/mesh)
  install(FILES "include/mesh/mesh_h1d_xml.h"        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/mesh)
  install(FILES "include/mesh/mesh_h2d_xml.h"        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/mesh)
  install(FILES "include/mesh/subdomains_h2d_xml.h"         DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/mesh)
  install(FILES ${XSD}              DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/xml_schemas)
  install(FILES ${INC_BC}           DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/boundary_conditions)
  install(FILES ${INC_SHAPESET}     DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/shapeset)
  install(FILES ${INC_FUNCTION}     DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/function)
  install(FILES "include/function/solution_h2d_xml.h"     DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/function)
  install(FILES ${INC_INTEGRALS}    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/integrals)
  install(FILES ${INC_LINEARIZER}   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/linearizer)
  install(FILES ${INC_ADAPT}        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/adapt)
  install(FILES ${INC_PROJECTIONS}  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/projections)
  install(FILES ${INC_PRECALC}      DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/precalculated_forms)
  install(FILES ${INC_QUAD}         DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/quadrature)
  install(FILES ${INC_REF_SEL}      DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/refinement_selectors)
  install(FILES ${INC_VIEWS}        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/views)
  install(FILES ${INC_WEAKFORM}     DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/weakform)
  install(FILES ${INC_WF_LIBRARY}   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hermes2d/weakform_library)

IF(EXISTS "hermes2d/test_examples")
  if(H2D_WITH_TEST_EXAMPLES)
    add_subdirectory(test_examples)
  endif(H2D_WITH_TEST_EXAMPLES)
ENDIF(EXISTS "hermes2d/test_examples")
