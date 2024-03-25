#pragma once

#include "../Serializer/Serializer.hpp"

namespace Syrius{

    class LightPanel{
    public:
        explicit LightPanel(Serializer& serializer);

        ~LightPanel();

        void draw();

    private:
        void drawLightOptions();

    private:
        Serializer& m_Serializer;
        std::vector<Resource<LightObject>>& m_Lights;

        int32 m_SelectedLight;

    };

}