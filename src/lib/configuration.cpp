#include "configuration.hpp"

#include <QDir>

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{
}

Configuration::confPath() const
{
    QDir cp(QDir::homePath().append("/.config/speechcontrol"));
    return cp.path();
}
