include(GetVersionFromGit)
include(FileUtils)

function(GetQRegExpEditorVersion _version)
	set(VERSION_FILE "${SOURCE_DIR}/version")
	if(NOT EXISTS "${VERSION_FILE}")
		GetGitVersion(_git_version)
		if (${_git_version} MATCHES "NOTFOUND")
			message(WARNING "Using default version")
			set(${_version} ${QREGEXP-EDITOR_DEFAULT_VERSION}
				PARENT_SCOPE)
			return()
		endif()
		set(${_version} ${_git_version} PARENT_SCOPE)
		return()
	else()
                # we are in a release pack
                # read the version from the version file
		FileReadFirstLine(FIRST_LINE "${SOURCE_DIR}/version")
                set(${_version} ${FIRST_LINE} PARENT_SCOPE)
		return()
	endif()
endfunction()
