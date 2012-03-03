/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *            (C) 2012 Adrian Borucki <gentoolx@gmail.com>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  SpeechControl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


#ifndef ASR_HPP
#define ASR_HPP

// Qt includes
#include <QMap>
#include <QDir>
#include <QObject>

// QGlib includes
#include <QGlib/Value>

// QGst includes
#include <QGst/Pipeline>
#include <QGst/Bus>
#include <QGst/Message>

// local includes
#include "config.hpp"
#include "export.hpp"

namespace SpeechControl {
class AcousticModel;
class Dictionary;
/**
 * @brief Implementation of automatic speech recognition.
 *
 * ASR (or automatic speech recognition) is an abstract class responsible
 * for handling GStreamer audio and PocketSphinx plugins and management
 * of dictionaries, language and acoustic models. It also performs
 * acoustic training and adjusts models to its needs.
 */
class SPCH_EXPORT ASR : public QObject {

    Q_OBJECT
    Q_DISABLE_COPY ( ASR )

protected:
    /**
     * @brief Represents the possible states of ASR.
     * @internal
     **/
    enum States {
        NotPrepared = 0,
        Ready,
        Running
    };

    States m_running;
    States m_ready;

    // GStreamer objects
    QGst::PipelinePtr   m_pipeline;     ///< Holds the pipeline for GStreamer.
    QGst::ElementPtr    m_psphinx;      ///< Holds our lucky PocketSphinx object.
    QGst::ElementPtr    m_vader;        ///< Holds the Vader element.
    QGst::BusPtr        m_bus;          ///< Holds the executing bus for GStreamer.

    /**
     * @brief Enacts all of the preparation steps.
     * @internal
     */
    void prepare();

public:
    /**
     * @brief Null constructor.
     * @param p_parent Defaults to 0.
     **/
    explicit ASR ( QObject* p_parent = 0 );

    /**
     * @brief Renders a new ASR instance with a pipeline.
     *
     * @param p_pipeline The pipeline to use for ASR.
     * @param p_parent Defaults to 0.
     **/
    ASR ( QGst::PipelinePtr p_pipeline, QObject* p_parent = 0 );

    /**
     * @brief Renders a new ASR instance from a GStreamer-compatible description.
     *
     * This constructor creates GStreamer elements from description.
     *
     * @param p_description The description to use to build with this ASR instance.
     * @param p_parent Defaults to 0.
     **/
    ASR ( const QString& p_description, QObject* p_parent = 0 );

    /**
     * @brief Destructor.
     *
     **/
    virtual ~ASR();

    /**
     * @brief Get description of the standard Bin
     * @return String with standard description.
     */
    static QString standardDescription();

    /**
     * @brief Get underlying decoder
     * @return Pointer to the decoder.
     */
    QGlib::Value decoder() const;

    /**
     * @brief Obtains the language model used by Sphinx.
     * @return A @c QDir to the langauge model in use.
     */
    QDir languageModel() const;

    /**
     * @brief Obtains the dictionary used.
     * @return A @c QDir pointing to the directory.
     */
    Dictionary* dictionary() const;

    /**
     * @brief Obtains the acoustic model in use.
     * @return The acoustic model used by this.
     */
    AcousticModel* acousticModel() const;

    /**
     * @brief Get the pointer to the internal Pipeline
     * @return Pointer to the internal Pipeline.
     */
    const QGst::PipelinePtr pipeline() const;

    /**
     * @brief Get 'pocketsphinx' element
     * @return Pointer to the 'pocketsphinx' element.
     */
    const QGst::ElementPtr pocketSphinxElement() const;

    /**
     * @brief Get 'vader' element
     * @return Pointer to the 'vader' element.
     */
    const QGst::ElementPtr vaderElement() const;

    /**
     * @brief Get ASR message bus
     * @return Pointer to the ASR bus.
     */
    const QGst::BusPtr busElement() const;

    /**
     * @brief Set PocketSphinx element property
     * @param property Name of the property.
     * @param value Value for the property.
     */
    void setPsProperty ( const QString& p_property, const QVariant& p_value );

    /**
     * @brief Set VADER element property
     * @param property Name of the property.
     * @param value Value for the propery.
     */
    void setVaderProperty ( const QString& p_property, const QVariant& p_value );

    /**
     * @brief Sets the language model to use.
     * @param path The path to the language to be used.
     */
    void setLanguageModel ( const QString& path );

    /**
     * @brief Sets the dictionary to be used.
     * @param path The path to the dictionary.
     */
    void setDictionary ( const QString& path );

    /**
     * @brief Sets the dictionary to be used.
     *
     * @param p_dictionary The Dictionary object to use.
     **/
    void setDictionary ( const Dictionary* p_dictionary );

    /**
     * @brief Sets the acoustic model to be used.
     * @param path The path to the acoustic model.
     */
    void setAcousticModel ( const QString& path );

    /**
     * @brief Sets the acoustic model to be used.
     *
     * @param p_acousticModel The AcousticModel to use.
     **/
    void setAcousticModel ( const AcousticModel* p_acousticModel );

    /**
     * @brief Check whether ASR is ready to use
     */
    bool isReady() const;

    /**
     * @brief Check if ASR is currently running.
     */
    bool isRunning() const;

    /**
     * @brief Pause the pipeline
     * This method simply forces VADER to be in the silent region.
     */
    void togglePause();

signals:
    /// @todo Useful or not?
    void finished ( const QString& result );

public slots:
    /**
     * @brief Starts ASR.
     *
     * Starts the ASR instance by invoking the pipeline's execution.
     */
    bool start();

    /**
     * @brief Stops ASR.
     *
     * Stops the ASR instance by halting the pipeline's execution.
     */
    void stop();

    /**
     * @brief Obtains a partial value from the specified text p_text and utterance p_uttid.
     *
     * @param p_text The text to be passed.
     * @param p_uttid The utterance to be passed.
     **/
    void formPartialResult ( QString& p_text, QString& p_uttid );

    /**
     * @brief Obtains a value from the specified text p_text and utterance p_uttid.
     *
     * @param p_text The text to be passed.
     * @param p_uttid The utterance to be passed.
     **/
    void formResult ( QString& p_text, QString& p_uttid );

    /**
     * @brief Invokes an application-wide message to be raised.
     *
     * @param p_message The message to be passed.
     **/
    virtual void applicationMessage ( const QGst::MessagePtr& p_message ) = 0;

private:
    /**
     * @brief Builds a pipeline from a description p_description.
     *
     * @param p_description The description to be used to build a pipeline.
     **/
    void buildPipeline ( QString p_description );
};

}

#endif // ASR_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
