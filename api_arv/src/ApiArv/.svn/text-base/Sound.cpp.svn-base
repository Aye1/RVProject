/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "Sound.h"
#include <fmod.h>
#include <iostream>
using namespace std;

namespace arv {

/*********************************************************************************************/

void printErrorFMOD() {
    #define PRINT_ERROR(message) std::cerr << "> FMOD error : " << message << "." << std::endl; return;
    switch(FSOUND_GetError()) {
        case FMOD_ERR_NONE             : PRINT_ERROR("No errors")
        case FMOD_ERR_BUSY             : PRINT_ERROR("Cannot call this command after FSOUND_Init. Call FSOUND_Close first")
        case FMOD_ERR_UNINITIALIZED    : PRINT_ERROR("This command failed because FSOUND_Init or FSOUND_SetOutput was not called")
        case FMOD_ERR_INIT             : PRINT_ERROR("Error initializing output device")
        case FMOD_ERR_ALLOCATED        : PRINT_ERROR("Error initializing output device, but more specifically, the output device is already in use and cannot be reused")
        case FMOD_ERR_PLAY             : PRINT_ERROR("Playing the sound failed")
        case FMOD_ERR_OUTPUT_FORMAT    : PRINT_ERROR("Soundcard does not support the features needed for this soundsystem (16bit stereo output)")
        case FMOD_ERR_COOPERATIVELEVEL : PRINT_ERROR("Error setting cooperative level for hardware")
        case FMOD_ERR_CREATEBUFFER     : PRINT_ERROR("Error creating hardware sound buffer")
        case FMOD_ERR_FILE_NOTFOUND    : PRINT_ERROR("File not found")
        case FMOD_ERR_FILE_FORMAT      : PRINT_ERROR("Unknown file format")
        case FMOD_ERR_FILE_BAD         : PRINT_ERROR("Error loading file")
        case FMOD_ERR_MEMORY           : PRINT_ERROR("Not enough memory or resources")
        case FMOD_ERR_VERSION          : PRINT_ERROR("The version number of this file format is not supported")
        case FMOD_ERR_INVALID_PARAM    : PRINT_ERROR("An invalid parameter was passed to this function")
        case FMOD_ERR_NO_EAX           : PRINT_ERROR("Tried to use an EAX command on a non EAX enabled channel or output")
        case FMOD_ERR_CHANNEL_ALLOC    : PRINT_ERROR("Failed to allocate a new channel")
        case FMOD_ERR_RECORD           : PRINT_ERROR("Recording is not supported on this machine")
        case FMOD_ERR_MEDIAPLAYER      : PRINT_ERROR("Windows Media Player not installed so cannot play wma or use internet streaming")
        case FMOD_ERR_CDDEVICE         : PRINT_ERROR("An error occured trying to open the specified CD device")
        }
    #undef PRINT_ERROR
    }

/*********************************************************************************************/

#define ERROR(message) std::cerr << "- error in arv::Sound : " << message << "." << std::endl;
#define ERROR_RETURN(message, ret) { ERROR(message) return ret; }
#define ERROR_MSG(message)              { ERROR(message) printErrorFMOD(); }
#define ERROR_MSG_RETURN(message, ret)  { ERROR(message) printErrorFMOD(); return ret; }

/******************************************************************************************/

class SoundSample : public Sound::Sample{
    public:
        static SoundSample* load(const char *name_or_data, int index= FSOUND_UNMANAGED, unsigned int mode = FSOUND_NORMAL, int offset = 0, int length = 0) {
            FSOUND_SAMPLE *handle = FSOUND_Sample_Load(index, name_or_data, mode, offset, length);
            if (handle == NULL) ERROR_MSG_RETURN("loading the sample failed", NULL);
            return new SoundSample(handle);
            }
        
        virtual ~SoundSample() {
            FSOUND_Sample_Free(handle);
            }
        
        virtual float duration() const {
            int freq;
            FSOUND_Sample_GetDefaults(handle, &freq, NULL, NULL, NULL);         // get the sampling frequency for this sound sample
            unsigned int samples = FSOUND_Sample_GetLength(handle);             // number of samples
            return float(samples) / float(freq);
            }
                
        virtual bool play() {
            channel = FSOUND_PlaySound(FSOUND_FREE, handle);
            return channel != -1;
            }
        
        virtual bool prepare() {
            return play() && pause();
            }
        
        virtual bool stop() const {
            return FSOUND_StopSound(channel);
            }

        virtual bool pause() const {
            return FSOUND_SetPaused(channel, true);
            }

        virtual bool resume() const {
            return FSOUND_SetPaused(channel, false);
            }

        virtual bool setLoopMode(bool loop) const {
            return FSOUND_Sample_SetMode(handle, loop ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF);
            }

    private:
        FSOUND_SAMPLE* handle;
        int channel;
    private:
        SoundSample(FSOUND_SAMPLE* handle) : handle(handle), channel(0) {}
    };

/******************************************************************************************/

Sound::Sound(int maxChannels) : maxChannels(maxChannels) {
    }

Sound::~Sound() {
    for(unsigned int i=0; i<samples.size(); i++) delete samples[i];
    samples.clear();
    }

bool Sound::init() {
    if (!FSOUND_SetOutput(FSOUND_OUTPUT_OSS)) ERROR_RETURN("SetOutput failed", false);
    // FSOUND_OUTPUT_WINMM or FSOUND_OUTPUT_DSOUND for windows
    const int frequency = FSOUND_GetOutputRate();
    if (!FSOUND_Init(frequency, maxChannels, 0)) ERROR_MSG_RETURN("Init failed", false);
    return true;
    }

void Sound::close() {
    FSOUND_Close();
    }

Sound::Sample* Sound::add(const char* name) {
    Sample *sample = SoundSample::load(name, FSOUND_UNMANAGED, FSOUND_NORMAL, 0, 0);
    if (sample != NULL) samples.push_back(sample);
    return sample;
    }

bool Sound::remove(int i) {
    if (!contains(i))
        ERROR_RETURN("cannot remove : sample does not exist", false)
    delete samples[i];
    samples.erase(samples.begin() + i);
    return true;
    }

Sound::Sample* Sound::sample(int i) const { return contains(i) ? samples[i] : NULL; }

bool Sound::contains(int i) const {
    return (i >= 0 && static_cast<unsigned int>(i) < samples.size());
    }

bool Sound::stop() const {
    return FSOUND_StopSound(FSOUND_ALL);
    }

bool Sound::pause() const {
    return FSOUND_SetPaused(FSOUND_ALL, true);
    }

bool Sound::resume() const {
    return FSOUND_SetPaused(FSOUND_ALL, false);
    }

}

