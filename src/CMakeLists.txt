add_compile_definitions("RIBOLLITA_VERSION=\"${RIBOLLITA_VERSION}\"")

add_library(
        ribollita MODULE
        ribollita.cpp
        mol.cpp
        mol_ops.cpp
        mol_formats.cpp
        mol_descriptors.cpp
        )

set_target_properties(ribollita PROPERTIES PREFIX "")

target_link_libraries(ribollita PUBLIC
                      RDKit::Descriptors RDKit::SmilesParse)

install(TARGETS ribollita LIBRARY DESTINATION ${PG_PKGLIBDIR})
install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/ribollita.control ${CMAKE_CURRENT_SOURCE_DIR}/ribollita--${RIBOLLITA_VERSION}.sql
        DESTINATION ${PG_SHAREDIR}/extension)

