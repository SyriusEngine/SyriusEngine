#include "SkyBoxPass.hpp"
#include "CameraDataPass.hpp"
#include "ProjectionPass.hpp"

namespace Syrius{

    SkyBoxPass::SkyBoxPass(ResourceView<Context>& context, Resource<ShaderLibrary>& shaderLibrary, const SkyBoxPassDesc& sbDesc):
    RenderPass(context, shaderLibrary, "SkyBoxPass", PassIDGenerator<SkyBoxPass>::getID()) {
        loadShader("SkyBoxPass");
        addDependency<CameraDataPass>();
        addDependency<ProjectionPass>();
        setupCaptureFrameBuffer();
        setupConversionObjects();
        setRadianceMap(sbDesc.radianceMap);

        m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("TexCoords", SR_FLOAT32_2);
        m_VertexLayout->addAttribute("FaceID", SR_FLOAT32_1);

        VertexBufferDesc vboDesc;
        vboDesc.count = 36;
        vboDesc.data = s_UnitCubeVertices;
        vboDesc.usage = SR_BUFFER_USAGE_STATIC;
        vboDesc.layout = m_VertexLayout;
        auto vbo = m_Context->createVertexBuffer(vboDesc);

        VertexArrayDesc vaDesc;
        vaDesc.vertexBuffer = vbo;
        vaDesc.vertexShader = m_VertexShader;
        m_SkyBoxCube = m_Context->createVertexArray(vaDesc);
    }

    SkyBoxPass::~SkyBoxPass() = default;

    void SkyBoxPass::execute(PassMap& passMap) {
        m_Context->getDefaultFrameBuffer()->setDepthFunc(SR_COMPARISON_FUNC_LESS_EQUAL);
        m_Context->beginRenderPass();

        m_CaptureFrameBuffer->getCubeColorAttachment(0)->bindShaderResource(0);
        m_Shader->bind();
        m_Context->draw(m_SkyBoxCube);

        m_Context->endRenderPass();
    }

    void SkyBoxPass::setRadianceMap(const Resource<Image> &radianceMap) {
        std::vector<glm::mat4> captureViews =  {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        Texture2DImageDesc radianceTextureDesc;
        radianceTextureDesc.image = createResourceView(radianceMap);
        auto radTex = m_Context->createTexture2D(radianceTextureDesc);
        radTex->bindShaderResource(0);
        m_IrradianceConversionShader->bind();
        m_ProjectionBuffer->bind(0);

        m_Context->beginRenderPass(m_CaptureFrameBuffer);
        for (const auto& view : captureViews) {
            SkyBoxProjectionData projectionData;
            projectionData.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
            projectionData.view = view;
            m_ProjectionBuffer->setData(&projectionData, sizeof(SkyBoxProjectionData));

            m_Context->draw(m_ConversionCube);
        }
        m_Context->endRenderPass();


    }

    void SkyBoxPass::setupCaptureFrameBuffer() {
        auto layout = m_Context->createFrameBufferLayout();
        CubeColorAttachmentDesc ccaDesc;
        ccaDesc.width = SKYBOX_MAP_SIZE;
        ccaDesc.height = SKYBOX_MAP_SIZE;
        ccaDesc.format = SR_TEXTURE_RGBA_F16;
        ccaDesc.clearColor[0] = 0.0f;
        ccaDesc.clearColor[1] = 0.0f;
        ccaDesc.clearColor[2] = 0.0f;
        ccaDesc.clearColor[3] = 1.0f;
        layout->addCubeColorAttachmentDesc(ccaDesc);

        ViewportDesc vpDesc;
        vpDesc.width = SKYBOX_MAP_SIZE;
        vpDesc.height = SKYBOX_MAP_SIZE;
        layout->addViewportDesc(vpDesc);

        m_CaptureFrameBuffer = m_Context->createFrameBuffer(layout);
    }

    void SkyBoxPass::setupConversionObjects() {
        // load the conversion shader
        auto& package = m_ShaderLibrary->getPackage("IrradianceConversion");
        auto icvsm = package.vertexShader;
        auto icfsm = package.fragmentShader;

        ShaderDesc shaderDesc;
        shaderDesc.vertexShader = icvsm;
        shaderDesc.fragmentShader = icfsm;
        m_IrradianceConversionShader = m_Context->createShader(shaderDesc);

        // setup unit cube
        auto layout = m_Context->createVertexLayout();
        layout->addAttribute("Position", SR_FLOAT32_3);
        layout->addAttribute("Normal", SR_FLOAT32_3);
        layout->addAttribute("TexCoords", SR_FLOAT32_2);
        layout->addAttribute("FaceID", SR_FLOAT32_1);

        VertexBufferDesc vboDesc;
        vboDesc.count = 36;
        vboDesc.data = s_UnitCubeVertices;
        vboDesc.usage = SR_BUFFER_USAGE_STATIC;
        vboDesc.layout = layout;
        auto vbo = m_Context->createVertexBuffer(vboDesc);

        VertexArrayDesc vaDesc;
        vaDesc.vertexBuffer = vbo;
        vaDesc.vertexShader = icvsm;
        m_ConversionCube = m_Context->createVertexArray(vaDesc);

        // setup project constant buffer
        SkyBoxProjectionData projectionData;
        projectionData.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        projectionData.view = glm::mat4(1.0f);
        ConstantBufferDesc cbDesc;
        cbDesc.data = &projectionData;
        cbDesc.size = sizeof(SkyBoxProjectionData);
        cbDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cbDesc.shaderStage = SR_SHADER_VERTEX;
        cbDesc.name = "SkyBoxProjectionData";
        m_ProjectionBuffer = m_Context->createConstantBuffer(cbDesc);
    }
}
