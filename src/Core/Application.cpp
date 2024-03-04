
#include "Core/Appplication.hpp"

#include "OpenGL/ProgramBase.hpp"

#include "Examples/00_hello_window.hpp"
#include "Examples/01_hello_triangles.hpp"
#include "Examples/02_shaders.hpp"
#include "Examples/03_textures.hpp"
#include "Examples/04_transformations.hpp"
#include "Examples/05_coordinate_systems.hpp"
#include "Examples/06_camera.hpp"
#include "Examples/07_lighting_color.hpp"
#include "Examples/08_lighting_basic.hpp"
#include "Examples/09_lighting_materials.hpp"
#include "Examples/10_lighting_lightmaps.hpp"
#include "Examples/11_lighting_casters.hpp"
#include "Examples/12_multiple_lights.hpp"
#include "Examples/13_mesh.hpp"
#include "Examples/x1_abstractions.hpp"

namespace ogl
{
/*****************************************************************************/
std::string Application::getHelp()
{
	return std::string(R"help(Help:
  0. Hello Window
  1. Hello Triangles
  2. Shaders
  3. Textures
  4. Transformations
  5. Coordinate Systems
  6. Cameras
  7. Lighting: Color
  8. Lighting: Basic
  9. Lighting: Materials
  10. Lighting: Lightmaps
  11. Lighting: Casters
  12. Multiple Lights
  91. Extra 1: Abstractions
)help");
}

template <typename T>
inline std::unique_ptr<ProgramBase> make()
{
	return std::make_unique<T>();
}

inline std::unique_ptr<ProgramBase> getProgramFromNumber(const i32 inNumber)
{
	switch (inNumber)
	{
		case 0: return make<Program::HelloWindow>();
		case 1: return make<Program::HelloTriangles>();
		case 2: return make<Program::Shaders>();
		case 3: return make<Program::Textures>();
		case 4: return make<Program::Transformations>();
		case 5: return make<Program::CoordinateSystems>();
		case 6: return make<Program::Cameras>();
		case 7: return make<Program::LightingColor>();
		case 8: return make<Program::LightingBasic>();
		case 9: return make<Program::LightingMaterials>();
		case 10: return make<Program::LightingLightmaps>();
		case 11: return make<Program::LightingCasters>();
		case 12: return make<Program::MultipleLights>();
		case 13: return make<Program::MeshProgram>();
		case 91: return make<Program::ExtraAbstractions>();
		default:
			return nullptr;
	}
}

/*****************************************************************************/
i32 Application::runProgramNumber(const i32 inNumber)
{
	auto program = getProgramFromNumber(inNumber);
	if (program == nullptr)
	{
		std::cout << "Unknown program number: " << inNumber << "\n\n"
				  << ogl::Application::getHelp() << std::flush;
		return 0;
	}

	return program->run();
}
}
