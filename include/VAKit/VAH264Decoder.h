
#ifndef __VAKit_VAH264Decoder_h
#define __VAKit_VAH264Decoder_h

#include "AVKit/Options.h"
#include "AVKit/Decoder.h"

#include "XSDK/Types.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavcodec/vaapi.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#ifndef WIN32
#include <va/va.h>
#include <va/va_drm.h>
#endif
}

namespace VAKit
{

class VAH264Decoder : public AVKit::Decoder
{
public:
    X_API VAH264Decoder( const struct AVKit::CodecOptions& options );

    X_API virtual ~VAH264Decoder() throw();

    X_API virtual void Decode( uint8_t* frame, size_t frameSize );
    X_API virtual void Decode( XIRef<XSDK::XMemory> frame );

    X_API virtual uint16_t GetInputWidth() const;
    X_API virtual uint16_t GetInputHeight() const;

    X_API virtual void SetOutputWidth( uint16_t outputWidth );
    X_API virtual uint16_t GetOutputWidth() const;

    X_API virtual void SetOutputHeight( uint16_t outputHeight );
    X_API virtual uint16_t GetOutputHeight() const;

    X_API virtual size_t GetYUV420PSize() const;
    X_API virtual void MakeYUV420P( uint8_t* dest );
    X_API virtual XIRef<XSDK::XMemory> MakeYUV420P();

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

#ifndef WIN32
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
    VASurfaceID _surfaceID;
    VAImage _derivedImage;
#endif
};

}

#endif
