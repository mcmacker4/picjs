#ifndef PICJS_PICTURE_H
#define PICJS_PICTURE_H

#include <node.h>
#include <node_object_wrap.h>

using namespace v8;
using namespace node;

namespace PicJS {

    class Picture : public ObjectWrap {

    // Class stuff
    private:

        uint32_t* buffer;
        uint32_t width;
        uint32_t height;
        
        explicit Picture(uint32_t width, uint32_t height);
        ~Picture();

        void setPixel(uint32_t x, uint32_t y, uint32_t color);
        uint32_t getPixel(uint32_t x, uint32_t y);

    // NodeJS Stuff
    public:
        
        static void Init(Local<Object> exports);

    private:

        static Persistent<Function> constructor;

        static void New(const FunctionCallbackInfo<Value>& args);
        static void SetPixel(const FunctionCallbackInfo<Value>& args);
        static void GetPixel(const FunctionCallbackInfo<Value>& args);

    };

}

#endif