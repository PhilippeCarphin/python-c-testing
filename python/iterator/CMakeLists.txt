
Python3_add_library(_iterator MODULE iterator.c)

target_link_libraries(_iterator PRIVATE ${PYTHON_LIBRARIES})
target_include_directories(_iterator PRIVATE ${PYTHON_INCLUDE_DIRS})
configure_file(setup.in.sh ${CMAKE_BINARY_DIR}/setup.sh @ONLY)

set(python_files
    __init__.py
)

foreach(f IN LISTS python_files)
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/${f} ${CMAKE_CURRENT_BINARY_DIR}/${f} SYMBOLIC)
endforeach()

install(FILES ${python_files} DESTINATION lib/python/iterator)
install(TARGETS _iterator DESTINATION lib/python/iterator)

add_custom_target(test_py
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND bash -c \". ${CMAKE_BINARY_DIR}/setup.sh && python3 ${CMAKE_SOURCE_DIR}/test.py\"
)
add_dependencies(test_py _iterator)


