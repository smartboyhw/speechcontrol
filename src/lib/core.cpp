#include "core.hpp"
#include "audio/devicemanager.hpp"

#include <QDir>
#include <QDebug>

#include <QGst/Init>

using namespace SpeechControl;

Core* Core::_instance = NULL;

Core::Core(int *argc, char **argv[], QObject *parent) : QObject(parent)
{
    // Init Qt-GStreamer
    if (argc && argv)
        QGst::init (argc, argv);
    else
        QGst::init();

    /// @todo Question: Should we really make these configuration catalogues in the library?
    /// Maybe the application should care about it itself.
    // Core directories
    QDir configDir;
    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/corpora");
    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/dictionaries");
    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/contents");
    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/sessions");
    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/models");

    //// Model directories
    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/models/lm");
    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/models/hmm");

    // Setup audio device manager
    Audio::DeviceManager::setup();
}

Core::~Core()
{
}

Core* Core::instance()
{
    return _instance;
}

QString Core::confPath() const
{
    QDir cp(QDir::homePath().append("/.config/speechcontrol"));
    return cp.path();
}

void Core::setup()
{
    _instance = new Core(NULL, NULL);
}

void Core::test()
{
    qDebug() << "############# TEST SUCCESSFUL #############";
}
