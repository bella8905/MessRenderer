/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - shader
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <streambuf>

#include "Shader.h"
#include "Geometry.h"
#include "Light.h"
#include "View.h"
#include "Utl_Include.h"

static const std::string SHADER_PATH_PREPEND = "../../common/";


/////////////////////////////////////////////////////////////////
//
// Shader / Shader Program Error Checking
//
/////////////////////////////////////////////////////////////////

// read in txt file as std std::strings
bool CShader::readFileToStr( const char* t_file, std::string& t_fileContentStr ) {
	ifstream in( t_file );
	if( !in || in.bad() ) {
		LogError << "read shader file <<" << t_file << ">> failed" << LogEndl;
		in.close();
		in.clear();
		return false;
	}

	t_fileContentStr = std::string( ( istreambuf_iterator<char>( in ) ), istreambuf_iterator<char>() );

	in.close();
	in.clear();

	return true;
}


std::string CShader::glTypeToString( GLenum t_type ) {
	switch( t_type ) {
	case GL_BOOL: return "bool";
	case GL_INT: return "int";
	case GL_FLOAT: return "float";
	case GL_FLOAT_VEC2: return "vec2";
	case GL_FLOAT_VEC3: return "vec3";
	case GL_FLOAT_VEC4: return "vec4";
	case GL_FLOAT_MAT2: return "mat2";
	case GL_FLOAT_MAT3: return "mat3";
	case GL_FLOAT_MAT4: return "mat4";
	case GL_SAMPLER_2D: return "sampler2D";
	case GL_SAMPLER_3D: return "sampler3D";
	case GL_SAMPLER_CUBE: return "samplerCube";
	case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
	default: break;
	}
	return "other";
}

// print errors in shader compilation
void CShader::printShaderInfoLog( const GLuint& t_shaderIndex ) {
    GLint logLength;
    glGetShaderiv( t_shaderIndex, GL_INFO_LOG_LENGTH, &logLength );
    std::string str;
	// don't use reserve,
	// we need the actual array to be logLength long
    str.resize( logLength );
	// don't user str.c_str()
	// this returns a const char* which we can't change the content it's pointing to.
	// even we can const_cast it, we'd better not do thing out of boundary...
    glGetShaderInfoLog( t_shaderIndex, logLength, NULL, &str[0] );

	LogError << "shader info log for GL index " << t_shaderIndex << LogEndl << str << LogEndl << LogEndl;
}

// print errors in shader linking
void CShader::printSPInfoLog( const GLuint& t_spIndex ) {
    GLint logLength;
    glGetProgramiv( t_spIndex, GL_INFO_LOG_LENGTH, &logLength );
    std::string str;
    str.resize( logLength );
	glGetProgramInfoLog( t_spIndex, logLength, NULL, &str[0]  );
	LogError << "program info log for GL index " << t_spIndex << LogEndl << str << LogEndl << LogEndl;
}


bool CShader::checkShaderCompileStatus( const GLuint& t_shaderIndex ) {
	int param = -1;
	glGetShaderiv( t_shaderIndex, GL_COMPILE_STATUS, &param );
	if( GL_TRUE != param ) {
		LogError << "GL shader index " << t_shaderIndex << " did not compile" << LogEndl;
		printShaderInfoLog( t_shaderIndex );
		return false;
	}
	return true;
}

bool CShader::checkSPLinkingStatus( const GLuint& t_spIndex ) {
	int param = -1;
	glGetProgramiv( t_spIndex, GL_LINK_STATUS, &param );
	if( GL_TRUE != param ) {
		LogError << "GL shader program index " << t_spIndex << " did not link successfully" << LogEndl;
		printSPInfoLog( t_spIndex );
		return false;
	}
	return true;
}

bool CShader::validateSP( const GLuint& t_spIndex ) {
	int param = -1;
	glValidateProgram( t_spIndex );
	glGetProgramiv( t_spIndex, GL_VALIDATE_STATUS, &param );
	LogMsg << "program " << t_spIndex << " GL_VALIDATE_STATUS = " << param << LogEndl;
	if( GL_TRUE != param ) {
		printSPInfoLog( t_spIndex );
		return false;
	}
	return true;
}

// print absolutely everything about a shader program - only useful if you get really
// stuck wondering why a shader isn't working properly 
void CShader::printSPInfo( const GLuint& sp ) {
	int params = -1;
	int i;

	LogMsg << "--------------------shader program " << sp << " info--------------------" << LogEndl;
	glGetProgramiv( sp, GL_LINK_STATUS, &params );
	LogMsg << "GL_LINK_STATUS = " << params << LogEndl;

	glGetProgramiv( sp, GL_ATTACHED_SHADERS, &params );
	LogMsg << "GL_ATTACHED_SHADERS = " << params << LogEndl;


	glGetProgramiv( sp, GL_ACTIVE_ATTRIBUTES, &params );
	LogMsg << "GL_ACTIVE_ATTRIBUTES = " << params << LogEndl;

	for( i = 0; i < params; ++i ) {
		char name[64];
		int maxLength = 64;
		int actualLength = 0;
		int size = 0;
		GLenum type;
		glGetActiveAttrib( sp, i, maxLength, &actualLength, &size, &type, name );
		if( size > 1 ) {
			int j;
			for( j = 0; j < size; ++j ) {
				ostringstream ss;
				int location;
				ss << name << "[" << j << "]";
				std::string longName = ss.str();
				location = glGetAttribLocation( sp, longName.c_str() );
				LogMsg << i << "i) type: " << glTypeToString( type ) << ", name: " << longName << ", location: " << location << LogEndl;
			}
		} else {
			int location = glGetAttribLocation( sp, name );
			LogMsg << i << ") type: " << glTypeToString( type ) << ", name: " << name << ", location: " << location << LogEndl;
		}
	}

	glGetProgramiv( sp, GL_ACTIVE_UNIFORMS, &params );
	LogMsg << "GL_ACTIVE_UNIFORMS = " << params << LogEndl;
	for( i = 0; i < params; ++i ) {
		char name[64];
		int maxLength = 64;
		int actualLength = 0;
		int size = 0;
		GLenum type;
		glGetActiveUniform( sp, i, maxLength, &actualLength, &size, &type, name );
		if( size > 1 ) {
			int j;
			for( j = 0; j < size; ++j ) {
				ostringstream ss;
				int location;
				ss << name << "[" << j << "]";
				std::string longName = ss.str();
				location = glGetUniformLocation( sp, longName.c_str() );
				LogMsg << i << "i) type: " << glTypeToString( type ) << ", name: " << longName << ", location: " << location << LogEndl;
			}
		} else {
			int location = glGetUniformLocation( sp, name );
			LogMsg << i << ") type: " << glTypeToString( type ) << ", name: " << name << ", location: " << location << LogEndl;
		}
	}

	printSPInfoLog( sp );
}

GLint CShader::createShader( const char* t_shaderFile, GLenum t_shaderType ) {
	std::string shaderContent;
	if( !readFileToStr( t_shaderFile, shaderContent ) ) return -1;

	const GLchar* p = 0;

	GLuint shaderIndex = glCreateShader( t_shaderType );
	p = ( const GLchar* )shaderContent.c_str();
	glShaderSource( shaderIndex, 1, &p, NULL );
	glCompileShader( shaderIndex );
	if( !checkShaderCompileStatus( shaderIndex ) )  return -1;

	return shaderIndex;

}

GLuint CShader::createShaderProgram( const GLint& t_vs, const GLint& t_gs, const GLint& t_ts, const GLint& t_fs ) {
	GLuint sp = glCreateProgram();
	glAttachShader( sp, t_vs );
	glAttachShader( sp, t_fs );
	if( t_gs >= 0 ) glAttachShader( sp, t_gs );
	if( t_ts >= 0 ) glAttachShader( sp, t_ts );

	glLinkProgram( sp );
	if( !checkSPLinkingStatus( sp ) ) return -1;
	assert( validateSP( sp ) );

	// delete shaders here to free memory
	// https://www.opengl.org/discussion_boards/showthread.php/182721-Does-glDeleteProgram-free-shaders
	glDetachShader( sp, t_vs );
	glDetachShader( sp, t_fs );
	glDeleteShader( t_vs );
	glDeleteShader( t_fs );

	if( t_gs >= 0 ) {
		glDetachShader( sp, t_gs );
		glDeleteShader( t_gs );
	}
	if( t_ts >= 0 ) {
		glDetachShader( sp, t_ts );
		glDeleteShader( t_ts );
	}

    // print sp info
    // printSPInfo( sp );

	return sp;
}

// bind shaders by vertex shader file and fragment shader fie
void CShader::createShaderProgram() {
	GLint vs = -1;
	GLint gs = -1;
	GLint ts = -1;
	GLint fs = -1;

	vs = createShader( _vs.c_str(), GL_VERTEX_SHADER );
	fs = createShader( _fs.c_str(), GL_FRAGMENT_SHADER );
	assert( vs >= 0 );
	assert( fs >= 0 );

	if( _gs != "" ) {
		gs = createShader( _gs.c_str(), GL_GEOMETRY_SHADER );
		assert( gs >= 0 );
	}

	// bypass tessellation shader for now
	if( _ts != "" ) {
		// ts = createShader( _ts.c_str(), gl_tessshader)
	}

	_sp = createShaderProgram( vs, gs, ts, fs );

	_spCreated = true;
}

// tell everyone we are going to use the shader
void CShader::bindShader() {
	if( !_spCreated ) {
		LogError << "shader not inited" << LogEndl;
		return;
	}

	glUseProgram( _sp );
}

void CShader::initSP( const std::string& t_vs, const std::string& t_fs, const std::string& t_gs, const std::string& t_ts ) {
	// every init sp call goes here, make sure we don't create multiple sps
	if( _spCreated ) {
		LogError << "shader program already inited" << LogEndl;
		return;
	}

	_vs = SHADER_PATH_PREPEND + t_vs;
	_fs = SHADER_PATH_PREPEND + t_fs;
	if( _gs != "" ) {
		_gs = SHADER_PATH_PREPEND + t_gs;
	}
	if( _ts != "" ) {
		_ts = SHADER_PATH_PREPEND + t_ts;
	}

	createShaderProgram();

    onInit();
}


/////////////////////////////////////////////////////////////////
//
// MVP shader
//
/////////////////////////////////////////////////////////////////
CMVPShader::CMVPShader() : _MVP_ubo( 0 ) {
	// initSP( PERSP_CAM_SHADER_VS_FILE, PERSP_CAM_SHADER_FS_FILE );
}

CMVPShader::~CMVPShader() {
}

void CMVPShader::onInit() {
    // uniforms
	glGenBuffers( 1, &_MVP_ubo );
	glBindBuffer( GL_UNIFORM_BUFFER, _MVP_ubo );
	glBufferData( GL_UNIFORM_BUFFER, sizeof( mat4 ) * 3, NULL, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    onInitMVPShader();
}


void CMVPShader::onDeinit() {
	glDeleteBuffers( 1, &_MVP_ubo );
	onDeinitMVPShader();
}

// bind perspective camera shader specific content for drawing
void CMVPShader::BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform ) {
	CView* view = View_GetActive();
	assert( view );

	CShader::bindShader();

	glBindBuffer( GL_UNIFORM_BUFFER, _MVP_ubo );
	mat4* matrices = ( mat4* )glMapBufferRange( GL_UNIFORM_BUFFER, 0, 3 * sizeof( mat4 ), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT );
	matrices[0] = view->GetWorld2ViewMatrix();
	matrices[1] = view->GetView2ProjMatrix();
	mat4 modelMat = ( t_object ) ? ( t_trandform * ( t_object->GetPreProcessedModelMat() ) ) : t_trandform;
	matrices[2] = modelMat;

	glUnmapBuffer( GL_UNIFORM_BUFFER );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	
	glBindBufferBase( GL_UNIFORM_BUFFER, 0, _MVP_ubo );

}


/////////////////////////////////////////////////////////////////
//
// single color shader
//
/////////////////////////////////////////////////////////////////
const std::string SINGLE_COLOR_SHADER_VS_FILE = "shaders/simple_lookAtCam.vert";
const std::string SINGLE_COLOR_SHADER_FS_FILE = "shaders/simple.frag";


CSingleColorShader::CSingleColorShader() : _vertexColor( vec4( 1.0f, 0.0f, 0.0f, 1.0f ) ), _uni_inputColorLoc( -1 ) {
	initSP( SINGLE_COLOR_SHADER_VS_FILE, SINGLE_COLOR_SHADER_FS_FILE );
}


void CSingleColorShader::onInitMVPShader() {
    _uni_inputColorLoc = glGetUniformLocation( _sp, "inputColor" );
    assert( _uni_inputColorLoc >= 0 );
}


// bind phong shader specific content for drawing
void CSingleColorShader::BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform ) {
    CMVPShader::BindShaderWithObjectForDrawing( t_object, t_material, t_trandform );
    glUniform4fv( _uni_inputColorLoc, 1, glm::value_ptr( _vertexColor ) );

}


/////////////////////////////////////////////////////////////////
//
// point size attenuation shader
//
/////////////////////////////////////////////////////////////////
const std::string POINT_SIZE_ATTENUATION_SHADER_VS_FILE = "shaders/point_size_attenuation.vert";
const std::string POINT_SIZE_ATTENUATION_SHADER_FS_FILE = "shaders/simple.frag";


CPointSizeAttenuationShader::CPointSizeAttenuationShader() {
    initSP( POINT_SIZE_ATTENUATION_SHADER_VS_FILE, POINT_SIZE_ATTENUATION_SHADER_FS_FILE );
}



/////////////////////////////////////////////////////////////////
//
// area counting shader
//
/////////////////////////////////////////////////////////////////
const std::string AREA_COUNTING_SHADER_VS_FILE = "shaders/simple_lookAtCam.vert";
const std::string AREA_COUNTING_SHADER_FS_FILE = "shaders/area_counting.frag";


CAreaCountingShader::CAreaCountingShader() : _area( 0 ) {
    initSP( AREA_COUNTING_SHADER_VS_FILE, AREA_COUNTING_SHADER_FS_FILE );
}


void CAreaCountingShader::onInitMVPShader() {
	glGenBuffers( 1, &_area_buffer );
	glBindBuffer( GL_ATOMIC_COUNTER_BUFFER, _area_buffer );
	glBufferData( GL_ATOMIC_COUNTER_BUFFER, sizeof( GLuint ), NULL, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ATOMIC_COUNTER_BUFFER, 0 );
};

void CAreaCountingShader::onDeinitMVPShader() {
    glDeleteBuffers( 1, &_area_buffer );
}


void CAreaCountingShader::BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform ) {
    CMVPShader::BindShaderWithObjectForDrawing( t_object, t_material, t_trandform );
    // use the buffer
	// reset data

	glBindBuffer( GL_ATOMIC_COUNTER_BUFFER, _area_buffer );
	GLuint* counter = ( GLuint* )glMapBufferRange( GL_ATOMIC_COUNTER_BUFFER, 0, sizeof( GLuint ), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT );
	counter[0] = 0;
	glUnmapBuffer( GL_ATOMIC_COUNTER_BUFFER );
	glBindBuffer( GL_ATOMIC_COUNTER_BUFFER, 0 );

	glBindBufferBase( GL_ATOMIC_COUNTER_BUFFER, 0, _area_buffer );

}

void CAreaCountingShader::PreDraw() {
	// also disable depth writing
	// if we set depth func to LESS
	// depth value written later won't overwrite the first one ( which we actually don't draw on screen )
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glDepthMask( GL_FALSE );
}

void CAreaCountingShader::PostDraw() {
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	glDepthMask( GL_TRUE );

	glMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT );
	// read back
	glBindBuffer( GL_ATOMIC_COUNTER_BUFFER, _area_buffer );
	GLuint* area = ( GLuint* )glMapBufferRange( GL_ATOMIC_COUNTER_BUFFER, 0, sizeof( GLuint ), GL_MAP_READ_BIT );

	_area = area[0];
	glUnmapBuffer( GL_ATOMIC_COUNTER_BUFFER );
	glBindBuffer( GL_ATOMIC_COUNTER_BUFFER, 0 );

	glBindBufferBase( GL_ATOMIC_COUNTER_BUFFER, 0, 0 );
}


/////////////////////////////////////////////////////////////////
//
// area based painting shader
//
/////////////////////////////////////////////////////////////////
const std::string AREA_PAINTING_SHADER_VS_FILE = "shaders/simple_lookAtCam.vert";
const std::string AREA_PAINTING_SHADER_FS_FILE = "shaders/area_painting.frag";


CAreaPaintingShader::CAreaPaintingShader( CAreaCountingShader* t_areaCountingShader ) : _areaCountingShader( t_areaCountingShader )  {
    initSP( AREA_PAINTING_SHADER_VS_FILE, AREA_PAINTING_SHADER_FS_FILE );
}



void CAreaPaintingShader::onInitMVPShader() {
	glGenBuffers( 1, &_area_uniform_buffer );
	glBindBuffer( GL_UNIFORM_BUFFER, _area_uniform_buffer );
	glBufferData( GL_UNIFORM_BUFFER, sizeof( unsigned int ), NULL, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

void CAreaPaintingShader::BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform ) {
	CMVPShader::BindShaderWithObjectForDrawing( t_object, t_material, t_trandform );
	
	if( _areaCountingShader ) {
		unsigned int area = _areaCountingShader->GetArea();
		// bind buffer
		glBindBuffer( GL_UNIFORM_BUFFER, _area_uniform_buffer );
		glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( unsigned int ), &area );

		glBindBufferBase( GL_UNIFORM_BUFFER, 1, _area_uniform_buffer );

	}

}

void CAreaPaintingShader::PostDraw() {
	glBindBufferBase( GL_UNIFORM_BUFFER, 1, 0 );
}



/////////////////////////////////////////////////////////////////
//
// phone shader
//
/////////////////////////////////////////////////////////////////
const std::string PHONG_SHADER_VS_FILE = "shaders/phong.vert";
const std::string PHONG_SHADER_FS_FILE = "shaders/phong.frag";


CPhongShader::CPhongShader() : _light( 0 ), _lighting_ubo( 0 ) {
	initSP( PHONG_SHADER_VS_FILE, PHONG_SHADER_FS_FILE );
}


void CPhongShader::onInitMVPShader() {
	glGenBuffers( 1, &_lighting_ubo );
	glBindBuffer( GL_UNIFORM_BUFFER, _lighting_ubo );
	glBufferData( GL_UNIFORM_BUFFER, sizeof( SLIGHTING ), NULL, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}


// bind phong shader specific content for drawing
void CPhongShader::BindShaderWithObjectForDrawing( CGeo* t_object, CMaterial* t_material, const mat4& t_trandform ) {
	assert( _light && t_material );
	CMVPShader::BindShaderWithObjectForDrawing( t_object, t_material, t_trandform );



	glBindBuffer( GL_UNIFORM_BUFFER, _lighting_ubo );
	SLIGHTING* lighting = ( SLIGHTING* )glMapBufferRange( GL_UNIFORM_BUFFER, 0, sizeof( SLIGHTING ), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT );
	lighting->_lightPosInWorld = _light->GetPos();
	lighting->_ls = Utl::ToDirection( _light->GetLs() );
	lighting->_ld = Utl::ToDirection( _light->GetLd() );
	lighting->_la = Utl::ToDirection( _light->GetLa() );
	lighting->_kd = t_material->GetKd()._Color;
	lighting->_ka = t_material->GetKa()._Color;

	if( t_material->GetHasSpecular() ) {
		lighting->_ks = t_material->GetKs()._Color;
		lighting->_splExp = t_material->GetSplExp();

	} else {
		// set ks to all zeros
		lighting->_ks = vec4( 0.f, 0.f, 0.f, 0.f );
		lighting->_splExp = 0.f;
	}

	glUnmapBuffer( GL_UNIFORM_BUFFER );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );

	glBindBufferBase( GL_UNIFORM_BUFFER, 2, _lighting_ubo );
	
}

/////////////////////////////////////////////////////////////////
//
// normal test shader
//
/////////////////////////////////////////////////////////////////
const std::string NORTEST_SHADER_VS_FILE = "shaders/normal_test.vert";
const std::string NORTEST_SHADER_FS_FILE = "shaders/normal_test.frag";


CTestNormalShader::CTestNormalShader() {
	initSP( NORTEST_SHADER_VS_FILE, NORTEST_SHADER_FS_FILE );
}


/////////////////////////////////////////////////////////////////
//
// shader container
//
/////////////////////////////////////////////////////////////////

CShaderContainer::CShaderContainer() {
	for( us i = 0; i < SD_COUNTER; ++i ) {
		_shaders[i] = 0;
	}
}


void CShaderContainer::Init() {
	if( _inited ) {
		LogError << "shader container already inited" << LogEndl;
		return;
	}

	CSingleColorShader* singlecolor = new CSingleColorShader();
	_shaders[SD_SINGLE_COLOR] = singlecolor;

	CPhongShader* phong = new CPhongShader();
	phong->SetLight( &g_simpleLight );
	_shaders[SD_PHONG] = phong;

	CTestNormalShader* normaltest = new CTestNormalShader();
	_shaders[SD_NORMAL_TEST] = normaltest;

	CAreaCountingShader* areaCount = new CAreaCountingShader();
	_shaders[SD_AREA_COUNT] = areaCount;

	CAreaPaintingShader* areaPaint = new CAreaPaintingShader( areaCount );
	_shaders[SD_AREA_PAINT] = areaPaint;
 
    CPointSizeAttenuationShader* pointAttenuation = new CPointSizeAttenuationShader();
    _shaders[SD_POINT_ATTENUATION] = pointAttenuation;

	_inited = true;
}


void CShaderContainer::Deinit() {
	if( !_inited ) {
		LogError << "shader container not inited" << LogEndl;
		return;
	}

	for( us i = 0; i < SD_COUNTER; ++i ) {
		if( _shaders[i] ) {
			delete _shaders[i];
			_shaders[i] = 0;
		}
	}


	_inited = false;
}

void CShaderContainer::BindShaderForDrawing( SHADER_TYPE t_type, CGeo* t_object, CMaterial* t_material, const mat4& t_transform ) {
	if( !_inited ) {
		LogError << "shader container not inited" << LogEndl;
		return;
	}

	if( !_shaders[t_type] ) {
		LogError << "shader not inited in shader container" << LogEndl;
		return;
	}

	_shaders[t_type]->BindShaderWithObjectForDrawing( t_object, t_material, t_transform );
}

CShader* CShaderContainer::GetShader( SHADER_TYPE t_type ) {
	if( !_inited ) return 0;
	if( t_type < 0 || t_type >= SD_COUNTER ) return 0;

	return _shaders[t_type];
}