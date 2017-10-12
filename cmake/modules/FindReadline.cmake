# - Try to find ImageMagick++
# Once done, this will define
#
#  Magick++_FOUND - system has Magick++
#  Magick++_INCLUDE_DIRS - the Magick++ include directories
#  Magick++_LIBRARIES - link these to use Magick++

include(LibFindMacros)

libfind_pkg_detect(Readline readline
  FIND_PATH readline/readline.h
  FIND_LIBRARY readline
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Readline_PROCESS_INCLUDES Readline_INCLUDE_DIR)
set(Readline_PROCESS_LIBS Readline_LIBRARY)
libfind_process(Readline)
