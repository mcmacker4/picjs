#include <node.h>
#include "Picture.h"

using namespace v8;

namespace PicJS {

    void InitModule(Local<Object> exports) {
        Picture::Init(exports);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, InitModule)

}