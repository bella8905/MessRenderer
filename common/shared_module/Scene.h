#pragma once

#include "Object.h"

class CScene {
public:
	CScene();
	virtual ~CScene();

public:
	vector<CObj> _objects;
	int _selectedObjIdx;

public:
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    //
    // objects
    //
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
	void AddObj( CObj& t_obj );
	void AddObjsFromFile( const std::string t_file );
    void ClearObjs();



	void Draw();
	void Update();


	void StopRotObj( const int& t_objIdx );
    void RevertSelectedObj() {
        if( _selectedObjIdx >= 0 && _selectedObjIdx < ( int )_objects.size() ) {
            _objects[_selectedObjIdx].RevertModelMatrix();
        }
    }

    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    //
    // Ray Intersection
    //
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
	// get the first hit object by a ray
	int GetRayHitObjIdx( const Utl::SRay& t_ray );
	// update arcball
	void RayIntersectWithArcball( const Utl::SRay& t_ray, const bool& t_isStart );

};

