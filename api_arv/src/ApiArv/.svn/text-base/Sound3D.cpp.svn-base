/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "Sound3D.h"
#include <iostream>

using namespace std;

#define ERROR_RETURN(msg, ret) { std::cerr << "- error in arv::Sound3D : " << msg << "." << std::endl; return ret; }
#define CHECK_AL_ERROR_RETURN(ret) if (checkALError()) return ret;

namespace {
    bool checkALError() {
        // alGetString()
        switch (alGetError()) {
            case AL_INVALID_NAME      : cerr << "- OpenAL error : Invalid name parameter."       << endl; return true;
            case AL_INVALID_ENUM      : cerr << "- OpenAL error : Invalid parameter."            << endl; return true;
            case AL_INVALID_VALUE     : cerr << "- OpenAL error : Invalid enum parameter value." << endl; return true;
            case AL_INVALID_OPERATION : cerr << "- OpenAL error : Illegal call."                 << endl; return true;
            case AL_OUT_OF_MEMORY     : cerr << "- OpenAL error : Unable to allocate memory."    << endl; return true;
            case AL_NO_ERROR:
            default:
                return false;
            };
        }
    }

namespace arv {

/*********************************************************************************************/
// Sound3D implementation :

Sound3D* Sound3D::create() {
    //ALCdevice* device = alcOpenDevice(NULL);
    ALCdevice* device = alcOpenDevice((ALubyte*)"DirectSound3D");
    if (device == NULL)
        ERROR_RETURN("failed to initialize OpenAL device", NULL);

    ALCcontext* context = alcCreateContext(device, NULL);
    if (context == NULL)
        ERROR_RETURN("failed to initialize OpenAL context", NULL);
    
    alcMakeContextCurrent(context);
    return new Sound3D(context, device);
    }

Sound3D::~Sound3D() {
    for (unsigned int i=0; i<sources.size(); i++)
        delete sources[i];
    // disable context :
    alcMakeContextCurrent(NULL);
    // release context(s) :
    alcDestroyContext(context);
    // close device :
    alcCloseDevice(device);
    }

void Sound3D::setListener(const float *position, const float* velocity, const float* frontVector, const float *upVector) {
    alListenerfv(AL_POSITION,    (ALfloat*)position);
    alListenerfv(AL_VELOCITY,    (ALfloat*)velocity);
    ALfloat orientation[6];
    for (int i=0; i<3; i++) {
        orientation[0+i] = frontVector[i];
        orientation[3+i] = upVector[i];
        }
    alListenerfv(AL_ORIENTATION, orientation);
    }

Sound3D::Source* Sound3D::addSource(const char *fileName, const float* position, const float *velocity) {
    // creation of the buffer :
    //-------------------------
        ALuint buffer;
        alGenBuffers(1, &buffer);
        CHECK_AL_ERROR_RETURN(NULL);
    
        //@ mieux : utiliser nouvelle fonction de ALUT :
        //alutCreateBufferFromFile (const char *filename);
    
        ALsizei size, freq;
        ALenum format;
        ALvoid *data;
        ALsizei bits;
        alutLoadWAV(fileName, &data, &format, &size, &bits, &freq);
        if (checkALError()) { alDeleteBuffers(1, &buffer); return NULL; }
    
        alBufferData(buffer, format, data, size, freq);
        if (checkALError()) { alDeleteBuffers(1, &buffer); return NULL; }
        
    // creation of the source :
    //-------------------------
        ALuint source;
        alGenSources(1, &source);
        if (checkALError()) { alDeleteBuffers(1, &buffer); return NULL; }
        
        alSourcei(source, AL_BUFFER, buffer);
        if (position != NULL) alSourcefv(source, AL_POSITION, (ALfloat*)position);
        if (velocity != NULL) alSourcefv(source, AL_VELOCITY, (ALfloat*)velocity);
    
    Source* src = new Source(source, buffer);
    sources.push_back(src);
    return src;
    }

void Sound3D::remove(Sound3D::Source* src) {
    for (unsigned int i=0; i<sources.size(); i++) {
        if (sources[i]->src == src->src) {
            delete sources[i];
            sources.erase(sources.begin() + i);
            break;
            }
        }
    }

Sound3D::Source* Sound3D::source(int i) const {
    if (i < 0 || (unsigned int)i >= sources.size()) return NULL;
    return sources[i];
    }

int Sound3D::numSources()  const {
    return sources.size();
    }
            
/*********************************************************************************************/
// Sound3D::Source implementation :

Sound3D::Source::~Source() {
    alDeleteSources(1, &src);
    alDeleteBuffers(1, &buf);
    }
            
void Sound3D::Source::setState(const float* position, const float* velocity) {
    if (position != NULL) alSourcefv(src, AL_POSITION, (ALfloat*)position);
    if (velocity != NULL) alSourcefv(src, AL_VELOCITY, (ALfloat*)velocity);
    }

void Sound3D::Source::play() {
    alSourcePlay(src);
    }

void Sound3D::Source::pause() {
    alSourcePause(src);
    }

void Sound3D::Source::stop() {
    alSourceStop(src);
    }

void Sound3D::Source::setLoopMode(bool loop) {
    alSourcei(src, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }

void Sound3D::Source::setGain(float gain) {
    alSourcef(src, AL_GAIN, gain);
    }

void Sound3D::Source::setPitch(float pitch) {
    alSourcef(src, AL_PITCH, pitch);
    }

}

