add_library(ribollita MODULE
        ribollita.cpp)

set_target_properties(ribollita PROPERTIES PREFIX "")

install(TARGETS ribollita LIBRARY DESTINATION ${PG_PKGLIBDIR})
install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/ribollita.control ${CMAKE_CURRENT_SOURCE_DIR}/ribollita--${RIBOLLITA_VERSION}.sql
        DESTINATION ${PG_SHAREDIR}/extension)

