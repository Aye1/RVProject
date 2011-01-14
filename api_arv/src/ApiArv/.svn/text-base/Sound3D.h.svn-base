/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_SOUND_3D__
#define __ARV_SOUND_3D__

#include <al.h>
#include <alc.h>
#include <alut.h> 
#include <vector>

namespace arv {

    /** Interface for spatial sound rendering. */
    class Sound3D {
        public:
            
            /** This class represents a sound source with a speed and velocity. */
            class Source {
                public:
                    ~Source();
                        /**< Destroys the object and frees accociated ressources.
                         */
                    void setState(const float* position, const float* velocity=NULL);
                        /**< Sets the spatial state of the source.
                         @param position an array containing 3 values, the (x,y,z) coordinates of the new position for this source,
                                         or \c NULL to leave position unchanged.
                         @param velocity an array containing 3 values, the (x,y,z) coordinates of the new velocity for this source,
                                         or \c NULL to leave velocity unchanged.
                         */
                    void play();
                        /**< Plays the sound.
                         * If this source was paused, sound continues to play is resumed from where it stopped.
                         */
                    void pause();
                        /**< Pauses the sound.
                         */
                    void stop();
                        /**< Stop the sound.
                         */
                    void setLoopMode(bool loop=false);
                        /**< Sets the looping mode.
                         @param loop sound will be looping if set to \c true, else it will play only once.
                         */
                    void setGain(float gain);
                        /**< Sets the gain (volume).
                         @param gain new value for the gain.
                         */
                    void setPitch(float pitch);
                        /**< Sets the pitch shift (frequency multiplier).
                         @param pitch new value for the pitch.
                         */
                    
                private:
                    Source(ALuint src, ALuint buf) : src(src), buf(buf) {}
                    ALuint src;    // AL name for this source
                    ALuint buf;    // AL name for this source's buffer
                friend class Sound3D;
                };
            
            
        public:
            static Sound3D* create();
                /**< Creates a new instance.
                 @return the newly created instance or \c NULL if some error occured.
                 */
            
            ~Sound3D();
                /**< Destroys this instance and frees associated resssources.
                 */
            
            void setListener(const float* position, const float* velocity, const float* frontVector, const float *upVector);
                /**< Sets the listener state.
                 @param position an array containing 3 values, the (x,y,z) coordinates of the listener's position.
                 @param velocity an array containing 3 values, the (x,y,z) coordinates of the listener's velocity.
                 @param frontVector an array containing 3 values, the (x,y,z) coordinates of the listener's front vector
                        (i.e. the direction where the listener is looking).
                 @param upVector an array containing 3 values, the (x,y,z) coordinates of the listener's up vector
                        (i.e. the vertical direction of the listener).
                 */
            
            Source* addSource(const char *fileName, const float* position=NULL, const float *velocity=NULL);
                /**< Adds a sound source to the scene.
                 @param fileName the name of an audio file (WAV).
                 @param position an array containing 3 values, the (x,y,z) coordinates of source's position, of \c NULL to leave the position unchanged.
                 @param velocity an array containing 3 values, the (x,y,z) coordinates of source's velocity, of \c NULL to leave the velocity unchanged.
                 @return the created sound source of \c NULL if some error occured.
                 */
            
            void remove(Source *src);
                /**< Removes and deletes the specified source.
                 @param src the source to remove.
                 */
            
            Source* source(int i) const;
                /**< Gets a sound source of the current scene.
                 @param i the index for the source.
                 @return the source corresponding to specified index or \c NULL if it does not exist.
                 */
            
            int numSources()  const;
                /**< Gets the number of sources for the current scene.
                 @return the current number of sources.
                 */
            
        private:
            Sound3D(ALCcontext *context, ALCdevice *device) : context(context), device(device) {}
        private:
            ALCcontext *context;            // OpenAL context
            ALCdevice  *device;             // OpenAL device
            std::vector<Source*> sources;   // the list of sources
        };
    
    }

#endif

