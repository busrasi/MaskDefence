#include "rgb_frame_helper.h"

QImage RGBHelper::imageWrapper(const QVideoFrame &frame)
{
#ifndef QT_NO_OPENGL
    if (frame.handleType() == QAbstractVideoBuffer::GLTextureHandle) {
        // Slow and inefficient path. Ideally what's on the GPU should remain on the GPU, instead of readbacks like this.
        QImage img(frame.width(), frame.height(), QImage::Format_RGBA8888);
        GLuint textureId = frame.handle().toUInt();
        QOpenGLContext *ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions *f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1, &fbo);
        GLuint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, frame.width(), frame.height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
        return img;
    } else
#endif // QT_NO_OPENGL
    {
        if (!frame.isReadable()) {
            qWarning("imageWrapper: No mapped image data available for read");
            return QImage();
        }

        QImage::Format fmt = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
        if (fmt != QImage::Format_Invalid)
            return QImage(frame.bits(), frame.width(), frame.height(), fmt);

        qWarning("imageWrapper: No matching QImage format");
    }

    return QImage();
}


/*
  Creates and returns a new video frame wrapping the OpenGL texture textureId. The size
  must be passed in size, together with the format of the underlying image data in
  format. When the texture originates from a QImage, use
  QVideoFrame::imageFormatFromPixelFormat() to get a suitable format. Ownership is not
  altered, the new QVideoFrame will not destroy the texture.
*/
QVideoFrame RGBHelper::frameFromTexture(uint textureId, const QSize &size, QVideoFrame::PixelFormat format)
{
#ifndef QT_NO_OPENGL
    return QVideoFrame(new TextureBuffer(textureId), size, format);
#else
    return QVideoFrame();
#endif // QT_NO_OPENGL
}
