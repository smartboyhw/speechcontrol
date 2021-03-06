/*
    Copyright (c) 2012, Synthetic Intellect Institute <sii@thesii.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the Synthetic Intellect Institute nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Synthetic Intellect Institute <sii@thesii.org> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Synthetic Intellect Institute <sii@thesii.org> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef FILEAUDIORECORDER_HPP
#define FILEAUDIORECORDER_HPP

#include <QtCore/QObject>
#include <QString>
#include <QMap>
#include <QFile>

#include <QGst/Pipeline>
#include <QGst/Bin>
#include <QGst/Pad>
#include <QGst/ElementFactory>
#include <QGst/PropertyProbe>

namespace SpeechControl
{

class System;

namespace Audio
{

class FileRecorder : public QObject
{
    Q_OBJECT

public:
    explicit FileRecorder(QObject* parent = 0);
    virtual ~FileRecorder();

    /**
     * @brief Initialize FileAudioRecorder with the destination file.
     *
     * @param outFile Path to the output file.
     * @param parent Parent QObject. Defaults to NULL.
     **/
    FileRecorder(QString _outFile, QObject* parent = 0);

    /**
     * @brief Initialize FileAudioRecorder with the destination file.
     *
     * @param outFile QFile instance of the output file.
     * @param parent Parent QObject. Defaults to NULL.
     **/
    FileRecorder(QFile& _outFile, QObject* parent = 0);

    /**
     * @brief Set output file using its path.
     *
     * @param path Path to the output file
     **/
    void setFile(QString path);

    /**
     * @brief Set output file using its instance.
     *
     * @param file QFile instance of the output file.
     **/
    void setFile(QFile& file);

    bool isActive() const;

public slots:
    void start();
    void stop();

signals:
    void started();
    void stopped();

private:
    QString outFile;
    bool active;

    QGst::PropertyProbePtr audioProbe;
    QGst::PipelinePtr pipeline;

    void findDevices();
    void probeForDevices(const QGst::PropertyProbePtr& propertyProbe);

    QGst::BinPtr createAudioSrcBin();

    void onBusMessage(const QGst::MessagePtr& message);
};

}
}
#endif // FILEAUDIORECORDER_HPP
