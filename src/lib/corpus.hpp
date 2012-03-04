/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
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

#ifndef CORPUS_HPP
#define CORPUS_HPP

#include <QMap>
#include <QUrl>
#include <QUuid>
#include <QObject>
#include <QStringList>

#include <sentence.hpp>

class QDomDocument;

namespace SpeechControl {
class Corpus;
class Sentence;
class Dictionary;

/**
 * @brief Represents a list of Corpus elements.
 **/
typedef QList<Corpus*> CorpusList;

/**
 * @brief Represents a mapping of QUuids to Corpus elements.
 **/
typedef QMap<QUuid, Corpus*> CorpusMap;

/**
 * @brief Represents a corpus of text to be used for training acoustic models.
 *
 * Corpuses are used throughout SpeechControl to encapsulate the text and audio-path
 * to be recorded for transcription. Here, the term "transcription" largely means the
 * act of recording text and linking it to its recorded audio to ensure that upon
 * the act of compiling a model, whether or not one is adapting or building, that
 * the appropriate text is linked up with its appropriate audio segment.
 *
 * This class allows developers to learn more, however, about not only what's been
 * recorded but also when did training begin, how much time has been poured into the
 * training process and when did it end. Since all of the information is stored to XML,
 * the amount of information that can be stored into the corpuses are only limited by
 * the capacity of XML.
 *
 * @see AcousticModel
 * @see Sentence
 * @see Dictionary
 */
class SPCH_EXPORT Corpus : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Corpus )
    Q_PROPERTY ( SentenceList Sentences READ sentences )    ///< Represents the Sentence objects of this Corpus.
    Q_PROPERTY ( Dictionary* Dictionary READ dictionary )   ///< Represents the Dictionary used by this Corpus.
    Q_PROPERTY ( const QUuid Uuid READ uuid )               ///< The identifying UUID of this Corpus.
    friend class Sentence;
    friend class Dictionary;

public:
    /**
     * @brief Default constructor.
     *
     * Loads a Corpus with the specified UUID.
     *
     * @note You can check if the load process was successful by evaluating @c isValid() == true.
     * @param p_uuid The UUID to load this Corpus with.
     **/
    explicit Corpus ( const QUuid& p_uuid );

    /**
     * @brief Destructor.
     *
     **/

    virtual ~Corpus();
    /**
     * @brief Obtains an existing Corpus from its specified UUID.
     * @param p_uuid The UUID of the corpus to be found.
     * @return A pointer to a @c Corpus object if found, else NULL.
     */
    static Corpus* obtain ( const QUuid& p_uuid );

    /**
     * @brief Generates a new, generic corpus from a list of strings.
     * @param p_list The list of strings ( as a @c QStringList ) to be used.
     * @return A pointer to the generated @c Corpus .
     */
    static Corpus* create ( const QStringList& p_list );

    /**
     * @brief Obtains a listing of all of the corpuses.
     * @return A list ( @c CorpusList ) of all of the corpuses discoverable by SpeechControl.
     * @note This list only pertains to the corpuses listed under the current user in the SpeechControl data directory.
     */
    static CorpusList allCorpuses();

    /**
     * @brief Determines the existence of a specified corpus.
     * @param p_uuid The UUID of a @c Corpus to be found.
     * @return True, if the @c Corpus exists; otherwise, returns false.
     * @note This method does a bit of nitty-gritty searching, just checks for a folder.
     */
    static bool exists ( const QUuid& p_uuid );

    /**
     * @brief Adds a sentence to this @c Corpus .
     * @param p_sentence The @c Sentence to be added.
     * @return The @c Sentence that was added.
     */
    Sentence* addSentence ( Sentence* p_sentence );

    /**
     * @brief Adds a sentence to this @c Corpus , in its more raw format.
     * @param p_text The text that represents the @c Sentence.
     * @param p_audio The @c QFile that represents the transcribed audio of the @c Sentence.
     * @return The @c Sentence that was formed and then added.
     */
    Sentence* addSentence ( const QString& p_text, const QFile* p_audio );

    /**
     * @brief Determines the time when training of this @c Corpus began.
     * @return A @c QDateTime symbolizing the time when training began.
     */
    const QDateTime timeStarted() const;

    /**
     * @brief Determines the time when training of this @c Corpus was last done.
     * @return A @c QDateTime symbolizing the time when training last occurred.
     */
    const QDateTime timeLastModified() const;

    /**
     * @brief Determines the time when training of this @c Corpus was completed.
     * @return A @c QDateTime symbolizing the time when training ended.
     */
    const QDateTime timeCompleted() const;

    /**
     * @brief Obtains the dictionary used in reference to this @c Corpus.
     * @return A pointer to a @c Dictionary object to refer to this @c Corpus.
     */
    Dictionary* dictionary() const;

    /**
     * @brief Obtains a list of @c Sentence objects that are held by this corpus.
     * @return A @c SentenceList holding all of the @c Sentence objects represented within this @c Corpus .
     */
    SentenceList sentences() const;

    /**
     * @brief Obtains a specific @c Sentence object at a specified index.
     * @param p_index The index that of which the sentence should be picked at.
     * @return A pointer to the @c Sentence object at that index, or NULL.
     */
    Sentence* sentenceAt ( const int& p_index ) const;

    /**
     * @brief Obtains the @c QUuid identifying this @c Corpus
     * @return A @c QUuid
     */
    const QUuid uuid() const;

    /**
     * @brief Erases the data of this @c Corpus.
     * @note This nullifies the corpus and makes it invalid.
     */
    void erase();

    /**
     * @brief Generates a copy of this @c Corpus.
     * @return A pointer to a copy of this @c Corpus, or NULL if the copy operation failed.
     */
    Corpus* clone() const;

    /**
     * @brief Determines if this Corpus is valid.
     * @return True if the Corpus is valid, false if not.
     */
    bool isValid() const;

    /**
     * @brief Stream operator to add sentences.
     * @param p_sentence The Sentence to add.
     * @return A reference to the Corpus object being streamed.
     **/
    Corpus& operator<< ( Sentence* p_sentence );

    /**
     * @brief Stream operator to add a list of sentences.
     *
     * @param p_sentenceList The list of Sentence objects to add.
     * @return A reference to the Corpus object being streamed.
     **/
    Corpus& operator<< ( SentenceList& p_sentenceList );

public slots:
    /**
     * @brief Loads the @c Corpus data using its identifying @c QUuid.
     * @param p_uuid The @c QUuid identifying this @c Corpus.
     */
    void load ( const QUuid& p_uuid );

    /**
     * @brief Saves this @c Corpus's information.
     */
    void save();

    /**
     * @brief Obtains the path to a Corpus's data.
     *
     * @param p_uuid The UUID of the Corpus.
     * @return A QUrl to the Corpus data or an invalid one if the UUID doesn't point to a valid Corpus.
     **/
    static QUrl getPath ( const QUuid& p_uuid );

private:
    /**
     * @brief Obtains a path to the Corpus XML data.
     * @return A QUrl with the location of the Corpus's data.
     **/
    QUrl audioPath() const;

    QUuid m_uuid;               ///< Holds the UUID of this Corpus.
    QDomDocument* m_dom;        ///< The QDomDoc representing the Corpus's data.
    SentenceList m_sntncLst;    ///< The list of sentences held by this Corpus.
    Dictionary* m_dict;         ///< The dictionary generated or being used by this Corpus.

    static CorpusMap s_lst;     ///< Used to centralize the obtaining of @c Corpus objects.
};
}

#endif // CORPUS_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
