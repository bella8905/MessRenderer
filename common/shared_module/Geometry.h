/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - geometry
//
//  MessRenderer - A very messy renderer
//
// a geo class including vao, vbos and ibo for rendering
// it doesn't include any transformation,  material / shader information
// make it a singleton, and never instantiate more than 1 instances
// pass in a transformation matrix, shader and matertial for rendering
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "assimp/scene.h"
#include "Utl_Include.h"
#include "Material.h"
#include "Shader.h"
#include <unordered_map>

using std::vector;
using std::string;

enum GEO_TYPE {
	GEO_TRIANGLE = 0,
	GEO_UNIT_CUBE,
	GEO_UNIT_SPHERE,
	GEO_SPHERE,
	GEO_SPIDER,

	// not a type, but the number of geos
	GEO_COUNTER
};


/////////////////////////////////////////////////////////////////
//
//  Geo aabb
//  
/////////////////////////////////////////////////////////////////
struct SBoundBox {
	vec3 _min, _max;
	vec3 _sideLentghs;
	vec3 _center;

	void recalculate() {
		float length = -1;
		for( int i = 0; i < 3; ++i ) {
			_sideLentghs[i] = _max[i] - _min[i];
		}

		_center = ( _min + _max ) * 0.5f;
	}

	void SetBounds( const vec3& t_point ) {
		_min.x = ( t_point.x < _min.x ) ? t_point.x : _min.x;
		_min.y = ( t_point.y < _min.y ) ? t_point.y : _min.y;
		_min.z = ( t_point.z < _min.z ) ? t_point.z : _min.z;
		_max.x = ( t_point.x > _max.x ) ? t_point.x : _max.x;
		_max.y = ( t_point.y > _max.y ) ? t_point.y : _max.y;
		_max.z = ( t_point.z > _max.z ) ? t_point.z : _max.z;

		recalculate();
	}

	void SetBounds( const SBoundBox& t_bounds ) {
		_min.x = min( _min.x, t_bounds._min.x );
		_min.y = min( _min.y, t_bounds._min.y );
		_min.z = min( _min.z, t_bounds._min.z );
		_max.x = max( _max.x, t_bounds._max.x );
		_max.y = max( _max.y, t_bounds._max.y );
		_max.z = max( _max.z, t_bounds._max.z );

		recalculate();
	}


	float GetLongestSide() {
		float length = -1;
		for( int i = 0; i < 3; ++i ) {
			float sideLength = _max[i] - _min[i];
			length = max( sideLength, length );
		}

		return length;
	}

	SBoundBox() : _min( vec3( std::numeric_limits<float>::infinity() ) ), _max( vec3( -std::numeric_limits<float>::infinity() ) ) {
	}


	void Translate( const vec3& t_translate ) {
		_min += t_translate;
		_max += t_translate;

		recalculate();
	}

	void Scale( const float& t_scale ) {
		_min *= t_scale;
		_max *= t_scale;

		recalculate();
	}

	void Transform( const mat4& t_tfm ) {
		_min = vec3( t_tfm * vec4( _min, 1.f ) );
		_max = vec3( t_tfm * vec4( _max, 1.f ) );

		recalculate();
	}

	void Reset() {
		_min = vec3( std::numeric_limits<float>::infinity() );
		_max = vec3( -std::numeric_limits<float>::infinity() );
	}


	// validate a bb
	// if it has no thickness along one axis, give it some
	void Validate() {
		bool isChanged = false;
		for( us i = 0; i < 3; ++i ) {
			if( Utl::Equals( _min[i], _max[i] ) ) {
				_min[i] -= 0.5f;
				_max[i] += 0.5f;
				isChanged = true;
			}
		}

		if( isChanged ) {
			recalculate();
		}
	}

};



/////////////////////////////////////////////////////////////////
//
//  Geo Base
//  
/////////////////////////////////////////////////////////////////
class CGeo {
public:
	CGeo();
	virtual ~CGeo() = 0;

protected:
	bool _inited;


	/////////////////////////////////////////////////////////////////
	//
	// a preprocessed model matrix, 
	// this is for transforming the model to a more meaningful status
	// for example, transforming a reading-in model to fit in a unit cube
	// never change it.
	//  
	/////////////////////////////////////////////////////////////////
	glm::mat4 _preprocessModelMatrix;
	/////////////////////////////////////////////////////////////////
	//
	// bound box is use to define the boundaries of the object,
	// used for ray based object picking
	//  
	/////////////////////////////////////////////////////////////////
	SBoundBox _boundBox;        // bound box when we haven't done any transformation( passed in model matrix is identical )
	// bool _drawBoundBox;

	// use same buffers to draw bound box for all models
	static GLuint _vao_boundBox, _vbo_boundBox, _ibo_boundBox;
	static bool _inited_boundBox;

protected:
	virtual bool initModel();
	virtual void deinitModel() = 0; // delete buffer, etc
	// void calModelMat();

public:
	// draw an instance of the object using a model matrix
	virtual void DrawModel( CShader* t_shader, CMaterial* t_material, const mat4& t_modelMatrix, bool t_drawBB );
	virtual void DrawModel( const SHADER_TYPE& t_shader, CMaterial* t_material, const mat4& t_modelMatrix, bool t_drawBB );
	bool IsInited() { return _inited; }

	const mat4& GetPreProcessedModelMat() { return _preprocessModelMatrix; }
	SBoundBox& GetBB() { return _boundBox; }

	// init / deinit boundbox buffers, only do it once
	static void InitBoundBox();
	static void DeinitBoundBox();
};


class CPrimGeo : public CGeo {
public:
	CPrimGeo() : _vao( 0 ), _vbo( 0 ) {}
	~CPrimGeo() { deinitModel(); }

protected:
	///////////////////////////////////////////////////////////////////////////////////
	//
	// vertex related buffers
	// vbo, ibo, vao
	//
	// keep track of vao loc so we can bind it, let it remember the setups of vbo and ibo
	// and draw our mesh later
	// but we also need to keep track of vbo loc so we can clear the buffer obj when we are done with this mesh
	// delete vao only removes the references to vbos
	// http://stackoverflow.com/questions/14274860/does-gldeletevertexarrays-lead-to-deletion-of-vbos-associated-with-vao-being-de
	//
	///////////////////////////////////////////////////////////////////////////////////
	GLuint _vao;
	GLuint _vbo;    // use a single buffer obj for pos and normal  
	GLuint _ibo;    // we'd like to use drawElement rather than drawArray. It saves space and is faster.

	///////////////////////////////////////////////////////////////////////////////////
	// 
	// uniforms
	// 
	///////////////////////////////////////////////////////////////////////////////////


	int _numOfIndices;

protected:
	/////////////////////////////////////////////////////////////////
	//
	// a simple vertex struct for prim geos
	// it only contains very basic data for vertex position and normal.
	// the prim geos don't have any texture bound.
	// the data is calculated instead of read from a model file.
	//  
	/////////////////////////////////////////////////////////////////
	struct SVertex_Simple {
		vec3 _pos;
		vec3 _normal;

		// texcoords
		// tangents
		// bitangents

		SVertex_Simple( const vec3& t_pos, const vec3& t_normal ) : _pos( t_pos ), _normal( t_normal ) {}
	};


	virtual bool initModel() = 0;
	void deinitModel();

	void genBufferData( const vector<SVertex_Simple>& t_vertices, const vector<GLuint>& t_indices );

public:
	virtual void DrawModel( CShader* t_shader, CMaterial* t_material, const mat4& t_modelMatrix, bool t_drawBB );
};


// triangle
class CTriangleGeo : public CPrimGeo {
public:
	CTriangleGeo() { initModel(); }
	~CTriangleGeo() { deinitModel(); }

protected:
	bool initModel();
};

// a unit cube with center (0,0,0) and side 1
class CUnitCubeGeo : public CPrimGeo {
public:
	CUnitCubeGeo() { initModel(); }
	~CUnitCubeGeo() { deinitModel(); }

protected:
	bool initModel();
};


// a unit sphere with center ( 0,0,0 ) and radius 1
class CUnitSphereGeo : public CPrimGeo {
public:
	CUnitSphereGeo() { initModel(); }
	~CUnitSphereGeo() { deinitModel(); }

protected:
	bool initModel();
};



// a object read from a model file
class CModelGeo : public CGeo {
public:
	CModelGeo( const string& t_file, bool t_unified = false ) : _fileName( t_file ), _unified( t_unified ) { initModel(); }
	~CModelGeo() = 0;


	struct SMesh {
		CModelGeo* _parent;
		// don't have reference to ass scene, the scene is already freed at mesh destructor time
		// const aiMesh* _aiMesh;
		int _numOfIndices;

		// generate buffer for each of the vertex attribute
		// this will save more space than create a vertext struct and transport all things to GPU
		// for sometimes we don't have texcoords or color for a vertex
        // vertex buffers
		// we actually only need the vao to keep track of all buffers.
		// TODO: REMOVE BUFFER IDS LATER
		GLuint _vao, _vbo, _nbo, _tbo, _ibo;
		us _mtlIdx;
		bool _hasTex, _hasFaces;
		bool _inited;
		SBoundBox _bounds;

		SMesh( CModelGeo* t_parent ) : _parent( t_parent ), _inited( false ), _numOfIndices( 0 ),
			_vao( 0 ), _vbo( 0 ), _nbo( 0 ), _tbo( 0 ), _ibo( 0 ),
			_hasTex( false ), _hasFaces( false ) {
		}

		void InitMesh( const aiMesh* t_aiMesh, bool t_unitScale = false );
		void DeinitMesh();
		void DrawMesh();
	};

	struct SMaterial {
		// texture id
		// it's possible we have multiple textures for a mesh.
		vector<GLuint> _textureIds;
		// mtl ubo 
		GLuint _mtl_ubo;

		SMaterial() : _mtl_ubo(0) {}
		void InitMaterial( const aiMaterial* t_aiMaterial, const std::unordered_map<std::string, GLuint>& t_textureMap );
		void DeinitMaterial();
		GLuint GetDiffuseTexture( uint t_texIdx );
	};

protected:
    static bool _bDumpSceneInfo;

	string _fileName;
	vector<SMesh> _meshes;
	vector<SMaterial> _materials;
	std::unordered_map<std::string, GLuint> _textureMap;

	bool _unified;       // if the model is scaled to fit a unit cube

protected:
	bool initModel();
	void deinitModel();
	///////////////////////////////////////////////////////////////////////////////////
	// 
	// init and load all textures in model
	// texture with the same path is only stored and loaded once
	// all loaded texture id will be stored in textureMap
	// 
	///////////////////////////////////////////////////////////////////////////////////
	void initTextures( const aiScene* t_aiScene, std::unordered_map<std::string, GLuint>& t_textureMap );
	void deinitTextures();
	///////////////////////////////////////////////////////////////////////////////////
	// 
	// populate all materials 
	// this will generate a material ubo for each material,
	// and also assign texture id for each material, which is loaded with initTextures().
	// 
	///////////////////////////////////////////////////////////////////////////////////
	void initMaterials( const aiScene* t_aiScene );
	///////////////////////////////////////////////////////////////////////////////////
	// 
	// populate data for each mesh 
	// this will generate all vertex buffers for meshes
	// 
	///////////////////////////////////////////////////////////////////////////////////
	void initMeshes( const aiScene* t_aiScene );
	///////////////////////////////////////////////////////////////////////////////////
	// 
	// find image path
	// 
	///////////////////////////////////////////////////////////////////////////////////
	bool findImagePath( std::string t_imagePath, std::string& t_out );

	SMaterial* GetMaterial( uint t_mtlIdx );

public:
	virtual void DrawModel( CShader* t_shader, CMaterial* t_material, const mat4& t_modelMatrix, bool t_drawBB );
};


// sphere
class CSphereGeo : public CModelGeo {
public:
	CSphereGeo( bool t_unified = false ) : CModelGeo( "../../common/models/sphere.dae", t_unified ) {}
	~CSphereGeo() {}
};


// spider
class CSpiderGeo : public CModelGeo {
public:
	CSpiderGeo( bool t_unified = false ) : CModelGeo( "../../common/models/spider/spider.obj", t_unified ) {}
	~CSpiderGeo() {}
};


// a interface to get all possible geos we have 
class CGeoContainer {
private:
	CGeoContainer();
	CGeoContainer( const CGeoContainer& t_cont );

private:
	CGeo* _geos[GEO_COUNTER];
	bool _inited;

public:
	static CGeoContainer& GetInstance() {
		static CGeoContainer instance;
		return instance;
	}

	void Init();
	void Deinit();

	SBoundBox* GetGeoBB( const GEO_TYPE& t_geoType );
	void DrawGeo( const GEO_TYPE& t_geoType, CShader* t_shader, CMaterial* t_material, const mat4& t_modelMatrix, const bool& t_drawBB );
	void DrawGeo( const GEO_TYPE& t_geoType, const SHADER_TYPE& t_shader, CMaterial* t_material, const mat4& t_modelMatrix, const bool& t_drawBB );
};
