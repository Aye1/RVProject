/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_MOUSE__
#define __ARV_MOUSE__

namespace arv {

    /** @brief Interface with a mouse.
    *   This interface provides direct access to mouse state (buttons state and absolute position
    *   on screen of the pointer). It can also be 'grabbed' to avoid mouse events to be send to
    *   other applications while the current program is running.
    */
    class Mouse {
        public:
            /** This type represents the three mouse buttons. */
            enum Button { LEFT = 0, MIDDLE = 1, RIGHT = 2 };
            
        public:
            bool init();
                /**< Initializes this mouse interface.
                 * Needs to be called before any other operation on the mouse.
                 @return \c true on success, \c false if some error occured.
                 */
            void update();
                /**< Updates the mouse state.
                 * Needs to be called before getting mouse state (position, buttons).
                 */
            void close();
                /**< Closes mouse interface.
                 * Needs to be called once mouse acquisition is terminated, before deleting this instance.
                 */
            
            int xPos() const;
                /**< Gets horizontal position of cursor (in pixels).
                 @return horizontal position of cursor in pixels, from left to right, starting from 0.
                 */
            int yPos() const;
                /**< Gets vertical position of cursor (in pixels).
                 @return vertical position of cursor in pixels, from top do bottom, starting from 0.
                 */
            void getPos(int *pos) const;
                /**< Gets cursor position.
                 * Cursor position is expressed in pixels, the origin (0,0) being the topmost leftmost pixel of the screen.
                 */
            
            bool isPressed(Button button)    const;
                /**< Tests if specified button is pressed.
                 @param button the button to test.
                 @return true iff the specified button was pressed.
                 */
            bool stateChanged(Button button) const;
                /**< Tests if specified button state changed since last update.
                 @param button the button to test.
                 @return true iff the specified button's state changed since last update.
                 */
            
            void grab() const;
                /**< Grabs the mouse events.
                 * Subsequent mouse events are not transmitted to the window manager.
                 @warning be careful with this feature, it prevents all usual window operations (e.g. not possible to close the window using the cross icon!).
                 */
            void ungrab() const;
                /**< Stops grabing mouse events.
                 * Needs to be called to end mouse grabbing after a call to \c grab().
                 */
            
        private:
            class X; X *data;
            int xCursor, yCursor;
            bool state[3], statePrev[3];
        };
    
    }

#endif

