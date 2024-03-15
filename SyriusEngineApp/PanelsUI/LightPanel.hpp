#pragma once

#include "../Include.hpp"
#include "../SceneObjects/LightObject.hpp"

namespace Syrius{

    class LightPanel{
    public:
        LightPanel(const ResourceView<RenderCommand>& renderCommand, Worker& dispatcher);

        ~LightPanel();

        void draw();

    private:
        void drawLightOptions();

    private:
        const ResourceView<RenderCommand>& m_RenderCommand;
        Worker& m_Dispatcher;

        int32 m_SelectedLight;

        std::vector<Resource<LightObject>> m_Lights;

    };

}