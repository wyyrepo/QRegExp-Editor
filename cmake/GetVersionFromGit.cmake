# Requires CMake 2.6 or newer (it uses the 'function' command)

include(GetGitRevisionDescription)

function(GetGitVersion _GitVersion)
	git_describe(GIT_OUT --match "v[0-9]*" --abbrev=4)
	if (NOT ${GIT_OUT} MATCHES "NOTFOUND")
		# get rid of the v
		string(REGEX  REPLACE "^v"  ""  STRIPPED_GIT_OUT ${GIT_OUT})
		set(${_GitVersion} ${STRIPPED_GIT_OUT} PARENT_SCOPE)
	else()
		set(${_GitVersion} ${GIT_OUT} PARENT_SCOPE)

	endif()
endfunction()
