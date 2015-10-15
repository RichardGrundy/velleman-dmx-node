
#include <node.h>
#include <v8.h>
#include "./dmx.h"

namespace vdmx {
 
 using v8::internal::Arguments;
 using v8::Handle;
 using v8::HandleScope;
 using v8::FunctionTemplate;
 using v8::Local;
 using v8::Function;
using v8::Object;
using v8::String;
using v8::Value;

   
Handle<Value> CreateFunction(const Arguments& args) {
  HandleScope scope;

  Local<FunctionTemplate> tpl = FunctionTemplate::New(dmxOpen);
  Local<Function> fn = tpl->GetFunction();
  fn->SetName(String::NewSymbol("dmxOpen")); // omit this to make it anonymous

  return scope.Close(fn);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateFunction)->GetFunction());
}

NODE_MODULE(vdmx, Init)

}