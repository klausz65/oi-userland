/*
 * The xorg-server.h installed on OpenIndiana SPARC (x11/server/xorg 1.19.7)
 * says X_BYTE_ORDER is X_LITTLE_ENDIAN.  Built against it, the driver leaves
 * out all its big-endian code, so the framebuffer aperture does not swap and
 * every colour on the screen is wrong.  Force-included on sparc until the
 * server's header is fixed.
 */
#ifndef __ASSEMBLER__
#include <xorg/xorg-server.h>
#undef X_BYTE_ORDER
#define X_BYTE_ORDER X_BIG_ENDIAN
#endif
