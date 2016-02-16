/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - shader
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "Camera.h"

class CGeo;
class CLight;
class CMaterial;

class CShader {
public:
	// CShader( const string& t_vs, const string& t_fs );
	CShader() : _spCreated( false ), _sp( 0 ) {}
	virtual ~CShader() { onDeinit(); }

protected:
	std::string _vs, _fs, _ts, _gs;
	GLuint _sp;
	bool _spCreated;

protected:
	void initSP( const std::string& t_vs, const std::string& t_fs, const std::string& t_gs = "", const std::string& t_ts = "" );
    
    /////////////////////////////////////////////////////////////////
    //
    // init and deinit per shader
    //
    // init takes place after shaders are compiled and linked
    // and shader program is validated
    //
    /////////////////////////////////////////////////////////////////
    virtual void onInit() {}
    virtual void onDeinit() {}


	// create a shader obj, return the shader obj index
	std::string glTypeToString( GLenum t_type );
	void printShaderInfoLog( const GLuint& t_shaderIndex );
	void printSPInfoLog( const GLuint& t_spIndex );
	bool checkShaderCompileStatus( const GLuint& t_shaderIndex );
	bool checkSPLinkingStatus( const GLuint& t_spIndex );
	bool validateSP( const GLuint& t_spIndex );
	void printSPInfo( const GLuint& sp );
	GLint createShader( const char* t_shaderFile, GLenum t_shaderType );
	GLuint createShaderProgram( const GLint& t_vs, const GLint& t_gs, const GLint& t_ts, const GLint& t_fs );
	void createShaderProgram();
	bool readFileToStr( const char* t_file, std::string& t_fileContentStr );
    virtual void onBindShader() {}

public:
	// attrib location
	/*GLuint _attr_pos;*/
	// always set attribute loc
	// 1: pos
	// 2: normal
	// 3: texcoords

public:
    /////////////////////////////////////////////////////////////////
    //
    // simply bind the shader for drawing
    // this doesn't need to be bound with an object
    //
    /////////////////////////////////////////////////////////////////
    void BindShader();

    /////////////////////////////////////////////////////////////////
    //
    // bind the shader with obj for drawing
    // this needs to be fed with some obj attributes,
    // such as the geotype, the material, and obj model transform matrix.
    //
    /////////////////////////////////////////////////////////////////
	virtual void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform ) {}
	/////////////////////////////////////////////////////////////////
	//
	// call this before draw calls
	// if anything need to be processed before draw
	//
	/////////////////////////////////////////////////////////////////
	virtual void PreDraw() {}
	/////////////////////////////////////////////////////////////////
	//
	// call this after draw calls
	// if anything need to be reverted before draw
	//
	/////////////////////////////////////////////////////////////////
	virtual void PostDraw() {}
};

/////////////////////////////////////////////////////////////////
//
// a shader which takes the M(model)V(view)P(projection) matrices.
// to transform points to NDC
//
/////////////////////////////////////////////////////////////////
class CMVPShader : public CShader {
public:
	CMVPShader();
	virtual ~CMVPShader() = 0;

public:
	// ubo
	GLuint _MVP_ubo;

protected:
    virtual void onInit();
	virtual void onDeinit();

    /////////////////////////////////////////////////////////////////
    //
    // init and deinit per MVP shader
    //
    /////////////////////////////////////////////////////////////////
    virtual void onInitMVPShader() {}
	virtual void onDeinitMVPShader() {}

public:
	void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform );
};


/////////////////////////////////////////////////////////////////
//
// a MVP shader which paint points with a single color
//
/////////////////////////////////////////////////////////////////
class CSingleColorShader : public CMVPShader {
public:
	CSingleColorShader();
	virtual ~CSingleColorShader() {}

public:
    GLint _uni_inputColorLoc;
    vec4 _vertexColor;

protected:
    virtual void onInitMVPShader();

public:
    void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform );
};



/////////////////////////////////////////////////////////////////
//
// a single color shader which paint points with size
// based on the distance to camera
//
/////////////////////////////////////////////////////////////////
class CPointSizeAttenuationShader : public CMVPShader {
public:
    CPointSizeAttenuationShader();
    ~CPointSizeAttenuationShader() {}

protected:

};


/////////////////////////////////////////////////////////////////
//
// a MVP shader which counts the covered area of the frame.
//
// the shader should hold a area counter variable which 
// can be used for subsequent to do the actual rendering.
// 
// this shader SHOULD NOT draw anything to the framebuffer.
//
/////////////////////////////////////////////////////////////////
class CAreaCountingShader : public CMVPShader {
public:
    CAreaCountingShader();
    ~CAreaCountingShader() {}

protected:
    GLuint _area_buffer;
	unsigned int _area;

    virtual void onInitMVPShader();
    virtual void onDeinitMVPShader();

public:
    void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform );
	void PreDraw();
	void PostDraw();

	unsigned int GetArea() { return _area;  }
};



/////////////////////////////////////////////////////////////////
//
// a MVP shader which simply paints the area with brightness
// based on distance.
//
// this shader should be a subsequence of an AreaCounting shader.
//
/////////////////////////////////////////////////////////////////
class CAreaPaintingShader : public CMVPShader {
public:
    CAreaPaintingShader( CAreaCountingShader* t_areaCountingShader );
    ~CAreaPaintingShader() {}

protected:
    virtual void onInitMVPShader();
	CAreaCountingShader* _areaCountingShader;
	GLuint _area_uniform_buffer;

public:
    void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform );
	void PostDraw();
};


/////////////////////////////////////////////////////////////////
//
// a MVP shader which simulates the blinn phone lighting model
//
/////////////////////////////////////////////////////////////////
class CPhongShader : public CMVPShader {
public:
	CPhongShader();
	~CPhongShader() {}

protected:
	// uniform location
	CLight* _light;

	GLuint _lighting_ubo;

	struct SLIGHTING {
		vec3 _lightPosInWorld;
		float _splExp;
		vec4 _ls, _ld, _la;
		vec4 _ks, _kd, _ka;
	};

protected:
    virtual void onInitMVPShader();

public:
	void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_transform );
	void SetLight( CLight* t_light ) { _light = t_light; }
};


/////////////////////////////////////////////////////////////////
//
// a shader used to test object normals
//
// normal is used to paint the point,
// eg., 
// a point with normal { 1, 0, 0 } will be painted to red
// a point with normal { 0, 1, 0 } will be painted to green
//
/////////////////////////////////////////////////////////////////
class CTestNormalShader : public CMVPShader {
public:
	CTestNormalShader();
	~CTestNormalShader() {}
};



// shader container
// a interface to get all possible shaders we have 
enum SHADER_TYPE { 
	SD_SINGLE_COLOR, 
	SD_PHONG, 
	SD_NORMAL_TEST, 
	SD_AREA_COUNT,
	SD_AREA_PAINT,
    SD_POINT_ATTENUATION,

	SD_COUNTER 
};

class CShaderContainer {
private:
	CShaderContainer();
	CShaderContainer( const CShaderContainer& t_cont );

private:
	CShader* _shaders[SD_COUNTER];
	bool _inited;

public:
	static CShaderContainer& GetInstance() {
		static CShaderContainer instance;
		return instance;
	}

	void Init();
	void Deinit();

	void BindShaderForDrawing( SHADER_TYPE t_type, CGeo* t_object, CMaterial* t_material, const mat4& t_transform );
	CShader* GetShader( SHADER_TYPE t_type );
};
