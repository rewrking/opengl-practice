#include "OpenGL/ProgramBase.hpp"

#include "OpenGL/BufferAttribList.hpp"

namespace ogl::Program
{
struct LightingCasters final : ogl::ProgramBase
{
	// a single cube (no indices)
	const std::vector<f32> m_vertices = {
		// clang-format off
		// positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		// clang-format on
	};

	const std::vector<Vec3f> m_cubePositions = {
		{ 0.0f, 0.0f, 0.0f },
		{ 2.0f, 5.0f, -15.0f },
		{ -1.5f, -2.2f, -2.5f },
		{ -3.8f, -2.0f, -12.3f },
		{ 2.4f, -0.4f, -3.5f },
		{ -1.7f, 3.0f, -7.5f },
		{ 1.3f, -2.0f, -2.5f },
		{ 1.5f, 2.0f, -2.5f },
		{ 1.5f, 0.2f, -1.5f },
		{ -1.3f, 1.0f, -1.5f }
	};

	f32 m_yaw = 0.0f;
	f32 m_pitch = 0.0f;

	Material m_lightingShader;
	Material m_lightCubeShader;

	TextureBuffer m_diffuseMap;
	TextureBuffer m_specularMap;

	MeshOld m_cubeMesh;
	MeshOld m_lightMesh;

	Mat4f m_view;
	Mat4f m_projection;

	virtual Settings getSettings() const final
	{
		return Settings("11: Lighting, Light Casters", 800, 600);
	}

	virtual void init() final
	{
		useDepthBuffer();
		setClearColor(25, 25, 25);
		setCameraEnabled(true);
		setWireframe(false);

		camera().setPosition(0.0f, 0.0f, 3.0f);

		m_yaw = 0.0f;
		m_pitch = 0.0f;

		m_lightingShader.loadFromFile("11_lighting_casters/phong.glsl");
		m_lightCubeShader.loadFromFile("11_lighting_casters/light_cube.glsl");

		m_cubeMesh.setGeometry({ Attrib::Position3D, Attrib::Normal3D, Attrib::TexCoord }, m_vertices);
		m_cubeMesh.setMaterial(m_lightingShader);

		m_lightMesh.setGeometry({ Attrib::Position3D, Attrib::Normal3D, Attrib::TexCoord }, m_vertices);
		m_lightMesh.setMaterial(m_lightCubeShader);

		if (!m_diffuseMap.load("container2.png"))
			return;

		if (!m_specularMap.load("container2_specular.png"))
			return;

		onMouseMove(static_cast<f64>(m_width), static_cast<f64>(m_height));
	}

	virtual void update() final
	{
		clearContext();

		// f32 timeValue = glfwGetTime();
		// f32 greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
		// shaderProgram.setVec4("u_Color", 0.0f, greenValue, 0.0f, 1.0f);

		m_projection = getProjectionMatrix();
		m_view = getViewMatrix();

		m_diffuseMap.bind(0);
		m_specularMap.bind(1);

		Vec3f lightPos{ -0.2f, -1.0f, -0.3f };

		// m_lightingShader.setVec4("u_LightColor", getColor(255, 255, 255));
		// m_lightingShader.setVec4("u_ObjectColor", getColor(255, 128, 79));
		// m_lightingShader.setVec3("u_LightPos", lightPos);
		// m_lightingShader.setVec3("u_ViewPos", m_camera.position());

		m_lightingShader.setTexture("u_Material.diffuse", m_diffuseMap);
		m_lightingShader.setTexture("u_Material.specular", m_specularMap);
		m_lightingShader.setFloat("u_Material.shininess", 32.0f);

		// f64 delta = glfwGetTime();

		Vec3f lightColor{ 1.0f, 1.0f, 1.0f };
		// lightColor.x = static_cast<f32>(std::sin(delta * 2.0));
		// lightColor.y = static_cast<f32>(std::sin(delta * 0.7));
		// lightColor.z = static_cast<f32>(std::sin(delta * 1.3));

		auto diffuseColor = lightColor * Vec3f(0.8f);
		auto ambientColor = diffuseColor * Vec3f(0.1f);

		m_lightingShader.setVec3("u_Light.position", camera().position());
		m_lightingShader.setVec3("u_Light.direction", camera().front());
		m_lightingShader.setFloat("u_Light.cutOff", glm::cos(glm::radians(12.5f)));
		m_lightingShader.setFloat("u_Light.outerCutOff", glm::cos(glm::radians(17.5f)));
		m_lightingShader.setVec3("u_ViewPos", camera().position());

		UNUSED(lightPos);
		// m_lightingShader.setVec3("u_Light.position", lightPos);

		m_lightingShader.setVec3("u_Light.ambient", ambientColor);
		m_lightingShader.setVec3("u_Light.diffuse", diffuseColor); // darken diffuse light a bit
		m_lightingShader.setVec3("u_Light.specular", 1.0f, 1.0f, 1.0f);

		// https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
		//
		m_lightingShader.setFloat("u_Light.constant", 1.0f);
		m_lightingShader.setFloat("u_Light.linear", 0.09f);
		m_lightingShader.setFloat("u_Light.quadratic", 0.032f);

		// f32 delta = static_cast<f32>(glfwGetTime());

		for (size_t i = 0; i < m_cubePositions.size(); ++i)
		{
			f32 angle = 20.0f * static_cast<f32>(i);
			auto model = glm::translate(Mat4f(1.0f), m_cubePositions.at(i));
			model = glm::rotate(model, glm::radians(angle), Vec3f(1.0f, 0.3f, 0.5f));
			auto normalMatrix = Mat3f(glm::transpose(glm::inverse(m_view * model)));

			m_lightingShader.setMat4("u_ProjectionViewModel", m_projection * m_view * model);
			m_lightingShader.setMat4("u_ViewModel", m_view * model);
			m_lightingShader.setMat3("u_NormalMatrix", normalMatrix);

			m_cubeMesh.draw();
		}

		// {
		// 	auto model = glm::translate(Mat4f(1.0f), lightPos);
		// 	model = glm::scale(model, Vec3f(0.2f)); // a smaller cube

		// 	m_lightCubeShader.setMat4("u_Projection", m_projection);
		// 	m_lightCubeShader.setMat4("u_View", m_view);
		// 	m_lightCubeShader.setMat4("u_Model", model);

		// 	m_lightMesh.draw();
		// }
	}

	virtual void dispose() final
	{
		m_lightMesh.dispose();
		m_lightCubeShader.dispose();

		m_lightingShader.dispose();
		m_cubeMesh.dispose();
	}
};
}
