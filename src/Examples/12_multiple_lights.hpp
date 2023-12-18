#include "OpenGL/ProgramBase.hpp"

#include "OpenGL/BufferAttribList.hpp"

namespace ogl::Program
{
struct MultipleLights final : ogl::ProgramBase
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

	const std::vector<Vec3f> m_pointLightPositions = {
		{ 0.7f, 0.2f, 2.0f },
		{ 2.3f, -3.3f, -4.0f },
		{ -4.0f, 2.0f, -12.0f },
		{ 0.0f, 0.0f, -3.0f }
	};

	f32 m_yaw = 0.0f;
	f32 m_pitch = 0.0f;

	Material m_lightingShader;
	Material m_lightCubeShader;

	Texture m_diffuseMap;
	Texture m_specularMap;

	Mesh m_cubeMesh;
	Mesh m_lightMesh;

	Mat4f m_view;
	Mat4f m_projection;

	virtual Settings getSettings() const final
	{
		return Settings("12: Lighting, Mutliple Lights", 800, 600);
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

		m_lightingShader.loadFromFile("12_multiple_lights/phong.glsl");
		m_lightCubeShader.loadFromFile("12_multiple_lights/light_cube.glsl");

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

		// m_lightingShader.setVec4("u_LightColor", getColor(255, 255, 255));
		// m_lightingShader.setVec4("u_ObjectColor", getColor(255, 128, 79));
		// m_lightingShader.setVec3("u_LightPos", lightPos);
		// m_lightingShader.setVec3("u_ViewPos", m_camera.position());

		m_lightingShader.setTexture("u_Material.diffuse", m_diffuseMap);
		m_lightingShader.setTexture("u_Material.specular", m_specularMap);

		// f64 delta = glfwGetTime();

		// Vec3f lightColor{ 1.0f, 1.0f, 1.0f };
		// lightColor.x = static_cast<f32>(std::sin(delta * 2.0));
		// lightColor.y = static_cast<f32>(std::sin(delta * 0.7));
		// lightColor.z = static_cast<f32>(std::sin(delta * 1.3));

		// auto diffuseColor = lightColor * Vec3f(0.8f);
		// auto ambientColor = diffuseColor * Vec3f(0.1f);

		m_lightingShader.setVec3("u_ViewPos", camera().position());
		m_lightingShader.setFloat("u_Material.shininess", 32.0f);

		// Directional light
		m_lightingShader.setVec3("u_DirLight.direction", -0.2f, -1.0f, -0.3f);
		m_lightingShader.setVec3("u_DirLight.ambient", 0.05f, 0.05f, 0.05f);
		m_lightingShader.setVec3("u_DirLight.diffuse", 0.4f, 0.4f, 0.4f);
		m_lightingShader.setVec3("u_DirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		m_lightingShader.setVec3("u_PointLights[0].position", m_pointLightPositions[0]);
		m_lightingShader.setVec3("u_PointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		m_lightingShader.setVec3("u_PointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		m_lightingShader.setVec3("u_PointLights[0].specular", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setFloat("u_PointLights[0].constant", 1.0f);
		m_lightingShader.setFloat("u_PointLights[0].linear", 0.09f);
		m_lightingShader.setFloat("u_PointLights[0].quadratic", 0.032f);
		// point light 2
		m_lightingShader.setVec3("u_PointLights[1].position", m_pointLightPositions[1]);
		m_lightingShader.setVec3("u_PointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		m_lightingShader.setVec3("u_PointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		m_lightingShader.setVec3("u_PointLights[1].specular", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setFloat("u_PointLights[1].constant", 1.0f);
		m_lightingShader.setFloat("u_PointLights[1].linear", 0.09f);
		m_lightingShader.setFloat("u_PointLights[1].quadratic", 0.032f);
		// point light 3
		m_lightingShader.setVec3("u_PointLights[2].position", m_pointLightPositions[2]);
		m_lightingShader.setVec3("u_PointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		m_lightingShader.setVec3("u_PointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		m_lightingShader.setVec3("u_PointLights[2].specular", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setFloat("u_PointLights[2].constant", 1.0f);
		m_lightingShader.setFloat("u_PointLights[2].linear", 0.09f);
		m_lightingShader.setFloat("u_PointLights[2].quadratic", 0.032f);
		// point light 4
		m_lightingShader.setVec3("u_PointLights[3].position", m_pointLightPositions[3]);
		m_lightingShader.setVec3("u_PointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		m_lightingShader.setVec3("u_PointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		m_lightingShader.setVec3("u_PointLights[3].specular", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setFloat("u_PointLights[3].constant", 1.0f);
		m_lightingShader.setFloat("u_PointLights[3].linear", 0.09f);
		m_lightingShader.setFloat("u_PointLights[3].quadratic", 0.032f);

		// u_SpotLight
		// m_lightingShader.setVec3("u_SpotLight.position", Vec3f(0.0f));
		m_lightingShader.setVec3("u_SpotLight.position", camera().position());
		m_lightingShader.setVec3("u_SpotLight.direction", camera().front());
		m_lightingShader.setVec3("u_SpotLight.ambient", 0.0f, 0.0f, 0.0f);
		m_lightingShader.setVec3("u_SpotLight.diffuse", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setVec3("u_SpotLight.specular", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setFloat("u_SpotLight.constant", 1.0f);
		m_lightingShader.setFloat("u_SpotLight.linear", 0.09f);
		m_lightingShader.setFloat("u_SpotLight.quadratic", 0.032f);
		m_lightingShader.setFloat("u_SpotLight.cutOff", glm::cos(glm::radians(12.5f)));
		m_lightingShader.setFloat("u_SpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// f32 delta = static_cast<f32>(glfwGetTime());

		m_lightingShader.setMat4("u_View", m_view);
		m_lightingShader.setMat4("u_Projection", m_projection);

		for (size_t i = 0; i < m_cubePositions.size(); ++i)
		{
			f32 angle = 20.0f * static_cast<f32>(i);
			auto model = glm::translate(Mat4f(1.0f), m_cubePositions.at(i));
			model = glm::rotate(model, glm::radians(angle), Vec3f(1.0f, 0.3f, 0.5f));

			m_lightingShader.setMat4("u_Model", model);

			m_cubeMesh.draw();
		}

		m_lightCubeShader.setMat4("u_View", m_view);
		m_lightCubeShader.setMat4("u_Projection", m_projection);

		for (auto& pos : m_pointLightPositions)
		{
			auto model = glm::translate(Mat4f(1.0f), pos);
			model = glm::scale(model, Vec3f(0.2f)); // a smaller cube

			m_lightCubeShader.setMat4("u_Model", model);

			m_lightMesh.draw();
		}
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
