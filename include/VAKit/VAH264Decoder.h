
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __VAKit_VAH264Decoder_h
#define __VAKit_VAH264Decoder_h

#include "AVKit/Options.h"
#include "AVKit/Decoder.h"
#include "AVKit/AVDeMuxer.h"
#include "AVKit/Packet.h"
#include "AVKit/PacketFactory.h"
#include "XSDK/Types.h"
#include "XSDK/XMemory.h"
#include "XSDK/XMutex.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavcodec/vaapi.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

#include <va/va.h>
#include <va/va_drm.h>
}

namespace VAKit
{

struct HWSurface
{
    VASurfaceID id;
    int refcount;
    unsigned int order;
};

static const int NUM_VA_BUFFERS = 16;

class VAH264Decoder : public AVKit::Decoder
{
public:
    X_API VAH264Decoder( const struct AVKit::CodecOptions& options );
    X_API VAH264Decoder( AVKit::AVDeMuxer& deMuxer, const struct AVKit::CodecOptions& options );
    X_API virtual ~VAH264Decoder() throw();

    X_API static bool HasHW( const XSDK::XString& devicePath );

    X_API virtual void Decode( XIRef<AVKit::Packet> frame );

    X_API virtual uint16_t GetInputWidth() const;
    X_API virtual uint16_t GetInputHeight() const;

    X_API virtual void SetOutputWidth( uint16_t outputWidth );
    X_API virtual uint16_t GetOutputWidth() const;

    X_API virtual void SetOutputHeight( uint16_t outputHeight );
    X_API virtual uint16_t GetOutputHeight() const;

    X_API virtual XIRef<AVKit::Packet> Get();

private:
    VAH264Decoder( const VAH264Decoder& obj );
    VAH264Decoder& operator = ( const VAH264Decoder& );

    void _FinishFFMPEGInit( uint8_t* frame, size_t frameSize );

    void _DestroyScaler();

    void _InitVAAPIDecoder();
    void _DestroyVAAPIDecoder();

    static enum PixelFormat _GetFormat( struct AVCodecContext *avctx, const enum PixelFormat *fmt );
    static int _GetBuffer( struct AVCodecContext* avctx, AVFrame* pic );
    static void _ReleaseBuffer( struct AVCodecContext* avctx, AVFrame* pic );

    AVCodec* _codec;
    AVCodecContext* _context;
    struct AVKit::CodecOptions _options;
    AVFrame* _frame;
    SwsContext* _scaler;
    uint16_t _outputWidth;
    uint16_t _outputHeight;
    bool _initComplete;
    int _fd;
    struct vaapi_context _vc;
    VAConfigAttrib _attrib;
    struct HWSurface _surfaces[NUM_VA_BUFFERS];
    VAImage _outputImage;
    XSDK::XMutex _surfaceLock;
    unsigned int _surfaceOrder;
    XIRef<AVKit::PacketFactory> _pf;
};

}

#endif
