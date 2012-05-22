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

#include <QString>
#include <QVariantMap>

namespace SpeechControl
{

class AcousticModel;
class NoiseDictionary;
struct AcousticModelPrivate;

struct AcousticModelPrivate {
    AcousticModelPrivate (const AcousticModel* p_model);
    QVariantMap m_params;           ///< Holds the properties of the model.
    QString m_path;                 ///< Holds the path to the base directory of the acoustic model.
    NoiseDictionary* m_noisedict;   ///< Holds information about the noise dictionary.
    AcousticModel* m_mdl;
    void loadFeatureParameters();
    void loadNoiseDictionary();
};

}
// kate: indent-mode cstyle; replace-tabs on; 
