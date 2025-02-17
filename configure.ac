#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#
#  Copyright (c) 2016, The OpenThread Authors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the copyright holder nor the
#     names of its contributors may be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#

#
# Declare autoconf version requirements
#
AC_PREREQ([2.68])

#
# Initialize autoconf for the package
#
AC_INIT([OPENTHREAD],
        m4_esyscmd([third_party/nlbuild-autotools/repo/scripts/mkversion -b `cat .default-version` .]),
        [openthread-devel@googlegroups.com],
        [openthread],
        [http://github.com/openthread/openthread])

# Tell the rest of the build system the absolute path where the
# nlbuild-autotools repository is rooted at.

AC_SUBST(nlbuild_autotools_stem,[third_party/nlbuild-autotools/repo])
AC_SUBST(abs_top_nlbuild_autotools_dir,[\${abs_top_srcdir}/\${nlbuild_autotools_stem}])

#
# OPENTHREAD interface current, revision, and age versions.
#
# Maintainters: Please manage these fields as follows:
#
#   Interfaces removed:    CURRENT++, AGE = 0, REVISION = 0
#   Interfaces added:      CURRENT++, AGE++,   REVISION = 0
#   No interfaces changed:                     REVISION++
#
#
AC_SUBST(LIBOPENTHREAD_VERSION_CURRENT,  [0])
AC_SUBST(LIBOPENTHREAD_VERSION_AGE,      [5])
AC_SUBST(LIBOPENTHREAD_VERSION_REVISION, [0])
AC_SUBST(LIBOPENTHREAD_VERSION_INFO,     [${LIBOPENTHREAD_VERSION_CURRENT}:${LIBOPENTHREAD_VERSION_REVISION}:${LIBOPENTHREAD_VERSION_AGE}])

#
# Check the sanity of the source directory by checking for the
# presence of a key watch file
#
AC_CONFIG_SRCDIR([include/openthread/error.h])

#
# Tell autoconf where to find auxilliary build tools (e.g. config.guess,
# install-sh, missing, etc.)
#
AC_CONFIG_AUX_DIR([third_party/nlbuild-autotools/repo/third_party/autoconf])

#
# Tell autoconf where to find auxilliary M4 macros
#
AC_CONFIG_MACRO_DIRS([third_party/nlbuild-autotools/repo/third_party/autoconf/m4 third_party/nlbuild-autotools/repo/autoconf/m4])

#
# Tell autoconf what file the package is using to aggregate C preprocessor
# defines.
#
AC_CONFIG_HEADERS([include/openthread-config-generic.h])

#
# Figure out what the canonical build and host tuples are.
#
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

#
# Mac OS X / Darwin ends up putting some versioning cruft on the end of its
# tuple that we don't care about in this script. Create "clean" variables
# devoid of it.
#

NL_FILTERED_CANONICAL_BUILD
NL_FILTERED_CANONICAL_HOST

#
# Configure automake with the desired options, indicating that this is not
# a native GNU package, that we want "silent" build rules, and that we want
# objects built in the same subdirectory as their source rather than collapsed
# together at the top-level directory.
#
# Disable silent build rules by either passing --disable-silent-rules to
# configure or passing V=1 to make
#
AM_INIT_AUTOMAKE([1.14 foreign silent-rules subdir-objects tar-pax])

#
# Silent build rules requires at least automake-1.11. Employ
# techniques for not breaking earlier versions of automake.
#
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_SILENT_RULES([yes])

#
# Enable maintainer mode to prevent the package from constantly trying
# to rebuild configure, Makefile.in, etc. Rebuilding such files rarely,
# if ever, needs to be done "in the field".
#
# Use the included 'bootstrap' script instead when necessary.
#
AM_MAINTAINER_MODE

#
# Host-os-specific checks
#

case ${host_os} in

    *darwin*)
        OPENTHREAD_TARGET=darwin
        OPENTHREAD_TARGET_DEFINES="-DOPENTHREAD_TARGET_DARWIN"
        ;;

    *linux*)
        OPENTHREAD_TARGET=linux
        OPENTHREAD_TARGET_DEFINES="-DOPENTHREAD_TARGET_LINUX"
        ;;

esac

AC_SUBST(OPENTHREAD_TARGET_DARWIN)
AM_CONDITIONAL([OPENTHREAD_TARGET_DARWIN], [test "${OPENTHREAD_TARGET}" = "darwin"])

AC_SUBST(OPENTHREAD_TARGET_LINUX)
AM_CONDITIONAL([OPENTHREAD_TARGET_LINUX], [test "${OPENTHREAD_TARGET}" = "linux"])

AC_SUBST(OPENTHREAD_TARGET_DEFINES)

#
# Checks for build host programs
#

# This is a hack to restore some old broken behavior that was
# removed in pull request #1527. It's use is highly discouraged,
# you should try to fix your build environment instead.
AC_ARG_ENABLE(no-executables-hack,
    [AS_HELP_STRING([--enable-no-executables-hack],
        [Enable hack that prevents link checks at configure time. Highly discouraged.])])
AC_MSG_CHECKING([whether to disable executable checking])
if test "${enable_no_executables_hack}" = "yes"
then
    AC_MSG_RESULT([yes])
    AC_NO_EXECUTABLES
    # Here we guess conservative values for tests that require link checks
    # to test for these features. This will prevent these checks from
    # being performed later in the configuration process.
    ac_cv_func_strlcat=${ac_cv_func_strlcat-no}
    ac_cv_func_strlcpy=${ac_cv_func_strlcpy-no}
    ac_cv_func_strnlen=${ac_cv_func_strnlen-no}
else
    AC_MSG_RESULT([no])
fi

# Passing -Werror to GCC-based or -compatible compilers breaks some
# autoconf tests (see
# http://lists.gnu.org/archive/html/autoconf-patches/2008-09/msg00014.html).
#
# If -Werror has been passed transform it into -Wno-error. We'll
# transform it back later with NL_RESTORE_WERROR.

NL_SAVE_WERROR

# Check for compilers.
#
# These should be checked BEFORE we check for and, implicitly,
# initialize libtool such that libtool knows what languages it has to
# work with.

AC_PROG_CPP
AC_PROG_CPP_WERROR

AC_PROG_CC
AC_PROG_CC_C_O

AC_PROG_CXXCPP

AC_PROG_CXX
AC_PROG_CXX_C_O

AM_PROG_AS

# Check for other compiler toolchain tools.

AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib)
AC_CHECK_TOOL(OBJCOPY, objcopy)
AC_CHECK_TOOL(STRIP, strip)

# Check for other host tools.

AC_PROG_INSTALL
AC_PROG_LN_S

AC_PATH_PROG(CMP, cmp)

#
# Checks for specific compiler characteristics
#

#
# Common compiler flags we would like to have.
#
#   -Wall                        CC, CXX
#

PROSPECTIVE_CFLAGS="-Wall -Wextra -Wshadow -Werror -std=c99 -pedantic-errors"
PROSPECTIVE_CXXFLAGS="-Wall -Wextra -Wshadow -Werror -std=gnu++98 -Wno-c++14-compat -fno-exceptions"

AC_CACHE_CHECK([whether $CC is Clang],
    [nl_cv_clang],
    [nl_cv_clang=no
    if test "x${GCC}" = "xyes"; then
        AC_EGREP_CPP([NL_CC_IS_CLANG],
            [/* Note: Clang 2.7 lacks __clang_[a-z]+__ */
#            if defined(__clang__) && defined(__llvm__)
             NL_CC_IS_CLANG
#            endif
            ],
            [nl_cv_clang=yes])
    fi
    ])

if test "${nl_cv_clang}" = "yes"; then
    PROSPECTIVE_CFLAGS="${PROSPECTIVE_CFLAGS} -Wconversion"
    PROSPECTIVE_CXXFLAGS="${PROSPECTIVE_CXXFLAGS} -Wconversion"
fi

AX_CHECK_COMPILER_OPTIONS([C],   ${PROSPECTIVE_CFLAGS})
AX_CHECK_COMPILER_OPTIONS([C++], ${PROSPECTIVE_CXXFLAGS})

# Check for and initialize libtool

LT_INIT
AC_PROG_LIBTOOL

# Disable building shared libraries by default (can be enabled with --enable-shared)

AC_DISABLE_SHARED

#
# Debug instances
#
AC_MSG_NOTICE([checking whether to build debug instances])

# Debug

NL_ENABLE_DEBUG([no])

AM_CONDITIONAL([OPENTHREAD_BUILD_DEBUG], [test "${nl_cv_build_debug}" = "yes"])

#
# Code coverage and compiler optimization
#

# Coverage

NL_ENABLE_COVERAGE([no])

AM_CONDITIONAL([OPENTHREAD_BUILD_COVERAGE], [test "${nl_cv_build_coverage}" = "yes"])

NL_ENABLE_COVERAGE_REPORTS([auto])

AM_CONDITIONAL([OPENTHREAD_BUILD_COVERAGE_REPORTS], [test "${nl_cv_build_coverage_reports}" = "yes"])

# Optimization

NL_ENABLE_OPTIMIZATION([yes])

AM_CONDITIONAL([OPENTHREAD_BUILD_OPTIMIZED], [test "${nl_cv_build_optimized}" = "yes"])

# Fuzz Targets

AC_MSG_CHECKING([whether to build fuzz targets])
AC_ARG_ENABLE(fuzz-targets,
    [AS_HELP_STRING([--enable-fuzz-targets],[Enable building of fuzz targets @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_fuzz_targets=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enableval} for --enable-fuzz-targets])
            ;;

        esac
    ],
    [enable_fuzz_targets=no])
AC_MSG_RESULT(${enable_fuzz_targets})

AM_CONDITIONAL([OPENTHREAD_ENABLE_FUZZ_TARGETS], [test "${enable_fuzz_targets}" = "yes"])

if test "${enable_fuzz_targets}" = "no" ; then
    PROSPECTIVE_CXXFLAGS="-fno-rtti"
    AX_CHECK_COMPILER_OPTIONS([C++], ${PROSPECTIVE_CXXFLAGS})
fi

# Address Sanitizer

AC_MSG_CHECKING([whether to build with Address Sanitizer support])
AC_ARG_ENABLE(address-sanitizer,
    [AS_HELP_STRING([--enable-address-sanitizer],[Enable Address Sanitizer support @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_address_sanitizer=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enableval} for --enable-address-sanitizer])
            ;;

        esac
    ],
    [enable_address_sanitizer=no])
AC_MSG_RESULT(${enable_address_sanitizer})

AM_CONDITIONAL([OPENTHREAD_WITH_ADDRESS_SANITIZER], [test "${enable_address_sanitizer}" = "yes"])

if test "${enable_address_sanitizer}" = "yes" ; then

    PROSPECTIVE_CFLAGS="-fsanitize=address"

    # Check if the compilers support address sanitizer
    AX_CHECK_COMPILER_OPTIONS([C],   ${PROSPECTIVE_CFLAGS})
    AX_CHECK_COMPILER_OPTIONS([C++], ${PROSPECTIVE_CFLAGS})

fi

#
# Code style
#

AC_SUBST(PRETTY, ["\${abs_top_srcdir}/script/clang-format.sh"])
AC_SUBST(PRETTY_ARGS, ["-style=file -i"])
AC_SUBST(PRETTY_CHECK, ["\${abs_top_srcdir}/script/clang-format-check.sh"])
AC_SUBST(PRETTY_CHECK_ARGS, [""])

#
# Tests
#
AC_MSG_NOTICE([checking whether to build tests])

# Tests

NL_ENABLE_TESTS([yes])

AM_CONDITIONAL([OPENTHREAD_BUILD_TESTS], [test "${nl_cv_build_tests}" = "yes"])

#
# Builtin mbedtls
#

AC_ARG_ENABLE(builtin-mbedtls,
    [AS_HELP_STRING([--enable-builtin-mbedtls],[Enable builtin mbedtls @<:@default=yes@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_builtin_mbedtls=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_builtin_mbedtls} for --enable-builtin-mbedtls])
            ;;
        esac
    ],
    [enable_builtin_mbedtls=yes])

if test "$enable_builtin_mbedtls" = "yes" -a ! "${MBEDTLS_CPPFLAGS}"; then
    MBEDTLS_CPPFLAGS="-I\${abs_top_srcdir}/third_party/mbedtls"
    MBEDTLS_CPPFLAGS="${MBEDTLS_CPPFLAGS} -I\${abs_top_srcdir}/third_party/mbedtls/repo/include"
    MBEDTLS_CPPFLAGS="${MBEDTLS_CPPFLAGS} -DMBEDTLS_CONFIG_FILE=\\\"mbedtls-config.h\\\""
fi

if test "$enable_builtin_mbedtls" = "yes"; then
    OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS=1
else
    OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS=0
fi

AC_MSG_CHECKING([whether mbed TLS should be enabled])
AC_MSG_RESULT(${enable_builtin_mbedtls})
AC_DEFINE_UNQUOTED([OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS], [${OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS}], [Define to 1 if you want to enable support for bultin-mbedtls.])
AM_CONDITIONAL([OPENTHREAD_ENABLE_BUILTIN_MBEDTLS], [test "${enable_builtin_mbedtls}" = "yes"])

#
# POSIX Application
#

AC_MSG_CHECKING([whether to build POSIX applicaton])
AC_ARG_ENABLE(posix-app,
    [AS_HELP_STRING([--enable-posix-app], [Build POSIX application @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_posix_app=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_posix_app} for --enable-posix-app])
            ;;
        esac
    ],
    [enable_posix_app=no])

if test "$enable_posix_app" = "yes"; then
    OPENTHREAD_PLATFORM_POSIX_APP=1
else
    OPENTHREAD_PLATFORM_POSIX_APP=0
fi

AC_MSG_RESULT(${enable_posix_app})
AC_SUBST(OPENTHREAD_PLATFORM_POSIX_APP)
AM_CONDITIONAL([OPENTHREAD_PLATFORM_POSIX_APP], [test "${enable_posix_app}" = "yes"])
AC_DEFINE_UNQUOTED([OPENTHREAD_PLATFORM_POSIX_APP], [${OPENTHREAD_PLATFORM_POSIX_APP}], [Define to 1 to build posix application.])

#
# POSIX Daemon
#

AC_MSG_CHECKING([whether to build POSIX applicaton in daemon mode])
AC_ARG_ENABLE(posix-app-daemon,
    [AS_HELP_STRING([--enable-posix-app-daemon], [Build POSIX application in daemon mode@<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_posix_app_daemon=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_posix_app} for --enable-posix-app-daemon])
            ;;
        esac
    ],
    [enable_posix_app_daemon=no])

if test "$enable_posix_app_daemon" = "yes"; then
    OPENTHREAD_ENABLE_POSIX_APP_DAEMON=1
else
    OPENTHREAD_ENABLE_POSIX_APP_DAEMON=0
fi

AC_MSG_RESULT(${enable_posix_app_daemon})
AC_SUBST(OPENTHREAD_ENABLE_POSIX_APP_DAEMON)
AM_CONDITIONAL([OPENTHREAD_ENABLE_POSIX_APP_DAEMON], [test "${enable_posix_app_daemon}" = "yes"])
AC_DEFINE_UNQUOTED([OPENTHREAD_ENABLE_POSIX_APP_DAEMON], [${OPENTHREAD_ENABLE_POSIX_APP_DAEMON}], [Define to 1 to build posix application in daemon mode.])

#
# FTD Library
#

AC_MSG_CHECKING([whether to build FTD library])
AC_ARG_ENABLE(ftd,
    [AS_HELP_STRING([--enable-ftd], [Build FTD library @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_ftd=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_ftd} for --enable-ftd])
            ;;
        esac
    ],
    [enable_ftd=no])

AC_MSG_RESULT(${enable_ftd})
AM_CONDITIONAL([OPENTHREAD_ENABLE_FTD], [test "${enable_ftd}" = "yes"])

#
# MTD Library
#

AC_MSG_CHECKING([whether to build MTD library])
AC_ARG_ENABLE(mtd,
    [AS_HELP_STRING([--enable-mtd], [Build MTD library @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_mtd=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_mtd} for --enable-mtd])
            ;;
        esac
    ],
    [enable_mtd=no])

AC_MSG_RESULT(${enable_mtd})
AM_CONDITIONAL([OPENTHREAD_ENABLE_MTD], [test "${enable_mtd}" = "yes"])

#
# Radio Library
#

AC_MSG_CHECKING([whether to build radio-only library])
AC_ARG_ENABLE(radio-only,
    [AS_HELP_STRING([--enable-radio-only], [Build radio-only library @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_radio_only=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_radio_only} for --enable-radio-only])
            ;;
        esac
    ],
    [enable_radio_only=no])

AC_MSG_RESULT(${enable_radio_only})
AM_CONDITIONAL([OPENTHREAD_ENABLE_RADIO_ONLY], [test "${enable_radio_only}" = "yes"])

#
# whether to build executables
#

AC_MSG_CHECKING([whether to build executables])
AC_ARG_ENABLE(executable,
    [AS_HELP_STRING([--enable-executable], [Build executables @<:@default=yes@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_executable=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_executable} for --enable-executable])
            ;;
        esac
    ],
    [enable_executable=yes])

AC_MSG_RESULT(${enable_executable})
AM_CONDITIONAL([OPENTHREAD_ENABLE_EXECUTABLE], [test "${enable_executable}" = "yes"])

#
# CLI Library
#

AC_MSG_CHECKING([whether to build CLI library])
AC_ARG_ENABLE(cli,
    [AS_HELP_STRING([--enable-cli], [Build CLI library @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_cli=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_cli} for --enable-cli])
            ;;
        esac
    ],
    [enable_cli=no])

AC_MSG_RESULT(${enable_cli})
AM_CONDITIONAL([OPENTHREAD_ENABLE_CLI], [test "${enable_cli}" = "yes"])

#
# NCP Library
#

AC_MSG_CHECKING([whether to build NCP library])
AC_ARG_ENABLE(ncp,
    [AS_HELP_STRING([--enable-ncp], [Build NCP library @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_ncp=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_ncp} for --enable-ncp])
            ;;
        esac
    ],
    [enable_ncp=no])

AC_MSG_RESULT(${enable_ncp})
AM_CONDITIONAL([OPENTHREAD_ENABLE_NCP], [test "${enable_ncp}" = "yes"])

#
# Readline - readline library to use for POSIX CLI
#

AC_ARG_WITH(
    [readline],
    [AS_HELP_STRING([--with-readline],[Specify the readline library (none|readline|edit) @<:@default=none@:>@.])],
    [],
    [with_readline=none])

case "${with_readline}" in
"none")
    ;;
"readline")
    # Prefer GNU readline installed by Homebrew.
    if test "${OPENTHREAD_TARGET}" = darwin && brew --prefix readline > /dev/null; then
        LDFLAGS="-L$(brew --prefix readline)/lib ${LDFLAGS}"
        CPPFLAGS="-I$(brew --prefix readline)/include ${CPPFLAGS}"
        PKG_CONFIG_PATH="$(brew --prefix)/lib/pkgconfig:$PKG_CONFIG_PATH"
    fi
    AC_CHECK_LIB([readline], [rl_callback_handler_install])
    ;;
"edit")
    AC_CHECK_LIB([edit], [rl_callback_handler_install])
    ;;
*)
    AC_MSG_ERROR([unexpected --with-readline=${with_readline}])
    ;;
esac

#
# Vendor Extension - Specify a C++ source file which will be built as part of OpenThread core library.
#

AC_ARG_WITH(
    [vendor-extension],
    [AS_HELP_STRING([--with-vendor-extension=<VENDOR_EXT.CPP>],[Specify a C++ source file built as part of OpenThread core library. @<:@default=none@:>@.])],
    [
        if test "${withval}" = "no"
        then OPENTHREAD_ENABLE_VENDOR_EXTENSION=0
        elif test '!' -f "${withval}"
        then AC_MSG_ERROR([Can't open ${withval} for --with-vendor-extension])
        else
            OPENTHREAD_ENABLE_VENDOR_EXTENSION=1

            # Get the absolute path.
            OPENTHREAD_VENDOR_EXTENSION_SOURCE=$(cd `dirname ${withval}` && pwd)/$(basename ${withval})
        fi
    ],
    [
        OPENTHREAD_ENABLE_VENDOR_EXTENSION=0
        with_vendor_extension=none
    ])

AC_MSG_CHECKING([for vendor extension])
AC_MSG_RESULT(${OPENTHREAD_VENDOR_EXTENSION_SOURCE-none})
AC_SUBST(OPENTHREAD_VENDOR_EXTENSION_SOURCE)
AC_DEFINE_UNQUOTED([OPENTHREAD_ENABLE_VENDOR_EXTENSION],[${OPENTHREAD_ENABLE_VENDOR_EXTENSION}],[Define to 1 if using vendor extension])
AM_CONDITIONAL([OPENTHREAD_ENABLE_VENDOR_EXTENSION], [test "${OPENTHREAD_ENABLE_VENDOR_EXTENSION}" = "1"])

#
# NCP Vendor Hook Source - Specify a C++ file that implements the NCP vendor hook.
#

AC_ARG_WITH(
    [ncp-vendor-hook-source],
    [AS_HELP_STRING([--with-ncp-vendor-hook-source=<VENDOR_HOOK.CPP>],[Specify a C++ file that implements the NCP vendor hook. @<:@default=none@:>@.])],
    [
        if test "${withval}" = "no"
        then OPENTHREAD_ENABLE_NCP_VENDOR_HOOK=0
        elif test '!' -f "${withval}"
        then AC_MSG_ERROR([Can't open ${withval} for --with-ncp-vendor-hook-source])
        else
            OPENTHREAD_ENABLE_NCP_VENDOR_HOOK=1

            # Get the absolute path.
            OPENTHREAD_NCP_VENDOR_HOOK_SOURCE=$(cd `dirname ${withval}` && pwd)/$(basename ${withval})
        fi
    ],
    [
        OPENTHREAD_ENABLE_NCP_VENDOR_HOOK=0
        with_ncp_vendor_hook_source=none
    ])

AC_MSG_CHECKING([for NCP vendor hook source])
AC_MSG_RESULT(${OPENTHREAD_NCP_VENDOR_HOOK_SOURCE-none})
AC_SUBST(OPENTHREAD_NCP_VENDOR_HOOK_SOURCE)
AC_DEFINE_UNQUOTED([OPENTHREAD_ENABLE_NCP_VENDOR_HOOK],[${OPENTHREAD_ENABLE_NCP_VENDOR_HOOK}],[Define to 1 if using NCP vendor hook])
AM_CONDITIONAL([OPENTHREAD_ENABLE_NCP_VENDOR_HOOK], [test "${OPENTHREAD_ENABLE_NCP_VENDOR_HOOK}" = "1"])

#
# NCP Spinel Encrypter
#

AC_ARG_WITH(
    [ncp-spinel-encrypter-libs],
    [AS_HELP_STRING([--with-ncp-spinel-encrypter-libs=<LIBSPINEL_ENCRYPTER.A>],[Specify library files (absolute paths) implementing the NCP Spinel Encrypter. @<:@default=none@:>@.])],
    [
        if test "${withval}" = "no"
        then OPENTHREAD_ENABLE_NCP_SPINEL_ENCRYPTER=0
        else
            OPENTHREAD_ENABLE_NCP_SPINEL_ENCRYPTER=1
            OPENTHREAD_NCP_SPINEL_ENCRYPTER_LIBS=${withval}
        fi
    ],
    [
        OPENTHREAD_ENABLE_NCP_SPINEL_ENCRYPTER=0
        with_ncp_spinel_encrypter_libs=none
    ])

AC_MSG_CHECKING([for NCP Spinel Encrypter])
AC_MSG_RESULT(${OPENTHREAD_NCP_SPINEL_ENCRYPTER_LIBS-none})
AC_SUBST(OPENTHREAD_NCP_SPINEL_ENCRYPTER_LIBS)
AC_DEFINE_UNQUOTED([OPENTHREAD_ENABLE_NCP_SPINEL_ENCRYPTER],[${OPENTHREAD_ENABLE_NCP_SPINEL_ENCRYPTER}],[Define to 1 if using NCP Spinel Encrypter])
AM_CONDITIONAL([OPENTHREAD_ENABLE_NCP_SPINEL_ENCRYPTER], [test "${OPENTHREAD_ENABLE_NCP_SPINEL_ENCRYPTER}" = "1"])

#
# Linker Map Output
#

AC_ARG_ENABLE(linker-map,
    [AS_HELP_STRING([--enable-linker-map],[Enable linker map output @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_linker_map=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_linker_map} for --enable-linker-map])
            ;;
        esac
    ],
    [enable_linker_map=no])

AC_MSG_CHECKING([whether to enable linker map output])
AC_MSG_RESULT(${enable_linker_map})
AM_CONDITIONAL([OPENTHREAD_ENABLE_LINKER_MAP], [test "${enable_linker_map}" = "yes"])

#
# Custom linker file
#

AC_ARG_WITH(
    [custom-linker-file],
    [AS_HELP_STRING([--with-custom-linker-file=<linkerfile.ld>],[Specify custom linker file (absolute path). @<:@default=none@:>@.])],
    [
        with_custom_linker_file=yes
        OPENTHREAD_CUSTOM_LINKER_FILE=${withval}
    ],
    [
        with_custom_linker_file=no
    ])

AC_SUBST(OPENTHREAD_CUSTOM_LINKER_FILE)
AM_CONDITIONAL([OPENTHREAD_ENABLE_CUSTOM_LINKER_FILE], [test "${with_custom_linker_file}" = "yes"])

#
# Examples
#

AC_ARG_WITH(examples,
    [AS_HELP_STRING([--with-examples=TARGET],
        [Specify the examples from one of: none, posix, cc1352, cc2538, cc2650, cc2652, efr32mg12, efr32mg21, gp712, kw41z, nrf52811, nrf52840, qpg6095, samr21 @<:@default=none@:>@.])],
    [
        case "${with_examples}" in
        none)
            ;;
        posix|cc1352|cc2538|cc2650|cc2652|efr32mg12|efr32mg21|gp712|kw41z|nrf52811|nrf52840|qpg6095|samr21)
            if test ${enable_posix_app} = "yes"; then
                AC_MSG_ERROR([--with-examples must be none when POSIX apps are enabled by --enable-posix-app])
            fi
            ;;
        *)
            AC_MSG_ERROR([Invalid value ${with_examples} for --with-examples])
            ;;
        esac
    ],
    [with_examples=none])

OPENTHREAD_ENABLE_EXAMPLES=${with_examples}

case ${with_examples} in

    posix)
        OPENTHREAD_EXAMPLES_POSIX=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_POSIX],[${OPENTHREAD_EXAMPLES_POSIX}],[Define to 1 if you want to use posix examples])
        ;;

    cc1352)
        OPENTHREAD_EXAMPLES_CC1352=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_CC1352],[${OPENTHREAD_EXAMPLES_CC1352}],[Define to 1 if you want to use cc1352 examples])
        ;;

    cc2538)
        OPENTHREAD_EXAMPLES_CC2538=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_CC2538],[${OPENTHREAD_EXAMPLES_CC2538}],[Define to 1 if you want to use cc2538 examples])
        ;;

    cc2650)
        OPENTHREAD_EXAMPLES_CC2650=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_CC2650],[${OPENTHREAD_EXAMPLES_CC2650}],[Define to 1 if you want to use cc2650 examples])
        ;;

    cc2652)
        OPENTHREAD_EXAMPLES_CC2652=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_CC2652],[${OPENTHREAD_EXAMPLES_CC2652}],[Define to 1 if you want to use cc2652 examples])
        ;;

    efr32mg12)
        OPENTHREAD_EXAMPLES_EFR32MG12=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_EFR32MG12],[${OPENTHREAD_EXAMPLES_EFR32MG12}],[Define to 1 if you want to use efr32mg12 examples])
        ;;

    efr32mg21)
        OPENTHREAD_EXAMPLES_EFR32MG21=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_EFR32MG21],[${OPENTHREAD_EXAMPLES_EFR32MG21}],[Define to 1 if you want to use efr32mg21 examples])
        ;;

    gp712)
        OPENTHREAD_EXAMPLES_GP712=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_GP712],[${OPENTHREAD_EXAMPLES_GP712}],[Define to 1 if you want to use gp712 examples])
        ;;

    kw41z)
        OPENTHREAD_EXAMPLES_KW41Z=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_KW41Z],[${OPENTHREAD_EXAMPLES_KW41Z}],[Define to 1 if you want to use kw41z examples])
        ;;

    nrf52811)
        OPENTHREAD_EXAMPLES_NRF52811=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_NRF52811],[${OPENTHREAD_EXAMPLES_NRF52811}],[Define to 1 if you want to use nrf52811 examples])
        ;;

    nrf52840)
        OPENTHREAD_EXAMPLES_NRF52840=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_NRF52840],[${OPENTHREAD_EXAMPLES_NRF52840}],[Define to 1 if you want to use nrf52840 examples])
        ;;

    qpg6095)
        OPENTHREAD_EXAMPLES_QPG6095=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_QPG6095],[${OPENTHREAD_EXAMPLES_QPG6095}],[Define to 1 if you want to use qpg6095 examples])
        ;;

    samr21)
        OPENTHREAD_EXAMPLES_SAMR21=1
        AC_DEFINE_UNQUOTED([OPENTHREAD_EXAMPLES_SAMR21],[${OPENTHREAD_EXAMPLES_SAMR21}],[Define to 1 if you want to use samr21 examples])
        ;;

esac

AC_MSG_CHECKING([whether to enable examples])
AC_MSG_RESULT(${OPENTHREAD_EXAMPLES})

AC_SUBST(OPENTHREAD_ENABLE_EXAMPLES)
AM_CONDITIONAL([OPENTHREAD_ENABLE_EXAMPLES], [test ${with_examples} != "none"])
AC_DEFINE_UNQUOTED([OPENTHREAD_ENABLE_EXAMPLES],[${OPENTHREAD_ENABLE_EXAMPLES}],[OpenThread examples])

AC_SUBST(OPENTHREAD_EXAMPLES_POSIX)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_POSIX], [test "${with_examples}" = "posix"])

AC_SUBST(OPENTHREAD_EXAMPLES_CC1352)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_CC1352], [test "${with_examples}" = "cc1352"])

AC_SUBST(OPENTHREAD_EXAMPLES_CC2538)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_CC2538], [test "${with_examples}" = "cc2538"])

AC_SUBST(OPENTHREAD_EXAMPLES_CC2650)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_CC2650], [test "${with_examples}" = "cc2650"])

AC_SUBST(OPENTHREAD_EXAMPLES_CC2652)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_CC2652], [test "${with_examples}" = "cc2652"])

AC_SUBST(OPENTHREAD_EXAMPLES_EFR32MG12)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_EFR32MG12], [test "${with_examples}" = "efr32mg12"])

AC_SUBST(OPENTHREAD_EXAMPLES_EFR32MG21)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_EFR32MG21], [test "${with_examples}" = "efr32mg21"])

AC_SUBST(OPENTHREAD_EXAMPLES_GP712)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_GP712], [test "${with_examples}" = "gp712"])

AC_SUBST(OPENTHREAD_EXAMPLES_KW41Z)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_KW41Z], [test "${with_examples}" = "kw41z"])

AC_SUBST(OPENTHREAD_EXAMPLES_NRF52811)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_NRF52811], [test "${with_examples}" = "nrf52811"])

AC_SUBST(OPENTHREAD_EXAMPLES_NRF52840)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_NRF52840], [test "${with_examples}" = "nrf52840"])

AC_SUBST(OPENTHREAD_EXAMPLES_QPG6095)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_QPG6095], [test "${with_examples}" = "qpg6095"])

AC_SUBST(OPENTHREAD_EXAMPLES_SAMR21)
AM_CONDITIONAL([OPENTHREAD_EXAMPLES_SAMR21], [test "${with_examples}" = "samr21"])

#
# Tools
#

AC_MSG_CHECKING([whether to build tools])
AC_ARG_ENABLE(tools,
    [AS_HELP_STRING([--disable-tools],[Disable building of tools @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            build_tools=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enableval} for --enable-tools])
            ;;

        esac
    ],
    [build_tools=yes])
AC_MSG_RESULT(${build_tools})
AM_CONDITIONAL([OPENTHREAD_BUILD_TOOLS], [test "${build_tools}" = "yes"])

#
# Documentation
#

# Determine whether or not documentation (via Doxygen) should be built
# or not, with 'auto' as the default and establish a default support
# value for GraphViz 'dot' support.

NL_ENABLE_DOCS([auto],[NO])

AM_CONDITIONAL(OPENTHREAD_BUILD_DOCS, [test "${nl_cv_build_docs}" = "yes"])

AM_CONDITIONAL(OPENTHREAD_POSIX, [test "${OPENTHREAD_PLATFORM_POSIX_APP}" = "1" -o "${OPENTHREAD_EXAMPLES_POSIX}" = "1"])

#
# Checks for libraries and packages.
#
# At minimum, the following packages are optional, depending on
# configuration:
#
#   * TBD
#
AC_MSG_NOTICE([checking required package dependencies])

# NL_WITH_PACKAGE(...)

#
# Check for headers
#

#---------------------------------------------------
# Enable BSD Security Features
# This enables strlcpy() and other friends in GNU land.
# While the references below generally speak of: "glibc"
# The ARM Embedded platform uses the nano instance of NEWLIB
# Which greatly follows and mirrors glibc.
# --------------------------------------------------
#
# References:
# 1) http://stackoverflow.com/questions/29201515/what-does-d-default-source-do
# 2) http://man7.org/linux/man-pages/man7/feature_test_macros.7.html
#
CFLAGS="${CFLAGS} -D_BSD_SOURCE=1 -D_DEFAULT_SOURCE=1"
CXXFLAGS="${CXXFLAGS} -D_BSD_SOURCE=1 -D_DEFAULT_SOURCE=1"

OLD_CFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} -Wno-error=address"
AC_HEADER_STDBOOL
CFLAGS="${OLD_CFLAGS}"
AC_HEADER_STDC

AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([string.h])

#
# Missing Functions
#
AC_CHECK_FUNC([strlcpy], [AC_DEFINE([HAVE_STRLCPY], [1], [Define if strlcpy exists.])])
AC_CHECK_FUNC([strlcat], [AC_DEFINE([HAVE_STRLCAT], [1], [Define if strlcat exists.])])
AC_CHECK_FUNC([strnlen], [AC_DEFINE([HAVE_STRNLEN], [1], [Define if strnlen exists.])])

#
# Check for types and structures
#
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

#
# Checks for library functions
#

if test "${ac_no_link}" != "yes"; then
    AC_CHECK_FUNCS([memcpy])
fi

# Add any mbedtls CPPFLAGS

CPPFLAGS="${CPPFLAGS} ${MBEDTLS_CPPFLAGS}"

# Add any code coverage CPPFLAGS and LDFLAGS

CPPFLAGS="${CPPFLAGS} ${NL_COVERAGE_CPPFLAGS}"
LIBS="${LIBS} ${NL_COVERAGE_LIBS}"
LDFLAGS="${LDFLAGS} ${NL_COVERAGE_LDFLAGS}"

# At this point, we can restore the compiler flags to whatever the
# user passed in, now that we're clear of an -Werror issues by
# transforming -Wno-error back to -Werror.

NL_RESTORE_WERROR

#
# Identify the various makefiles and auto-generated files for the package
#
AC_CONFIG_FILES([
Makefile
include/Makefile
include/openthread/Makefile
include/openthread/platform/Makefile
src/Makefile
src/cli/Makefile
src/ncp/Makefile
src/core/Makefile
src/posix/Makefile
src/posix/platform/Makefile
third_party/Makefile
third_party/jlink/Makefile
third_party/mbedtls/Makefile
third_party/NordicSemiconductor/Makefile
third_party/silabs/Makefile
examples/Makefile
examples/apps/Makefile
examples/apps/cli/Makefile
examples/apps/ncp/Makefile
examples/platforms/Makefile
examples/platforms/cc1352/Makefile
examples/platforms/cc2538/Makefile
examples/platforms/cc2650/Makefile
examples/platforms/cc2652/Makefile
examples/platforms/efr32mg12/Makefile
examples/platforms/efr32mg12/sleepy-demo/Makefile
examples/platforms/efr32mg12/sleepy-demo/sleepy-demo-ftd/Makefile
examples/platforms/efr32mg12/sleepy-demo/sleepy-demo-mtd/Makefile
examples/platforms/efr32mg21/Makefile
examples/platforms/efr32mg21/sleepy-demo/Makefile
examples/platforms/efr32mg21/sleepy-demo/sleepy-demo-ftd/Makefile
examples/platforms/efr32mg21/sleepy-demo/sleepy-demo-mtd/Makefile
examples/platforms/gp712/Makefile
examples/platforms/kw41z/Makefile
examples/platforms/nrf52811/Makefile
examples/platforms/nrf52840/Makefile
examples/platforms/qpg6095/Makefile
examples/platforms/samr21/Makefile
examples/platforms/posix/Makefile
examples/platforms/utils/Makefile
tools/Makefile
tools/harness-automation/Makefile
tools/harness-thci/Makefile
tools/spi-hdlc-adapter/Makefile
tests/Makefile
tests/fuzz/Makefile
tests/scripts/Makefile
tests/scripts/thread-cert/Makefile
tests/unit/Makefile
doc/Makefile
])

#
# Generate the auto-generated files for the package
#
AC_OUTPUT

#
# Summarize the package configuration
#

AC_MSG_NOTICE([

  Configuration Summary
  ---------------------
  Package                                   : ${PACKAGE_NAME}
  Version                                   : ${PACKAGE_VERSION}
  Interface                                 : ${LIBOPENTHREAD_VERSION_INFO//:/.}
  Build system                              : ${build}
  Host system                               : ${host}
  Host architecture                         : ${host_cpu}
  Host OS                                   : ${host_os}
  Cross compiling                           : ${cross_compiling}
  Build shared libraries                    : ${enable_shared}
  Build static libraries                    : ${enable_static}
  Build debug libraries                     : ${nl_cv_build_debug}
  Build optimized libraries                 : ${nl_cv_build_optimized}
  Build coverage libraries                  : ${nl_cv_build_coverage}
  Build coverage reports                    : ${nl_cv_build_coverage_reports}
  Address sanitizer support                 : ${enable_address_sanitizer}
  Lcov                                      : ${LCOV:--}
  Genhtml                                   : ${GENHTML:--}
  Build tests                               : ${nl_cv_build_tests}
  Build fuzz targets                        : ${enable_fuzz_targets}
  Build tools                               : ${build_tools}
  OpenThread tests                          : ${with_tests}
  Prefix                                    : ${prefix}
  Documentation support                     : ${nl_cv_build_docs}
  Doxygen                                   : ${DOXYGEN:--}
  GraphViz dot                              : ${DOT:--}
  C Preprocessor                            : ${CPP}
  C Compiler                                : ${CC}
  C++ Preprocessor                          : ${CXXCPP}
  C++ Compiler                              : ${CXX}
  Assembler Compiler                        : ${CCAS}
  Archiver                                  : ${AR}
  Archive Indexer                           : ${RANLIB}
  Symbol Stripper                           : ${STRIP}
  Object Copier                             : ${OBJCOPY}
  C Preprocessor flags                      : ${CPPFLAGS:--}
  C Compile flags                           : ${CFLAGS:--}
  C++ Compile flags                         : ${CXXFLAGS:--}
  Assembler flags                           : ${CCASFLAGS:--}
  Link flags                                : ${LDFLAGS:--}
  Link libraries                            : ${LIBS}
  Link maps                                 : ${enable_linker_map}
  Pretty                                    : ${PRETTY:--}
  Pretty args                               : ${PRETTY_ARGS:--}
  Pretty check                              : ${PRETTY_CHECK:--}
  Pretty check args                         : ${PRETTY_CHECK_ARGS:--}
  OpenThread FTD support                    : ${enable_ftd}
  OpenThread MTD support                    : ${enable_mtd}
  OpenThread Radio Only support             : ${enable_radio_only}
  OpenThread CLI support                    : ${enable_cli}
  OpenThread NCP support                    : ${enable_ncp}
  OpenThread NCP Vendor Hook Source         : ${with_ncp_vendor_hook_source}
  OpenThread NCP Spinel Encrypter           : ${with_ncp_spinel_encrypter_libs}
  OpenThread Vendor Extension Source        : ${with_vendor_extension}
  OpenThread builtin mbedtls support        : ${enable_builtin_mbedtls}
  OpenThread Examples                       : ${with_examples}
  OpenThread POSIX Application              : ${enable_posix_app}

])
