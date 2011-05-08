/* Copyright (c) 2011 Cody Miller, Daniel Norris, Brett Hitchcock.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *****************************************************************************/
 
#ifndef _PIRATE_COMMON_H_
#define _PIRATE_COMMON_H_

//! Every player is assigned a unique integer ID ranging from 1 to 9999999
typedef unsigned int player_id;

//! Some actions in the game occur without a target. In this case, the target
//! should be specified as NO_PLAYER.
const player_id NO_PLAYER = 1000000;



///////////////////////////////////////////////////////////////////////////////
// Smart pointers
///////////////////////////////////////////////////////////////////////////////

//! The OgreSDK does not ship Boost with the ptr_container library, so we will
//! use regular STL containers of Boost's ptr_containers. Unfortunately, the 
//! syntax of this can be kludgy, so these macros are supposed to save a bit of 
//! typing and improve readability. They should be used to declare new 
//! smart pointers in method invocations.

//! This is used to create a smart pointer to an Action
#define SPA(x) (boost::shared_ptr<Action>(x))








///////////////////////////////////////////////////////////////////////////////
// Actions
///////////////////////////////////////////////////////////////////////////////

/** Although a single Action class represents all possible actions, it order to
 * transfer an action across the network, the code needs to know what type
 * of action it is transferring. This enum is used only in the case of 
 * transporting an action across the network.
 */

enum action_t {
  UNKNOWN,
  MOVE_PLAYER,
  WARP_PLAYER,
  ATTACK_PLAYER
};


#endif
