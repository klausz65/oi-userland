#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#

#
# Copyright 2015 Alexander Pyhalov
# Copyright 2015 Ken Mays
#

BUILD_STYLE = meson
ifeq ($(shell uname -p),sparc)
# OpenIndiana SPARC: gcc 13 is its newest, and 32-bit libGL has users there
# (SDL, SDL2, libepoxy, glut).
GCC_VERSION = 13
BUILD_BITS = 64_and_32
endif
include ../../../make-rules/shared-macros.mk

COMPONENT_NAME= mesa
COMPONENT_VERSION= 21.3.9
COMPONENT_REVISION= 7
COMPONENT_SUMMARY= The Mesa 3-D Graphics Library
COMPONENT_SRC= mesa-$(COMPONENT_VERSION)
COMPONENT_ARCHIVE= mesa-$(COMPONENT_VERSION).tar.xz
COMPONENT_ARCHIVE_HASH= sha256:91fe6f20339884e37a7c47bfba42fb1cd397512c9ed5ad423de369f047cd8b5c
COMPONENT_ARCHIVE_URL= https://archive.mesa3d.org/older-versions/21.x/$(COMPONENT_ARCHIVE)
COMPONENT_PROJECT_URL = http://www.mesa3d.org/

TEST_TARGET= $(NO_TESTS)
include $(WS_MAKE_RULES)/common.mk

PATCH_LEVEL= 0

ARCHLIBSUBDIR.64= /$(MACH64)
ARCHLIBSUBDIR= $(ARCHLIBSUBDIR.$(BITS))

SERVERMOD_SUBDIR.64= /$(MACH64)
SERVERMOD_SUBDIR= $(SERVERMOD_SUBDIR.$(BITS))
X11_SERVERMODS_DIR= /usr/lib/xorg/modules
X11_SERVERLIBS_DIR= /usr/lib/xorg

# Identify source_file files in hard links for manifest generator.
#PKG_HARDLINKS +=	usr/lib/xorg/modules/dri/radeon_dri.so

CONFIGURE_OPTIONS += --libdir='lib/mesa$(ARCHLIBSUBDIR)'

PKG_HARDLINKS += usr/lib/xorg/modules/dri/$(MACH64)/kms_swrast_dri.so

# Command line options to GNU autoconf configure script
# r300 is for the XVR-300 (RV370) of the Sun workstations.
GALLIUM_DRIVERS.i386 = swrast, i915, r600
GALLIUM_DRIVERS.sparc = swrast, i915, r300, r600
CONFIGURE_OPTIONS += -Dgallium-drivers='$(GALLIUM_DRIVERS.$(MACH))'
VDPAU_LIBS.i386 = vdpau/libvdpau_r600.so.1.0.0
VDPAU_LIBS.sparc = vdpau/libvdpau_r300.so.1.0.0 vdpau/libvdpau_r600.so.1.0.0
CONFIGURE_OPTIONS += -Dvulkan-drivers=''
CONFIGURE_OPTIONS += -Dgbm=enabled
CONFIGURE_OPTIONS += -Dglvnd=false
# Wayland currently requires linux specific socket options and epoll for events.
CONFIGURE_OPTIONS += -Dplatforms='x11'
# llvm is disabled due to requiring llvm 16 we do not have 16
CONFIGURE_OPTIONS += -Dllvm=false
CONFIGURE_OPTIONS += -Dglx='auto'
CONFIGURE_OPTIONS += -Degl=enabled
CONFIGURE_OPTIONS += -Dgles1=enabled
CONFIGURE_OPTIONS += -Dgles2=enabled
CONFIGURE_OPTIONS += -Dosmesa=true
CONFIGURE_OPTIONS += -Dshared-glapi=enabled
CONFIGURE_OPTIONS += -Dgallium-xvmc=enabled
CONFIGURE_OPTIONS += -Dgallium-xa=enabled
# i965 is for Intel integrated graphics, x86 only.
DRI_DRIVERS.i386 = i965
DRI_DRIVERS.sparc =
CONFIGURE_OPTIONS += -Ddri-drivers='$(DRI_DRIVERS.$(MACH))'
CONFIGURE_OPTIONS += -Dshader-cache=false
CONFIGURE_OPTIONS += -Ddri-drivers-path='$(X11_SERVERMODS_DIR)/dri$(SERVERMOD_SUBDIR)'
CONFIGURE_OPTIONS += -Delf-tls=false

CFLAGS += $(XPG7MODE)
CFLAGS += -Wno-incompatible-pointer-types -D_POSIX_PTHREAD_SEMANTICS
CPPFLAGS += -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -Drestrict=__restrict__ -DHAVE_NOATEXIT
#LDFLAGS += -Wl,-zgnu-version-script-compat
# This code has init cycles and multiple C++ libraries, and misbehaves
# when lazy loading is used. Since we're unable to restructure third
# party code at that level, we must instead disable lazy loading.
LDFLAGS += -Wl,-znolazyload
LDFLAGS += -Wl,-z,rescan-now
CFLAGS += -Wstack-protector -fdata-sections -fstack-protector-all
LD_OPTIONS += -z ignore -lssp -lsocket -lnsl
#LD_OPTIONS += -z discard-unused=dependencies
#LD_OPTIONS += -M $(COMPONENT_DIR)/mapfile.externs
CONFIGURE_ENV += PYTHON=$(PYTHON)
ifeq ($(MACH),sparc)
# Workaround for an illumos ld bug: GNU binutils linked on sparc have _end
# inside their own data, so the heap overwrites it and ar dies with SIGBUS
# on libmesa_util.a.  Drop this when binutils has been relinked.
CONFIGURE_ENV += AR=/usr/bin/ar
endif
CONFIGURE_ENV += CPPFLAGS="$(CPPFLAGS)"

# Add RPATH to xorg lib directory where is libdrm.

# Both word sizes install the GL headers, in either order.
MOVE_GL_HEADERS = \
	if [ -d $(PROTOUSRDIR)/include/GL ]; then \
		$(MKDIR) $(PROTOUSRDIR)/include/mesa ; \
		$(CP) -r $(PROTOUSRDIR)/include/GL/. $(PROTOUSRDIR)/include/mesa/ ; \
		$(RM) -r $(PROTOUSRDIR)/include/GL ; \
	fi

# 32-bit: nothing links libglapi into /usr/lib, so its users need the runpath.
COMPONENT_POST_INSTALL_ACTION.32 += \
	for f in libGLESv1_CM.so.1.1.0 libGLESv2.so.2.0.0 libOSMesa.so.8.0.0 libgbm.so.1.0.0 libEGL.so.1.0.0 libGL.so.1.2.0 libxatracker.so.2.5.0 libXvMCr600.so.1.0.0 $(VDPAU_LIBS.$(MACH)); do \
	elfedit -e "dyn:runpath /usr/lib/mesa:/usr/lib/xorg:$(GCC_LIBDIR)" \
	  $(PROTOUSRLIBDIR)/mesa/$$f ; \
	done ; \
	elfedit -e "dyn:runpath /usr/lib/mesa:/usr/lib/xorg:$(GCC_LIBDIR)" \
	  $(PROTOUSRLIBDIR)/xorg/modules/dri/i915_dri.so ; \
	$(MOVE_GL_HEADERS)

COMPONENT_POST_INSTALL_ACTION.64 += \
	for f in libgbm.so.1.0.0 libEGL.so.1.0.0 libGL.so.1.2.0 libxatracker.so.2.5.0 libXvMCr600.so.1.0.0 $(VDPAU_LIBS.$(MACH)); do \
	elfedit -e "dyn:runpath /usr/lib/mesa/$(MACH64):/usr/lib/xorg/$(MACH64):$(GCC_LIBDIR)" \
	  $(PROTOUSRLIBDIR)/mesa/$(MACH64)/$$f ; \
	done ; \
	elfedit -e "dyn:runpath /usr/lib/mesa/$(MACH64):/usr/lib/xorg/$(MACH64):$(GCC_LIBDIR)" \
	  $(PROTOUSRLIBDIR)/xorg/modules/dri/$(MACH64)/i915_dri.so ; \
	$(MOVE_GL_HEADERS)

# Manually added build dependencies
ifeq ($(MACH),sparc)
# 32-bit Mesa
REQUIRED_PACKAGES += library/expat/32
endif
REQUIRED_PACKAGES += x11/header/x11-protocols
REQUIRED_PACKAGES += x11/library/libpthread-stubs
REQUIRED_PACKAGES += system/header/header-drm
REQUIRED_PACKAGES += library/graphics/libvdpau

# Auto-generated dependencies
REQUIRED_PACKAGES += $(GCC_RUNTIME_PKG)
REQUIRED_PACKAGES += $(GXX_RUNTIME_PKG)
REQUIRED_PACKAGES += library/expat
REQUIRED_PACKAGES += library/zlib
REQUIRED_PACKAGES += service/opengl/ogl-select
REQUIRED_PACKAGES += system/library
REQUIRED_PACKAGES += system/library/math
REQUIRED_PACKAGES += x11/header/x11-protocols
REQUIRED_PACKAGES += x11/library/glu
REQUIRED_PACKAGES += x11/library/libdrm
REQUIRED_PACKAGES += x11/library/libx11
REQUIRED_PACKAGES += x11/library/libxcb
REQUIRED_PACKAGES += x11/library/libxext
REQUIRED_PACKAGES += x11/library/libxfixes
REQUIRED_PACKAGES += x11/library/libxshmfence
REQUIRED_PACKAGES += x11/library/libxv
REQUIRED_PACKAGES += x11/library/libxvmc
REQUIRED_PACKAGES += x11/library/libxxf86vm
