run_pg_config(--pgxs PGXS_MAKEFILE)
get_filename_component(PGXS_MAKEFILESDIR ${PGXS_MAKEFILE} PATH)
set(PG_REGRESSDIR ${PGXS_MAKEFILESDIR}/../test/regress)

find_program(PG_REGRESS pg_regress HINTS ${PG_REGRESSDIR} REQUIRED)

set(REGRESS
  create_extension
  mol_formats
  mol_ops
  mol_descriptors
  )

add_custom_target(regression_test
                  COMMAND ${PG_REGRESS}
                    --inputdir="${CMAKE_CURRENT_SOURCE_DIR}"
                    --outputdir="${CMAKE_CURRENT_BINARY_DIR}"
                    --bindir="${PG_BINDIR}"
                    --dbname regression
                    ${REGRESS}
                  COMMENT Run pg_regress
                  USES_TERMINAL
                  )
