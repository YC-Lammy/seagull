/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(GPU_PROCESS) && ENABLE(WEBGL)

#include "Connection.h"
#include "GPUConnectionToWebProcess.h"
#include "GraphicsContextGLIdentifier.h"
#include "RemoteRenderingBackend.h"
#include "ScopedWebGLRenderingResourcesRequest.h"
#include "StreamServerConnection.h"
#include <WebCore/ExtensionsGL.h>
#include <WebCore/GraphicsContextGLOpenGL.h>
#include <WebCore/NotImplemented.h>
#include <wtf/ThreadAssertions.h>
#include <wtf/WeakPtr.h>

#if PLATFORM(COCOA)
#include <WebCore/GraphicsContextGLIOSurfaceSwapChain.h>
#endif

#if PLATFORM(MAC)
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

#if PLATFORM(COCOA)
namespace WTF {
class MachSendRight;
}
#endif

namespace WebKit {

// GPU process side implementation of that receives messages about GraphicsContextGL calls
// and issues real GraphicsContextGL calls based on the received messages.
// The implementation is largely generated by running Tools/Scripts/generate-gpup-webgl.
class RemoteGraphicsContextGL : public ThreadSafeRefCounted<RemoteGraphicsContextGL>, private WebCore::GraphicsContextGL::Client {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static Ref<RemoteGraphicsContextGL> create(GPUConnectionToWebProcess&, WebCore::GraphicsContextGLAttributes&&, GraphicsContextGLIdentifier, RemoteRenderingBackend&, IPC::StreamConnectionBuffer&&);
    ~RemoteGraphicsContextGL() override;
    void stopListeningForIPC(Ref<RemoteGraphicsContextGL>&& refFromConnection);

    // IPC::StreamServerConnection<RemoteGraphicsContextGL> template contract implementation.
    void didReceiveStreamMessage(IPC::StreamServerConnectionBase&, IPC::Decoder&);
#if PLATFORM(MAC)
    void displayWasReconfigured();
#endif

protected:
    RemoteGraphicsContextGL(GPUConnectionToWebProcess&, GraphicsContextGLIdentifier, RemoteRenderingBackend&, IPC::StreamConnectionBuffer&&);
    void initialize(WebCore::GraphicsContextGLAttributes&&);

    void workQueueInitialize(WebCore::GraphicsContextGLAttributes&&);
    virtual void platformWorkQueueInitialize(WebCore::GraphicsContextGLAttributes&&) { };
    void workQueueUninitialize();
    template<typename T>
    bool send(T&& message) const { return m_streamConnection->connection().send(WTFMove(message), m_graphicsContextGLIdentifier); }

    // GraphicsContextGL::Client overrides.
    void didComposite() final;
    void forceContextLost() final;
    void recycleContext() final;
    void dispatchContextChangedNotification() final;

    // Messages to be received.
    void reshape(int32_t width, int32_t height);
    void ensureExtensionEnabled(String&&);
    void notifyMarkContextChanged();
#if PLATFORM(COCOA)
    virtual void prepareForDisplay(CompletionHandler<void(WTF::MachSendRight&&)>&&) = 0;
#else
    void prepareForDisplay(CompletionHandler<void()>&&);
#endif
    void getError(CompletionHandler<void(uint32_t)>&&);
    void synthesizeGLError(uint32_t error);
    void paintRenderingResultsToCanvas(WebCore::RenderingResourceIdentifier, CompletionHandler<void()>&&);
    void paintCompositedResultsToCanvas(WebCore::RenderingResourceIdentifier, CompletionHandler<void()>&&);
    void copyTextureFromMedia(WebCore::MediaPlayerIdentifier, uint32_t texture, uint32_t target, int32_t level, uint32_t internalFormat, uint32_t format, uint32_t type, bool premultiplyAlpha, bool flipY, CompletionHandler<void(bool)>&&);
    void simulateEventForTesting(WebCore::GraphicsContextGL::SimulatedEventForTesting);

#include "RemoteGraphicsContextGLFunctionsGenerated.h" // NOLINT

    void paintPixelBufferToImageBuffer(std::optional<WebCore::PixelBuffer>&&, WebCore::RenderingResourceIdentifier, CompletionHandler<void()>&&);

    WeakPtr<GPUConnectionToWebProcess> m_gpuConnectionToWebProcess;
    RefPtr<IPC::StreamServerConnection<RemoteGraphicsContextGL>> m_streamConnection;
    RefPtr<WebCore::GraphicsContextGLOpenGL> m_context WTF_GUARDED_BY_LOCK(m_streamThread);
    GraphicsContextGLIdentifier m_graphicsContextGLIdentifier;
    Ref<RemoteRenderingBackend> m_renderingBackend;
    ScopedWebGLRenderingResourcesRequest m_renderingResourcesRequest;
    NO_UNIQUE_ADDRESS ThreadAssertion m_streamThread;
};

IPC::StreamConnectionWorkQueue& remoteGraphicsContextGLStreamWorkQueue();

} // namespace WebKit

#endif
