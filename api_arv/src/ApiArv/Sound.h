/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_SOUND__
#define __ARV_SOUND__

#include <vector>

namespace arv {

    /** A sound sample set allowing to play multiple sound samples. */
    class Sound {
        public:
            /** Representation of a single sound sample.
             * The only was to instantiate this class is to use the \c add() method of class \c Sound.
             */
            class Sample {
                public:
                    virtual float duration() const = 0;
                        /**< Computes this sample duration.
                         @return duration in seconds.
                         */
                    virtual bool play() = 0;
                        /**< Plays the sample.
                         @return \c true on success, false if some error occured.
                         */
                    virtual bool prepare() = 0;
                        /**< Prepares the sample to be played.
                         * This is equivalent to calling \c pause() just after calling \c play().
                         * It allows to prepare multiple samples to be played at the same time by calling \c resume() once they are all prepared, e.g:
                         @code
                         * Sample *sample1 = sound->load(...);      // <sound> type is Sound*
                         * Sample *sample2 = sound->load(...);
                         * sample1->prepare();
                         * sample2->prepare();
                         * sound->resume();     // <sample1> and <sample2> start playing together
                         @endcode
                         @return \c true on success, false if some error occured.
                         */
                    virtual bool stop() const = 0;
                        /**< Stops playing the sample.
                         @return \c true on success, false if some error occured.
                         */
                    virtual bool pause() const = 0;
                        /**< Pauses the samples if it is playing.
                         @return \c true on success, false if some error occured.
                         */
                    virtual bool resume() const = 0;
                        /**< Resumes playing of the sample if it was previously paused.
                         @return \c true on success, false if some error occured.
                         */
                    virtual bool setLoopMode(bool loop) const = 0;
                        /**< Sets the loop mode.
                         * If \c loop = \c true, the sample will loop, else it will just play once.
                         @return \c true on success, false if some error occured.
                         */
                protected:
                    virtual ~Sample() {}
                    friend class Sound;     // to allow class Sound to destroy Sample instances
                };
        
        public:
            Sound(int maxChannels = 32);
                /**< Creates a new sound set.
                 @param maxChannels the maximum of sound samples that can be played at the same time.
                 @pre \p maxChannels must be less than or equal to \c 1024.
                 */
            ~Sound();
                /**< Destroys this object.
                 * The \c close() method must have been called before.
                 */
            
            bool init();
                /**< Initalizes the sound device.
                 * Must be called before any other of the following methods.
                 @return \c true on success, false if some error occured.
                 */
            void close();
                /**< Closes the sound device.
                 * Must be called before destroying this object.
                 */
            
            Sample* add(const char* fileName);
                /**< Loads a sound sample.
                 @param fileName a valid sound sample file name (allowed formats : wav, mp3, ogg).
                 */
            bool remove(int i);
                /**< Removes a sample from this sample set.
                 @param i the index of the sample to remove.
                 */
    
            bool contains(int i) const;
                /**< Tells if specified index is a valid sample index.
                 @return \c true iff the specified index is a valid index.
                 */
            Sample* sample(int i) const;
                /**< Retrieves a sample from its index.
                 @param i the index of the sample to obtain.
                 @return the corresponding sample or \c NULL if it does not exist (i.e. if \c contains(i) returns \c false).
                 */
            
            bool stop() const;
                /**< Stops all currently playing sound samples.
                 @return \c true on success, false if some error occured.
                 */
            bool pause() const;
                /**< Pauses all currently playing sound samples.
                 @return \c true on success, false if some error occured.
                 */
            bool resume() const;
                /**< Resumes all currently paused sound samples.
                 @return \c true on success, false if some error occured.
                 */
            
        private:
            std::vector<Sample*> samples;
            int maxChannels;
        };
    
    }

#endif        

