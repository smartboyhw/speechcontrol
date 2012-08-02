#ifndef CORE_HPP
#define CORE_HPP

#include "global.hpp"

#include <QObject>
#include <QDebug>

namespace SpeechControl {

class Core : public QObject
{
    Q_OBJECT
    static Core *_instance;

public:
    explicit Core(int *argc, char **argv[], QObject *parent = 0);
    virtual ~Core();

    static Core* instance();

    /**
     * @brief Get basic configuration path.
     * @return Basic configuration path
     */
    QString confPath() const;

    /**
     * @brief Setup configuration.
     */
    static void setup();

    void test();

};

} // SpeechControl

#endif // CORE_HPP
