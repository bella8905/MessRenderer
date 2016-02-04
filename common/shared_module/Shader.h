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

public:
	// attrib location
	/*GLuint _attr_pos;*/
	// always set attribute loc
	// 1: pos
	// 2: normal
	// 3: texcoords

public:
	void BindShader();
	virtual void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform ) = 0;
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
	~CMVPShader() = 0;

public:
	// uniform variable names
	GLint _uni_viewMatLoc, _uni_projMatLoc, _uni_modelMatLoc;


protected:
    virtual void onInit();

    /////////////////////////////////////////////////////////////////
    //
    // init per MVP shader
    //
    /////////////////////////////////////////////////////////////////
    virtual void onInitMVPShader() {}

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
	~CSingleColorShader() {}

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

    virtual void onInitMVPShader();
    virtual void onDeinit();

public:
    void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform );
};



/////////////////////////////////////////////////////////////////
//
// a MVP shader which simply paints the area with brightness
// based on distance.
//
// this shader should be a subsequence of an AreaCounting shader.
//
/////////////////////////////////////////////////////////////////
class CAreaBasedPaintingShader : public CMVPShader {
public:
    CAreaBasedPaintingShader();
    ~CAreaBasedPaintingShader() {}

protected:
    virtual void onInitMVPShader();

public:
    // void BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform );
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
	GLint _uni_lightPos, _uni_lightLs, _uni_lightLd, _uni_lightLa, _uni_mtlKs, _uni_mtlKd, _uni_mtlKa, _uni_mtlSplExp;
	CLight* _light;

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
enum SHADER_TYPE { SD_SINGLE_COLOR, SD_PHONG, SD_NORMAL_TEST, SD_COUNTER };
class CShaderContainer {
private:
	CShaderContainer();
	CShaderContainer( const CShaderContainer& t_cont );
	void operator=( const CShaderContainer& t_cont );

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
};
