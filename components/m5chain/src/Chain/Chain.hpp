/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _CHAIN_HPP_
#define _CHAIN_HPP_

#include "ChainCommon.hpp"
#include "ChainEncoder.hpp"
#include "ChainAngle.hpp"
#include "ChainJoystick.hpp"
#include "ChainKey.hpp"
#include "ChainToF.hpp"
#include "ChainUart.hpp"
#include "ChainSwitch.hpp"
#include "ChainPIR.hpp"
#include "ChainMIC.hpp"
#include "ChainPedal.hpp"

class Chain : virtual public ChainCommon,
              virtual public ChainEncoder,
              virtual public ChainAngle,
              virtual public ChainJoystick,
              virtual public ChainKey,
              virtual public ChainToF,
              virtual public ChainUart,
              virtual public ChainSwitch,  
              virtual public ChainPIR,
              virtual public ChainMIC,
              virtual public ChainPedal {
public:
private:
};

#endif  // _CHAIN_HPP_
