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


#ifndef ABSTRACTSPHINX_HPP
#define ABSTRACTSPHINX_HPP

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

// PocketSphinx includes
#include <pocketsphinx.h>

// local includes
#include <config.hpp>
#include <export.hpp>


namespace SpeechControl
{

class LanguageModel;

class AbstractAudioSource;
class AcousticModel;
class Dictionary;
/**
 * @brief Implementation of automatic speech recognition.
 *
 * AbstractSphinx is an abstract class responsible
 * for handling GStreamer audio and PocketSphinx plugins and management
 * of dictionaries, language and acoustic models. It also performs
 * acoustic training and adjusts models to its needs.
 */
class SPCH_EXPORT AbstractSphinx : public QObject
{

    Q_OBJECT
    Q_DISABLE_COPY (AbstractSphinx)

protected:
    /**
     * @brief Represents the possible states of the AbstractSphinx.
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
    explicit AbstractSphinx (QObject* p_parent = 0);

    /**
     * @brief Renders a new the AbstractSphinx instance with a pipeline.
     *
     * @param p_pipeline The pipeline to use for the AbstractSphinx.
     * @param p_parent Defaults to 0.
     **/
    AbstractSphinx (QGst::PipelinePtr p_pipeline, QObject* p_parent = 0);

    /**
     * @brief Renders a new the AbstractSphinx instance from a GStreamer-compatible description.
     *
     * This constructor creates GStreamer elements from description.
     *
     * @param p_description The description to use to build with this the AbstractSphinx instance.
     * @param p_parent Defaults to 0.
     **/
    AbstractSphinx (const QString& p_description, QObject* p_parent = 0);

    /**
     * @brief Destructor.
     *
     **/
    virtual ~AbstractSphinx();

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
     */
    LanguageModel* languageModel() const;

    /**
     * @brief Obtains the dictionary used.
     */
    Dictionary* dictionary() const;

    /**
     * @brief Obtains the acoustic model in use.
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
     * @brief Get the AbstractSphinx message bus
     * @return Pointer to the the AbstractSphinx bus.
     */
    const QGst::BusPtr busElement() const;

    /**
     * @brief Gets the element representing the current audio source.
     *
     **/
    const QGst::ElementPtr audioSrcElement() const ;

    /**
     * @brief Gets the element representing the volume element.
     **/
    const QGst::ElementPtr volumeElement() const;

    /**
     * @brief Set PocketSphinx element property
     * @param p_property Name of the property.
     * @param p_value Value for the property.
     */
    void setPsProperty (const QString& p_property, const QVariant& p_value);

    /**
     * @brief Set VADER element property
     * @param p_property Name of the property.
     * @param p_value Value for the propery.
     */
    void setVaderProperty (const QString& p_property, const QVariant& p_value);

    /**
     * @brief Sets the language model to use.
     * @param path The path to the language to be used.
     */
    void setLanguageModel (const QString& p_path);

    /**
     * @brief Sets the language model to use.
     * @param p_languageModel The LanguageModel to be used.
     **/
    void setLanguageModel (const LanguageModel* p_languageModel);

    /**
     * @brief Sets the dictionary to be used.
     * @param p_path The path to the dictionary.
     */
    void setDictionary (const QString& p_path);

    /**
     * @brief Sets the dictionary to be used.
     *
     * @param p_dictionary The Dictionary object to use.
     **/
    void setDictionary (const Dictionary* p_dictionary);

    /**
     * @brief Sets the acoustic model to be used.
     * @param p_path The path to the acoustic model.
     */
    void setAcousticModel (const QString& p_path);

    /**
     * @brief Sets the acoustic model to be used.
     *
     * @param p_acousticModel The AcousticModel to use.
     **/
    void setAcousticModel (const AcousticModel* p_acousticModel);

    /**
     * @brief Check whether the AbstractSphinx is ready to use
     */
    bool isReady() const;

    /**
     * @brief Check if the AbstractSphinx is currently running.
     */
    bool isRunning() const;

    /**
     * @brief Pauses the pipeline.
     *
     * @note This method simply forces VADER to be in the silent region.
     */
    void togglePause();

signals:
    /**
     * Emitted by the AbstractSphinx instance when recognition has produced a result.
     */
    void finished (const QString& p_result);

public slots:
    /**
     * @brief Starts the AbstractSphinx.
     *
     * Starts the the AbstractSphinx instance by invoking the pipeline's execution.
     */
    bool start();

    /**
     * @brief Stops the AbstractSphinx.
     *
     * Stops the the AbstractSphinx instance by halting the pipeline's execution.
     */
    bool stop();

    /**
     * @brief Obtains a partial value from the specified text p_text and utterance p_uttid.
     *
     * @param p_text The text to be passed.
     * @param p_uttid The utterance to be passed.
     **/
    void formPartialResult (QString& p_text, QString& p_uttid);

    /**
     * @brief Obtains a value from the specified text p_text and utterance p_uttid.
     *
     * @param p_text The text to be passed.
     * @param p_uttid The utterance to be passed.
     **/
    void formResult (QString& p_text, QString& p_uttid);

protected slots:

    /**
     * @brief Invokes an application-wide message to be raised.
     *
     * @param p_message The message to be passed.
     **/
    virtual void applicationMessage (const QGst::MessagePtr& p_message) = 0;

protected:
    /**
     * @brief Builds a pipeline from a description p_description.
     *
     * @param p_description The description to be used to build a pipeline.
     **/
    void buildPipeline (QString p_description);
};


class SPCH_EXPORT AudioSourceSphinx : public AbstractSphinx
{
    Q_OBJECT
    Q_DISABLE_COPY(AudioSourceSphinx)

public:
    explicit AudioSourceSphinx (QObject* p_parent = 0);
    AudioSourceSphinx (AbstractAudioSource* p_source, QObject* p_parent = 0);
    virtual ~AudioSourceSphinx();
    AbstractAudioSource* source();
    void setSource(AbstractAudioSource* p_source);

private:
    AbstractAudioSource* m_src;
    virtual void applicationMessage (const QGst::MessagePtr& p_message);
    void linkSource ();
};

}

#endif // ABSTRACTSPHINX_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
