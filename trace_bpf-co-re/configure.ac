AC_INIT([catapult-bpf-printk], [3.3])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC

PKG_CHECK_MODULES([libelf], [libelf])
PKG_CHECK_MODULES([libbpf], [libbpf])
PKG_CHECK_MODULES([libbpf], [libbpf >= 1.0.0],
   [],
   [AC_DEFINE([HAVE_LIBBPF0], [1], [Define if you have libbpf0])]
)
[
    cat <<EOF  > config.mk ]
# user can control verbosity similar to kernel builds (e.g., V=1)
ifeq ("\$(origin V)", "command line")
	VERBOSE = \$(V)
endif

ifndef VERBOSE
	VERBOSE = 0
endif

ifeq (\$(VERBOSE),1)
	Q =
else
	Q = @
endif

ifeq (\$(VERBOSE),0)
	MAKEFLAGS += --no-print-directory
endif

ifeq (\$(VERBOSE), 0)
      QUIET_CC       = @echo '    CC       '\$@;
      QUIET_CLANG    = @echo '    CLANG    '\$@;
      QUIET_LLC      = @echo '    LLC      '\$@;
      QUIET_LINK     = @echo '    LINK     '\$@;
      QUIET_INSTALL  = @echo '    INSTALL  '\$@;
      QUIET_GEN      = @echo '    GEN      '\$@;]
endif
EOF

[
    : ${PKG_CONFIG:=pkg-config}
    : ${CC=gcc}
    : ${CLANG=clang}
    : ${LLC=llc}
    : ${BPFTOOL=bpftool}
    for TOOL in $PKG_CONFIG $CC $CLANG $LLC $BPFTOOL; do
            if [ ! $(command -v ${TOOL} 2>/dev/null) ]; then
                echo "*** ERROR: Cannot find tool ${TOOL}" ;
                exit 1;
            fi;
    done
    clang_version=$($CLANG --version | sed -n '/clang version/ s/^.*clang version // p')
    clang_major_version=$(echo "$clang_version" | sed 's/\..*$//')
    if [ "$clang_major_version" -lt "11" ]; then
            echo "ERROR: Need clang version >= 11, found $clang_major_version ($clang_version)"
            exit 1
    fi
    ARCH_BPF=$(uname -m | sed 's/x86_64/x86/' | sed 's/aarch64/arm64/' | sed 's/ppc64le/powerpc/' | sed 's/mips.*/mips/')
    ARCH_NAME=$($CC -print-multiarch 2>/dev/null)
    if [ -z "$ARCH_INCLUDES" ] && [ -n "$ARCH_NAME" ]; then
        for dir in $(echo | $CC -Wp,-v -E - 2>&1 | grep '^ '); do
            idir="${dir}/${ARCH_NAME}"
            [ -d "$idir" ] && ARCH_INCLUDES="-I${idir} $ARCH_INCLUDES"
        done
    fi
    echo "clang: $clang_version"
    echo "PKG_CONFIG:=${PKG_CONFIG}" >>config.mk
    echo "CC:=${CC}" >>config.mk
    echo "CLANG:=${CLANG}" >>config.mk
    echo "LLC:=${LLC}" >>config.mk
    echo "BPFTOOL:=${BPFTOOL}" >>config.mk
    echo "ARCH_INCLUDES:=${ARCH_INCLUDES}" >>config.mk
    echo "ARCH_BPF:=${ARCH_BPF}" >>config.mk
]

BPF_TOOLCHAIN_MAKE_RULES=config.mk
AC_SUBST_FILE([BPF_TOOLCHAIN_MAKE_RULES])
BPF_PROG_MAKE_RULES=bpf.mk
AC_SUBST_FILE([BPF_PROG_MAKE_RULES])

AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([Makefile include/Makefile headers/Makefile src/Makefile])
AC_OUTPUT
