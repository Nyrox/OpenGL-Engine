#pragma once
#include <Core/Definitions.h>
#include <Core/Shader.h>
#include <vector>

class Shader;

enum class ScalarType {
	F32 = 1,
	I32
};

struct ScalarValue {
	union {
		float f32;
		int32 i32;
	} value;

	ScalarType type;
};

enum class ShadingModel {
	OPAGUE_PBR_DEFAULT = 0,
	TRANSPARENT_PBR_DEFAULT = 1
};

class Material;

struct MaterialInstance {
	explicit MaterialInstance(Material* t_material) : material(t_material) { }

	Material* material;
	std::vector<uint32> samplerLocations;
	std::vector<ScalarValue> scalars;
};

struct Scalar {
	Scalar() = default;
	Scalar(uint32 t_uL) : uniformLocation(t_uL) { }

	uint32 uniformLocation;
};

struct Sampler {
	Sampler() = default;
	Sampler(uint32 t_b) : binding(t_b) { }

	uint32 binding;
};

struct Material {
	Material() : defaultMaterialInstance(this) {

	}

	Shader geometryPassShader;

	MaterialInstance defaultMaterialInstance;
	std::vector<Sampler> samplers;
	std::vector<Scalar> scalars;

	ShadingModel shadingModel;
};