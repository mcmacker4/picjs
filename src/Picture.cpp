#include "Picture.h"

#include <cstring>

namespace PicJS {

    /*
    Pixel union methods
    */

    void Pixel::set(Channel channel, uint8_t value) {
        this->channels[channel] = value;
    }

    /*
    Picture Class methods
    */

    Picture::Picture(uint32_t width, uint32_t height) {
        this->width = width;
        this->height = height;
        this->pixels = new Pixel[width*height];
        memset(this->pixels, 0x00, width * height * sizeof(Pixel));
    }

    Picture::~Picture() {
        delete [] this->pixels;
    }

    inline void Picture::setPixel(uint32_t x, uint32_t y, Channel channel, uint32_t value) {
        if(x >= this->width || y >= this->height) return;
        value = value < 0xFF ? value : 0xFF;
        this->getPixel(x, y)->set(channel, (uint8_t) (value & 0xFF));
    }

    inline Pixel* Picture::getPixel(uint32_t x, uint32_t y) {
        if(x >= this->width || y >= this->height) return nullptr;
        return this->pixels + (y * this->width + x);
    }

    void Picture::resize(uint32_t newWidth, uint32_t newHeight) {

        //Allocate new buffer
        Pixel *newBuffer = new Pixel[newWidth * newHeight];
        memset(newBuffer, 0x0, newWidth * newHeight * sizeof(Pixel));

        //Calculate limits to avoid out-of-bounds reading and writing
        uint32_t maxWidth = this->width < newWidth ? this->width : newWidth;
        uint32_t maxHeight = this->height < newHeight ? this->height : newHeight;

        //Copy pixel by pixel, i'm sure there is a better way, but why should i care?
        for(uint32_t x = 0; x < maxWidth; x++) {
            for(uint32_t y = 0; y < maxHeight; y++) {
                newBuffer[y * newWidth + x] = this->pixels[y * this->width + x];
            }
        }

        Pixel* oldBuffer = this->pixels;

        //Set new data
        this->pixels = newBuffer;
        this->width = newWidth;
        this->height = newHeight;

        //Unallocate old buffer
        delete [] oldBuffer;

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
            String::NewFromUtf8(isolate, "width"), GetWidth);
        //Picture.height (accessor)
        constructorTemplate->InstanceTemplate()->SetAccessor(
            String::NewFromUtf8(isolate, "height"), GetHeight);

        //Assign prototype methods
        NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "setPixel", SetPixel);
        NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getPixel", GetPixel);
        NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "resize", Resize);

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
        if(args.Length() < 2 || !args[0]->IsUint32() || !args[1]->IsUint32()) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "ArgumentException. Expected two integers.")));
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
        if(args.Length() < 3 || !args[0]->IsUint32() || !args[1]->IsUint32() || !args[2]->IsObject()) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "ArgumentException. Usage: setPixel(width: Int, height: Int, pixel: Object).")));
            return;
        }

        uint32_t x = args[0]->Uint32Value();
        uint32_t y = args[1]->Uint32Value();
        Local<Object> jsPixel = args[2]->ToObject(isolate);

        Picture* picture = ObjectWrap::Unwrap<Picture>(args.This());

        Local<Value> jsRed = jsPixel->Get(String::NewFromUtf8(isolate, "red"));
        if(jsRed->IsUint32() && !jsRed->IsNull())
            picture->setPixel(x, y, RED, jsRed->Uint32Value());
        
        Local<Value> jsGreen = jsPixel->Get(String::NewFromUtf8(isolate, "green"));
        if(jsGreen->IsUint32() && !jsRed->IsNull())
            picture->setPixel(x, y, GREEN, jsGreen->Uint32Value());

        Local<Value> jsBlue = jsPixel->Get(String::NewFromUtf8(isolate, "blue"));
        if(jsBlue->IsUint32() && !jsBlue->IsNull())
            picture->setPixel(x, y, BLUE, jsBlue->Uint32Value());
        
        Local<Value> jsAlpha = jsPixel->Get(String::NewFromUtf8(isolate, "alpha"));
        if(jsAlpha->IsUint32() && !jsAlpha->IsNull())
            picture->setPixel(x, y, ALPHA, jsAlpha->Uint32Value());

    }

    void Picture::GetPixel(const FunctionCallbackInfo<Value>& args) {

        Isolate* isolate = args.GetIsolate();

        //Not enough arguments
        if(args.Length() < 2 || !args[0]->IsUint32() || !args[1]->IsUint32()) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "ArgumentException. Expected two integers.")));
            return;
        }

        uint32_t x = args[0]->Uint32Value();
        uint32_t y = args[1]->Uint32Value();

        Picture* picture = ObjectWrap::Unwrap<Picture>(args.This());
        Pixel* pixel = picture->getPixel(x, y);

        if(pixel == nullptr) {
            args.GetReturnValue().SetNull();
            return;
        }

        Local<Object> pixelObj = Object::New(isolate);
        pixelObj->Set(String::NewFromUtf8(isolate, "red"), Uint32::New(isolate, pixel->channels[RED]));
        pixelObj->Set(String::NewFromUtf8(isolate, "green"), Uint32::New(isolate, pixel->channels[GREEN]));
        pixelObj->Set(String::NewFromUtf8(isolate, "blue"), Uint32::New(isolate, pixel->channels[BLUE]));
        pixelObj->Set(String::NewFromUtf8(isolate, "alpha"), Uint32::New(isolate, pixel->channels[ALPHA]));

        pixelObj->Set(String::NewFromUtf8(isolate, "uintValue"), Uint32::New(isolate, pixel->value));

        args.GetReturnValue().Set(pixelObj);

    }

    void Picture::Resize(const FunctionCallbackInfo<Value>& args) {

        Isolate* isolate = args.GetIsolate();
        
        //Check arguments
        if(args.Length() < 2 || !args[0]->IsUint32() || !args[1]->IsUint32()) {
            isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "ArgumentException. Usage: resize(newWidth: Int, newHeight: Int).")));
        }

        Picture* picture = ObjectWrap::Unwrap<Picture>(args.This());
        uint32_t newWidth = args[0]->Uint32Value();
        uint32_t newHeight = args[1]->Uint32Value();

        picture->resize(newWidth, newHeight);

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