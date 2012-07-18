#ifndef CORE_HPP
#define CORE_HPP

#include "global.hpp"

#include <QObject>
#include <QDebug>

namespace SpeechControl {

class Core : public QObject
{
    Q_OBJECT
    SC_SINGLETON(Core)

public:
    explicit Core(QObject *parent = 0);
    Core(int *argc, char **argv[], QObject *parent = 0);

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

} // SpeechControl

#endif // CORE_HPP
