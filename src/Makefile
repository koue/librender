#
MK_PROFILE=	no
LOCALBASE?=	/usr/local
LIB=		render
LIBDIR=		${LOCALBASE}/lib
INCSDIR=	${LOCALBASE}/include
CFLAGS+=	-Werror -Wstrict-prototypes -fPIC -I${.CURDIR}
SRCS=		render.h render.c
INCS=		render.h
NO_OBJ=		yes

.include <bsd.lib.mk>
