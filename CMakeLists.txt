# Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
# SPDX-License-Identifier: MIT
# See LICENSE file for details.

# Common declarations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE STRING "" FORCE)

cmake_minimum_required(VERSION 3.12...3.19)

project(modgen)


# Qt Setup
# QtNetwork is only for QtMultimedia
set(QT_MODULE_LIST Core Gui Widgets)
find_package(Qt5 COMPONENTS ${QT_MODULE_LIST} REQUIRED)
add_definitions(-DCASCLIB_NO_AUTO_LINK_LIBRARY -DSTORMLIB_NO_AUTO_LINK)

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

# stormlib
set(stormRoot ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stormlib/src)
set(stormInc ${stormRoot})
set(stormSrc 
	adpcm/adpcm.cpp
	bzip2/blocksort.c
	bzip2/bzlib.c
	bzip2/compress.c
	bzip2/crctable.c
	bzip2/decompress.c
	bzip2/huffman.c
	bzip2/randtable.c
	FileStream.cpp
	huffman/huff.cpp
	jenkins/lookup3.c
	libtomcrypt/src/hashes/hash_memory.c
	libtomcrypt/src/hashes/md5.c
	libtomcrypt/src/hashes/sha1.c
	libtomcrypt/src/math/ltm_desc.c
	libtomcrypt/src/math/multi.c
	libtomcrypt/src/math/rand_prime.c
	libtomcrypt/src/misc/base64_decode.c
	libtomcrypt/src/misc/crypt_argchk.c
	libtomcrypt/src/misc/crypt_find_hash.c
	libtomcrypt/src/misc/crypt_find_prng.c
	libtomcrypt/src/misc/crypt_hash_descriptor.c
	libtomcrypt/src/misc/crypt_hash_is_valid.c
	libtomcrypt/src/misc/crypt_libc.c
	libtomcrypt/src/misc/crypt_ltc_mp_descriptor.c
	libtomcrypt/src/misc/crypt_prng_descriptor.c
	libtomcrypt/src/misc/crypt_prng_is_valid.c
	libtomcrypt/src/misc/crypt_register_hash.c
	libtomcrypt/src/misc/crypt_register_prng.c
	libtomcrypt/src/misc/zeromem.c
	libtomcrypt/src/pk/asn1/der_decode_bit_string.c
	libtomcrypt/src/pk/asn1/der_decode_boolean.c
	libtomcrypt/src/pk/asn1/der_decode_choice.c
	libtomcrypt/src/pk/asn1/der_decode_ia5_string.c
	libtomcrypt/src/pk/asn1/der_decode_integer.c
	libtomcrypt/src/pk/asn1/der_decode_object_identifier.c
	libtomcrypt/src/pk/asn1/der_decode_octet_string.c
	libtomcrypt/src/pk/asn1/der_decode_printable_string.c
	libtomcrypt/src/pk/asn1/der_decode_sequence_ex.c
	libtomcrypt/src/pk/asn1/der_decode_sequence_flexi.c
	libtomcrypt/src/pk/asn1/der_decode_sequence_multi.c
	libtomcrypt/src/pk/asn1/der_decode_short_integer.c
	libtomcrypt/src/pk/asn1/der_decode_utctime.c
	libtomcrypt/src/pk/asn1/der_decode_utf8_string.c
	libtomcrypt/src/pk/asn1/der_encode_bit_string.c
	libtomcrypt/src/pk/asn1/der_encode_boolean.c
	libtomcrypt/src/pk/asn1/der_encode_ia5_string.c
	libtomcrypt/src/pk/asn1/der_encode_integer.c
	libtomcrypt/src/pk/asn1/der_encode_object_identifier.c
	libtomcrypt/src/pk/asn1/der_encode_octet_string.c
	libtomcrypt/src/pk/asn1/der_encode_printable_string.c
	libtomcrypt/src/pk/asn1/der_encode_sequence_ex.c
	libtomcrypt/src/pk/asn1/der_encode_sequence_multi.c
	libtomcrypt/src/pk/asn1/der_encode_set.c
	libtomcrypt/src/pk/asn1/der_encode_setof.c
	libtomcrypt/src/pk/asn1/der_encode_short_integer.c
	libtomcrypt/src/pk/asn1/der_encode_utctime.c
	libtomcrypt/src/pk/asn1/der_encode_utf8_string.c
	libtomcrypt/src/pk/asn1/der_length_bit_string.c
	libtomcrypt/src/pk/asn1/der_length_boolean.c
	libtomcrypt/src/pk/asn1/der_length_ia5_string.c
	libtomcrypt/src/pk/asn1/der_length_integer.c
	libtomcrypt/src/pk/asn1/der_length_object_identifier.c
	libtomcrypt/src/pk/asn1/der_length_octet_string.c
	libtomcrypt/src/pk/asn1/der_length_printable_string.c
	libtomcrypt/src/pk/asn1/der_length_sequence.c
	libtomcrypt/src/pk/asn1/der_length_short_integer.c
	libtomcrypt/src/pk/asn1/der_length_utctime.c
	libtomcrypt/src/pk/asn1/der_length_utf8_string.c
	libtomcrypt/src/pk/asn1/der_sequence_free.c
	libtomcrypt/src/pk/ecc/ltc_ecc_map.c
	libtomcrypt/src/pk/ecc/ltc_ecc_mul2add.c
	libtomcrypt/src/pk/ecc/ltc_ecc_mulmod.c
	libtomcrypt/src/pk/ecc/ltc_ecc_points.c
	libtomcrypt/src/pk/ecc/ltc_ecc_projective_add_point.c
	libtomcrypt/src/pk/ecc/ltc_ecc_projective_dbl_point.c
	libtomcrypt/src/pk/pkcs1/pkcs_1_mgf1.c
	libtomcrypt/src/pk/pkcs1/pkcs_1_oaep_decode.c
	libtomcrypt/src/pk/pkcs1/pkcs_1_pss_decode.c
	libtomcrypt/src/pk/pkcs1/pkcs_1_pss_encode.c
	libtomcrypt/src/pk/pkcs1/pkcs_1_v1_5_decode.c
	libtomcrypt/src/pk/pkcs1/pkcs_1_v1_5_encode.c
	libtomcrypt/src/pk/rsa/rsa_exptmod.c
	libtomcrypt/src/pk/rsa/rsa_free.c
	libtomcrypt/src/pk/rsa/rsa_import.c
	libtomcrypt/src/pk/rsa/rsa_make_key.c
	libtomcrypt/src/pk/rsa/rsa_sign_hash.c
	libtomcrypt/src/pk/rsa/rsa_verify_hash.c
	libtomcrypt/src/pk/rsa/rsa_verify_simple.c
	libtommath/bncore.c
	libtommath/bn_fast_mp_invmod.c
	libtommath/bn_fast_mp_montgomery_reduce.c
	libtommath/bn_fast_s_mp_mul_digs.c
	libtommath/bn_fast_s_mp_mul_high_digs.c
	libtommath/bn_fast_s_mp_sqr.c
	libtommath/bn_mp_2expt.c
	libtommath/bn_mp_abs.c
	libtommath/bn_mp_add.c
	libtommath/bn_mp_addmod.c
	libtommath/bn_mp_add_d.c
	libtommath/bn_mp_and.c
	libtommath/bn_mp_clamp.c
	libtommath/bn_mp_clear.c
	libtommath/bn_mp_clear_multi.c
	libtommath/bn_mp_cmp.c
	libtommath/bn_mp_cmp_d.c
	libtommath/bn_mp_cmp_mag.c
	libtommath/bn_mp_cnt_lsb.c
	libtommath/bn_mp_copy.c
	libtommath/bn_mp_count_bits.c
	libtommath/bn_mp_div.c
	libtommath/bn_mp_div_2.c
	libtommath/bn_mp_div_2d.c
	libtommath/bn_mp_div_3.c
	libtommath/bn_mp_div_d.c
	libtommath/bn_mp_dr_is_modulus.c
	libtommath/bn_mp_dr_reduce.c
	libtommath/bn_mp_dr_setup.c
	libtommath/bn_mp_exch.c
	libtommath/bn_mp_exptmod.c
	libtommath/bn_mp_exptmod_fast.c
	libtommath/bn_mp_expt_d.c
	libtommath/bn_mp_exteuclid.c
	libtommath/bn_mp_fread.c
	libtommath/bn_mp_fwrite.c
	libtommath/bn_mp_gcd.c
	libtommath/bn_mp_get_int.c
	libtommath/bn_mp_grow.c
	libtommath/bn_mp_init.c
	libtommath/bn_mp_init_copy.c
	libtommath/bn_mp_init_multi.c
	libtommath/bn_mp_init_set.c
	libtommath/bn_mp_init_set_int.c
	libtommath/bn_mp_init_size.c
	libtommath/bn_mp_invmod.c
	libtommath/bn_mp_invmod_slow.c
	libtommath/bn_mp_is_square.c
	libtommath/bn_mp_jacobi.c
	libtommath/bn_mp_karatsuba_mul.c
	libtommath/bn_mp_karatsuba_sqr.c
	libtommath/bn_mp_lcm.c
	libtommath/bn_mp_lshd.c
	libtommath/bn_mp_mod.c
	libtommath/bn_mp_mod_2d.c
	libtommath/bn_mp_mod_d.c
	libtommath/bn_mp_montgomery_calc_normalization.c
	libtommath/bn_mp_montgomery_reduce.c
	libtommath/bn_mp_montgomery_setup.c
	libtommath/bn_mp_mul.c
	libtommath/bn_mp_mulmod.c
	libtommath/bn_mp_mul_2.c
	libtommath/bn_mp_mul_2d.c
	libtommath/bn_mp_mul_d.c
	libtommath/bn_mp_neg.c
	libtommath/bn_mp_n_root.c
	libtommath/bn_mp_or.c
	libtommath/bn_mp_prime_fermat.c
	libtommath/bn_mp_prime_is_divisible.c
	libtommath/bn_mp_prime_is_prime.c
	libtommath/bn_mp_prime_miller_rabin.c
	libtommath/bn_mp_prime_next_prime.c
	libtommath/bn_mp_prime_rabin_miller_trials.c
	libtommath/bn_mp_prime_random_ex.c
	libtommath/bn_mp_radix_size.c
	libtommath/bn_mp_radix_smap.c
	libtommath/bn_mp_rand.c
	libtommath/bn_mp_read_radix.c
	libtommath/bn_mp_read_signed_bin.c
	libtommath/bn_mp_read_unsigned_bin.c
	libtommath/bn_mp_reduce.c
	libtommath/bn_mp_reduce_2k.c
	libtommath/bn_mp_reduce_2k_l.c
	libtommath/bn_mp_reduce_2k_setup.c
	libtommath/bn_mp_reduce_2k_setup_l.c
	libtommath/bn_mp_reduce_is_2k.c
	libtommath/bn_mp_reduce_is_2k_l.c
	libtommath/bn_mp_reduce_setup.c
	libtommath/bn_mp_rshd.c
	libtommath/bn_mp_set.c
	libtommath/bn_mp_set_int.c
	libtommath/bn_mp_shrink.c
	libtommath/bn_mp_signed_bin_size.c
	libtommath/bn_mp_sqr.c
	libtommath/bn_mp_sqrmod.c
	libtommath/bn_mp_sqrt.c
	libtommath/bn_mp_sub.c
	libtommath/bn_mp_submod.c
	libtommath/bn_mp_sub_d.c
	libtommath/bn_mp_toom_mul.c
	libtommath/bn_mp_toom_sqr.c
	libtommath/bn_mp_toradix.c
	libtommath/bn_mp_toradix_n.c
	libtommath/bn_mp_to_signed_bin.c
	libtommath/bn_mp_to_signed_bin_n.c
	libtommath/bn_mp_to_unsigned_bin.c
	libtommath/bn_mp_to_unsigned_bin_n.c
	libtommath/bn_mp_unsigned_bin_size.c
	libtommath/bn_mp_xor.c
	libtommath/bn_mp_zero.c
	libtommath/bn_prime_tab.c
	libtommath/bn_reverse.c
	libtommath/bn_s_mp_add.c
	libtommath/bn_s_mp_exptmod.c
	libtommath/bn_s_mp_mul_digs.c
	libtommath/bn_s_mp_mul_high_digs.c
	libtommath/bn_s_mp_sqr.c
	libtommath/bn_s_mp_sub.c
	lzma/C/LzFind.c
	lzma/C/LzFindMt.c
	lzma/C/LzmaDec.c
	lzma/C/LzmaEnc.c
	lzma/C/Threads.c
	pklib/explode.c
	pklib/implode.c
	SBaseCommon.cpp
	SBaseFileTable.cpp
	SBaseSubTypes.cpp
	SCompression.cpp
	SFileAddFile.cpp
	SFileAttributes.cpp
	SFileCompactArchive.cpp
	SFileCreateArchive.cpp
	SFileExtractFile.cpp
	SFileFindFile.cpp
	SFileGetFileInfo.cpp
	SFileListFile.cpp
	SFileOpenArchive.cpp
	SFileOpenFileEx.cpp
	SFilePatchArchives.cpp
	SFileReadFile.cpp
	SFileVerify.cpp
	sparse/sparse.cpp
	zlib/adler32.c
	zlib/compress_zlib.c
	zlib/crc32.c
	zlib/deflate.c
	zlib/inffast.c
	zlib/inflate.c
	zlib/inftrees.c
	zlib/trees.c
	zlib/zutil.c
	#DllMain.c
	)
set(stormSrcFull)
foreach(file ${stormSrc})
	list(APPEND stormSrcFull ${stormInc}/${file})
endforeach()

add_library(stormlib STATIC ${stormSrcFull})
target_include_directories(stormlib PUBLIC ${stormInc})

# source
set(sourceRoot ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB src 
	${sourceRoot}/*.hpp 
	${sourceRoot}/*.cpp
	${sourceRoot}/Storage/*.hpp 
	${sourceRoot}/Storage/*.cpp
	${sourceRoot}/ConfigPages/*.hpp 
	${sourceRoot}/ConfigPages/*.cpp
	${sourceRoot}/ValueEditors/*.hpp 
	${sourceRoot}/ValueEditors/*.cpp
	)

if (WIN32 AND NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(w32 "WIN32")
endif()
add_executable(modgen ${w32} ${src})
set_target_properties(modgen PROPERTIES AUTOMOC ON AUTOUIC OFF AUTORCC OFF)
target_include_directories(modgen PRIVATE ${sourceRoot})
target_link_libraries(modgen PRIVATE Qt5::Widgets casclib stormlib)
install(TARGETS modgen RUNTIME DESTINATION bin)


#[[
########## Installation (Win32 only at the moment) ##########
#]]

if(WIN32)
# Qt install - plugins
set(QtPluginsList)
if(APPLE)
    list(APPEND QtPluginsList Qt5::QCocoaIntegrationPlugin)
elseif(WIN32)
    list(APPEND QtPluginsList Qt5::QWindowsIntegrationPlugin Qt5::QWindowsVistaStylePlugin)
endif()
foreach(plugin ${QtPluginsList})
    get_target_property(imploc_RELEASE ${plugin} IMPORTED_LOCATION_RELEASE)
    get_target_property(imploc_DEBUG   ${plugin} IMPORTED_LOCATION_DEBUG)

    get_filename_component(pluginPath ${imploc_RELEASE} DIRECTORY)
    get_filename_component(pluginDir ${pluginPath} NAME)
    if (EXISTS "${imploc_DEBUG}")
        install(FILES ${imploc_DEBUG}   DESTINATION bin/${pluginDir} CONFIGURATIONS Debug )
    endif()
    install(FILES ${imploc_RELEASE} DESTINATION bin/${pluginDir} CONFIGURATIONS Release )
endforeach()

# Qt install - shared libs
foreach(qt5Module ${QT_MODULE_LIST})
    get_target_property(imploc_RELEASE Qt5::${qt5Module} IMPORTED_LOCATION_RELEASE)
    get_target_property(imploc_DEBUG   Qt5::${qt5Module} IMPORTED_LOCATION_DEBUG)
    if (EXISTS "${imploc_DEBUG}")
        install(FILES ${imploc_DEBUG}   DESTINATION bin CONFIGURATIONS Debug    )
    endif()
    install(FILES ${imploc_RELEASE} DESTINATION bin CONFIGURATIONS Release  )
endforeach()

# Compiler runtime
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
if (NOT(CMAKE_BUILD_TYPE STREQUAL Release))
    set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
endif()
include (InstallRequiredSystemLibraries)

endif(WIN32)
