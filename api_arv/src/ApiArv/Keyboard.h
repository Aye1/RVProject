/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_KEYBOARD__
#define __ARV_KEYBOARD__

#include <vector>

namespace arv {

    /** @brief Interface with a keyboard. */
    class Keyboard {
        public:
            /** This type is used to represent key codes.
             * Key codes can be obtained from their string representation using the \c keyCode() method:
             @code
             *  Keyboard kb;
             *  ...     // keyboard initialization
             *  Keyboard::Code keyA = kb->keyCode("A");
             *  if (kb.isPressed(keyA)) {
             *      ... // key 'A' is pressed
             *      }
             @endcode
             * String representations of most keys are straight forward ("A", "1", "Space", ect.),
             * but some could be tricky to guess (and furthermore theu are case sensitive). So the
             * easiest way to get string representations for any key, use the \c activeKeys() method
             * together with the \c keyString() method :
             @code
             *  Keyboard kb;
             *  kb.init();
             *  while (acquisition) {
             *      kb.update();
             *      std::vector<Keyboard::Code> keys = kb.activeKeys();
             *      for(unsigned int i=0; i<keys.size(); i++)
             *          std::cout << kb.keyString(keys[i]) << endl;
             *      }
             *  kb.close();
             @endcode
             */
            typedef unsigned char Code;
    
        public:
            Keyboard();     /**< Creates a new instance of \c Keyboard.*/

            bool init();
                /**< Initialization of the keyboard interface.
                 @return \c true on success, \c false if some error occured.
                 */
            void update();
                /**< Updates keyboard state.
                 * Needs to be called before any state querying method.
                 */
            void close();
                /**< Closes keyboard interface.
                 * Needs to be called when this interface is not needed anymore, before destroying this object.
                 */
            
            bool  exists(char *s)     const;
                /**< Tests if the specified string corresponds to any existing key representation.
                 @param s the case sensitive string representation to test.
                 @return true iff the specified string corresponds to an existing key representation.
                 */
            Code  keyCode(char *s)    const;
                /**< Converts the (case sensitive) string representation of a key into its corresponding code.
                 @param s the case sensitive string representation to convert.
                 @return the corresponding key code. Result if unspecified if \c exists(s) returns \c false.
                 */
            char *keyString(Code key) const;
                /**< Returns the string representation of the specified key.
                 @param key the key code to convert.
                 @return the string representation of the specified key.
                 */
            
            bool isPressed(Code key)    const;
                /**< Tests if the specified key is pressed.
                 @param key the key to test.
                 @return true iff the specified key was pressed during last update.
                 */
            bool stateChanged(Code key) const;
                /**< Tests if the state of the specified key just changed.
                 @param key the key to test.
                 @return true iff the state of the specified key changed between the two last updates.
                 */
    
            bool justPressed(Code key) const;
                /**< Tests if the specified specific key was just pressed.
                 @param key the key to test.
                 @return \c true iff the specified key has been pressed between the two last updates.
                 */
            bool justReleased(Code key) const;
                /**< Tests if the specified specific key was just released.
                 @param key the key to test.
                 @return \c true iff the specified key has been released between the two last updates.
                 */
            
            std::vector<Code> activeKeys() const;
                /**< Returns the list of keys that were pressed during last update.
                 @return the list of all the keys that were pressed during last update.
                 */
    
            void grab() const;
                /**< Grabs the keyboard events.
                 * Subsequent key events are not transmitted to the window manager.
                 @warning be careful with this feature, it will block even the \c Ctrl-C event
                 *        is blocked so using a \c kill command in an other shell (opened using the mouse)
                 *        would be necessary if the program freezes in this mode.
                 */
            void ungrab() const;
                /**< Stops grabing keyboard events.
                 * Needs to be called to end keyboard grabbing after a call to \c grab().
                 */
            
        private:
            class X; X* data;
            char state[32];         // keyboard state (1 bit per key)
            char lastState[32];     // keyboard state before last update
        };
    
    }

#endif

