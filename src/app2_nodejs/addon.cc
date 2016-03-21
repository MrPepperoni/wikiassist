#include <nan.h>

#include "understandinglib/understanding.h"

void getSuggestedReading(const Nan::FunctionCallbackInfo<v8::Value>& info) {

    if (info.Length() < 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsString() || !info[1]->IsNumber()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }
    // get the param
    v8::String::Utf8Value param1(info[0]->ToString());
    //
    //         // convert it to string
    std::string arg0 = std::string(*param1);
    int arg1 = info[1]->NumberValue();
    wiki::ConnectionDesc cd{ "siddhartha", "wiki", "wiki", "3e6196a6feb44d0d66a67ad27685491f" };
    wiki::Wiki w( cd );

    std::set<std::string> const& rv = understanding::getSuggestedTitles( w, arg0, arg1 );
    std::string result;
    for( std::set<std::string>::const_iterator i = rv.begin(), e = rv.end(); i != e; ++i )
    {
        result += *i + " ";
    }

//    Nan::MaybeLocal<v8::String> num = Nan::New(result);

//    info.GetReturnValue().Set(num);
    info.GetReturnValue().Set( Nan::New<v8::String>( result.c_str() ).ToLocalChecked() );
}

void Init(v8::Local<v8::Object> exports) {
    exports->Set(Nan::New("getSuggestedReading").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(getSuggestedReading)->GetFunction());
}

NODE_MODULE(addon, Init)
