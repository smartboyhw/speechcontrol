/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
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
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef XDO_HPP
#define XDO_HPP

#include <QObject>

struct xdo;

namespace SpeechControl {
namespace Dictation {
/**
 * @brief Allows the emulating of keyboard events to the X Windowing system.
 *
 * In order to properly implement the dictation feature of SpeechControl, the ability
 * to send keyboard signals to the X Windowing system.
 **/
class KeyboardEmulator : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Destructor.
     **/
    virtual ~KeyboardEmulator ( );

    /**
     * @brief Sends a key to the X system.
     *
     * Passes p_character as the key to be passed into the
     * X windowing system.
     *
     * @param p_character The character to be passed into X.
     * @return bool True if it was passed successfully, false otherwise.
     **/
    bool sendKey ( const QChar& p_character );

    /**
     * @brief Sends a set of keys to the X system.
     *
     * Sends a string of keys into the X windowing system.
     *
     * @note This is a convenience method for sendKey().
     * @param p_characters The characters to be passed into X.
     * @return bool True if all of the characters passed in successfully, false if one fails.
     **/
    bool sendKeys ( const QString& p_characters );

    /**
     * @brief Obtains a pointer to the KeyboardEmulator instance.
     * @return :Dictation::KeyboardEmulator*
     **/
    static KeyboardEmulator* instance();

private:
    /**
     * @brief Constructor.
     **/
    explicit KeyboardEmulator ( );
    xdo* m_xdo;
    static KeyboardEmulator* s_inst;        ///< Instance.
};
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
