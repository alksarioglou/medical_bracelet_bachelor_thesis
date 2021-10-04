#include <HeartSpeed.h>
#include <MyTimer2.h>


/*!
 * @file heart_test.ino
 * @copyright   [DFRobot](http://www.dfrobot.com), 2016
 * @copyright   GNU Lesser General Public License
 * @author [jianghao](hao.jiang@dfrobot.com)
 * @version  V1.0
 * @date  20160-07-8
 */

/*!
 * @brief Library for DFRobot's IR Position RAW_DATA
 * @author [jianghao](hao.jiang@dfrobot.com)
 */
#include "HeartSpeed.h"

HeartSpeed heartspeed(A1);                  ///<The serial port for at observe pulse.
//HeartSpeed heartspeed(A1,RAW_DATA);       ///<The serial port mapper, observation of ECG diagram.

/* Print the position result */
void mycb(uint8_t rawData, int value)
{
  if(rawData){
    Serial.println(value);
  }else{
    Serial.print("HeartRate Value = "); Serial.println(value);
  }
}
void setup() {
  Serial.begin(115200);
  heartspeed.setCB(mycb);    ///Callback function.
  heartspeed.begin();///The pulse test.
}

void loop() {

}

/******************************************************************************
  Copyright (C) <2016>  <jianghao>
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  Contact: hao.jiang@dfrobot.com
 ******************************************************************************/
