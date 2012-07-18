#include "core.hpp"
#include "audio/devicemanager.hpp"

#include <QDir>

#include <QGst/Init>

using namespace SpeechControl;

Core::Core(QObject *parent) :
    QObject(parent)
{
}

Core::Core(int *argc, char **argv[], QObject *parent)
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

QString Core::confPath() const
{
    QDir cp(QDir::homePath().append("/.config/speechcontrol"));
    return cp.path();
}

void Core::setup()
{
    s_inst = new Core(NULL, NULL);
}
