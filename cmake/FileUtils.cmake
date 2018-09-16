function(FileReadFirstLine fileData _fileName)
	file(STRINGS ${_fileName} data LIMIT_COUNT 1)
	set(${fileData} ${data} PARENT_SCOPE)
endfunction()
