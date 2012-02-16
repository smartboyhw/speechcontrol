#ifndef CORPUS_HPP
#define CORPUS_HPP

#include <QUrl>
#include <QUuid>
#include <QObject>
#include <QStringList>

#include "sentence.hpp"

class QDomDocument;

namespace SpeechControl
{

class Corpus;

class Sentence;

class Dictionary;

typedef QList<Corpus*> CorpusList;
typedef QMap<QUuid, Corpus*> CorpusMap;

/**
 * @brief Represents a corpus of text to be used for training acoustic models.
 *
 * Corpuses are used throughtout SpeechControl to encapsulate the text and audioPath
 * to be recorded for transcription. Here, the term "transcription" largely means the
 * act of recording text and linking it to its recorded audio to ensure that upon
 * the act of compiling a model, whether or not one is adapting or building, that
 * the approriate text is linked up with its approriate audio segment.
 *
 * This class allows developers to learn more, however, about not only what's been
 * recorded but also when did training begin, how much time has been poured into the
 * training process and when did it end. Since all of the information is stored to XML,
 * the amount of information that can be stored into the corpuses are only limited by
 * the capacity of XML.
 *
 * @todo Consider allowing more meta-data to be added to corpuses in the future.
 *
 * @see AcousticModel
 * @see Sentence
 * @see Dictionary
 */

class Corpus : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Corpus)
    Q_PROPERTY (SentenceList Sentences READ sentences)
    Q_PROPERTY (Dictionary* Dictionary READ dictionary)
    Q_PROPERTY (const QUuid Uuid READ uuid)

    friend class Sentence;

    friend class Dictionary;

public:
    Corpus (const QUuid&);
    virtual ~Corpus();
    /**
     * @brief Obtains an existing Corpus from its specified UUID.
     * @param p_uuid The UUID of the corpus to be found.
     * @return A pointer to a @c Corpus object if found, else NULL.
     */
    static Corpus* obtain (const QUuid&);

    /**
     * @brief Generates a new, generic corpus from a list of strings.
     * @param p_lst The list of strings ( as a @c QStringList ) to be used.
     * @return A pointer to the generated @c Corpus .
     */
    static Corpus* create (const QStringList&);

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
     * @note This method does a bit of nitty gritty searching, just checks for a folder.
     */
    static const bool exists (const QUuid&);

    /**
     * @brief Adds a sentence to this @c Corpus .
     * @param p_sntct The @c Sentence to be added.
     * @return The @c Sentence that was added.
     */
    Sentence* addSentence (Sentence*);

    /**
     * @brief Adds a sentence to this @c Corpus , in its more raw format.
     * @param p_str The text that represents the @c Sentence.
     * @param p_audio The @c QFile that represents the transcribed audio of the @c Sentence.
     * @return The @c Sentence that was formed and then added.
     */
    Sentence* addSentence (const QString&, const QFile*);

    Corpus& operator<< (Sentence*);
    Corpus& operator<< (SentenceList&);

    const QDateTime timeStarted() const;
    const QDateTime timeLastModified() const;
    const QDateTime timeCompleted() const;
    Dictionary* dictionary() const;
    SentenceList sentences() const;
    Sentence* sentenceAt (const int&) const;
    const QUuid uuid() const;
    void erase() const;

public slots:
    void load (const QUuid&);
    void save();

private:
    static QUrl getPath (const QUuid&);
    static CorpusMap s_lst;

    QUuid m_uuid;
    QUrl audioPath() const;
    QDomDocument* m_dom;
    SentenceList m_sntncLst;
    Dictionary* m_dict;
};

}
#endif // CORPUS_HPP
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on; 
