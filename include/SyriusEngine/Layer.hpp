#pragma once

#include "Core/SyriusEngineInclude.hpp"

namespace Syrius{

    class SR_API Layer{
    public:
        Layer() = default;

        virtual ~Layer() = default;

        /**
         * @brief Called when the layer is attached to the engine's layer stack
         */
        virtual void onAttach() = 0;

        /**
         * @brief Called when the layer is detached from the engine's layer stack
         */
        virtual void onDetach() = 0;

        /**
         * @brief Called when the layer is updated. This function is called once a frame. Use this function to update
         *        the layer's logic
         */
        virtual void onUpdate() = 0;

        /**
         * @brief Called when the window receives an event. This function can be called zero or more times a frame. Use
         *        this function to update logic that depends on certain events. If you do not want an event to travel
         *        further down the layer stack, return false. If you want the event to travel further down the layer
         *        stack return true.
         * @param event Syrius Event
         * @return bool true if the event should travel further down the layer stack, false otherwise.
         */
        virtual bool onEvent(const Event& event) = 0;
    };

}