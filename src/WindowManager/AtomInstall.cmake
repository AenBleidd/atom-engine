# Installation instructions for the window manager

INSTALL (
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin/${ROOT_PROJECT_NAME}
	LIBRARY DESTINATION lib/${ROOT_PROJECT_NAME}
	ARCHIVE DESTINATION lib/static/${ROOT_PROJECT_NAME}
)

# Add uninstall target
ADD_CUSTOM_TARGET ( uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )
