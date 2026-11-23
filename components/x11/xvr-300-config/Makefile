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
# Copyright 2026 isoriano1968
#

# Configuration for the Sun XVR-300 (Radeon RV370) with Xorg and MATE on
# OpenIndiana SPARC.  No upstream source: the package is files/.

include ../../../make-rules/shared-macros.mk

COMPONENT_NAME=		xvr-300-config
COMPONENT_VERSION=	1.0
COMPONENT_REVISION=	2
COMPONENT_SUMMARY=	Xorg and MATE settings for the Sun XVR-300
COMPONENT_FMRI=		x11/server/xorg/config/xvr-300
COMPONENT_CLASSIFICATION=	System/X11
COMPONENT_LICENSE=	CDDL
COMPONENT_LICENSE_FILE=	$(COMPONENT_NAME).license

include $(WS_MAKE_RULES)/prep-download.mk
include $(WS_MAKE_RULES)/justmake.mk
include $(WS_MAKE_RULES)/ips.mk

PATH=$(PATH.gnu)

XORG_CONF_D=	$(PROTO_DIR)/etc/X11/xorg.conf.d
DCONF_D=	$(PROTO_DIR)/etc/dconf/db/local.d

download::
$(BUILD_DIR)/.prep: Makefile
	$(MKDIR) -p "$(@D)"
	$(TOUCH) $@
prep::	$(BUILD_DIR)/.prep
clean::
	$(RM) -r $(BUILD_DIR)
clobber:: clean
build:: $(BUILD_DIR)/.prep

install: $(BUILD_DIR)/.installed
$(BUILD_DIR)/.installed: $(BUILD_DIR)/.prep $(wildcard files/*)
	$(MKDIR) -p $(XORG_CONF_D) $(DCONF_D)
	$(CP) files/50-xvr-300.conf files/60-xvr-300-keyboard.conf $(XORG_CONF_D)/
	$(CP) files/50-xvr-300 $(DCONF_D)/
	$(TOUCH) $@

test: $(NO_TESTS)

# The driver with the byte-order fixes
REQUIRED_PACKAGES += x11/server/xorg/driver/xorg-video-ati
