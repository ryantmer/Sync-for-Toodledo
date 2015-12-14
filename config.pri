# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/About.qml) \
        $$quote($$BASEDIR/assets/ActivityDialog.qml) \
        $$quote($$BASEDIR/assets/AddPage.qml) \
        $$quote($$BASEDIR/assets/Cover.qml) \
        $$quote($$BASEDIR/assets/EditPage.qml) \
        $$quote($$BASEDIR/assets/ListPage.qml) \
        $$quote($$BASEDIR/assets/ListPageEditable.qml) \
        $$quote($$BASEDIR/assets/ListPageNoNav.qml) \
        $$quote($$BASEDIR/assets/ListPageReadOnly.qml) \
        $$quote($$BASEDIR/assets/Login.qml) \
        $$quote($$BASEDIR/assets/MainTabbedPane.qml) \
        $$quote($$BASEDIR/assets/Navigation.qml) \
        $$quote($$BASEDIR/assets/Settings.qml) \
        $$quote($$BASEDIR/assets/images/all_tasks.png) \
        $$quote($$BASEDIR/assets/images/checkmark.png) \
        $$quote($$BASEDIR/assets/images/ic_add.png) \
        $$quote($$BASEDIR/assets/images/ic_contexts.png) \
        $$quote($$BASEDIR/assets/images/ic_folders.png) \
        $$quote($$BASEDIR/assets/images/ic_goals.png) \
        $$quote($$BASEDIR/assets/images/ic_info.png) \
        $$quote($$BASEDIR/assets/images/ic_locations.png) \
        $$quote($$BASEDIR/assets/images/ic_reload.png) \
        $$quote($$BASEDIR/assets/images/ic_save.png) \
        $$quote($$BASEDIR/assets/images/ic_settings.png)
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/FilterDataModel.cpp) \
        $$quote($$BASEDIR/src/LoginManager.cpp) \
        $$quote($$BASEDIR/src/PropertiesManager.cpp) \
        $$quote($$BASEDIR/src/SyncForToodledo.cpp) \
        $$quote($$BASEDIR/src/main.cpp)

    HEADERS += \
        $$quote($$BASEDIR/src/FilterDataModel.hpp) \
        $$quote($$BASEDIR/src/LoginManager.hpp) \
        $$quote($$BASEDIR/src/PropertiesManager.hpp) \
        $$quote($$BASEDIR/src/SyncForToodledo.hpp)
}

CONFIG += precompile_header

PRECOMPILED_HEADER = $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs) \
        $$quote($$BASEDIR/../assets/images/*.qml) \
        $$quote($$BASEDIR/../assets/images/*.js) \
        $$quote($$BASEDIR/../assets/images/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}.ts)
