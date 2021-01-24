# cmake -D PostgreSQL_ROOT=/opt/postgresql ..

cmake_minimum_required(VERSION 3.14)

project(ribollita)

set(RIBOLLITA_VERSION "0.0")

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra) # -pedantic -Werror)
endif()

find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

find_program(PG_CONFIG pg_config REQUIRED)

macro(run_pg_config arg var)
    execute_process(COMMAND ${PG_CONFIG} ${arg}
                    RESULT_VARIABLE pgsql_config_result
                    OUTPUT_VARIABLE ${var}
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(NOT ${pgsql_config_result} EQUAL 0 OR NOT ${var})
        message(FATAL_ERROR "${PG_CONFIG} ${arg} failed")
    endif()
endmacro()

run_pg_config(--bindir PG_BINDIR)
run_pg_config(--sharedir PG_SHAREDIR)
run_pg_config(--pkglibdir PG_PKGLIBDIR)

find_package(RDKit 2020.09.1 REQUIRED)
include_directories(${RDKit_INCLUDE_DIRS})

add_library(ribollita MODULE
        ribollita.cpp)

set_target_properties(ribollita PROPERTIES PREFIX "")

install(TARGETS ribollita LIBRARY DESTINATION ${PG_PKGLIBDIR})
install(FILES
        ${CMAKE_SOURCE_DIR}/ribollita.control ${CMAKE_SOURCE_DIR}/ribollita--${RIBOLLITA_VERSION}.sql
        DESTINATION ${PG_SHAREDIR}/extension)

run_pg_config (--pgxs PGXS_MAKEFILE)
get_filename_component(PGXS_MAKEFILESDIR ${PGXS_MAKEFILE} PATH)
set(PG_REGRESSDIR ${PGXS_MAKEFILESDIR}/../test/regress)

find_program(PG_REGRESS pg_regress HINTS ${PG_REGRESSDIR} REQUIRED)

set(REGRESS create_extension copytext reversetext)

add_custom_target(regression_test
                  COMMAND ${PG_REGRESS}
                    --inputdir="${CMAKE_SOURCE_DIR}/regress"
                    --outputdir="${CMAKE_BINARY_DIR}"
                    --bindir="${PG_BINDIR}"
                    --dbname regression
                    ${REGRESS}
                  COMMENT Run pg_regress
                  USES_TERMINAL
                  )
