cmake_minimum_required(VERSION 3.16)

if(WIN32)
	set(OS_SPECIFIC_DIR "win32")
else()
	set(OS_SPECIFIC_DIR "linux")
endif()

add_executable(helper
		${OS_SPECIFIC_DIR}/main.cpp
		${OS_SPECIFIC_DIR}/privileges.cpp
		${OS_SPECIFIC_DIR}/privileges.h)
source_group(src FILES REGULAR_EXPRESSION ".*")
set_target_properties(helper PROPERTIES
	CXX_STANDARD 20
)

if (MSVC)
	target_compile_definitions(helper PRIVATE UNICODE)
	set_target_properties(helper PROPERTIES
		LINK_FLAGS
		"/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
	)
	target_link_options(helper PRIVATE
		$<$<CONFIG:RelWithDebInfo>:/LTCG /INCREMENTAL:NO /OPT:REF /OPT:ICF>
	)
else()
	target_link_options(helper PRIVATE
			$<$<CONFIG:RelWithDebInfo>:-flto=auto>
	)
endif()

install(FILES $<TARGET_FILE:helper> DESTINATION bin
	PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
)
if (WIN32)
	install(FILES $<TARGET_PDB_FILE:helper> DESTINATION pdb)
endif()
