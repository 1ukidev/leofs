# https://github.com/fntlnz/fuse-example

# Find the FUSE includes and library
#
#  FUSE_INCLUDE_DIR - where to find fuse.h, etc.
#  FUSE_LIBRARIES   - List of libraries when using FUSE.
#  FUSE_FOUND       - True if FUSE lib is found.

# check if already in cache, be silent
if(FUSE_INCLUDE_DIR)
    set(FUSE_FIND_QUIETLY TRUE)
endif(FUSE_INCLUDE_DIR)

# find includes
find_path(
    FUSE_INCLUDE_DIR fuse.h
    /usr/local/include/osxfuse
    /usr/local/include
    /usr/include
)

# find lib
if(APPLE)
    set(FUSE_NAMES libosxfuse.dylib fuse)
else(APPLE)
    set(FUSE_NAMES fuse)
endif(APPLE)
    find_library(
        FUSE_LIBRARIES
        NAMES ${FUSE_NAMES}
        PATHS /lib64 /lib /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib /usr/lib/x86_64-linux-gnu
    )

include("FindPackageHandleStandardArgs")
find_package_handle_standard_args(
    "FUSE"
    DEFAULT_MSG
    FUSE_INCLUDE_DIR
    FUSE_LIBRARIES
)

mark_as_advanced(FUSE_INCLUDE_DIR FUSE_LIBRARIES)
