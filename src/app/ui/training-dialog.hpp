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


#ifndef TRAINING_WINDOW_HPP
#define TRAINING_WINDOW_HPP

// Qt includes
#include <QDialog>

// local includes
#include <core.hpp>

namespace Ui
{
/**
 * @brief Generated class used for SpeechControl::Windows::TrainingDialog.
 */
class Training;
}

namespace SpeechControl
{
class Phrase;
class Session;
class DeviceAudioSource;
namespace Windows
{

/**
 * @brief Represents a dialog that manages the graphical processing of acoustic data collection.
 *
 * The training dialog of SpeechControl permits users to build up Session objects with data for
 * training. A progress indicator is provided as well as a visual display of what text is currently
 * needed for training. With this dialog, it's possible to adequately collect volumes of text for
 * training needed to adapt and perfect a model.
 **/
class TrainingDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * Initializes a new Training dialog.
     * @param p_parent The parent QWidget this dialog will latch onto.
     **/
    explicit TrainingDialog (QWidget* p_parent = 0);

    /**
     * @brief Destructor.
     **/
    virtual ~TrainingDialog();
    /**
     * @brief Initiates a new Training session.
     * Starts a new training session with the specified session p_session.
     * @param p_session The @c Session to train.
     * @param p_device The @c DeviceAudioSource to use for recording.
     * @return void
     **/
    static void startTraining (Session* session, DeviceAudioSource* device = 0);

    /**
     * @brief Changes the Session object used by this Training dialog.
     * @warning This can cause serious damage, since it doesn't reload info in the window.
     * @param p_session The @c Session to use in place of the currently used @c Session.
     * @return void
     **/
    void setSession (Session* p_session);

    void setDevice(DeviceAudioSource* p_device);

    DeviceAudioSource* deviceSource() const;

    /**
     * @brief Obtains the Session currently being trained.
     * @return Session* A pointer to the Session being trained or NULL if no session is being trained.
     **/

    Session* session() const;

public slots:
    /**
     * @brief Stops the collection process.
     * @return void
     **/
    void stopCollecting();

    /**
     * @brief Starts the collection process.
     * @return void
     **/
    void startCollecting();

    /**
     * @brief Opens the Training dialog.
     * @see QDialog::open()
     * @overload
     * @return void
     **/
    virtual void open();
    void on_mic_BufferObtained (QByteArray);

private slots:
    void updateProgress (const double p_progress);
    void onMicStoppedListening();
    void onMicStartedListening();
    void on_pushButtonClose_clicked();
    void on_pushButtonProgress_toggled (const bool&);
    void on_pushButtonReset_clicked();
    void on_pushButtonUndo_clicked();
    void on_pushButtonNext_clicked();

private:

    /**
     * @brief Moves training to a specific position within the Session.
     * Focuses training to the p_index'th Phrase in Phrase p_sentence.
     * @param p_index uint The position of the Phrase within the specified sentence.
     * @param p_sentence Phrase* Defaults to 0 (which changes to the current sentence, m_curSntct)
     * @return void
     **/
    void navigateToPart (const uint& p_index);

    /**
     * @brief Moves the focused text to a spot ahead.
     * Moves the current focus of training one position forward.
     * @return void
     **/
    void navigateNextPart();

    Phrase* currentPhrase();
    Phrase* initialPhrase();

    /**
     * @brief Moves the focused text to a previous spot.
     * Moves the current focus of training one position backwards.
     * @return void
     **/
    void navigatePreviousPart();
    bool currentPhraseCompleted();

    Ui::Training* m_ui;              /// The object used to manage the dialog's widgets.
    DeviceAudioSource* m_mic;        /// The Microphone used by this dialog.
    Session* m_session;              /// The current Session being trained.
    uint m_currentPosition;          /// The position of the phrase that of which training began with when the dialog opened.
    uint m_initialPosition;          /// The position of the current phrase being focused on.
    QByteArray m_data;               /// The collected data from the device buffer.
};

}
}

#endif // TRAINING_WINDOW_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
