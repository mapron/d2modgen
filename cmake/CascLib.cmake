# casclib
set(cascRoot ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/casclib/src)
set(cascInc ${cascRoot})
set(cascSrc 
	CascDecompress.cpp
	CascDecrypt.cpp
	CascDumpData.cpp
	CascFiles.cpp
	CascFindFile.cpp
	CascIndexFiles.cpp
	CascOpenFile.cpp
	CascOpenStorage.cpp
	CascReadFile.cpp
	CascRootFile_Diablo3.cpp
	CascRootFile_Install.cpp
	CascRootFile_MNDX.cpp
	CascRootFile_OW.cpp
	CascRootFile_Text.cpp
	CascRootFile_TVFS.cpp
	CascRootFile_WoW.cpp
	common/Common.cpp
	common/Directory.cpp
	common/Csv.cpp
	common/FileStream.cpp
	common/FileTree.cpp
	common/ListFile.cpp
	common/RootHandler.cpp
	common/Mime.cpp
	common/Sockets.cpp
	# DllMain.c
	jenkins/lookup3.c
	hashes/md5.cpp
	hashes/sha1.cpp
	zlib/adler32.c
	zlib/crc32.c
	zlib/inffast.c
	zlib/inflate.c
	zlib/inftrees.c
	zlib/trees.c
	zlib/zutil.c
	overwatch/apm.cpp
	overwatch/cmf.cpp
	overwatch/aes.cpp
	
	CascCommon.h
	CascLib.h
	CascPort.h
	CascStructs.h
	common/Common.h
	common/Directory.h
	common/Csv.h
	common/Array.h
	common/FileTree.h
	common/ListFile.h
	common/Map.h
	common/RootHandler.h
	common/Mime.h
	common/Sockets.h
	#FileStream.h
	hashes/md5.h
	hashes/sha1.h
	zlib/deflate.h
	)
set(cascSrcFull)
foreach(file ${cascSrc})
	list(APPEND cascSrcFull ${cascInc}/${file})
endforeach()

add_library(casclib STATIC ${cascSrcFull})
target_include_directories(casclib PUBLIC ${cascInc})
