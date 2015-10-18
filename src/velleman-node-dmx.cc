#include<iostream>                                                                                                                                 
#include <node.h>
#include <nan.h>
using namespace std;

/***/
// ==========================================================================
// Velleman K8062 DMX controller library for VM116/K8062
// ==========================================================================

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "dmx.h"

#define CLAMP_TO_UBYTE(v) ((v) > 255 ? 255 : (v))

int   * maxChanAddr;        // control register for # of channels to update
ubyte * exitAddr;           // control register to exit deamon
ubyte * chanData;           // 512 byte array of channel data

ubyte * shm;                // shared memory segment containing data & ctrl regs
int     shmid = -1;         // handel to shared memory segment

// ==========================================================================
// open the DMX connection
// ==========================================================================

int dmxOpen()
{
  // get the shared memory created by the deamon

    shmid = shmget ( 0x56444D58 , sizeof ( ubyte ) * 515 , 0666 );

    if ( shmid == -1 ) {
      printf ( "error[%d] - is dmxd running?\n" , errno );
      return ( errno );
    }

    // set up control and data registers

    shm = ( ubyte *) shmat ( shmid, NULL, 0 );

    maxChanAddr  = ( int * ) shm;
    exitAddr     = ( ubyte * ) maxChanAddr + 2;
    chanData     = ( ubyte * ) maxChanAddr + 3;
    return 0;
}

void _dmxOpen(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Number> ret = Nan::New(dmxOpen());
    info.GetReturnValue().Set(ret);
}

// ==========================================================================
// close the DMX connection
// ==========================================================================

void dmxClose()
{
    if ( shmid != -1 ) shmdt ( shm );
}

void _dmxClose(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    dmxClose();
}

// ==========================================================================
// dmxSetMaxChannels -- set the maximum # of channels to send
// ==========================================================================

void dmxSetMaxChannels ( int maxChannels )
{
    *maxChanAddr = maxChannels;
}

void _dmxSetMaxChannels(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsNumber()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }
    int nMaxChannels = info[0]->IntegerValue(); // IntegerValue is int & NumberValue is double

    dmxSetMaxChannels(nMaxChannels);
}

// ==========================================================================
// dmxSetValue -- set the value for a DMX channel
// ==========================================================================

void dmxSetValue ( ubyte channel , ubyte data )
{
    //printf("\n%u %u\n", channel, data);
    chanData[channel] = data;
}

void _dmxSetValue(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsNumber() || !info[1]->IsNumber()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }
    
    uint32_t nChannel = CLAMP_TO_UBYTE(info[0]->Uint32Value());
    uint32_t nData    = CLAMP_TO_UBYTE(info[1]->Uint32Value());

    dmxSetValue((uint8_t)nChannel, (uint8_t)nData);
}

void Init(v8::Local<v8::Object> exports)
{
  exports->Set(Nan::New("dmxOpen").ToLocalChecked(),
	  Nan::New<v8::FunctionTemplate>(_dmxOpen)->GetFunction());

  exports->Set(Nan::New("dmxClose").ToLocalChecked(),
	  Nan::New<v8::FunctionTemplate>(_dmxClose)->GetFunction());

  exports->Set(Nan::New("dmxSetMaxChannels").ToLocalChecked(),
	  Nan::New<v8::FunctionTemplate>(_dmxSetMaxChannels)->GetFunction());

  exports->Set(Nan::New("dmxSetValue").ToLocalChecked(),
	  Nan::New<v8::FunctionTemplate>(_dmxSetValue)->GetFunction());
}

NODE_MODULE(addon, Init)
