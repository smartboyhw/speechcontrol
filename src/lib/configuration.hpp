#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "global.hpp"

#include <QObject>
#include <QDebug>

class Configuration : public QObject
{
    Q_OBJECT
    SC_SINGLETON(Configuration)

public:
    explicit Configuration(QObject *parent = 0);
    Configuration(int *argc, char **argv[], QObject *parent = 0);

    /**
     * @brief Get basic configuration path.
     * @return Basic configuration path
     */
    QString confPath() const;

    /**
     * @brief Setup configuration.
     */
    static void setup();

};

#endif // CONFIGURATION_HPP
