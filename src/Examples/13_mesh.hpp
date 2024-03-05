#include "OpenGL/ProgramBase.hpp"

#include "OpenGL/BufferAttribList.hpp"

namespace ogl::Program
{
struct MeshProgram final : ogl::ProgramBase
{
	Material m_material;

	Model m_model;

	Mat4f m_view;
	Mat4f m_projection;

	virtual Settings getSettings() const final
	{
		return Settings("13: Mesh", 800, 600);
	}

	virtual void init() final
	{
		useDepthBuffer();
		setClearColor(25, 25, 25);
		setCameraEnabled(true);
		setWireframe(false);

		camera().setPosition(0.0f, 0.0f, 3.0f);

		if (!m_material.loadFromFile("13_mesh/model_loading.glsl"))
			return;

		if (!m_model.load("backpack/backpack.obj"))
			return;

		onMouseMove(static_cast<f64>(m_width), static_cast<f64>(m_height));
	}

	virtual void update() final
	{
		clearContext();

		if (!m_model.loaded())
			return;

		m_material.setVec3("u_ViewPos", camera().position());
		m_material.setFloat("u_Material.shininess", 100.0f);

		// Directional light
		m_material.setVec3("u_DirLight.direction", -0.2f, -0.5f, 1.0f);
		m_material.setVec3("u_DirLight.ambient", Color(152, 130, 132));
		m_material.setVec3("u_DirLight.diffuse", Color(128, 164, 164));
		m_material.setVec3("u_DirLight.specular", Color(255, 255, 255));

		m_projection = getProjectionMatrix();
		m_view = getViewMatrix();

		m_material.setMat4("u_View", m_view);
		m_material.setMat4("u_Projection", m_projection);

		// translate it down so it's at the center of the scene
		auto model = glm::translate(Mat4f(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		// it's a bit too big for our scene, so scale it down
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		m_material.setMat4("u_Model", model);

		m_model.draw(m_material);
	}

	virtual void dispose() final
	{
		m_model.dispose();
		m_material.dispose();
	}
};
}
