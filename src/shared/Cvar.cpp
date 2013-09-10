/*
===========================================================================

Daemon GPL Source Code
Copyright (C) 2013 Unvanquished Developers

This file is part of the Daemon GPL Source Code (Daemon Source Code).

Daemon Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Daemon Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Daemon Source Code.  If not, see <http://www.gnu.org/licenses/>.

===========================================================================
*/

#include "Cvar.h"
#include "../engine/framework/CvarSystem.h"

#include "../engine/qcommon/q_shared.h"
#include "../engine/qcommon/qcommon.h"
#include "./String.h"

namespace Cvar {

    CvarProxy::CvarProxy(std::string name) : name(std::move(name)) {
    }

    void CvarProxy::Register(std::string description, int flags, std::string defaultValue) {
        ::Cvar::Register(this, name, std::move(description), flags, std::move(defaultValue));
    }

    void CvarProxy::SetValue(std::string value) {
        ::Cvar::SetValue(name, std::move(value));
    }

    bool ParseCvarValue(std::string value, int& result) {
        //TODO: this accepts "1a" as a valid int
        return Str::ToInt(std::move(value), result);
    }

    std::string SerializeCvarValue(int value) {
        return std::to_string(value);
    }

}
