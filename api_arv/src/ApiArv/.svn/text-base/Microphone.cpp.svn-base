/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "Microphone.h"
#include <fmod.h>
#include <math.h>
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

#define ERROR(message) std::cerr << "- error in Microphone : " << message << "." << std::endl;
#define ERROR_RETURN(message, ret) { ERROR(message) return ret; }
#define ERROR_MSG(message)              { ERROR(message) printErrorFMOD(); }
#define ERROR_MSG_RETURN(message, ret)  { ERROR(message) printErrorFMOD(); return ret; }

/******************************************************************************************/

Microphone::Microphone() : record(NULL) {}

bool Microphone::init() {
    if (!FSOUND_SetOutput(FSOUND_OUTPUT_OSS)) ERROR_MSG_RETURN("set output failed", false);;    //@@ necessaire ?
    //FSOUND_SetDriver(1);              //@@@ a voir
    //FSOUND_Record_SetDriver(1);       //@@@ a voir

    //FSound_SetBufferSize()
    if (!FSOUND_Init(44100, 32, 0)) ERROR_MSG_RETURN("init failed", false);
    
    /*
    int n = FSOUND_Record_GetNumDrivers();
    cout << "- number of record drivers = " << n << endl;
    for (int i=0; i<n; i++) {
        const char *name = FSOUND_Record_GetDriverName(i);
        cout << "- driver " << i << " = " << (name == NULL ? "NULL" : name) << endl;
        }

    n = FSOUND_GetNumDrivers();
    cout << "- number of drivers = " << n << endl;
    for (int i=0; i<n; i++) {
        const char *name = FSOUND_GetDriverName(i);
        cout << "- driver " << i << " = " << (name == NULL ? "NULL" : name) << endl;
        }
    */
    return true;
    }

void Microphone::close() {
    FSOUND_Close();
    }

void Microphone::alloc(float time) {
    int frequency = 44100;
    int samples = (int)ceilf(time * frequency);
    if (record != NULL) FSOUND_Sample_Free(record);
    record = FSOUND_Sample_Alloc(FSOUND_UNMANAGED, samples, FSOUND_STEREO | FSOUND_16BITS , frequency, 255, 128, 255);
    }

void Microphone::start() {
    FSOUND_Record_StartSample(record, false);
    }

void Microphone::stop() {
    FSOUND_Record_Stop();
    // FSOUND_PlaySound(FSOUND_FREE, micro_handle.record);
    }

bool Microphone::save(char* fileName) {
    if (record == NULL) ERROR_RETURN("there is no recorded sound to save", false);
    
    unsigned int mode = FSOUND_Sample_GetMode(record);
    int bits = (mode & FSOUND_16BITS) ? 16 : 8;
    int channels = (mode & FSOUND_STEREO) ? 2 : 1;
    int lenbytes = FSOUND_Sample_GetLength(record) * channels * bits / 8;
    int rate;
    FSOUND_Sample_GetDefaults(record, &rate, NULL, NULL, NULL);

    #if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
        #pragma pack(push, 1)
    #endif

    // WAV structures :
    //-----------------
    struct RiffChunk {
        signed char id[4];
        int size;
        };

    struct FmtChunk {
        RiffChunk chunk;
        unsigned short wFormatTag;         // format type
        unsigned short nChannels;          // number of channels (i.e. mono, stereo...)
        unsigned int   nSamplesPerSec;     // sample rate
        unsigned int   nAvgBytesPerSec;    // for buffer estimation
        unsigned short nBlockAlign;        // block size of data
        unsigned short wBitsPerSample;     // number of bits per sample of mono data
        };

    struct DataChunk {
        RiffChunk chunk;
        };

    struct WavHeader {
        RiffChunk chunk;
        signed char rifftype[4];
        };
    
    WavHeader wavHeader = { {{'R', 'I', 'F', 'F'}, sizeof(FmtChunk) + sizeof(RiffChunk) + lenbytes }, {'W', 'A', 'V', 'E'} };
    FmtChunk  fmtChunk  = { {{'f', 'm', 't', ' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, channels, rate, rate * channels * bits / 8, 1 * channels * bits / 8, bits };
    DataChunk dataChunk = { {{'d', 'a', 't', 'a'}, lenbytes } };

    #if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
        #pragma pack(pop)
    #endif
    
    FILE *fp = fopen(fileName, "wb");
    if (fp == NULL) ERROR_RETURN("problem during file creation", false)

    // write out the WAV header :
    //---------------------------
    fwrite(&wavHeader, sizeof(wavHeader), 1, fp);
    fwrite(&fmtChunk,  sizeof(fmtChunk),  1, fp);
    fwrite(&dataChunk, sizeof(dataChunk), 1, fp);

    // lock the sample to get acces to the data :
    //-------------------------------------------
    void *ptr1, *ptr2;
    unsigned int len1, len2;
    FSOUND_Sample_Lock(record, 0, lenbytes, &ptr1, &ptr2, &len1, &len2);
    fwrite(ptr1, len1, 1, fp);
    FSOUND_Sample_Unlock(record, ptr1, ptr2, len1, len2);

    fclose(fp);
    
    return true;
    }

}

