project (schedule)

aux_source_directory (src SRC)
add_executable (${PROJECT_NAME} ${SRC})

target_include_directories (${PROJECT_NAME} PUBLIC inc)
