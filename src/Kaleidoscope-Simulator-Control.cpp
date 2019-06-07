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

#include "Kaleidoscope-Simulator-Control.h"

#include "kaleidoscope/plugin/LEDControl.h"
#include "kaleidoscope/plugin/FocusSerial.h"

namespace kaleidoscope {
namespace plugin {
   
void SimulatorControl::checkEnabledToggled(byte row, byte col, uint8_t key_state)
{
   bool changes = false;
   
   // Check for toggling enabled state.
   //
   if((magic_switch_1_row_ == row) && (magic_switch_1_col_ == col)) {
      if (keyToggledOn(key_state)) {
         magic_switch_1_active_ = true;
         changes = true;
      }
      else if(keyToggledOff(key_state)) {
         magic_switch_1_active_ = false;
         changes = true;
      }
   }
   
   if((magic_switch_2_row_ == row) && (magic_switch_2_col_ == col)) {
      if (keyToggledOn(key_state)) {
         magic_switch_2_active_ = true;
         changes = true;
      }
      else if(keyToggledOff(key_state)) {
         magic_switch_2_active_ = false;
         changes = true;
      }
   }
   
   if(changes && (magic_switch_1_active_ && magic_switch_2_active_)) {
      
      enabled_ = !enabled_;
      
//       if(enabled_) {
//          LEDControl::set_all_leds_to(255, 0, 0);
//       }
//       else {
//          LEDControl::set_all_leds_to(0, 0, 0);
//       }
//       
//       LEDControl::syncLeds();
   }
}

EventHandlerResult SimulatorControl::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state)
{
   this->checkEnabledToggled(row, col, key_state);
   
   if(!enabled_) { 
      return EventHandlerResult::OK;
   }
   
   uint16_t pos = row*KeyboardHardware.matrix_columns + col;
   uint8_t byte_id = pos/n_bytes_;
   uint8_t bit_id = pos%n_bytes_;
   
   if(keyToggledOn(key_state)) {
      bitSet(key_bitfield_[byte_id], bit_id);
   }
   else if(keyToggledOff(key_state)) {
      bitClear(key_bitfield_[byte_id], bit_id);
   }
   
   // If we are active we simply consume all events and thus make 
   // sure that no keyboard output is generated.
   //
   return EventHandlerResult::EVENT_CONSUMED;
}
      
EventHandlerResult SimulatorControl::beforeEachCycle()
{
   if(!enabled_) { return EventHandlerResult::OK; }

   for(uint8_t byte_id = 0; byte_id < n_bytes_; ++byte_id) {
      Focus.send(key_bitfield_[byte_id]);
   }
   Focus.sendRaw("\n");
   
   return EventHandlerResult::OK;
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::SimulatorControl SimulatorControl;
