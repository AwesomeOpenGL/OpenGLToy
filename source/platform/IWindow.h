#pragma once

#include "platform/IInputService.h"
#include "platform/WindowTypes.h"

namespace Alice::Platform
{
    class IWindow
    {
    public:
        virtual ~IWindow() = default;

        virtual bool Create(const WindowDesc& desc) = 0;
        virtual void Destroy() = 0;
        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;
        virtual bool ShouldClose() const = 0;
        virtual void RequestClose() = 0;
        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;
        virtual float GetAspectRatio() const = 0;
        virtual void SetCursorMode(CursorMode mode) = 0;
        virtual void SetVSyncEnabled(bool enabled) = 0;
        virtual void* GetNativeHandle() const = 0;
        virtual IInputService& GetInputService() = 0;
        virtual const IInputService& GetInputService() const = 0;
    };
}
