/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_YOKE__
#define __ARV_YOKE__

class jsJoystick;

namespace arv {

    /** Interface with a yoke (joystick).
     * Here is a typical way of using it:
     @code
     *  Yoke* yoke = new Yoke(0);
     *  if (yoke == NULL) error();
     *  while (acquisition) {
     *      for (int i=0; i<yoke->numaxes(); i++) {
     *          float a = yoke->axisValue(i);
     *          ..  // do whatever useful with this value
     *          }
     *      if (yoke->buttonPressed(0)) {
     *          ... // the button 0 is pressed
     *          }
     *      ...
     *      }
     *  delete yoke;
     @endcode
     */
    class Yoke {
        public:
            static Yoke* create(int port=0);
                /**< Creates a new instance.
                  @param port the port into which the device is plugged.
                  @return the newly created instance or \c NULL if an error occured.
                  */

            ~Yoke();
                /**< Frees ressources associated with this object.
                 */

            void update();
                /**< Updates the state of the device.
                 * Needs to be called before querying state (with \c axisValue() or \c buttonPressed()).
                 */
            
            int   numAxes()          const { return n; }
                /**< Gets the number of axes for this joystick.
                 @return the number of axes (degrees of freedom) for this joystick.
                 */
                 
            float axisValue(int i)   const { return axes[i]; }
                /**< Returns the current state of the specified axis.
                 @param i the axis to query.
                 @return the value associated to the specified axis.
                 @pre 0 <= \p i < \c numAxes()
                 */
                 
            bool  buttonPressed(int i) const;
                /**< Tells if the specified button is pressed.
                 @return \c true iff the specified button is pressed.
                 @pre 0 <= \p i < 32 */
            
        private:
            Yoke(jsJoystick* handle);
        private:
            jsJoystick* handle;     // PLIB handle for a joystick
            int buttons;            // the buttons state
            float *axes;            // the axes states
            int n;                  // the number of axes
        };

    }

#endif

