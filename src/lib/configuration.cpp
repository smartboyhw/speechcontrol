#include "configuration.hpp"
#include "audio/devicemanager.hpp"

#include <QDir>

#include <QGst/Init>

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{
}

Configuration::Configuration(int *argc, char **argv[], QObject *parent)
{
    // Init Qt-GStreamer
    if (argc && argv)
        QGst::init (argc, argv);
    else
        QGst::init();

    /// @todo Question: Should we really make these configuration catalogues in the library?
    /// Maybe the application should care about it itself.
    // Configuration directories
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

QString Configuration::confPath() const
{
    QDir cp(QDir::homePath().append("/.config/speechcontrol"));
    return cp.path();
}

Configuration::setup()
{
    s_inst = new Configuration(NULL, NULL);
}
