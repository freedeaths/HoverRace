
# Process and install a launch script.
function(copy_wrapper_script src dest)
	configure_file(${src} ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${dest})
	file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${dest}
		DESTINATION ${CMAKE_BINARY_DIR}
		FILE_PERMISSIONS
			OWNER_READ OWNER_WRITE OWNER_EXECUTE
			GROUP_READ GROUP_WRITE GROUP_EXECUTE
			WORLD_READ             WORLD_EXECUTE)
endfunction()

