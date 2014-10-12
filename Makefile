QMAKE_TARGET  = Toodledo10
PROJECT_DIR	  := $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
I18N_DIR	  := $(PROJECT_DIR)/translations

include mk/cs-base.mk

QMAKE_CFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_LFLAGS_RELEASE += -Wl,-z,relro -pie
QMAKE_POST_LINK = ntoarm-objcopy --only-keep-debug ${DESTDIR}/${QMAKE_TARGET} ${DESTDIR}/${QMAKE_TARGET}.sym && ntoarm-objcopy --strip-all -R.ident --add-gnu-debuglink "${DESTDIR}/${QMAKE_TARGET}.sym" "$@" "${DESTDIR}/${QMAKE_TARGET}"
QMAKE_CFLAGS_RELEASE += -fstack-protector-strong
QMAKE_CXXFLAGS_RELEASE += -fstack-protector-strong
QMAKE_LFLAGS_RELEASE += -Wl,-z,relro