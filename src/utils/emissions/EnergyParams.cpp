/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2022 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    EnergyParams.cpp
/// @author  Jakob Erdmann
/// @date    Sept 2021
///
// A class for parameters used by the emission models
/****************************************************************************/
#include <config.h>
#include <utils/common/StringUtils.h>
#include <utils/common/ToString.h>
#include <utils/vehicle/SUMOVTypeParameter.h>

#include "EnergyParams.h"


// ===========================================================================
// method definitions
// ===========================================================================

EnergyParams::EnergyParams(const SUMOVTypeParameter* typeParams) {

    // default values from
    // Kurczveil, T., López, P.Á., & Schnieder, E. (2014). Implementation of an Energy Model and a Charging Infrastructure in SUMO.
    myMap[SUMO_ATTR_VEHICLEMASS] = 1000.;
    myMap[SUMO_ATTR_FRONTSURFACEAREA] = 5.;
    myMap[SUMO_ATTR_AIRDRAGCOEFFICIENT] = 0.6;
    myMap[SUMO_ATTR_INTERNALMOMENTOFINERTIA] = 0.01;
    myMap[SUMO_ATTR_RADIALDRAGCOEFFICIENT] = 0.5;
    myMap[SUMO_ATTR_ROLLDRAGCOEFFICIENT] = 0.01;
    myMap[SUMO_ATTR_CONSTANTPOWERINTAKE] = 100.;
    myMap[SUMO_ATTR_PROPULSIONEFFICIENCY] = 0.9;
    myMap[SUMO_ATTR_RECUPERATIONEFFICIENCY] = 0.8;
    myMap[SUMO_ATTR_RECUPERATIONEFFICIENCY_BY_DECELERATION] = 0.0;
    myMap[SUMO_ATTR_ANGLE] = 0.;  // actually angleDiff in the last step
    // @todo set myVecMap defaults as needed

    // Default values for the MMPEVEM
    myMap[SUMO_ATTR_WHEELRADIUS] = 0.3588;                // [m]
    myMap[SUMO_ATTR_MAXIMUMTORQUE] = 310.0;               // [Nm]
    // @todo SUMO_ATTR_MAXIMUMPOWER predates the MMPEVEM emission model. Do you want to set this somewhere else or to another value?
    myMap[SUMO_ATTR_MAXIMUMPOWER] = 107000.0;             // [W]
    myMap[SUMO_ATTR_GEAREFFICIENCY] = 0.96;               // [1]
    myMap[SUMO_ATTR_GEARRATIO] = 10.0;                    // [1]
    myMap[SUMO_ATTR_MAXIMUMRECUPERATIONTORQUE] = 95.5;    // [Nm]
    myMap[SUMO_ATTR_MAXIMUMRECUPERATIONPOWER] = 42800.0;  // [W]
    myMap[SUMO_ATTR_INTERNALBATTERYRESISTANCE] = 0.1142;  // [Ohm]
    myMap[SUMO_ATTR_NOMINALBATTERYVOLTAGE] = 396.0;       // [V]
    myCharacteristicMapMap.insert(std::pair<SumoXMLAttr, CharacteristicMap>(SUMO_ATTR_POWERLOSSMAP, CharacteristicMap("2,1|-1e9,1e9;-1e9,1e9|0,0,0,0")));  // P_loss_EM = 0 W for all operating points in the default EV power loss map

    if (typeParams != nullptr) {
        for (auto item : myMap) {
            myMap[item.first] = typeParams->getDouble(toString(item.first), item.second);
        }
        for (auto item : myVecMap) {
            myVecMap[item.first] = typeParams->getDoubles(toString(item.first), item.second);
        }
        for (auto item : myCharacteristicMapMap) {
            std::string characteristicMapString = typeParams->getParameter(toString(item.first), "");
            if (characteristicMapString != "") {
                myCharacteristicMapMap.at(item.first) = CharacteristicMap(typeParams->getParameter(toString(item.first)));
            }
        }
    }
}




EnergyParams::~EnergyParams() {}


void
EnergyParams::setDouble(SumoXMLAttr attr, double value) {
    myMap[attr] = value;
}

double
EnergyParams::getDouble(SumoXMLAttr attr) const {
    auto it = myMap.find(attr);
    if (it != myMap.end()) {
        return it->second;
    } else {
        throw UnknownElement("Unknown Energy Model parameter: " + toString(attr));
    }
}

const std::vector<double>&
EnergyParams::getDoubles(SumoXMLAttr attr) const {
    auto it = myVecMap.find(attr);
    if (it != myVecMap.end()) {
        return it->second;
    } else {
        throw UnknownElement("Unknown Energy Model parameter: " + toString(attr));
    }
}

const CharacteristicMap&
EnergyParams::getCharacteristicMap(SumoXMLAttr attr) const {
    auto it = myCharacteristicMapMap.find(attr);
    if (it != myCharacteristicMapMap.end()) {
        return it->second;
    } else {
        throw UnknownElement("Unknown Energy Model parameter: " + toString(attr));
    }
}

bool
EnergyParams::knowsParameter(SumoXMLAttr attr) const {
    return myMap.find(attr) != myMap.end()
           || myVecMap.find(attr) != myVecMap.end()
           || myCharacteristicMapMap.find(attr) != myCharacteristicMapMap.end();
}

/****************************************************************************/
