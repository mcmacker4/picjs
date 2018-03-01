#include "Picture.h"

#include <cstring>

namespace PicJS {

    // Class methods
    Picture::Picture(uint32_t width, uint32_t height) {
        this->width = width;
        this->height = height;
        this->buffer = new uint32_t[width*height];
        memset(this->buffer, 0x00, width * height * sizeof(int32_t));
    }

    Picture::~Picture() {
        delete [] this->buffer;
    }

    void Picture::setPixel(uint32_t x, uint32_t y, uint32_t color) {
        this->buffer[y * this->width + x] = color; 
    }

    uint32_t Picture::getPixel(uint32_t x, uint32_t y) {
        return this->buffer[y * this->width + x];
    }

    // NodeJS Stuff

    Persistent<Function> Picture::constructor;
    
    void Picture::Init(Local<Object> exports) {

        Isolate* isolate = exports->GetIsolate();

        //Declare constructor
        Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New(isolate, New);
        constructorTemplate->SetClassName(String::NewFromUtf8(isolate, "Picture"));
        constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        //Picture.width (accessor)
        constructorTemplate->InstanceTemplate()->SetAccessor(
            String::NewFromUtf8(isolate, "width"),
            GetWidth
        );
        //Picture.height (accessor)
        constructorTemplate->InstanceTemplate()->SetAccessor(
            String::NewFromUtf8(isolate, "height"),
            GetHeight
        );

        //Assign prototype methods
        NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "setPixel", SetPixel);
        NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getPixel", GetPixel);

        constructor.Reset(isolate, constructorTemplate->GetFunction());
        exports->Set(String::NewFromUtf8(isolate, "Picture"), constructorTemplate->GetFunction());

    }

    void Picture::New(const FunctionCallbackInfo<Value>& args) {

        Isolate* isolate = args.GetIsolate();

        //Not a constructor call.
        if(!args.IsConstructCall()) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Function is a constructor. Please use the `new` keyword.")));
            return;
        }

        //Not enough arguments
        if(args.Length() < 2) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Invalid number of arguments. Expected two integers.")));
            return;
        }

        //Invalid argument type
        if(!args[0]->IsUint32() || !args[1]->IsUint32()) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Invalid argument type. Expected two integers.")));
            return;
        }
        
        uint32_t width = args[0]->Uint32Value();
        uint32_t height = args[1]->Uint32Value();

        Picture* picture = new Picture(width, height);
        picture->Wrap(args.This());

        args.GetReturnValue().Set(args.This());

    }

    void Picture::SetPixel(const FunctionCallbackInfo<Value>& args) {

        Isolate* isolate = args.GetIsolate();

        //Not enough arguments
        if(args.Length() < 3) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Invalid number of arguments. Expected three integers.")));
            return;
        }

        //Invalid argument types
        if(!args[0]->IsUint32() || !args[1]->IsUint32() || !args[2]->IsUint32()) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Invalid argument type. Expected three integers.")));
            return;
        }

        uint32_t x = args[0]->Uint32Value();
        uint32_t y = args[1]->Uint32Value();
        uint32_t color = args[2]->Uint32Value();

        Picture* picture = ObjectWrap::Unwrap<Picture>(args.This());
        picture->setPixel(x, y, color);

    }

    void Picture::GetPixel(const FunctionCallbackInfo<Value>& args) {

        Isolate* isolate = args.GetIsolate();

        //Not enough arguments
        if(args.Length() < 2) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Invalid number of arguments. Expected two integers.")));
            return;
        }

        //Invalid argument types
        if(!args[0]->IsUint32() || !args[1]->IsUint32()) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Invalid argument type. Expected two integers.")));
            return;
        }

        uint32_t x = args[0]->Uint32Value();
        uint32_t y = args[1]->Uint32Value();

        Picture* picture = ObjectWrap::Unwrap<Picture>(args.This());
        uint32_t color = picture->getPixel(x, y);

        args.GetReturnValue().Set(Uint32::New(isolate, color));

    }

    void Picture::GetWidth(Local<Name> property, const PropertyCallbackInfo<Value>& info) {
        Picture* picture = ObjectWrap::Unwrap<Picture>(info.This());
        info.GetReturnValue().Set(Uint32::New(info.GetIsolate(), picture->width));
    }

    void Picture::GetHeight(Local<Name> property, const PropertyCallbackInfo<Value>& info) {
        Picture* picture = ObjectWrap::Unwrap<Picture>(info.This());
        info.GetReturnValue().Set(Uint32::New(info.GetIsolate(), picture->height));
    }

}