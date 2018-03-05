#ifndef PICJS_PICTURE_H
#define PICJS_PICTURE_H

#include <node.h>
#include <node_object_wrap.h>

using namespace v8;
using namespace node;

namespace PicJS {

    enum Channel {
        RED = 0,
        GREEN,
        BLUE,
        ALPHA
    };

    typedef union {
        uint8_t channels[4];
        uint32_t value;

        inline void set(Channel channel, uint8_t value);
    } Pixel;

    class Picture : public ObjectWrap {

    // Class stuff
    private:

        Pixel* pixels;
        uint32_t width;
        uint32_t height;
        
        explicit Picture(uint32_t width, uint32_t height);
        ~Picture();

        inline void setPixel(uint32_t x, uint32_t y, Channel channel, uint32_t value);
        inline Pixel* getPixel(uint32_t x, uint32_t y);

        void resize(uint32_t newWidth, uint32_t newHeight);

    // NodeJS Stuff
    public:
        
        static void Init(Local<Object> exports);

    private:

        static Persistent<Function> constructor;

        //Node class methods
        static void New(const FunctionCallbackInfo<Value>& args);
        static void SetPixel(const FunctionCallbackInfo<Value>& args);
        static void GetPixel(const FunctionCallbackInfo<Value>& args);
        static void Resize(const FunctionCallbackInfo<Value>& args);

        //Getters, used for accessors
        static void GetWidth(Local<Name> property, const PropertyCallbackInfo<Value>& info);
        static void GetHeight(Local<Name> property, const PropertyCallbackInfo<Value>& info);

    };

}

#endif