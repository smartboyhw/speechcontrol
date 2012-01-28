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

#include <QtCore/QObject>

#include <QGlib/Value>

#include <QGst/Pipeline>
#include <QGst/Bus>
#include <QGst/Message>

namespace SpeechControl
{

/**
 * @brief Automatic Speech Recognition class
 * ASR objects hold configured GStreamer pipelines using PocketSphinx to perform speech
 * recognition. Their role is to provide an easy way to connect proper signals, access and
 * set desired configuration options and run, passing ready data to the main application.
 */

class ASR : public QObject
{

    Q_OBJECT

protected:
    QGst::PipelinePtr _pipeline;
    QGst::ElementPtr _psphinx;
    QGst::BusPtr _bus;

public:
    explicit ASR (QObject* parent = 0);

    ASR (QGst::PipelinePtr pipeline, QObject* parent = 0);

    ASR (const char* description, QObject* parent = 0);

    ASR (const QString& description, QObject* parent = 0);

    /**
     * @brief Get description of the standard Bin
     * @returns String with standard description.
     */
    static QString getStandardDescription();

    /**
     * @brief Get the pointer to the internal Pipeline
     * @returns Pointer to the internal Pipeline.
     */
    const QGst::PipelinePtr getPipeline();

    /**
     * @brief Get 'pocketsphinx' element
     * @returns Pointer to the 'pocketsphinx' element.
     */
    const QGst::ElementPtr getPocketSphinx();

    /**
     * @brief Get ASR message bus
     * @returns Pointer to the ASR bus.
     */
    const QGst::BusPtr getBus();

    /**
     * @brief Run the pipeline
     */
    void run();

    /**
     * @brief Pause the pipeline
     */
    void pause();

public slots:
    void asrPartialResult (const QGlib::Value& text, const QGlib::Value& uttid);
    void asrResult (const QGlib::Value& text, const QGlib::Value& uttid);
    virtual void applicationMessage (const QGst::MessagePtr& message) = 0;
};

}

#endif // ASR_HPP
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;
