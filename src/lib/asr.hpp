/**
 *   @file asr.hpp
 *   Copyright (C) 2012  Adrian Borucki <gentoolx@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef ASR_HPP
#define ASR_HPP

#define MODELDIR "/usr/share/pocketsphinx/model"

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QDir>

#include <QGlib/Value>

#include <QGst/Pipeline>
#include <QGst/Bus>
#include <QGst/Message>

namespace SpeechControl
{

/**
 * @brief Automatic Speech Recognition class
 * ASR is an abstract class responsible for handling GStreamer
 * audio and PocketSphinx plugins and management of dictionaries,
 * language and acoustic models. It also performs acoustic training and
 * adjusts models to its needs.
 */

class ASR : public QObject
{

    Q_OBJECT

protected:
    enum ASRState {
        NotReady = 0,
        Ready
    } _state;

    // Gstreamer objects
    QGst::PipelinePtr   _pipeline;
    QGst::ElementPtr    _psphinx;
    QGst::ElementPtr    _vader;
    QGst::BusPtr        _bus;

    // Map from gstreamer elements to their names
    QMap<QString, QString> _gstElements;

    /**
     * @brief Do all needed preparation steps
     */
    void _prepare();

public:
    explicit ASR (QObject* parent = 0);

    ASR (QGst::PipelinePtr pipeline, QObject* parent = 0);

    /**
     * This constructor creates GStreamer elements from description.
     */
    ASR (const char* description, QObject* parent = 0);

    /**
     * This constructor creates GStreamer elements from description.
     */
    ASR (const QString& description, QObject* parent = 0);

    virtual ~ASR();

    /**
     * @brief Get description of the standard Bin
     * @returns String with standard description.
     */
    static QString getStandardDescription();

    /**
     * @brief Get underlying decoder
     * @returns Pointer to the decoder.
     */
    QGlib::Value getPsDecoder() const;

    QDir getLanguageModel() const;

    QDir getDictionary() const;

    QDir getAcousticsModel() const;

    /**
     * @brief Get the pointer to the internal Pipeline
     * @returns Pointer to the internal Pipeline.
     */
    const QGst::PipelinePtr getPipeline() const;

    /**
     * @brief Get 'pocketsphinx' element
     * @returns Pointer to the 'pocketsphinx' element.
     */
    const QGst::ElementPtr getPocketSphinx() const;

    /**
     * @brief Get 'vader' element
     * @returns Pointer to the 'vader' element.
     */
    const QGst::ElementPtr getVader() const;

    /**
     * @brief Get ASR message bus
     * @returns Pointer to the ASR bus.
     */
    const QGst::BusPtr getBus() const;

    /**
     * @brief Set PocketSphinx element property
     * @param property Name of the property.
     * @param value Value for the property.
     */
    template<typename T>
    void setPsProperty(const QString& property, T value)
    {
        _psphinx->setProperty(property.toStdString().c_str(), value);
    }

    /**
     * @brief Set VADER element property
     * @param property Name of the property.
     * @param value Value for the propery.
     */
    template<typename T>
    void setVaderProperty(const QString& property, T value)
    {
        _vader->setProperty(property.toStdString().c_str(), value);
    }

    void setLanguageModel(const QString& path);

    void setDictionary(const QString& path);

    void setAcousticModel(const QString& path);

    /**
     * @brief Check whether ASR is ready to use
     */
    bool ready() const;
    
    /**
     * @brief Run the pipeline
     */
    void run();

    /**
     * @brief Pause the pipeline
     * This method simply forces VADER to be in the silent region.
     */
    void pause();

    /**
     * @brief Stop the pipeline
     * This method puts the whole pipline in the StatePaused state.
     */
    void stop();

signals:
    /// @todo Useful or not?
    void finished(QString& result);
        
public slots:
    void asrPartialResult (const QString& text, const QString& uttid);
    void asrResult (const QString& text, const QString& uttid);
    virtual void applicationMessage (const QGst::MessagePtr& message) = 0;
};

}

#endif // ASR_HPP
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;
