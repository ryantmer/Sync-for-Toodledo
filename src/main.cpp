#include "ToodleDoTen.hpp"
#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>

#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);
    ToodledoTen toodleDoTen;
    return Application::exec();
}
