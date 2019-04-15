#-------------------------------------------------------------------------------
#
#  RootMacros.cmake
#
#  Macros and definitions regarding ROOT components.
#
#-------------------------------------------------------------------------------

if(CMAKE_GENERATOR MATCHES Makefiles)
  set(fast /fast)
  set(always-make --always-make)
endif()
#-------------------------------------------------------------------------------
#
#  function ROOTTEST_ADD_TESTDIRS([EXCLUDED_DIRS] dir)
#
#  Scans all subdirectories for CMakeLists.txt files. Each subdirectory that
#  contains a CMakeLists.txt file is then added as a subdirectory.
#-------------------------------------------------------------------------------
function(ROOTTEST_ADD_TESTDIRS)

  set(dirs "")
  CMAKE_PARSE_ARGUMENTS(ARG "" "" "EXCLUDED_DIRS" ${ARGN})
  set(curdir ${CMAKE_CURRENT_SOURCE_DIR})

  file(GLOB found_dirs ${curdir} ${curdir}/*)

  # If there are excluded directories through EXCLUDED_DIRS,
  # add_subdirectory() for them will not be applied
  if(ARG_EXCLUDED_DIRS)
    foreach(excluded_dir ${ARG_EXCLUDED_DIRS})
      list(REMOVE_ITEM found_dirs "${CMAKE_CURRENT_SOURCE_DIR}/${excluded_dir}")
    endforeach()
  endif()

  foreach(f ${found_dirs})
    if(IS_DIRECTORY ${f})
      if(EXISTS "${f}/CMakeLists.txt" AND NOT ${f} STREQUAL ${curdir})
        list(APPEND dirs ${f})
      endif()
    endif()
  endforeach()

  list(SORT dirs)

  foreach(d ${dirs})
    string(REPLACE "${curdir}/" "" d ${d})
    add_subdirectory(${d})
    # create .rootrc in binary directory to avoid filling $HOME/.root_hist
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${d}/.rootrc "
Rint.History:  .root_hist
")
  endforeach()

endfunction()

#-------------------------------------------------------------------------------
#
#  function ROOTTEST_SET_TESTOWNER(owner)
#
#  Specify the owner of the tests in the current directory. Note, that the owner
#  can be specified for each test individually, as well.
#
#-------------------------------------------------------------------------------
function(ROOTTEST_SET_TESTOWNER owner)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
               PROPERTY ROOTTEST_TEST_OWNER ${owner})
endfunction(ROOTTEST_SET_TESTOWNER)

#-------------------------------------------------------------------------------
#
# function ROOTTEEST_TARGETNAME_FROM_FILE(<resultvar> <filename>)
#
# Construct a target name for a given file <filename> and store its name into
# <resultvar>. The target name is of the form:
#
#   roottest-<directorypath>-<filename_WE>
#
#-------------------------------------------------------------------------------
function(ROOTTEST_TARGETNAME_FROM_FILE resultvar filename)

  get_filename_component(realfp ${filename} ABSOLUTE)
  get_filename_component(filename_we ${filename} NAME_WE)

  string(REPLACE "${ROOTTEST_DIR}" "" relativepath ${realfp})
  string(REPLACE "${filename}"     "" relativepath ${relativepath})

  string(REPLACE "/" "-" targetname ${relativepath}${filename_we})
  set(${resultvar} "roottest${targetname}" PARENT_SCOPE)

endfunction(ROOTTEST_TARGETNAME_FROM_FILE)

#-------------------------------------------------------------------------------
#
# function ROOTTEST_ADD_AUTOMACROS(DEPENDS [dependencies ...])
#
# Automatically adds all macros in the current source directory to the list of
# tests that follow the naming scheme:
#
#   run*.C, run*.cxx, assert*.C, assert*.cxx, exec*.C, exec*.cxx
#
#-------------------------------------------------------------------------------
function(ROOTTEST_ADD_AUTOMACROS)
  CMAKE_PARSE_ARGUMENTS(ARG "" "" "DEPENDS;WILLFAIL;EXCLUDE" ${ARGN})

  file(GLOB automacros run*.C run*.cxx assert*.C assert*.cxx exec*.C exec*.cxx)

  foreach(dep ${ARG_DEPENDS})
    if(${dep} MATCHES "[.]C" OR ${dep} MATCHES "[.]cxx" OR ${dep} MATCHES "[.]h")
      ROOTTEST_COMPILE_MACRO(${dep})
      list(APPEND auto_depends ${COMPILE_MACRO_TEST})
    else()
      list(APPEND auto_depends ${dep})
    endif()
  endforeach()

  foreach(am ${automacros})
    get_filename_component(auto_macro_filename ${am} NAME)
    get_filename_component(auto_macro_name  ${am} NAME_WE)
    if(${auto_macro_name} MATCHES "^run")
      string(REPLACE run "" auto_macro_subname ${auto_macro_name})
    elseif(${auto_macro_name} MATCHES "^exec")
      string(REPLACE exec "" auto_macro_subname ${auto_macro_name})
    else()
      set(auto_macro_subname ${auto_macro_name})
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${auto_macro_name}.ref)
      set(outref OUTREF ${auto_macro_name}.ref)
    elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${auto_macro_subname}.ref)
      set(outref OUTREF ${auto_macro_subname}.ref)
    else()
      set(outref "")
    endif()

    ROOTTEST_TARGETNAME_FROM_FILE(targetname ${auto_macro_filename})

    foreach(wf ${ARG_WILLFAIL})
      if(${auto_macro_name} MATCHES ${wf})
        set(arg_wf WILLFAIL)
      endif()
    endforeach()

    set(selected 1)
    foreach(excl ${ARG_EXCLUDE})
      if(${auto_macro_name} MATCHES ${excl})
        set(selected 0)
        break()
      endif()
    endforeach()

    if(selected)
      ROOTTEST_ADD_TEST(${targetname}-auto
                        MACRO ${auto_macro_filename}${${auto_macro_name}-suffix}
                        ${outref}
                        ${arg_wf}
                        DEPENDS ${auto_depends})
    endif()
  endforeach()

endfunction(ROOTTEST_ADD_AUTOMACROS)

#-------------------------------------------------------------------------------
#
# macro ROOTTEST_COMPILE_MACRO(<filename> [BUILDOBJ object] [BUILDLIB lib]
#                                         [DEPENDS dependencies...])
#
# This macro creates and loads a shared library containing the code from
# the file <filename>. A test that performs the compilation is created.
# The target name of the created test is stored in the variable
# COMPILE_MACRO_TEST which can be accessed by the calling CMakeLists.txt in
# order to manage dependencies.
#
#-------------------------------------------------------------------------------
macro(ROOTTEST_COMPILE_MACRO filename)
  CMAKE_PARSE_ARGUMENTS(ARG "" "BUILDOBJ;BUILDLIB" "DEPENDS"  ${ARGN})

  # Add defines to root_compile_macro, in order to have out-of-source builds
  # when using the scripts/build.C macro.
  get_directory_property(DirDefs COMPILE_DEFINITIONS)

  foreach(d ${DirDefs})
    list(APPEND RootMacroDirDefines "-e;#define ${d}")
  endforeach()

  set(RootMacroBuildDefines
        -e "#define CMakeEnvironment"
        -e "#define CMakeBuildDir \"${CMAKE_CURRENT_BINARY_DIR}\""
        -e "gSystem->AddDynamicPath(\"${CMAKE_CURRENT_BINARY_DIR}\")"
        -e "gROOT->SetMacroPath(\"${CMAKE_CURRENT_SOURCE_DIR}\")"
        -e "gInterpreter->AddIncludePath(\"-I${CMAKE_CURRENT_BINARY_DIR}\")"
        -e "gSystem->AddIncludePath(\"-I${CMAKE_CURRENT_BINARY_DIR}\")"
        ${RootMacroDirDefines})

  set(root_compile_macro root.exe ${RootMacroBuildDefines} -q -l -b)

  get_filename_component(realfp ${filename} ABSOLUTE)

  set(BuildScriptFile ${ROOTTEST_DIR}/scripts/build.C)

  set(BuildScriptArg \(\"${realfp}\",\"${ARG_BUILDLIB}\",\"${ARG_BUILDOBJ}\"\))

  set(compile_macro_command ${root_compile_macro}
                            ${BuildScriptFile}${BuildScriptArg}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  if(ARG_DEPENDS)
    set(deps ${ARG_DEPENDS})
  endif()

  ROOTTEST_TARGETNAME_FROM_FILE(COMPILE_MACRO_TEST ${filename})

  set(compile_target ${COMPILE_MACRO_TEST}-compile-macro)

  add_custom_target(${compile_target}
                    COMMAND ${compile_macro_command}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    VERBATIM)

  if(ARG_DEPENDS)
    add_dependencies(${compile_target} ${deps})
  endif()

  set(COMPILE_MACRO_TEST ${COMPILE_MACRO_TEST}-build)

  add_test(NAME ${COMPILE_MACRO_TEST}
           COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
                                    --target ${compile_target}${fast}
                                    -- ${always-make})
  set_property(TEST ${COMPILE_MACRO_TEST} PROPERTY FAIL_REGULAR_EXPRESSION "Warning in")
  set_property(TEST ${COMPILE_MACRO_TEST} PROPERTY ENVIRONMENT ${ROOTTEST_ENVIRONMENT})
  if(CMAKE_GENERATOR MATCHES Ninja)
    set_property(TEST ${COMPILE_MACRO_TEST} PROPERTY RUN_SERIAL true)
  endif()

endmacro(ROOTTEST_COMPILE_MACRO)

#-------------------------------------------------------------------------------
#
# macro ROOTTEST_GENERATE_DICTIONARY(<dictname>
#                                    [LINKDEF linkdef]
#                                    [DEPENDS deps]
#                                    [OPTIONS opts]
#                                    [files ...]      )
#
# This macro generates a dictionary <dictname> from the provided <files>.
# A test that performs the dictionary generation is created.  The target name of
# the created test is stored in the variable GENERATE_DICTIONARY_TEST which can
# be accessed by the calling CMakeLists.txt in order to manage dependencies.
#
#-------------------------------------------------------------------------------
macro(ROOTTEST_GENERATE_DICTIONARY dictname)
  CMAKE_PARSE_ARGUMENTS(ARG "NO_ROOTMAP" "" "LINKDEF;DEPENDS;OPTIONS" ${ARGN})

  set(CMAKE_ROOTTEST_DICT ON)

  if(ARG_NO_ROOTMAP)
    set(CMAKE_ROOTTEST_NOROOTMAP ON)
  endif()

  # roottest dictionaries do not need to be relocatable. Instead, allow
  # dictionaries to find the input headers even from the source directory
  # - without ROOT_INCLUDE_PATH - by passing the full path to rootcling:
  set(FULL_PATH_HEADERS )
  foreach(hdr ${ARG_UNPARSED_ARGUMENTS})
    if(IS_ABSOLUTE ${hdr})
      list(APPEND FULL_PATH_HEADERS ${hdr})
    else()
      list(APPEND FULL_PATH_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/${hdr})
    endif()
  endforeach()

  ROOT_GENERATE_DICTIONARY(${dictname} ${FULL_PATH_HEADERS}
                           MODULE ${dictname}
                           LINKDEF ${ARG_LINKDEF}
                           OPTIONS ${ARG_OPTIONS}
                           DEPENDENCIES ${ARG_DEPENDS})

  ROOTTEST_TARGETNAME_FROM_FILE(GENERATE_DICTIONARY_TEST ${dictname})

  set(GENERATE_DICTIONARY_TEST ${GENERATE_DICTIONARY_TEST}-build)

  set(targetname_libgen ${dictname}libgen)

  add_library(${targetname_libgen} EXCLUDE_FROM_ALL SHARED ${dictname}.cxx)
  set_target_properties(${targetname_libgen} PROPERTIES  ${ROOT_LIBRARY_PROPERTIES} )
  target_link_libraries(${targetname_libgen} ${ROOT_LIBRARIES})

  set_target_properties(${targetname_libgen} PROPERTIES PREFIX "")

  set_property(TARGET ${targetname_libgen}
               PROPERTY OUTPUT_NAME ${dictname})

  set_property(TARGET ${targetname_libgen}
               APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

  add_dependencies(${targetname_libgen} ${dictname})

  add_test(NAME ${GENERATE_DICTIONARY_TEST}
           COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
                                    --target  ${targetname_libgen}${fast}
                                    -- ${always-make})

  set_property(TEST ${GENERATE_DICTIONARY_TEST} PROPERTY ENVIRONMENT ${ROOTTEST_ENVIRONMENT})
  if(CMAKE_GENERATOR MATCHES Ninja)
    set_property(TEST ${GENERATE_DICTIONARY_TEST} PROPERTY RUN_SERIAL true)
  endif()


endmacro(ROOTTEST_GENERATE_DICTIONARY)

#-------------------------------------------------------------------------------
#
# macro ROOTTEST_GENERATE_REFLEX_DICTIONARY(<targetname> <dictionary>
#                                              [SELECTION sel...]
#                                              [headerfiles...]     )
#
# This macro generates a reflexion dictionary and creates a shared library.
# A test that performs the dictionary generation is created.  The target name of
# the created test is stored in the variable GENERATE_REFLEX_TEST which can
# be accessed by the calling CMakeLists.txt in order to manage dependencies.
#
#-------------------------------------------------------------------------------
macro(ROOTTEST_GENERATE_REFLEX_DICTIONARY dictionary)
  CMAKE_PARSE_ARGUMENTS(ARG "NO_ROOTMAP" "SELECTION;LIBNAME" "LIBRARIES;OPTIONS"  ${ARGN})

  set(CMAKE_ROOTTEST_DICT ON)

  if(ARG_NO_ROOTMAP)
    set(CMAKE_ROOTTEST_NOROOTMAP ON)
  else()
    set(CMAKE_ROOTTEST_NOROOTMAP OFF)
  endif()

  set(ROOT_genreflex_cmd ${ROOT_BINARY_DIR}/genreflex)

  ROOTTEST_TARGETNAME_FROM_FILE(targetname ${dictionary})

  set(targetname_libgen ${targetname}-libgen)

  # targetname_dictgen is the targetname constructed by the
  # REFLEX_GENERATE_DICTIONARY macro and is used as a dependency.
  set(targetname_dictgen ${targetname}-dictgen)

  if(ARG_OPTIONS)
    set(reflex_pass_options OPTIONS ${ARG_OPTIONS})
  endif()

  REFLEX_GENERATE_DICTIONARY(${dictionary} ${ARG_UNPARSED_ARGUMENTS}
                             SELECTION ${ARG_SELECTION}
                             ${reflex_pass_options})

  add_library(${targetname_libgen} EXCLUDE_FROM_ALL SHARED ${gensrcdict})
  set_target_properties(${targetname_libgen} PROPERTIES  ${ROOT_LIBRARY_PROPERTIES} )

  if(ARG_LIBNAME)
    set_target_properties(${targetname_libgen} PROPERTIES PREFIX "")
    set_property(TARGET ${targetname_libgen}
                 PROPERTY OUTPUT_NAME ${ARG_LIBNAME})
  else()
    set_property(TARGET ${targetname_libgen}
                 PROPERTY OUTPUT_NAME ${dictionary}_dictrflx)
  endif()

  add_dependencies(${targetname_libgen}
                   ${targetname_dictgen})

  target_link_libraries(${targetname_libgen}
                        ${ARG_LIBRARIES}
                        ${ROOT_LIBRARIES})

  set_property(TARGET ${targetname_libgen}
               APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

  set(GENERATE_REFLEX_TEST ${targetname_libgen}-build)

  add_test(NAME ${GENERATE_REFLEX_TEST}
           COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
                                    --target ${targetname_libgen}${fast}
                                    -- ${always-make})

  set_property(TEST ${GENERATE_REFLEX_TEST} PROPERTY ENVIRONMENT ${ROOTTEST_ENVIRONMENT})
  if(CMAKE_GENERATOR MATCHES Ninja)
    set_property(TEST ${GENERATE_REFLEX_TEST} PROPERTY RUN_SERIAL true)
  endif()


endmacro(ROOTTEST_GENERATE_REFLEX_DICTIONARY)

#-------------------------------------------------------------------------------
#
# macro ROOTTEST_GENERATE_EXECUTABLE(<executable>
#                                    [LIBRARIES lib1 lib2 ...]
#                                    [COMPILE_FLAGS flag1 flag2 ...]
#                                    [DEPENDS ...]  )
# This macro generates an executable the the building of it becames a test
#
#-------------------------------------------------------------------------------
macro(ROOTTEST_GENERATE_EXECUTABLE executable)
  CMAKE_PARSE_ARGUMENTS(ARG "" "" "LIBRARIES;COMPILE_FLAGS;DEPENDS" ${ARGN})

  set(exec_sources)
  foreach(exec_src_file ${ARGN})
    get_filename_component(exec_src_file ${exec_src_file} ABSOLUTE)
    if(EXISTS ${exec_src_file})
      list(APPEND exec_sources ${exec_src_file})
    endif()
  endforeach()

  add_executable(${executable} EXCLUDE_FROM_ALL ${exec_sources})
  set_target_properties(${executable} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  set_property(TARGET ${executable}
               APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

  if(ARG_DEPENDS)
    add_dependencies(${executable} ${ARG_DEPENDS})
  endif()

  if(ARG_LIBRARIES)
    target_link_libraries(${executable} ${ARG_LIBRARIES})
  endif()

  if(ARG_COMPILE_FLAGS)
    set_target_properties(${executable} PROPERTIES COMPILE_FLAGS ${ARG_COMPILE_FLAGS})
  endif()

  ROOTTEST_TARGETNAME_FROM_FILE(GENERATE_EXECUTABLE_TEST ${executable})

  set(GENERATE_EXECUTABLE_TEST ${GENERATE_EXECUTABLE_TEST}-build)

  add_test(NAME ${GENERATE_EXECUTABLE_TEST}
           COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
                                    --target ${executable}${fast}
                                    -- ${always-make})
  set_property(TEST ${GENERATE_EXECUTABLE_TEST} PROPERTY ENVIRONMENT ${ROOTTEST_ENVIRONMENT})
  if(CMAKE_GENERATOR MATCHES Ninja)
    set_property(TEST ${GENERATE_EXECUTABLE_TEST} PROPERTY RUN_SERIAL true)
  endif()

endmacro()

#-------------------------------------------------------------------------------
#
# function ROOTTEST_ADD_OLDTEST()
#
# This function defines a single tests in the current directory that calls the legacy
# make system to run the defined tests.
#
#-------------------------------------------------------------------------------
function(ROOTTEST_ADD_OLDTEST)
  CMAKE_PARSE_ARGUMENTS(ARG "" "" "LABELS;TIMEOUT" ${ARGN})

  ROOTTEST_ADD_TEST( make
                     COMMAND make cleantest
                     WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR}
                     DEPENDS roottest-root-io-event
                     LABELS ${ARG_LABELS} TIMEOUT ${ARG_TIMEOUT})
endfunction()
