#include <nan.h>
#include "includes/tcp_wrap.h"
#include <sys/socket.h>


// for mac
#if defined(__APPLE__)

#define SOL_IP IPPROTO_IP
#define SO_ORIGINAL_DST 80

// int uv___stream_fd(const uv_stream_t* handle);
// #define uv__stream_fd(handle) (uv___stream_fd((const uv_stream_t*) (handle)))

// for Linux
#else

#include <linux/netfilter_ipv4.h>

#endif


#define uv__stream_fd(handle) ((handle)->io_watcher.fd)

void OriginalDst(const Nan::FunctionCallbackInfo<v8::Value>& info) {

    node::TCPWrap* const wrap = node::Unwrap<node::TCPWrap>(info[0]->ToObject());
    uv_stream_t* handle = reinterpret_cast<uv_stream_t*>(wrap->UVHandle());

    struct sockaddr_in destaddr;
    socklen_t len=sizeof(sockaddr);
    // getsockopt(
    //     uv__stream_fd(handle), SOL_IP, SO_ORIGINAL_DST
    //     , (struct sockaddr *) &destaddr, &len);

    getsockname(uv__stream_fd(handle),(struct sockaddr *) &destaddr, &len) ;

    unsigned short port = ntohs( destaddr.sin_port );
    char * addr = inet_ntoa(destaddr.sin_addr);

    v8::Local<v8::Array> ret = Nan::New<v8::Array>() ;
    ret->Set(0,Nan::New<v8::String>(addr).ToLocalChecked()) ;
    ret->Set(1,Nan::New<v8::Uint32>((uint32_t)port)) ;

    info.GetReturnValue().Set(ret);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("originalDst").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(OriginalDst)->GetFunction());
}

NODE_MODULE(hello, Init)
