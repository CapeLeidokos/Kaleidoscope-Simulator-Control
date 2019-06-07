/* -*- mode: c++ -*-
 * Kaleidoscope-Simulator-Control 
 *     -- Serial control for a virtual firmware runs
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Kaleidoscope.h>

namespace kaleidoscope {
namespace plugin {
   
class SimulatorControl : public Plugin {
   public:
    
      void setEnabled(bool state) { enabled_ = state; }
      bool getEnabled() const { return enabled_; }
      
      void setActivationKeys(uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2) {
         magic_switch_1_row_ = row1;
         magic_switch_1_col_ = col1;
         magic_switch_2_row_ = row2;
         magic_switch_2_col_ = col2;
      }

      EventHandlerResult onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state);
      EventHandlerResult beforeEachCycle();

   private:
   
      void checkEnabledToggled(byte row, byte col, uint8_t key_state);
      
   private:
      
      bool enabled_ = false;
      
      bool magic_switch_1_active_ = false;
      bool magic_switch_2_active_ = false;
      
      uint8_t magic_switch_1_row_ = 0;
      uint8_t magic_switch_1_col_ = 0;
      
      uint8_t magic_switch_2_row_ = 0;
      uint8_t magic_switch_2_col_ = 15;
      
      static constexpr uint8_t n_bytes_ = 
         KeyboardHardware.matrix_rows*KeyboardHardware.matrix_columns/8;
         
      uint8_t key_bitfield_[n_bytes_];
};
   
} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::SimulatorControl SimulatorControl;
