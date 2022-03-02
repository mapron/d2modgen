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
	md5/md5.cpp
	zlib/adler32.c
	zlib/crc32.c
	zlib/inffast.c
	zlib/inflate.c
	zlib/inftrees.c
	zlib/trees.c
	zlib/zutil.c
	
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
	md5/md5.h
	zlib/deflate.h
	)
set(cascSrcFull)
foreach(file ${cascSrc})
	list(APPEND cascSrcFull ${cascInc}/${file})
endforeach()

add_library(casclib STATIC ${cascSrcFull})
target_include_directories(casclib PUBLIC ${cascInc})
