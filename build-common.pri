CONFIG -= debug_and_release
CONFIG( debug, debug|release )  {
  CONFIG -= release
}
else {
  CONFIG -= debug
  CONFIG += release
}

debug:DEFINES += "DEBUG"

DESTDIR = $${PWD}
TRASH_ROOT = $${PWD}/intermediate/$$basename($${_PRO_FILE_PWD_})

release:OBJECTS_DIR = $${TRASH_ROOT}/release/
release:MOC_DIR = $${TRASH_ROOT}/release/
release:RCC_DIR = $${TRASH_ROOT}/release/
release:UI_DIR = $${TRASH_ROOT}/release/

debug:OBJECTS_DIR = $${TRASH_ROOT}/debug/
debug:MOC_DIR = $${TRASH_ROOT}/debug/
debug:RCC_DIR = $${TRASH_ROOT}/debug/
debug:UI_DIR = $${TRASH_ROOT}/debug/
