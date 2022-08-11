#

LOCALBASE?=	/usr/local
PROGS=		render_test

CFLAGS= 	-Werror \
		-I${.CURDIR}/../src

LDFLAGS+=	-L${.CURDIR}/../src

LDADD=		-lrender

MAN=
MK_DEBUG_FILES=	no
MAKEOBJDIR= .

# keep this empty otherwise installation fails
install:

test:
	./render_test

.include <bsd.progs.mk>
