#include "configuration.hpp"

#include <QDir>

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{
}

QString Configuration::confPath() const
{
    QDir cp(QDir::homePath().append("/.config/speechcontrol"));
    return cp.path();
}
