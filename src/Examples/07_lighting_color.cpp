#include "OpenGL/ProgramBase.hpp"

#include "OpenGL/BufferAttribList.hpp"

namespace ogl
{
struct Program final : ProgramBase
{
	// a single cube (no indices)
	const std::vector<f32> m_vertices = {
		// clang-format off
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
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

	u32 m_vbo = 0;
	u32 m_ebo = 0;

	u32 m_vao = 0;
	u32 m_vaoLight = 0;

	Vec2f m_lastMouse{ 0.0f, 0.0f };

	f32 m_yaw = 0.0f;
	f32 m_pitch = 0.0f;

	Material m_lightingShader;
	Material m_lightCubeshader;

	Mat4f m_view;
	Mat4f m_projection;
	Mat4f m_model;

	Camera m_camera = Camera(Vec3f{ 1.0f, 1.0f, 5.0f });

	virtual bool processInput(GLFWwindow* window) final
	{
		bool res = ProgramBase::processInput(window);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			m_camera.processKeyboard(CameraMovement::Forward, Clock.deltaTime);
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			m_camera.processKeyboard(CameraMovement::Backward, Clock.deltaTime);

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			m_camera.processKeyboard(CameraMovement::Left, Clock.deltaTime);
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			m_camera.processKeyboard(CameraMovement::Right, Clock.deltaTime);

		return res;
	}

	virtual Settings getSettings() const final
	{
		return Settings("07: Lighting, Color", 800, 600);
	}

	virtual void init() final
	{
		glCheck(glEnable(GL_DEPTH_TEST));

		setClearColor(25, 25, 25);

		m_lastMouse.x = static_cast<f32>(m_width / 2);
		m_lastMouse.y = static_cast<f32>(m_height / 2);
		m_yaw = 0.0f;
		m_pitch = 0.0f;

		m_lightingShader = Material::make({
			"07_lighting_color/colors.vert",
			"07_lighting_color/colors.frag",
		});
		m_lightCubeshader = Material::make({
			"07_lighting_color/light_cube.vert",
			"07_lighting_color/light_cube.frag",
		});

		{
			auto image = Image::make("container.jpg");

			/*
				GL_REPEAT
				GL_MIRRORED_REPEAT
				GL_CLAMP_TO_EDGE
				GL_CLAMP_TO_BORDER
			*/
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

			/*
				GL_NEAREST
				GL_LINEAR

			// control mipmaps levels
				GL_NEAREST_MIPMAP_NEAREST
				GL_LINEAR_MIPMAP_NEAREST
				GL_NEAREST_MIPMAP_LINEAR
				GL_LINEAR_MIPMAP_LINEAR
			*/
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

			glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(image.width), static_cast<GLsizei>(image.height), 0, GL_RGB, GL_UNSIGNED_BYTE, image.pixels.data()));
			glCheck(glGenerateMipmap(GL_TEXTURE_2D));
		}
		{
			glCheck(glGenVertexArrays(1, &m_vao));
			glCheck(glGenBuffers(1, &m_vbo));
			glCheck(glGenBuffers(1, &m_ebo));

			glCheck(glBindVertexArray(m_vao));

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
			glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW));

			// glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
			// glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW));

			{
				BufferAttribList attribList({ MeshAttribute::Position3D });
				for (auto& attrib : attribList.attribs)
				{
					glCheck(glVertexAttribPointer(attrib.position, attrib.size, GL_FLOAT, GL_FALSE, sizeof(f32) * attribList.size, (void*)(attrib.offset * sizeof(f32))));
					glCheck(glEnableVertexAttribArray(attrib.position));
				}
			}

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

			// no!
			// glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			glCheck(glGenVertexArrays(1, &m_vaoLight));
			glCheck(glBindVertexArray(m_vaoLight));

			// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

			glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 3, (void*)0));
			glCheck(glEnableVertexAttribArray(0));

			glCheck(glBindVertexArray(0));
		}

		// wireframe!
		// glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

		onMouseMove(static_cast<f64>(m_width), static_cast<f64>(m_height));
	}

	virtual void update() final
	{
		glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// f32 timeValue = glfwGetTime();
		// f32 greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
		// shaderProgram.setUniform4f("u_Color", 0.0f, greenValue, 0.0f, 1.0f);

		{
			constexpr f32 near = 0.1f;
			constexpr f32 far = 100.0f;
			// m_projection = Mat4f(1.0f);
			m_projection = glm::perspective(m_camera.getFieldOfView(), static_cast<f32>(m_width) / static_cast<f32>(m_height), near, far);
		}

		{
			// auto cameraPos = Vec3f{ 0.0f, 0.0f, -3.0f };
			// note that we're translating the scene in the reverse direction of where we want to move
			// m_view = Mat4f(1.0f);
			// m_view = glm::translate(Mat4f(1.0f), cameraPos);

			// constexpr f64 radius = 10.0;
			// f32 camX = static_cast<f32>(std::sin(glfwGetTime()) * radius);
			// f32 camZ = static_cast<f32>(std::cos(glfwGetTime()) * radius);
			// auto cameraPos = Vec3f{ camX, 0.0, camZ };

			m_view = m_camera.getViewMatrix();
		}

		glCheck(glBindVertexArray(m_vao));

		m_lightingShader.bind();
		m_lightingShader.setUniform4f("u_LightColor", getColor(255, 255, 255));
		m_lightingShader.setUniform4f("u_ObjectColor", getColor(255, 128, 79));

		// f32 delta = static_cast<f32>(glfwGetTime());

		{
			m_lightingShader.setUniformMatrix4f("u_Projection", m_projection);
			m_lightingShader.setUniformMatrix4f("u_View", m_view);

			// f32 angle = 20.0f * 0.0f + (10.0f * delta);
			m_model = Mat4f(1.0f);
			// m_model = glm::rotate(m_model, glm::radians(angle), Vec3f{ 1.0f, 0.3f, 0.5f });
			m_lightingShader.setUniformMatrix4f("u_Model", m_model);

			glCheck(glDrawArrays(GL_TRIANGLES, 0, static_cast<i32>(m_vertices.size())));
		}

		m_lightCubeshader.bind();

		Vec3f lightPos{ 1.2f, 1.0f, 2.0f };

		{
			m_lightCubeshader.setUniformMatrix4f("u_Projection", m_projection);
			m_lightCubeshader.setUniformMatrix4f("u_View", m_view);

			m_model = glm::translate(Mat4f(1.0f), lightPos);
			m_model = glm::scale(m_model, Vec3f(0.2f)); // a smaller cube

			m_lightCubeshader.setUniformMatrix4f("u_Model", m_model);

			glCheck(glBindVertexArray(m_vaoLight));
			glCheck(glDrawArrays(GL_TRIANGLES, 0, static_cast<i32>(m_vertices.size())));
		}

		glCheck(glBindVertexArray(0));
	}

	virtual void dispose() final
	{
		glCheck(glDeleteVertexArrays(1, &m_vaoLight));
		glCheck(glDeleteVertexArrays(1, &m_vao));
		glCheck(glDeleteBuffers(1, &m_vbo));
		glCheck(glDeleteBuffers(1, &m_ebo));
		m_lightingShader.dispose();
		m_lightCubeshader.dispose();
	}

	virtual void onMouseMove(const f64 inX, const f64 inY) final
	{
		f32 xpos = static_cast<f32>(inX);
		f32 ypos = static_cast<f32>(inY);

		f32 xoffset = xpos - m_lastMouse.x;
		f32 yoffset = m_lastMouse.y - ypos;
		m_lastMouse.x = xpos;
		m_lastMouse.y = ypos;

		m_camera.processMouseMovement(xoffset, yoffset);
	}
};
}

OGL_RUN_MAIN(ogl::Program);
