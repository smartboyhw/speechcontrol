#ifndef CONTENT_TEST_H
#define CONTENT_TEST_H

#include <qt4/QtCore/QObject>

namespace SpeechControl
{
class TestContent : public QObject
{
    Q_OBJECT

private slots:
    void buildWikipediaContent();

};
}

#endif // CONTENT_TEST_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
