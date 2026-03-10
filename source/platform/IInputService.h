#pragma once

namespace Alice::Platform
{
    class IInputService
    {
    public:
        virtual ~IInputService() = default;

        virtual void BeginFrame() = 0;
        virtual bool IsKeyDown(int key) const = 0;
        virtual bool WasKeyPressed(int key) const = 0;
        virtual bool WasKeyReleased(int key) const = 0;
        virtual double GetMouseX() const = 0;
        virtual double GetMouseY() const = 0;
        virtual double GetMouseDeltaX() const = 0;
        virtual double GetMouseDeltaY() const = 0;
        virtual double ConsumeScrollDeltaY() = 0;
    };
}
