#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "global.hpp"

#include <QObject>

class Configuration : public QObject
{
    Q_OBJECT
    SC_SINGLETON(Configuration)

public:
    explicit Configuration(QObject *parent = 0);

    /**
     * @brief Get basic configuration path.
     * @return Basic configuration path
     */
    QString confPath() const;

signals:
    
public slots:
    
};

#endif // CONFIGURATION_HPP
