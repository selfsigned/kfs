FROM alpine as BUILD
LABEL org.opencontainers.image.source="https://github.com/l-aurelie/kfs"

# dependencies needed to build gcc and binutils
RUN apk add --no-cache build-base \
    gmp-dev \
    mpc1-dev \
    mpfr-dev \
    texinfo \
    curl \
    xz

# tool versions
ARG GCC_VERS=12.2.0
ARG BINUTILS_VERS=2.40

WORKDIR /build
# download gcc and binutils and extract them
# Could do everything in this step and remove the build artifacts to get a much smaller docker image
#   but we can also just extract the artifacts we want in a second stage
RUN curl -O https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERS}/gcc-${GCC_VERS}.tar.xz && \
    curl -O https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERS}.tar.xz  && \
    find . -name "*.xz" -exec tar -xf {} \; && \
    rm *.xz 

#### building binutils ####
#Export cross-build variables
ENV PREFIX="/opt/cross"
ENV TARGET=i686-elf
ENV PATH="$PREFIX/bin:$PATH"

WORKDIR /build/binutils-${BINUTILS_VERS}/build
RUN ../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror && \
    make && \
    make install

#### building gcc ####
WORKDIR /build/gcc-${GCC_VERS}/build
RUN ../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers && \
    make all-gcc && \
    make all-target-libgcc && \
    make install-gcc && \
    make install-target-libgcc

#### Second stage ####
FROM alpine

# Not DRY but docker didn't give me a choice
ENV PREFIX="/opt/cross"
ENV TARGET=i686-elf
ENV PATH="$PREFIX/bin:$PATH"

COPY --from=BUILD /opt/cross /opt/cross