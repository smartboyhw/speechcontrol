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
#include <QString>
#include <QObject>
#include <QDateTime>
#include <QStringList>

#include <app/sessions/phrase.hpp>

class QDomDocument;

namespace SpeechControl
{
class Corpus;
class Dictionary;

/**
 * @brief Represents a list of Corpus elements.
 **/
typedef QList<Corpus*> CorpusList;

/**
 * @brief Represents a mapping of QStrings to Corpus elements.
 **/
typedef QMap<QString, Corpus*> CorpusMap;

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
 * @see Phrase
 * @see Dictionary
 */
class SPCH_EXPORT Corpus : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Corpus)
    friend class Phrase;

public:
    /**
     * @brief Default constructor.
     *
     * Loads a Corpus with the specified ID.
     *
     * @note You can check if the load process was successful by evaluating @c isValid() == true.
     * @param p_id The ID to load this Corpus with.
     **/
    explicit Corpus (const QString& p_id);

    /**
     * @brief Destructor.
     *
     **/

    virtual ~Corpus();
    /**
     * @brief Obtains an existing Corpus from its specified ID.
     * @param p_id The ID of the corpus to be found.
     * @return A pointer to a @c Corpus object if found, else NULL.
     */
    static Corpus* obtain (const QString& p_id);

    /**
     * @brief Generates a new, generic corpus from a list of strings.
     * @param p_text The list of strings ( as a @c QStringList ) to be used.
     * @return A pointer to the generated @c Corpus .
     */
    static Corpus* create (const QStringList& p_text);

    /**
     * @brief Obtains a listing of all of the corpuses.
     * @return A list ( @c CorpusList ) of all of the corpuses discoverable by SpeechControl.
     * @note This list only pertains to the corpuses listed under the current user in the SpeechControl data directory.
     */
    static CorpusList allCorpuses();

    /**
     * @brief Determines the existence of a specified corpus.
     * @param p_id The ID of a @c Corpus to be found.
     * @return True, if the @c Corpus exists; otherwise, returns false.
     * @note This method does a bit of nitty-gritty searching, just checks for a folder.
     */
    static bool exists (const QString& p_id);

    /**
     * @brief Adds a phrase to this @c Corpus .
     * @param p_phrase The @c Phrase to be added.
     * @return The @c Phrase that was added.
     */
    Phrase* addPhrase (Phrase* p_phrase);

    /**
     * @brief Adds a phrase to this @c Corpus , in its more raw format.
     * @param p_text The text that represents the @c Phrase.
     * @param p_audio The @c QFile that represents the transcribed audio of the @c Phrase.
     * @return The @c Phrase that was formed and then added.
     */
    Phrase* addPhrase (const QString& p_text, const QFile* p_audio);

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
     * @brief Obtains a list of @c Phrase objects that are held by this corpus.
     * @return A @c PhraseList holding all of the @c Phrase objects represented within this @c Corpus .
     */
    PhraseList phrases() const;

    /**
     * @brief Obtains a specific @c Phrase object at a specified index.
     * @param p_index The index that of which the phrase should be picked at.
     * @return A pointer to the @c Phrase object at that index, or NULL.
     */
    Phrase* phraseAt (const int& p_index) const;

    /**
     * @brief Obtains the @c QString identifying this @c Corpus
     * @return A @c QString
     */
    const QString id() const;

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
     * @brief Generates a transcription with the accompanying suffix and prefix for silence.
     *
     * @param p_silencePrefix The prefix for silence (Defaults to < s >).
     * @param p_silenceSuffix The suffix for silence (Default to < /s >)
     **/
    QFile* transcription (QString const& p_silencePrefix = "<s>",
                          QString const& p_silenceSuffix = "</s>") const;

    /**
     * @brief Generates a file ids file of all of the audio sources.
     **/
    QFile* fileIds() const;

    /**
     * @brief Stream operator to add phrases.
     * @param p_phrase The Phrase to add.
     * @return A reference to the Corpus object being streamed.
     **/
    Corpus& operator<< (Phrase* p_phrase);

    /**
     * @brief Stream operator to add a list of phrases.
     *
     * @param p_phraseList The list of Phrase objects to add.
     * @return A reference to the Corpus object being streamed.
     **/
    Corpus& operator<< (PhraseList& p_phraseList);

public slots:
    /**
     * @brief Loads the @c Corpus data using its identifying @c QString.
     * @param p_id The @c QString identifying this @c Corpus.
     */
    void load (const QString& p_id);

    /**
     * @brief Saves this @c Corpus's information.
     */
    void save();

    /**
     * @brief Obtains the path to a Corpus's data.
     *
     * @param p_id The ID of the Corpus.
     * @return A QString to the Corpus data or an invalid one if the ID doesn't point to a valid Corpus.
     **/
    static QString getPath (const QString& p_id);

    /**
     * @brief Obtains a path to the Corpus XML data.
     * @return A QString with the location of the Corpus's data.
     **/
    QString audioPath() const;

private:
    void nullify();
    QString m_id;               ///< Holds the ID of this Corpus.
    QDomDocument* m_dom;        ///< The QDomDocument representing the Corpus's data.
    Dictionary* m_dict;         ///< The dictionary generated or being used by this Corpus.
    PhraseList m_phraseList;    ///< The list of phrases held by this Corpus.

    static CorpusMap s_lst;     ///< Used to centralize the obtaining of @c Corpus objects.
};
}

#endif // CORPUS_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
