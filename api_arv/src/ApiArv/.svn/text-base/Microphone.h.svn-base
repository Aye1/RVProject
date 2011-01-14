/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_MICROPHONE__
#define __ARV_MICROPHONE__

class FSOUND_SAMPLE;

namespace arv {

    /** Interface with a microphone allowing to record sound samples.
     * The typical way to use this class to record a sound follws:
     @code
     *  Microphone micro;
     *  if (!micro.init()) error();
     *  micro.alloc(5.0f);  // ready to record 5 seconds of sound
     *  micro.start();      // start recording ...
     *  sleep(5);           // wait 5 seconds for the sound to record
     *  micro.stop();       // ... stop recording
     *  if (micro.save("record.wav"))
     *      std::cout << "> sound sample saved successfuly." << std::endl;
     *  else
     *      std::cout << "> error while saving sound sample." << std::endl;
     *  micro.close();
     @endcode
     */
    class Microphone {
        public:
            Microphone();
                /**<
                 * Instantiates a new microhone interface.
                 */
            
            bool init();
                /**< Recording device initialization.
                 * Needs to be called before any recording.
                 @return \c true on success, \c false if some error occured.
                 */
            void close();
                /**< Closes sound device.
                 * Needs to be called before destroying this object.
                 */
            
            void alloc(float time);
                /**< Allocates time for sound recording.
                 * Needs to be called before starting recording with \c start().
                 @param time the maximum recording time for the next recording.
                 */
            void start();
                /**< Starts recording.
                 * Method \c alloc() must have been called before.
                 */
            void stop();
                /**< Stops recording.
                 * Needs to be called before saving the recorded sound with \c save().
                 */
            
            bool save(char* fileName);
                /**< Saves recorded sound into the specified WAV file.
                 @param fileName name of the WAV audio file to write.
                 @return \c true on success, \c false if some error occured.
                 */
            
        private:
            FSOUND_SAMPLE* record;
        };
    
    }

#endif

