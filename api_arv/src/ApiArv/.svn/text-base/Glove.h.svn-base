/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_GLOVE__
#define __ARV_GLOVE__

#include "SerialPort.h"

class CCyberGlove;

namespace arv {

    #define GLOVE_DOF 18
    
    /** Interface for the CyberGlove device. */
    class Glove {
        public:
            /** This class represents the state of the glove.
             * It is used by class \c Glove to return the current state of the CyberGlove.
             * It consists in 18 integer values corresponding to angles defined at different locations on the hand, as illustrated on the following figure: @image html glove.jpg
             */
            class State {
                public:
                    State();
                        /**< Constructor.
                         * All values are set to 0.
                         */
                    inline       int& operator()(int i)       { return val[i]; }
                        /**< Accessor to values.
                         @param i the index of the value to query.
                         @return a reference on the value corresponding to specified index.
                         */
                    inline const int& operator()(int i) const { return val[i]; }
                        /**< Cont accessor to values.
                         @param i the index of the value to query.
                         @return a const reference on the value corresponding to specified index.
                         */
                    void substract(const State &refState);
                        /**< Substract values of a reference state from values of this state.
                         * This is useful to make this state relative to a reference state (e.g. obtained once during calibration).
                         @param refState the reference state to substract to this state.
                         */
                    void set(const State &state);
                        /**< Sets this state's values to values from another state.
                         @param state the state to set.
                         */
                    
                private:
                    int val[GLOVE_DOF];
                };
        public:
            static Glove* create(SerialPort port = PORT1);
                /**< Creates a new instance.
                 @param port the serial port into which the CyberGlove is plugged.
                 @return the newly created instance or \c NULL if some errro occured.
                 */
            
            ~Glove();
                /**< Destroys this object and frees associated ressources. */
            
            void update(State &state);
                /**< Updates supplied state to the current glove state.
                 @param state gets the updated current state of the glove.
                 */
        
        private:
            Glove(CCyberGlove* glove) : glove(glove) {}
            CCyberGlove* glove;
        };
    
    }

#endif

